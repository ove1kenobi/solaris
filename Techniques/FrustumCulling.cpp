#include "../pch.h"
#include "FrustumCulling.h"

const bool FrustumCulling::Initialize(PlayerCamera& playerCamera) noexcept
{
	DirectX::XMFLOAT4X4 PMatrix = playerCamera.getPMatrix();
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);
	m_BoundingFrustum = DirectX::BoundingFrustum(pMatrix);
	return true;
}

void FrustumCulling::CreateShadowFrustum(DirectX::XMMATRIX shadowProjectionMatrix) noexcept
{
	m_ShadowBoundingFrustum = DirectX::BoundingFrustum(shadowProjectionMatrix);
}

void FrustumCulling::CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera, struct RenderData& renderData) noexcept
{
	/*Gameobjects in the vector comes in the order of:
	  1. Planets.
	  2. Orbits.
	  3. Sun.
	  4. Spaceship
	  5. (TO BE) Asteroids
	  
	  We use this information in the frustum culling!*/
	renderData = {};
	renderData.sunCulled = true;
	renderData.allObjects = &gameObjects;
	DirectX::XMFLOAT4X4 VMatrix = playerCamera.getVMatrix();
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	for (unsigned int i{ 0u }; i < gameObjects.size(); ++i)
	{
		//The objects is either a planet or the sun:
		if (gameObjects[i]->ShallBeTestedForCulling())
		{
			DirectX::BoundingSphere boundingSphere = gameObjects[i]->GetBoundingSphere();
			boundingSphere.Transform(boundingSphere, vMatrix);
			if (m_BoundingFrustum.Contains(boundingSphere) > DirectX::DISJOINT)
			{
				renderData.culledObjects.push_back(gameObjects[i]);
				renderData.totalNrOfObjects++;
				if (gameObjects[i]->GetTag() == "Planet")
				{
					//Add it to its own vector only filled with planets to be sorted based on depth:
					DirectX::XMFLOAT3 planetToCamera = playerCamera.getPos() - gameObjects[i]->GetCenter();
					gameObjects[i]->SetDistanceToCamera(std::sqrtf(planetToCamera.x * planetToCamera.x + planetToCamera.y * planetToCamera.y 
						+ planetToCamera.z * planetToCamera.z));
					renderData.culledPlanetsDepthSorted.push_back(gameObjects[i]);
					//Also update nr of planets in scene:
					renderData.nrOfPlanetsInView++;
				}
				else if (gameObjects[i]->GetTag() == "Sun")
				{
					renderData.sunCulled = false;
				}
				else if (gameObjects[i]->GetTag() == "Asteroid")
				{
					renderData.nrOfAsteroidsInView++;
				}
			}
		}// The object is either an orbit or the ship, and they are to be rendered always
		 // Therefore, include by default:
		else
		{
			renderData.culledObjects.push_back(gameObjects[i]);
			renderData.totalNrOfObjects++;
			if (gameObjects[i]->GetTag() == "Orbit")
			{
				renderData.totalNrOfOrbits++;
			}
			else if (gameObjects[i]->GetTag() == "SpaceShip")
			{
				/* Add spaceship upgrades to render queue */
				SpaceShip* ship = static_cast<SpaceShip*>(gameObjects[i]);
				std::vector<GameObject*> upgrades = ship->GetUpgrades();
				for (GameObject* u : upgrades)
				{
					// Filter out possible empty upgrades
					if (u) renderData.culledObjects.push_back(u);
					renderData.totalNrOfObjects++;
				}
			}
		}
	}
	/*ALL ITEMS in culledObjects are still in the order of:
	  Planets -> Orbits -> Sun -> SpaceShip*/
#if defined(DEBUG) | defined (_DEBUG)
	ImGui::Begin("Frustum Culling");
	ImGui::Text("Objects being rendered: %d\nPlanets being rendered: %d\nSun rendered: %d\nAsteroids rendered: %d", 
		renderData.culledObjects.size(), renderData.nrOfPlanetsInView, !renderData.sunCulled, renderData.nrOfAsteroidsInView);
	ImGui::End();
#endif
	//It remains to sort the culled planets based on distance:
	if (renderData.culledPlanetsDepthSorted.size() > 0)
	{
		std::sort(renderData.culledPlanetsDepthSorted.begin(),
			renderData.culledPlanetsDepthSorted.end(),
			[](const GameObject* lhs, const GameObject* rhs)
			{
				return lhs->GetDistanceToCamera() < rhs->GetDistanceToCamera();
			});
	}
}

std::vector<GameObject*> FrustumCulling::CullShadowObjects(std::vector<GameObject*>* gameObjects, const DirectX::XMMATRIX& viewMatrix, const size_t& nrOfPlanets) noexcept
{
	std::vector<GameObject*> culledPlanets;
	for (unsigned int i{ 0u }; i < nrOfPlanets; ++i)
	{
		DirectX::BoundingSphere boundingSphere = *(*gameObjects)[i]->GetModel()->GetBoundingSphere();
		boundingSphere.Transform(boundingSphere, viewMatrix);
		if (m_ShadowBoundingFrustum.Contains(boundingSphere) > DirectX::DISJOINT)
		{
			culledPlanets.push_back((*gameObjects)[i]);
		}
	}
	return culledPlanets;
}

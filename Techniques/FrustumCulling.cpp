#include "../pch.h"
#include "FrustumCulling.h"

const bool FrustumCulling::Initialize(PlayerCamera& playerCamera) noexcept
{
	m_BoundingFrustum = DirectX::BoundingFrustum(playerCamera.getPMatrix());
	return true;
}

void FrustumCulling::CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera, struct CulledData& culledData) noexcept
{
	/*Gameobjects in the vector comes in the order of:
	  1. Planets.
	  2. Orbits.
	  3. Sun.
	  4. Spaceship
	  5. (TO BE) Asteroids
	  
	  We use this information in the frustum culling!*/

	culledData = {};
	culledData.sunCulled = true;
	for (unsigned int i{ 0u }; i < gameObjects.size(); ++i)
	{
		//The objects is either a planet or the sun:
		if (gameObjects[i]->ShallBeTestedForCulling())
		{
			DirectX::BoundingSphere boundingSphere = *gameObjects[i]->GetModel()->GetBoundingSphere();
			boundingSphere.Transform(boundingSphere, playerCamera.getVMatrix());
			if (m_BoundingFrustum.Contains(boundingSphere) > DirectX::DISJOINT)
			{
				culledData.culledObjects.push_back(gameObjects[i]);
				culledData.totalNrOfObjects++;
				if (gameObjects[i]->GetTag() == "Planet")
				{
					//Add it to its own vector only filled with planets to be sorted based on depth:
					DirectX::XMFLOAT3 planetToCamera = playerCamera.getPos() - gameObjects[i]->GetCenter();
					gameObjects[i]->SetDistanceToCamera(std::sqrtf(planetToCamera.x * planetToCamera.x + planetToCamera.y * planetToCamera.y 
						+ planetToCamera.z * planetToCamera.z));
					culledData.culledPlanetsDepthSorted.push_back(gameObjects[i]);
					//Also update nr of planets in scene:
					culledData.nrOfPlanetsInView++;
				}
				else if (gameObjects[i]->GetTag() == "Sun")
				{
					culledData.sunCulled = false;
				}
			}
		}// The object is either an orbits or the ship, and they are to be rendered always
		 // Therefore, include by default:
		else
		{
			if (gameObjects[i]->GetTag() == "Orbit")
			{
				culledData.totalNrOfOrbits++;
			}
			culledData.culledObjects.push_back(gameObjects[i]);
			culledData.totalNrOfObjects++;
		}
	}
	/*ALL ITEMS in culledObjects are still in the order of:
	  Planets -> Orbits -> Sun -> SpaceShip*/
#if defined(DEBUG) | defined (_DEBUG)
	ImGui::Begin("Frustum Culling");
	ImGui::Text("Objects being rendered: %d\nPlanets being rendered: %d\nSun rendered: %d", 
		culledData.culledObjects.size(), culledData.nrOfPlanetsInView, !culledData.sunCulled);
	ImGui::End();
#endif
	//It remains to sort the culled planets based on distance:
	if (culledData.culledPlanetsDepthSorted.size() > 0)
	{
		std::sort(culledData.culledPlanetsDepthSorted.begin(),
			culledData.culledPlanetsDepthSorted.end(),
			[](const GameObject* lhs, const GameObject* rhs)
			{
				return lhs->GetDistanceToCamera() < rhs->GetDistanceToCamera();
			});
	}
}

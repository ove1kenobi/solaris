#include "../pch.h"
#include "FrustumCulling.h"

const bool FrustumCulling::Initialize(PlayerCamera& playerCamera) noexcept
{
	m_BoundingFrustum = DirectX::BoundingFrustum(playerCamera.getPMatrix());
	return true;
}

void FrustumCulling::UpdateFrustum(PlayerCamera& playerCamera) noexcept
{
	//Not necessary:
	m_BoundingFrustum.Transform(m_BoundingFrustum, playerCamera.getVMatrix());
}

const std::vector<GameObject*> FrustumCulling::CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera) noexcept
{
	// TODO: insert return statement here
	std::vector<GameObject*> culledObjects;

	for (unsigned int i{ 0u }; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i]->HasBoundingVolume())
		{
			DirectX::BoundingSphere boundingSphere = *gameObjects[i]->GetModel()->GetBoundingSphere();
			boundingSphere.Transform(boundingSphere, playerCamera.getVMatrix());
			if (m_BoundingFrustum.Contains(boundingSphere) > DirectX::DISJOINT)
			{
				culledObjects.push_back(gameObjects[i]);
			}
		}
		else
		{
			//Is orbit or ship and should be added to culledObjects (FEW vertices and/or visible pretty much everywhere!):

		}
	}
#if defined(DEBUG) | defined (_DEBUG)
	ImGui::Begin("Frustum Culling");
	ImGui::Text("Objects on screen: %d", culledObjects.size());
	ImGui::End();
#endif

	return culledObjects;
}

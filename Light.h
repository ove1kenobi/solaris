#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Light {
private:
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMVECTOR ambient;
	DirectX::XMVECTOR diffuse;
	D3D11_BUFFER_DESC buffer;
protected:
public:
	Light(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse);
	~Light();
	bool CreateViewMatrix();
	bool CreateProjectionMatrix();
};

//Directional Lights
//have no source, only a direction and color
//Point Lights
//have a position, a fall off point or range, and a color
//Spotlights
//have a direction, a position, a color, two angles, and a fall off point or range.


/*
CODE FROM OLD PROJECT
//light and buffer
ID3D11Buffer* LightBuffer = nullptr;
struct PointLight
{
	XMMATRIX LightView;
	XMMATRIX LightProjection;
	XMVECTOR Position;
	XMVECTOR Direction;
	XMVECTOR Ambient;
	XMVECTOR Diffuse;
};
PointLight light;

void CreateLight() {
	light.Position = XMVectorSet(-20.0f, 15.0f, -20.0f, 0.0f);
	light.Direction = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	light.Ambient = XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);
	light.Diffuse = XMVectorSet(0.8f, 0.8f, 0.8f, 1.0f);

	XMVECTOR lookAtPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	light.LightView = XMMatrixLookAtLH(light.Position, lookAtPos, direction);

	light.LightProjection = XMMatrixOrthographicLH((float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);

		//VIEW
	XMVECTOR camPos = XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f);				//camera position
	XMVECTOR lookAtPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);			//camera focal point
	XMVECTOR direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);			//camera direction

	WorldViewProjection.camPosition = camPos;
	WorldViewProjection.squareView = XMMatrixLookAtLH(camPos, lookAtPos, direction);
	WorldViewProjection.haveNormalmap = false;

	//PROJECTION
	float aspectRadio = (float)WIDTH / (float)HEIGHT;
	WorldViewProjection.squareProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45),											//fieldOfview
		aspectRadio,													//Aspect ratio
		0.1f,															//closest plane
		100.0f															//furthest plane
	);


	//LIGHT BUFFER
	D3D11_BUFFER_DESC bufferDescLight;
	memset(&bufferDescLight, 0, sizeof(bufferDescLight));
	bufferDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.Usage = D3D11_USAGE_DEFAULT;
	bufferDescLight.ByteWidth = sizeof(light);
	D3D11_SUBRESOURCE_DATA dataLight;
	dataLight.pSysMem = &light;
	gDevice->CreateBuffer(&bufferDescLight, &dataLight, &LightBuffer);
}
*/
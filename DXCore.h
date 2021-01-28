//TODO: Move includes to pch (Emil F)
#pragma once
#include <wrl.h>
#include <assert.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

class DXCore
{
private:
	
public:
	DXCore();
	virtual ~DXCore();
	const bool Initialize();
};
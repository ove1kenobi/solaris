#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <wrl.h>
#include <assert.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#if defined(DEBUG) | defined (_DEBUG)
	#ifndef HR
		#define HR(x, var)																	\
		{																					\
			HRESULT hr = (x);																\
			if (FAILED(hr))																	\
			{																				\
				std::string errorString = "\0";												\
				errorString.append("Error: ");												\
				errorString.append(var);													\
				errorString.append(" failed.");												\
				errorString.append("\nFile: ");												\
				errorString.append(__FILE__);												\
				errorString.append(".\nLine: ");											\
				errorString.append(std::to_string(__LINE__));								\
				errorString.append(".\nSee console output for detailed debug information.");\
				MessageBoxA(nullptr, errorString.c_str(), "ERROR", MB_OK | MB_ICONERROR);	\
				return false;																\
			}																				\
		}
	#endif
#else
	#ifndef HR
	#define HR(x, var) (x)
	#endif
#endif

#if defined(DEBUG) | defined (_DEBUG)
#ifndef HR_E																				
#define HR_E(x, y, var)																		\
		{																					\
			HRESULT hr = (x);																\
			if (FAILED(hr))																	\
			{																				\
				std::string errorString = "\0";												\
				errorString.append("Error: ");												\
				errorString.append(var);													\
				errorString.append(" failed.");												\
				errorString.append("\nFile: ");												\
				errorString.append(__FILE__);												\
				errorString.append(".\nLine: ");											\
				errorString.append(std::to_string(__LINE__));								\
				errorString.append(".\nSee console output for detailed debug information.");\
				Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob = (y);							\
				if (pErrorBlob != nullptr)													\
				{																			\
					OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());				\
				}																			\
				MessageBoxA(nullptr, errorString.c_str(), "ERROR", MB_OK | MB_ICONERROR);	\
				return false;																\
			}																				\
		}
#endif
#else
#ifndef HR_E
#define HR_E(x, y, var) (x)
#endif
#endif

#if defined(DEBUG) | defined (_DEBUG)
#ifndef HR_A
#define HR_A(x, var)																		\
		{																					\
			HRESULT hr = (x);																\
			if (FAILED(hr))																	\
			{																				\
				std::string errorString = "\0";												\
				errorString.append("Error: ");												\
				errorString.append(var);													\
				errorString.append(" failed.");												\
				errorString.append("\nFile: ");												\
				errorString.append(__FILE__);												\
				errorString.append(".\nLine: ");											\
				errorString.append(std::to_string(__LINE__));								\
				errorString.append(".\nSee console output for detailed debug information.");\
				MessageBoxA(nullptr, errorString.c_str(), "ERROR", MB_OK | MB_ICONERROR);	\
				assert(SUCCEEDED(hr));														\
			}																				\
		}
#endif
#else
#ifndef HR_A
#define HR_A(x, var) (x)
#endif
#endif

#if defined(DEBUG) | defined (_DEBUG)
#ifndef HR_X
#define HR_X(x, var)																		\
		{																					\
			HRESULT hr = (x);																\
			if (FAILED(hr))																	\
			{																				\
				std::string errorString = "\0";												\
				errorString.append("Error: ");												\
				errorString.append(var);													\
				errorString.append(" failed.");												\
				errorString.append("\nFile: ");												\
				errorString.append(__FILE__);												\
				errorString.append(".\nLine: ");											\
				errorString.append(std::to_string(__LINE__));								\
				errorString.append(".\nSee console output for detailed debug information.");\
				MessageBoxA(nullptr, errorString.c_str(), "ERROR", MB_OK | MB_ICONERROR);	\
				assert(SUCCEEDED(hr));														\
			}																				\
		}
#endif
#else
#ifndef HR_X
#define HR_X(x, var) (x)
#endif
#endif
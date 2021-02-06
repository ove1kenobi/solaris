#pragma once
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
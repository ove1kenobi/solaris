#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
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
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>
#include <d3d11.h>
//For 2D rendering
//---------------
#include <d2d1.h>
#pragma comment(lib, "d2d1")
//---------------
#include <dxgi.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <crtdbg.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <random>
#include <memory>
#include <chrono>
#include <stdexcept>
#include <stdarg.h>
#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>


#include <d3d9.h>
#include <d3dx9math.h>

struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; D3DCOLOR color; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)
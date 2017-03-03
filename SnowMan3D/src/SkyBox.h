#pragma once

#include "directx_common.h"

/*
1. 加载地形 Terrain
2. 加载天空盒 SkyBox 
*/

struct SKYBOXVERTEX {
	float x, y, z;
	float u, v;
};
const DWORD SKYBOXFVF = D3DFVF_XYZ | D3DFVF_TEX1;

class SkyBox {
public:
	SkyBox(LPDIRECT3DDEVICE9 d3ddev);
	void Init();
	void Render(D3DXMATRIX *world);
	void Clean();

private:
	LPDIRECT3DDEVICE9 _d3ddev;

	// sky box
	float _skyLength = 20000.f;
	LPDIRECT3DVERTEXBUFFER9 _skyBuffer;
	LPDIRECT3DINDEXBUFFER9 _skyIndices;
	LPDIRECT3DTEXTURE9 _skyTexs[5];
};
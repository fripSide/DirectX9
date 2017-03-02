#pragma once

#include "directx_common.h"

/*
1. 加载地形 Terrain
2. 加载天空盒 SkyBox
3. 光照效果 
4. 粒子效果
*/

struct SKYBOXVERTEX {
	float x, y, z;
	float u, v;
};
const DWORD SKYBOXFVF = D3DFVF_XYZ | D3DFVF_TEX1;

class Scenes {
public:
	Scenes(LPDIRECT3DDEVICE9 d3ddev);
	void Init();
	void Render(D3DXMATRIX *world);
	void Clean();

private:
	void initTerrain();

	void initSkyBox();
	void renderSkyBox(D3DXMATRIX *world);
	void cleanSkyBox();


	LPDIRECT3DDEVICE9 _d3ddev;
	LPDIRECT3DVERTEXBUFFER9 _floor = NULL;
	LPDIRECT3DTEXTURE9 _tex = NULL;

	// terrain


	// sky box
	float _skyLength = 20000.f;
	LPDIRECT3DVERTEXBUFFER9 _skyBuffer;
	LPDIRECT3DINDEXBUFFER9 _skyIndices;
	LPDIRECT3DTEXTURE9 _skyTexs[5];
};
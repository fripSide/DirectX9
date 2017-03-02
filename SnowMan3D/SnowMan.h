#pragma once
#include "directx_common.h"

class SnowMan {

public:
	SnowMan(LPDIRECT3DDEVICE9 d3ddev);
	void Init();
	void Render(D3DXMATRIX *world);
	void Clean();

private:
	LPDIRECT3DDEVICE9 _d3ddev;
	LPD3DXMESH _head;
	LPD3DXMESH _body;
};
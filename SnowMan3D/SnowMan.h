#pragma once
#include "directx_common.h"

class SnowMan {

public:
	SnowMan(LPDIRECT3DDEVICE9 d3ddev);
	void Init();
	void Render(Vertex *position);
	void Clean();

private:
	D3DMATERIAL9 initMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	

private:
	LPDIRECT3DDEVICE9 _d3ddev;
	LPD3DXMESH _head;
	LPD3DXMESH _body;
	LPD3DXMESH _pillar;
	D3DMATERIAL9 Mtrls[4];
	D3DXMATRIX _mh, _mb;

	const D3DXCOLOR      WHITE = D3DCOLOR_XRGB(255, 255, 255);
	const D3DXCOLOR      BLACK = D3DCOLOR_XRGB(0, 0, 0);
	const D3DXCOLOR        RED = D3DCOLOR_XRGB(255, 0, 0);
	const D3DXCOLOR      GREEN = D3DCOLOR_XRGB(0, 255, 0);
	const D3DXCOLOR       BLUE = D3DCOLOR_XRGB(0, 0, 255);
	const D3DXCOLOR     YELLOW = D3DCOLOR_XRGB(255, 255, 0);
	const D3DXCOLOR       CYAN = D3DCOLOR_XRGB(0, 255, 255);
	const D3DXCOLOR    MAGENTA = D3DCOLOR_XRGB(255, 0, 255);
};
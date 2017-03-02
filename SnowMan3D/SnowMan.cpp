#include "SnowMan.h"

SnowMan::SnowMan(LPDIRECT3DDEVICE9 d3ddev) {
	_d3ddev = d3ddev;

}

void SnowMan::Init() {
	// 横向和竖向的切面数都为1000
	D3DXCreateSphere(_d3ddev, 0.3f, 30, 30, &_head, NULL);
	D3DXCreateSphere(_d3ddev, 0.5f, 30, 30, &_body, NULL);
}

void SnowMan::Render(D3DXMATRIX *world) {
	D3DXMATRIX T, R, P, S, h, b;

	if (world) T = *world;
	else D3DXMatrixIdentity(&T);

	D3DXMatrixTranslation(&h, 0.0f, 0.2f, 0.0f);
	D3DXMatrixTranslation(&b, 0.0f, -.5f, 0.0f);

	_d3ddev->SetTexture(NULL, NULL);
	_d3ddev->SetMaterial(&d3d::WHITE_MTRL);
	P = h * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &P);
	_head->DrawSubset(0);
	P = b * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &P);
	_body->DrawSubset(0);

}

void SnowMan::Clean() {
	d3d::Release<LPD3DXMESH>(_head);
	d3d::Release<LPD3DXMESH>(_body);
}
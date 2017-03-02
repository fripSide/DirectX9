#include "SnowMan.h"

SnowMan::SnowMan(LPDIRECT3DDEVICE9 d3ddev) {
	_d3ddev = d3ddev;
}

void SnowMan::Init() {
	// 横向和竖向的切面数都为30
	D3DXCreateSphere(_d3ddev, 0.7f, 30, 30, &_head, NULL);
	D3DXCreateSphere(_d3ddev, 1.f, 30, 30, &_body, NULL);
	D3DXCreateCylinder(_d3ddev, 0.4f, 0.f, 0.8f, 30, 30, &_cap, NULL);
}

void SnowMan::Render(D3DXMATRIX *world) {
	D3DXMATRIX T, R, P, S, W;

	if (world) W = *world;
	else D3DXMatrixIdentity(&W);

	_d3ddev->SetTexture(NULL, NULL);

	// 身体
	_d3ddev->SetMaterial(&d3d::WHITE_MTRL);
	D3DXMatrixTranslation(&T, 0.0f, -0.5f, 0.0f);
	P = W * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &P);
	_body->DrawSubset(0);

	// 头部
	D3DXMatrixTranslation(&T, 0.0f, 1.15f, 0.0f);
	_d3ddev->SetMaterial(&d3d::WHITE_MTRL);
	P = W * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &P);
	_head->DrawSubset(0);

	// 帽子
	D3DXMatrixTranslation(&T, 0.f, 2.1f, 0.f);
	D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);
	_d3ddev->SetMaterial(&d3d::RED_MTRL);
	P = R * W * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &P);
	_cap->DrawSubset(0);


}

void SnowMan::Clean() {
	d3d::Release<LPD3DXMESH>(_head);
	d3d::Release<LPD3DXMESH>(_body);
	d3d::Release<LPD3DXMESH>(_cap);
}
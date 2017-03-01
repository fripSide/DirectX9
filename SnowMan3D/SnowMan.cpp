#include "SnowMan.h"

SnowMan::SnowMan(LPDIRECT3DDEVICE9 d3ddev) {
	_d3ddev = d3ddev;

}


void SnowMan::Init() {
	// 横向和竖向的切面数都为1000
	D3DXCreateSphere(_d3ddev, 0.3f, 30, 30, &_head, NULL);
	D3DXCreateSphere(_d3ddev, 0.5f, 30, 30, &_body, NULL);

	//
	// Build world matrices - position the objects in world space.
	//

	D3DXMatrixTranslation(&_mh, 0.0f, 0.2f, 10.0f);
	D3DXMatrixTranslation(&_mb, 0.0f, -.5f, 10.0f);

	Mtrls[0] = initMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	Mtrls[1] = initMtrl(RED, RED, RED, BLACK, 2.0f);
	Mtrls[2] = initMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	Mtrls[3] = initMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);

	D3DXCreateCylinder(_d3ddev, 0.5f, 0.5f, 5.f, 20, 20, &_pillar, NULL);

	_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, false);

	
}

D3DMATERIAL9 SnowMan::initMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p) {
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

void SnowMan::Render(Vertex *p) {
	_d3ddev->SetTexture(NULL, NULL);
	_d3ddev->SetMaterial(&Mtrls[0]);
	_d3ddev->SetTransform(D3DTS_WORLD, &_mh);
	_head->DrawSubset(0);
	_d3ddev->SetTransform(D3DTS_WORLD, &_mb);
	_body->DrawSubset(0);

	D3DXMATRIX T, R, P, S;
	D3DXMatrixScaling(&S, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);

	_d3ddev->SetMaterial(&Mtrls[3]);
	_d3ddev->SetTexture(NULL, NULL);
	for (int i = 0; i < 5; i++) {
		D3DXMatrixTranslation(&T, -5.f, -2.5f, -15.f + i * 7.5f);
		P = R * T * S;
		_d3ddev->SetTransform(D3DTS_WORLD, &P);
		_pillar->DrawSubset(0);

		D3DXMatrixTranslation(&T, 5.f, -2.5f, -15.f + i * 7.5f);
		P = R * T * S;
		_d3ddev->SetTransform(D3DTS_WORLD, &P);
		_pillar->DrawSubset(0);
	}
}

void SnowMan::Clean() {
	d3d::Release<LPD3DXMESH>(_head);
	d3d::Release<LPD3DXMESH>(_body);

	d3d::Release<LPD3DXMESH>(_pillar);
}
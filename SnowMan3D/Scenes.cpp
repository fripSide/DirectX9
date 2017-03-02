#include "Scenes.h"

Scenes::Scenes(LPDIRECT3DDEVICE9 d3ddev) {
	_d3ddev = d3ddev;
}

void Scenes::Init() {
	initTerrain();
	initSkyBox();
}


void Scenes::Render(D3DXMATRIX *world) {
	_d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DXMATRIX T, R, P, S;
	float scale = 1.f;
	D3DXMatrixScaling(&S, scale, scale, scale);

	// draw floor
	D3DXMatrixIdentity(&T);
	T = S * T;
	_d3ddev->SetTransform(D3DTS_WORLD, &T);
	_d3ddev->SetMaterial(&d3d::WHITE_MTRL);
	_d3ddev->SetTexture(0, _tex);
	_d3ddev->SetStreamSource(0, _floor, 0, sizeof(Vertex));
	_d3ddev->SetFVF(Vertex::FVF);
	_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	renderSkyBox(world);
}

void Scenes::Clean() {
	d3d::Release<IDirect3DVertexBuffer9*>(_floor);
	d3d::Release<IDirect3DTexture9*>(_tex);

	cleanSkyBox();
}

void Scenes::initTerrain() {
	_d3ddev->CreateVertexBuffer(6 * sizeof(Vertex),
		NULL,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&_floor,
		NULL);

	Vertex *v;
	_floor->Lock(0, 0, (void**)&v, 0);

	float y = -2.0f;

	v[0] = Vertex(-2000.0f, y, -2000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-2000.0f, y, 2000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(2000.0f,  y, 2000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-2000.0f, y, -2000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(2000.0f, y, 2000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(2000.0f, y, -2000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	_floor->Unlock();

	D3DXCreateTextureFromFile(_d3ddev, L"res/snowland.bmp",
		&_tex);
}


void Scenes::initSkyBox() {
	_d3ddev->CreateVertexBuffer(20 * sizeof(SKYBOXVERTEX),
		NULL,
		SKYBOXFVF,
		D3DPOOL_MANAGED,
		&_skyBuffer,
		NULL);
	_d3ddev->CreateIndexBuffer(36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_skyIndices,
		0);

	float len = _skyLength * 0.5f;

	SKYBOXVERTEX *vertexs;
	_skyBuffer->Lock(0, 0, (void**)&vertexs, 0);
	// front
	vertexs[0] = {-len, 0.f, len, 0.f, 1.f};
	vertexs[1] = {-len, len, len, 0.f, 0.f };
	vertexs[2] = { len, 0.f, len, 1.f, 1.f};
	vertexs[3] = { len, len, len, 1.f, 0.f };

	// back
	vertexs[4] = { len, 0.f, -len, 0.f, 1.f };
	vertexs[5] = { len, len, -len, 0.f, 0.f };
	vertexs[6] = { -len, 0.f, -len, 1.f, 1.f };
	vertexs[7] = { -len, len, -len, 1.f, 0.f };

	// left 
	vertexs[8] = {-len, 0.f, -len, 0.f, 1.f};
	vertexs[9] = { -len, len, -len, 0.f, 0.f };
	vertexs[10] = { -len, 0.f, len, 1.f, 1.f };
	vertexs[11] = {-len, len, len, 1.f, 0.f};

	// right
	vertexs[12] = {len, 0.f, len, 0.f, 1.f};
	vertexs[13] = { len, len, len, 0.f, 0.f };
	vertexs[14] = { len, 0.f, -len, 1.f, 1.f };
	vertexs[15] = {len, len, -len, 1.f, 0.f};

	// top
	vertexs[16] = { len, len, -len, 1.f, 0.f };
	vertexs[17] = {len, len, len, 1.f, 1.f};
	vertexs[18] = {-len, len, -len, 0.f, 0.f};
	vertexs[19] = {-len, len, len, 0.f, 1.f};

	_skyBuffer->Unlock();

	WORD *indices = NULL;
	_skyIndices->Lock(0, 0, (void**)&indices, 0);
	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 5;
	indices[3] = 0; indices[4] = 4; indices[5] = 5;


	// left side
	indices[6] = 0; indices[7] = 4; indices[8] = 7;
	indices[9] = 0; indices[10] = 3; indices[11] = 7;

	// right side
	indices[12] = 1; indices[13] = 5; indices[14] = 6;
	indices[15] = 1; indices[16] = 2; indices[17] = 6;


	// back side
	indices[18] = 3; indices[19] = 7; indices[20] = 6;
	indices[21] = 3; indices[22] = 2; indices[23] = 6;

	// top side
	indices[24] = 7; indices[25] = 6; indices[26] = 5;
	indices[27] = 7; indices[28] = 4; indices[29] = 5;

	// bottom side
	indices[30] = 3; indices[31] = 0; indices[32] = 7;
	indices[33] = 3; indices[34] = 2; indices[35] = 1;

	_skyIndices->Unlock();

	// load textures
	D3DXCreateTextureFromFile(_d3ddev, L"res/skyfront.jpg", &_skyTexs[0]);
	D3DXCreateTextureFromFile(_d3ddev, L"res/skyback.jpg", &_skyTexs[1]);
	D3DXCreateTextureFromFile(_d3ddev, L"res/skyleft.jpg", &_skyTexs[2]);
	D3DXCreateTextureFromFile(_d3ddev, L"res/skyright.jpg", &_skyTexs[3]);
	D3DXCreateTextureFromFile(_d3ddev, L"res/skytop.jpg", &_skyTexs[4]);
}

void Scenes::renderSkyBox(D3DXMATRIX *world) {
	_d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	D3DXMATRIX t, v, s;
	float scale = 200.f;
	D3DXMatrixTranslation(&v, 0.0f, -3500.0f, 0.f);
	D3DXMatrixScaling(&s, scale, scale, scale);
	D3DXMatrixIdentity(&t);
	//_d3ddev->SetMaterial(&mtrl);
	//t = t * v * s;
	_d3ddev->SetTransform(D3DTS_WORLD, &v);
	_d3ddev->SetStreamSource(NULL, _skyBuffer, 0, sizeof(SKYBOXVERTEX));
	_d3ddev->SetFVF(SKYBOXFVF);
	//_d3ddev->SetIndices(_skyIndices);

	for (int i = 0; i < 5; i++) {
		_d3ddev->SetTexture(0, _skyTexs[i]);
		_d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	} 
}

void Scenes::cleanSkyBox() {
	d3d::Release<LPDIRECT3DVERTEXBUFFER9>(_skyBuffer);
	d3d::Release<LPDIRECT3DINDEXBUFFER9>(_skyIndices);
	for (int i = 0; i < 5; i++) {
		d3d::Release<LPDIRECT3DTEXTURE9>(_skyTexs[i]);
	}
}
#include "SnowMan3D.h"

void SnowMan3D::init(LPDIRECT3DDEVICE9 device) {
	_device = device;
	// init snow man
	D3DCOLOR col = D3DCOLOR_XRGB(255, 255, 255);
	CUSTOMVERTEX objs[] = {
		{420.0f, 150.0f, 0.5f, 100.0f, col },
		{420.0f, 350.0f, 0.5f, 100.0f, col },
		{220.0f, 150.0f, 0.5f, 100.0f, col },
		{220.0f, 350.0f, 0.5f, 100.0f, col },
	};
	_device->CreateVertexBuffer(sizeof(objs), 0, CUSTOMFVF, D3DPOOL_DEFAULT, &_snowManBuf, NULL);

	void *ptr;
	_snowManBuf->Lock(0, sizeof(objs), (void **)&ptr, 0);
	memcpy(ptr, objs, sizeof(objs));
	_snowManBuf->Unlock();

	printf("init finish...");
}

void SnowMan3D::render() {
	_device->SetStreamSource(0, _snowManBuf, 0, sizeof(CUSTOMVERTEX));
	_device->SetFVF(CUSTOMFVF);
	_device->DrawPrimitive(D3DPT_LINELIST, 0, 2);
}
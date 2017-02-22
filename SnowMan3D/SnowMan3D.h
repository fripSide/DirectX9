#pragma once
#include "directx_common.h"
#include <cstdio>

/***
1. 封装基本的初始化
2. 完全封装DirectX init
*/
class SnowMan3D {

public:
	static SnowMan3D* instance() {
		if (_instance == NULL) {
			_instance = new SnowMan3D();
		}
		return _instance;
	}

	void initD3D(HWND hWnd);
	void render();
	void cleanD3D();

private:
	SnowMan3D() {};
	SnowMan3D(SnowMan3D const &);
	SnowMan3D& operator=(SnowMan3D const &);


	void initBuffers();

private:
	static SnowMan3D *_instance;
	LPDIRECT3D9 _d3d;
	LPDIRECT3DDEVICE9 _d3ddev;
	LPDIRECT3DVERTEXBUFFER9 _snowManBuf;
	
};
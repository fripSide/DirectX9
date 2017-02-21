#pragma once
#include "directx_common.h"
#include <cstdio>

/***
1. 封装基本的初始化
2. 完成封装DirectX init
*/
class SnowMan3D {

public:
	static SnowMan3D& instance() {
		static SnowMan3D _instance;
		return _instance;
	}

	void init(LPDIRECT3DDEVICE9 device);
	void render();

private:
	SnowMan3D() {};
	SnowMan3D(SnowMan3D const &);
	SnowMan3D& operator=(SnowMan3D const &);

	LPDIRECT3DDEVICE9 _device;
	LPDIRECT3DVERTEXBUFFER9 _snowManBuf;

};
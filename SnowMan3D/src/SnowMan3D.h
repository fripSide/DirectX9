#pragma once
#include "directx_common.h"
#include <cstdio>
#include "SnowMan.h"
#include "SkyBox.h"
#include "SnowParticle.h"
#include "SnowTerrian.h"
#include "DirectxInput.h"

/***
1. 封装DirectX基本的初始化
2. 完全封装object init和render
3. 封装相机
*/
class SnowMan3D {

public:
	static SnowMan3D* instance() {
		if (_instance == NULL) {
			_instance = new SnowMan3D();
		}
		return _instance;
	}

	void InitD3D(HWND hWnd, HINSTANCE hInstance);
	void Render();
	void CleanD3D();

private:
	SnowMan3D() {};
	SnowMan3D(SnowMan3D const &);
	SnowMan3D& operator = (const SnowMan3D &);

	float getFPS();
	bool initScenes();
	void initLights();

	// 相机视角控制
	void handlerUserInput(float timeDelta);
	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	//void roll(float angle);  // rotate on look vector
	void getViewMatrix(D3DXMATRIX *v);

private:
	static SnowMan3D *_instance;
	HWND _hwnd = NULL;
	LPDIRECT3D9 _d3d = NULL;
	LPDIRECT3DDEVICE9 _d3ddev = NULL;

	float _lastTime = 0.f; // 上一帧的时间
	D3DXVECTOR3 _right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 _up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 _look = D3DXVECTOR3(0.0f, 0.f, 1.0f);
	D3DXVECTOR3 _pos = D3DXVECTOR3(0.f, 0.f, -20.f);
	bool _first = true; // 初始化相机位置
	POINT lastMousePosition; // 鼠标位置
	DirectxInput *_dxInput;

	LPD3DXFONT _pTextFPS = NULL; // FPS
	wchar_t _strFPS[50];

	// 场景部件
	ID3DXFont *_fontFPS = NULL;
	SnowMan *sman = NULL;
	LPD3DXMESH _box = NULL;
	SkyBox *skyBox = NULL;
	Snow *snow = NULL;
	SnowTerrian *snowTerrian = NULL;
};
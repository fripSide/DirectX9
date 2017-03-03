#include "SnowMan3D.h"
#include <time.h>
#include <tchar.h>
#include <cstdio>

SnowMan3D* SnowMan3D::_instance = NULL;
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//========================================================
// directx_common
//
void d3d::log(const char * fmt, ...) {
	FILE *fp = fopen("log.txt", "w");
	va_list args;
	va_start(args, fmt);
	fprintf(fp, fmt, args);
	va_end(args);
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p) {
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

d3d::BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = d3d::INFINITY_FLT;
	_min.y = d3d::INFINITY_FLT;
	_min.z = d3d::INFINITY_FLT;

	_max.x = -d3d::INFINITY_FLT;
	_max.y = -d3d::INFINITY_FLT;
	_max.z = -d3d::INFINITY_FLT;
}

bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float d3d::GetRandomFloat(float lowBound, float highBound) {
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

DWORD d3d::FtoDw(float f) {
	return *((DWORD*)&f);
}

void d3d::GetRandomVector(
	D3DXVECTOR3* out,
	D3DXVECTOR3* min,
	D3DXVECTOR3* max) {
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

float d3d::Lerp(float a, float b, float t) {
	return a - (a*t) + (b*t);
}

//========================================================
//
//
void SnowMan3D::InitD3D(HWND hWnd, HINSTANCE hInstance) {
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	_hwnd = hWnd;

	_dxInput = new DirectxInput();
	_dxInput->Init(hWnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	_d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&_d3ddev);

	if (!initScenes()) { // call the function to initialize the all the objects in the scenes
		MessageBox(_hwnd, _T("Init Scenes failed"), _T(""), NULL);
	} 


	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f,
		300000.0f);
	_d3ddev->SetTransform(D3DTS_PROJECTION, &proj);

	//_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);   //关闭光照  
	//_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //开启背面消隐  
	//_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  //设置线框填充模式  

}

void SnowMan3D::initLights() {
	D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = col * 0.4f;
	light.Diffuse = col;
	light.Specular = col * 0.6f;
	light.Direction = dir;
	_d3ddev->SetLight(0, &light);
	_d3ddev->LightEnable(0, true);
	_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);  //镜面反射
}

void SnowMan3D::Render() { 
	//计算每帧时间
	float currTime = (float) ::timeGetTime();
	float timeDelta = (currTime - _lastTime) * 0.001f;

	handlerUserInput(timeDelta);
	// 更新雪花位置
	snow->update(timeDelta);

	// 开始绘制
	_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_d3ddev->BeginScene();

	D3DXMATRIX V, I, T, P, Ry;
	// 设置相机
	getViewMatrix(&V);
	_d3ddev->SetTransform(D3DTS_VIEW, &V);

	// 加载光照
	initLights();

	// 绘制地形
	float _floor = -2.5f; //地平线
	D3DXMatrixTranslation(&T, 0.f, _floor, 0.f);
	snowTerrian->draw(&T, false);

	// 移动天空盒
	skyBox->Render(&T);

	// 画雪人
	D3DXMATRIX v1, v2, v3;
	D3DXMatrixTranslation(&v1, 0.f, _floor, 0.f); // 雪人1 位置
	// 雪人2绕Y轴旋转
	float y = ::timeGetTime() / 1000.f;
	D3DXMatrixRotationY(&Ry, y);
	D3DXMatrixTranslation(&v2, 0.f, -1.0f, 5.f);
	D3DXMatrixTranslation(&v3, 0.f, -3.3f, 5.f);
	v2 = v2 * Ry, v3 = v3 * Ry;

	// 判断雪人2 是否到了雪人1 的背面
	bool behind = false;
	D3DXVECTOR3 PP, S1, S2;
	S1 = S2 = { 0.f, 0.f, 0.f };
	//D3DXVec3TransformCoord(&PP, &PP, &v1);
	D3DXVec3TransformCoord(&S1, &S1, &v1);
	D3DXVec3TransformCoord(&S2, &S2, &v2);
	float PAx = S1.x - _pos.x, PAz = S1.z - _pos.z;
	float ABx = S2.x - S1.x, ABz = S2.z - S1.z;
	float val = PAx * ABx + PAz * ABz; //cos < 0, 雪人2到了背后， 
	behind = val > 0;

	d3d::log("S2: x=%f z=%f", S2.x, S2.z);
	//D3DXMatrixTranslation(&v2, S2.x, S2.y, S2.z);

	if (!behind) {
		// 雪人1
		sman->Render(&v1);

		// 画箱子
		_d3ddev->SetTransform(D3DTS_WORLD, &v3);
		_d3ddev->SetTexture(NULL, NULL);
		_d3ddev->SetMaterial(&d3d::DESERT_SAND_MTRL);
		_box->DrawSubset(0);

		// 雪人2
		sman->Render(&v2);
	} else { // 转到了背面

		// 画箱子
		_d3ddev->SetTransform(D3DTS_WORLD, &v3);
		_d3ddev->SetTexture(NULL, NULL);
		_d3ddev->SetMaterial(&d3d::DESERT_SAND_MTRL);
		_box->DrawSubset(0);

		// 雪人2
		sman->Render(&v2);

		// 雪人1
		sman->Render(&v1);
	}

	// 下雪
	D3DXMatrixIdentity(&I);
	_d3ddev->SetTransform(D3DTS_WORLD, &I);
	snow->render();


	//显示帧数
	RECT formatRect;
	GetClientRect(_hwnd, &formatRect);

	//在窗口右上角处，显示每秒帧数  
	formatRect.top = 5;
	int charCount = swprintf_s(_strFPS, 20, _T("FPS:%0.3f"), getFPS());
	_pTextFPS->DrawText(NULL, _strFPS, charCount, &formatRect, DT_TOP | DT_LEFT, D3DCOLOR_RGBA(0, 239, 136, 255));

	_d3ddev->EndScene();
	_d3ddev->Present(NULL, NULL, NULL, NULL);

	_lastTime = currTime;
}


void SnowMan3D::CleanD3D() {
	sman->Clean();
	delete sman;
	skyBox->Clean();
	delete skyBox;
	d3d::Release<LPD3DXMESH>(_box);
	d3d::Delete<PSystem*>(snow);
	d3d::Delete<SnowTerrian*>(snowTerrian);
	d3d::Delete<DirectxInput*>(_dxInput);
	d3d::Release<LPD3DXFONT>(_pTextFPS);

	delete _instance;
	_instance = NULL;
}

bool SnowMan3D::initScenes() {
	sman = new SnowMan(_d3ddev);
	sman->Init();
	skyBox = new SkyBox(_d3ddev);
	skyBox->Init();

	// 初始化雪花
	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-100.0f, -3.0f, -100.0f);
	boundingBox._max = D3DXVECTOR3(100.0f, 22.0f, 100.0f);
	snow = new Snow(&boundingBox, 5000);
	snow->init(_d3ddev, L"res/snowflake.dds");

	// 地形
	snowTerrian = new SnowTerrian(_d3ddev, "res/castlehm257.raw", 64, 64, 100, -3.5f);
	D3DXVECTOR3 lightDirection(0.f, 1.f, 0.f);
	//snowTerrian->genTexture(&lightDirection);
	snowTerrian->loadTexture(L"res/snowland.bmp");
	
	// 箱子
	D3DXCreateBox(_d3ddev, 2.f, 2.f, 2.f, &_box, NULL);

	// FPS
	D3DXCreateFont(_d3ddev, 36, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &_pTextFPS);

	return true;
}

void SnowMan3D::handlerUserInput(float timeDelta) {
	_dxInput->GetInput();

	if (_dxInput->IsKeyDown(DIK_W))
		walk(4.0f * timeDelta);

	if (_dxInput->IsKeyDown(DIK_S))
		walk(-4.0f * timeDelta);

	if (::GetAsyncKeyState('A') & 0x8000f)
		strafe(-4.0f * timeDelta);

	if (::GetAsyncKeyState('D') & 0x8000f)
		strafe(4.0f * timeDelta);

	if (::GetAsyncKeyState('R') & 0x8000f)
		fly(4.0f * timeDelta);

	if (::GetAsyncKeyState('F') & 0x8000f)
		fly(-4.0f * timeDelta);

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		pitch(1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		pitch(-1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		yaw(-1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		yaw(1.0f * timeDelta);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000f || _first) {
		// 按空格键复位到最初的位置
		_right = D3DXVECTOR3(1.f, 0.f, 0.f);
		_pos = D3DXVECTOR3(0.f, 0.f, -20.f);
		_look = D3DXVECTOR3(0.f, 0.f, 1.f);
		_up = D3DXVECTOR3(0.f, 1.f, 0.f);
		_first = false;
	}

	// 鼠标位置
	float x = _dxInput->MouseDX() * 0.0006f;
	float y = _dxInput->MouseDY() * 0.0006f;
	float z = _dxInput->MouseDZ() * 0.3f;
	if (_dxInput->IsMouseButtonDown(0)) {
		strafe(x);
		walk(y);
	}
	fly(z);
	pitch(z * -0.01f);
	
}

void SnowMan3D::strafe(float units) {
	_pos += D3DXVECTOR3(_right.x, 0.f, _right.z) * units;
}

void SnowMan3D::walk(float units) {
	_pos += D3DXVECTOR3(_look.x, 0.f, _look.z) * units;
}

void SnowMan3D::fly(float units) {
	_pos.y += units;
	if (_pos.y < 0.f) _pos.y = 0.f;
	if (_pos.y > 20.f) _pos.y = 20.f;
}

void SnowMan3D::pitch(float angle) {
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void SnowMan3D::yaw(float angle) {
	D3DXMATRIX T;
	D3DXMatrixRotationY(&T, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void SnowMan3D::getViewMatrix(D3DXMATRIX *V) {
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

float SnowMan3D::getFPS() {

	static float  fps = 0; //我们需要计算的FPS值  
	static int    frameCount = 0;//帧数  
	static float  currentTime = 0.0f;//当前时间  
	static float  lastTime = ::timeGetTime() * 0.001f;  

	frameCount++;  
	currentTime = ::timeGetTime() * 0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间
	//每秒计算一次帧数  
	if (currentTime - lastTime > 1.0f) {
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值  
		lastTime = currentTime;   
		frameCount = 0;  
	}

	return fps;

}
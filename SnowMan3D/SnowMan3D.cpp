#include "SnowMan3D.h"
#include <time.h>
#include <tchar.h>

SnowMan3D* SnowMan3D::_instance = NULL;
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p) {
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

void SnowMan3D::InitD3D(HWND hWnd) {
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	_hwnd = hWnd;


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

	D3DXMATRIX mv;
	D3DXMatrixLookAtLH(&mv, &_pos, &_look, &_up);
	_d3ddev->SetTransform(D3DTS_VIEW, &mv);
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

	_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	setCameraView(timeDelta);
	_d3ddev->BeginScene();

	// 加载光照
	initLights();

	// 画场景和雪人
	D3DXMATRIX v;
	D3DXMatrixTranslation(&v, 0.f, -2.5f, 0.f);
	scene->Render(&v);

	D3DXMatrixTranslation(&v, 0.f, -2.5f, 0.f);
	smanCenter->Render(&v);

	D3DXMATRIX sa, Ry;
	D3DXMatrixTranslation(&sa, 0.f, -2.5f, 1.5f);
	float y = ::timeGetTime() / 1000.f;
	// 绕Y轴旋转
	D3DXMatrixRotationY(&Ry, y);
	sa = sa * Ry;
	smanArround->Render(&sa);
	_d3ddev->EndScene();
	_d3ddev->Present(NULL, NULL, NULL, NULL);

	_lastTime = currTime;
}


void SnowMan3D::CleanD3D() {
	smanCenter->Clean();
	delete smanCenter;
	smanArround->Clean();
	delete smanArround;
	scene->Clean();
	delete scene;
	delete _instance;
	_instance = NULL;
}

bool SnowMan3D::initScenes() {
	smanCenter = new SnowMan(_d3ddev);
	smanCenter->Init();
	smanArround = new SnowMan(_d3ddev);
	smanArround->Init();
	scene = new Scenes(_d3ddev);
	scene->Init();
	return true;
}

void SnowMan3D::setCameraView(float timeDelta) {

	if (::GetAsyncKeyState('W') & 0x8000f)
		walk(4.0f * timeDelta);

	if (::GetAsyncKeyState('S') & 0x8000f)
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

	D3DXMATRIX V;
	//D3DXMatrixLookAtLH(&V, &_pos, &_look, &_up);
	getViewMatrix(&V);
	_d3ddev->SetTransform(D3DTS_VIEW, &V);
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
	static float  lastTime = clock() * 0.001f;  

	frameCount++;  
	currentTime = clock() * 0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间
	//每秒计算一次帧数  
	if (currentTime - lastTime > 1.0f) {
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值  
		lastTime = currentTime;   
		frameCount = 0;  
	}

	return fps;

}
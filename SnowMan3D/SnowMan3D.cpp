#include "SnowMan3D.h"

SnowMan3D* SnowMan3D::_instance = NULL;


void SnowMan3D::initD3D(HWND hWnd) {
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;

	// create a device class using this information and the info from the d3dpp stuct
	_d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&_d3ddev);

	initBuffers();    // call the function to initialize the triangle
}

void SnowMan3D::render() {
	_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	_d3ddev->BeginScene();

	// select which vertex format we are using
	_d3ddev->SetFVF(CUSTOMFVF);

	// select the vertex buffer to display
	_d3ddev->SetStreamSource(0, _snowManBuf, 0, sizeof(CUSTOMVERTEX));

	// copy the vertex buffer to the back buffer
	_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	_d3ddev->EndScene();

	_d3ddev->Present(NULL, NULL, NULL, NULL);
}


void SnowMan3D::cleanD3D() {
	_snowManBuf->Release();    // close and release the vertex buffer
	_d3ddev->Release();    // close and release the 3D device
	_d3d->Release();    // close and release Direct3D
	delete _instance;
	_instance = NULL;
}

void SnowMan3D::initBuffers() {
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
	};

	// create a vertex buffer interface called v_buffer
	_d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&_snowManBuf,
		NULL);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	_snowManBuf->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	_snowManBuf->Unlock();
}

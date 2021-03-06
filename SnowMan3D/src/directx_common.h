#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <time.h>   
#include <cstdarg>

#pragma comment(lib,"d3d9.lib")  
#pragma comment(lib,"d3dx9.lib")  
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib") 
#pragma comment(lib, "winmm.lib ")  



struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; D3DCOLOR color; };
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)



// define the screen resolution
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Vertex {
	Vertex() {}
	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v) {
		_x = x;  _y = y;  _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;  _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF;
};


namespace d3d {

	void log(const char * fmt, ...);

	template<class T> void Release(T t) {
		if (t) {
			t->Release();
			t = NULL;
		}
	}

	template<class T> void Delete(T t) {
		if (t) {
			delete t;
			t = NULL;
		}
	}

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR      GREY(D3DCOLOR_XRGB(169, 169, 169));
	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
	const D3DXCOLOR BEACH_SAND(D3DCOLOR_XRGB(255, 249, 157));
	const D3DXCOLOR DESERT_SAND(D3DCOLOR_XRGB(250, 205, 135));
	const D3DXCOLOR LIGHTGREEN(D3DCOLOR_XRGB(60, 184, 120));
	const D3DXCOLOR  PUREGREEN(D3DCOLOR_XRGB(0, 166, 81));
	const D3DXCOLOR  DARKGREEN(D3DCOLOR_XRGB(0, 114, 54));
	const D3DXCOLOR LIGHT_YELLOW_GREEN(D3DCOLOR_XRGB(124, 197, 118));
	const D3DXCOLOR  PURE_YELLOW_GREEN(D3DCOLOR_XRGB(57, 181, 74));
	const D3DXCOLOR  DARK_YELLOW_GREEN(D3DCOLOR_XRGB(25, 123, 48));
	const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
	const D3DXCOLOR DARKBROWN(D3DCOLOR_XRGB(115, 100, 87));

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
	const D3DMATERIAL9 GREY_MTRL = InitMtrl(GREY, GREY, GREY, BLACK, 2.0f);
	const D3DMATERIAL9 BEACH_SAND_MTRL = InitMtrl(BEACH_SAND, BEACH_SAND, BEACH_SAND, BLACK, 2.f);
	const D3DMATERIAL9 DESERT_SAND_MTRL = InitMtrl(DESERT_SAND, DESERT_SAND, DESERT_SAND, BLACK, 2.f);

	const float INFINITY_FLT = FLT_MAX;
	const float EPSILON = 0.001f;


	struct BoundingBox {
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};
	//
	// Randomness
	//

	// Desc: Return random float in [lowBound, highBound] interval.
	float GetRandomFloat(float lowBound, float highBound);


	// Desc: Returns a random vector in the bounds specified by min and max.
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

	DWORD FtoDw(float f);

	float Lerp(float a, float b, float t);
}
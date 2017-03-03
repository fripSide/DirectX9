//=============================================================================
// Desc: DirectInput键盘输入处理类源文件
// 2013年 2月03日  Create by 浅墨 
//=============================================================================
#include "DirectxInput.h"


//-----------------------------------------------------------------------------
// Desc: 构造函数
//-----------------------------------------------------------------------------
DirectxInput::DirectxInput()
{
	m_pDirectInput = NULL;
	m_KeyboardDevice = NULL;
	ZeroMemory(m_keyBuffer, sizeof(char) * 256);
	m_MouseDevice = NULL;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


//-----------------------------------------------------------------------------
// Name：DInputClass::Init(）
// Desc: 初始化DirectInput键盘及鼠标输入设备
//-----------------------------------------------------------------------------
HRESULT DirectxInput::Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	//HRESULT hr;
	//初始化一个IDirectInput8接口对象
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

	//进行键盘设备的初始化
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, NULL);
	m_KeyboardDevice->SetCooperativeLevel(hWnd, keyboardCoopFlags);
	m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	m_KeyboardDevice->Acquire();
	m_KeyboardDevice->Poll();

	//进行鼠标设备的初始化
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);
	m_MouseDevice->SetCooperativeLevel(hWnd, mouseCoopFlags);
	m_MouseDevice->SetDataFormat(&c_dfDIMouse);
	m_MouseDevice->Acquire();
	m_KeyboardDevice->Poll();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name：DInputClass::GetInput()
// Desc: 用于获取输入信息的函数
//-----------------------------------------------------------------------------
void DirectxInput::GetInput()
{
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void**)&m_keyBuffer);
	//获取键盘输入消息
	if (hr)
	{
		m_KeyboardDevice->Acquire();
		m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (LPVOID)m_keyBuffer);
	}

	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	//获取鼠标输入消息
	if (hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	}
}

//-----------------------------------------------------------------------------
// Name：DInputClass::IsKeyDown()
// Desc: 判断键盘上某个键是否按下
//-----------------------------------------------------------------------------
bool DirectxInput::IsKeyDown(int iKey)
{
	if (m_keyBuffer[iKey] & 0x80)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// Name：DInputClass::IsMouseButtonDown()
// Desc: 判断鼠标上某键是否按下
//-----------------------------------------------------------------------------
bool DirectxInput::IsMouseButtonDown(int button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) != 0;
}

//-----------------------------------------------------------------------------
// Name：DInputClass::MouseDX
// Desc: 返回鼠标指针的X轴坐标值
//-----------------------------------------------------------------------------
float DirectxInput::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------------------------------------------------
// Name：DInputClass::MouseDY
// Desc: 返回鼠标指针的Y轴坐标值
//-----------------------------------------------------------------------------
float DirectxInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// Name：DInputClass::MouseDZ
// Desc: 返回鼠标指针的Z轴坐标值（滚轮）
//-----------------------------------------------------------------------------
float DirectxInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}

//-----------------------------------------------------------------------------
// Desc: 析构函数
//-----------------------------------------------------------------------------
DirectxInput::~DirectxInput(void)
{
	if (m_KeyboardDevice != NULL)
		m_KeyboardDevice->Unacquire();
	if (m_MouseDevice != NULL)
		m_MouseDevice->Unacquire();
	d3d::Release<IDirectInputDevice8*>(m_KeyboardDevice);
	d3d::Release<IDirectInputDevice8*>(m_MouseDevice);
	d3d::Release<IDirectInput8*>(m_pDirectInput);
}

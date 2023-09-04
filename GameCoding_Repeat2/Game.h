#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	void CreateDeviceAndSwapChain();

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	// DX
private:
	ComPtr<ID3D11Device>		_device			= nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext	= nullptr;
	ComPtr<IDXGISwapChain>		_swapChain		= nullptr;
};

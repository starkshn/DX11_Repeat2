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
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewPort();

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	// DX
private:
	ComPtr<ID3D11Device>			_device			= nullptr;
	ComPtr<ID3D11DeviceContext>		_deviceContext	= nullptr;
	ComPtr<IDXGISwapChain>			_swapChain		= nullptr;

private:
	ComPtr<ID3D11RenderTargetView>	_renderTargetView	= nullptr;
	D3D11_VIEWPORT					_viewport			= {0};
	float _clearColor[4]			= {0.2f, 0.2f, 0.8f, 0.f};
};

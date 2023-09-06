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
	void CreateGeometry();
	void CreateInputLayout();

	void CreateVS();
	void CreatePS();

private:
	void CreateSRV();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);


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
	float _clearColor[4]			= {1.f, 1.f, 1.f, 0.f};

private:
	vector<Vertex>					_vertices;
	ComPtr<ID3D11Buffer>			_vertexBuffer	= nullptr;
	ComPtr<ID3D11InputLayout>		_inputLayout	= nullptr;

	// VS
	ComPtr<ID3D11VertexShader>		_vertexShader	= nullptr;
	ComPtr<ID3DBlob>				_vsBlob			= nullptr;

	// PS
	ComPtr<ID3D11PixelShader>		_pixelShader	= nullptr;
	ComPtr<ID3DBlob>				_psBlob			= nullptr;

private:
	ComPtr<ID3D11ShaderResourceView> _shaderResouceView	= nullptr;
	ComPtr<ID3D11Buffer>			 _indexBuffer		= nullptr;
	vector<uint32>					 _indices;
};

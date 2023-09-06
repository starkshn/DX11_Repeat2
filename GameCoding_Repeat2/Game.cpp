#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd)
{
	_hWnd = hWnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();

	CreateRenderTargetView();
	SetViewPort();

	CreateGeometry();		// VertextBuffer	GPU에게 건내줌
	CreateVS();				// Vertex Shader	생성
	CreateInputLayout();	// InputLayout		생성
	CreatePS();				// Pixel Shader		생성

	CreateSRV();
}

void Game::Update()
{
}

void Game::Render()
{
	RenderBegin();

	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset); // Vertex Buffer
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0); // Index Buffer
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResouceView.GetAddressOf());

		// OM
		// _deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0); // 제출한다.
	C(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;					// 800
		desc.BufferDesc.Height = _height;				// 600
		desc.BufferDesc.RefreshRate.Numerator = 60;		// 화면 주사율
		desc.BufferDesc.RefreshRate.Denominator = 1;	// 분모
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 각각 8비트
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;		// 멀티 샘플링 개수
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;			// 후면 버퍼 개수
		desc.OutputWindow = _hWnd;		// 어떤 윈도우 핸들 설정?
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	C(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer
	(
		0, __uuidof(ID3D11Texture2D),
		(void**)backBuffer.GetAddressOf()
	);
	C(hr);

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	C(hr);
}

void Game::SetViewPort()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{

	// Vertex Data
	{
		// 13
		// 02
		_vertices.resize(4);
		
		_vertices[0].position	= Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].uv			= Vec2(0.f, 1.f);

		_vertices[1].position	= Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].uv			= Vec2(0.f, 0.f);

		_vertices[2].position	= Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].uv			= Vec2(1.f, 1.f);

		_vertices[3].position	= Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].uv			= Vec2(1.f, 0.f);

	}

	// Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc));
		desc.Usage		= D3D11_USAGE_IMMUTABLE;
		desc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth	= (uint32)sizeof(Vertex) * _vertices.size();

		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
	}

	// Index
	{
		_indices = {0, 1, 2, 2, 1, 3};
	}

	// Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _indices.data();
		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		C(hr);
	}
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader
	(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf()
	);
	C(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader
	(
		_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf()
	);
	C(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"메이플.png", WIC_FLAGS_NONE, &md, img);

	hr = ::CreateShaderResourceView
	(
		_device.Get(), img.GetImages(), img.GetImageCount(),
		md, _shaderResouceView.GetAddressOf()
	);
	C(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
	C(hr);
}

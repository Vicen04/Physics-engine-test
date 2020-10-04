#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "Camera.h"
#include <Windows.h>
#include "ParticleSystem.h"

#include "GameObject.h"

//using namespace DirectX;

class CollisionHandler;
class Movable;



struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	XMFLOAT3 LightVecW;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
	
	SurfaceInfo surface;

	Light light;

	XMFLOAT3 EyePosW;
	float HasTexture;
};

class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;

	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;

	ID3D11Buffer*           _pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;

	ID3D11Buffer*           _pConstantBuffer;

	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	ID3D11ShaderResourceView * _pGlassTextureRV = nullptr;
	ID3D11ShaderResourceView * _pStoneTextureRV = nullptr;
	ID3D11ShaderResourceView * _pMetalTextureRV = nullptr;
	ID3D11ShaderResourceView * _pSpongeTextureRV = nullptr;
	ID3D11ShaderResourceView * _pPlasticTextureRV = nullptr;
	ID3D11ShaderResourceView * _pWaterTextureRV = nullptr;

	ID3D11ShaderResourceView * _pGroundTextureRV = nullptr;

	ID3D11SamplerState * _pSamplerLinear = nullptr;

	Light basicLight;

	vector<std::shared_ptr<GameObject>> _gameObjects;

	std::shared_ptr<Movable> _player;
	std::shared_ptr<Movable> _player2;
	std::shared_ptr<ParticleSystem> _particles;
	bool controlBall = false;

	vector<XMFLOAT3> box, plane;

	std::unique_ptr <Camera> _camera;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 0.2f;
	float _cameraOrbitRadiusMax = 5000.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;
	float _presetFrameInterval = 0.06f;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();
	std::vector<XMFLOAT3> ballVertices;
	Geometry gBall;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);
	void Update();
	void HandleCollision(GameObject * object1, GameObject * object2, float time);
	bool HandleFloorCollision(GameObject * object1, GameObject * object2);
	void Draw();
};

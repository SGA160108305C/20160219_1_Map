#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Initialize(HWND handle)
{
	if (hasInitialized) return;//should be initialized only once.
	hasInitialized = true;

	windowHandle = handle;

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;	//하드웨어의 능력치
	int	vertexProcessing;
	direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dPP;		//화면 표시 파라메터
	ZeroMemory(&d3dPP, sizeof(D3DPRESENT_PARAMETERS));
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.Windowed = true;
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dPP.EnableAutoDepthStencil = true;
	d3dPP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		windowHandle,
		vertexProcessing,
		&d3dPP,
		&direct3dDevice);

	/*
	성공했을 경우는,D3D_OK 를 돌려준다.
	실패했을 경우는, 다음의 몇개의 값을 돌려준다.

	D3DERR_INVALIDCALL 메서드의 호출이 무효이다. 예를 들어, 메서드의 파라미터에 무효인 값이 설정되어 있는 경우 등이다.
	D3DERR_NOTAVAILABLE 이 장치는, 문의한 테크닉을 지원 하고 있지 않다.
	D3DERR_OUTOFVIDEOMEMORY Direct3D 가 처리를 실시하는데 충분한 디스플레이 메모리가 없다.

	*/
	if ( result != D3D_OK )
	{
		MessageBoxA(windowHandle, "CreateDevice Fail!!", "D3D Critical Error", MB_OK);
		return;
	}

	D3DXFONT_DESC font_desc;
	ZeroMemory(&font_desc, sizeof(D3DXFONT_DESC));
	font_desc.Height = 50;
	font_desc.Width = 25;
	font_desc.Weight = FW_NORMAL;
	font_desc.MipLevels = D3DX_DEFAULT;
	font_desc.Italic = false;
	font_desc.CharSet = DEFAULT_CHARSET;
	font_desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	font_desc.Quality = DEFAULT_QUALITY;
	font_desc.PitchAndFamily = FF_DONTCARE;
	wcscpy_s(font_desc.FaceName, L"휴먼편지체");	//글꼴 스타일
	//AddFontResource(L"./Data/Font/umberto.ttf");
	//wcscpy_s(font_desc.FaceName, L"umberto");

	D3DXCreateFontIndirect(
		GameManager::GetDevice(),
		&font_desc,
		&font);

	//조명 초기화 및 세팅
	ZeroMemory(&mainLight, sizeof(D3DLIGHT9));
	mainLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	mainLight.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	mainLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	mainLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	D3DXVECTOR3 direction(-1.0f, -2.0f, -1.0f);
	D3DXVec3Normalize(&direction, &direction);
	mainLight.Direction = direction;

	direct3dDevice->SetLight(0, &mainLight);
	direct3dDevice->LightEnable(0, true);

	GameManager::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		
	//카메라 생성
	camera = new Camera;
	camera->Initialize();

	//랜덤 초기화
	RandomUtil::Initialize();
	
	//GameState들 초기화
	GameStateManager::Get().Initialize();
	
}

void GameManager::Destroy()
{
	//카메라 제거
	SAFE_DELETE(camera);

	SAFE_RELEASE(font);

	//GameState들 정리
	GameStateManager::Get().Destroy();		
	
	//텍스쳐 정리
	TextureManager::Get().Destroy();

	//디바이스 릴리즈
	ULONG result = 0;
	result = SAFE_RELEASE(direct3dDevice);
#ifdef _DEBUG
	assert(result == 0 && "디바이스를 이용해 생성한 객체 중 해제되지 않은 객체가 있습니다.");
#endif

	SAFE_RELEASE(direct3d);
}

void GameManager::Loop(double tick)
{
	currentTickTime = tick;

	FPSTimeElapsed += currentTickTime;
	FPSFrameCount++;

	if (FPSTimeElapsed > 1.0f)
	{
		frameRate = FPSFrameCount;
		FPSFrameCount = 0;
		FPSTimeElapsed = 0.0f;
	}

	Update();
	Render();
}

void GameManager::Update()
{
		
	//카메라 업데이트
	if ( camera )
	{
		camera->Update();
	}	

	//GameState 업데이트
	GameStateManager::Get().Update();

}

void GameManager::Render()
{
	direct3dDevice->Clear(
		0, nullptr,	//화면전체 클리어
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 50),
		1.0f,	//z값 0~1. 1이면 가장 멀다
		0);		//스텐실 버퍼용 세팅

	//direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//와이어 프레임 모드로 그리기

	direct3dDevice->BeginScene();

	// 그림 그리기 -------------------------------------------------

	if (font)
	{
		RECT rc = { 10, 10, 11, 11 };
		char buff[256] = "\0";
		sprintf_s(buff, "FPS : %d", frameRate);
		font->DrawTextA(
			NULL,
			LPCSTR(buff),
			strlen(buff),
			&rc,
			DT_TOP | DT_LEFT | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 255));
	}

	//GameState 그리기
	GameStateManager::Get().Render();

	//--------------------------------------------------------------

	direct3dDevice->EndScene();

	direct3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT GameManager::GameInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//무엇이든 한 메세지에 대해 처리가 있었으면 그 다음은 무시
	LRESULT result = 0;
	if ( result == 0 && camera )
	{
		camera->CameraInputProc(hWnd, message, wParam, lParam);
	}
	if ( result == 0 )
	{
		result = GameStateManager::Get().InputProc(hWnd, message, wParam, lParam);
	}
	
	return result;
}
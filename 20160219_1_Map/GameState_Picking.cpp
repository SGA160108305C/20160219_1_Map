#include "stdafx.h"
#include "GameState_Picking.h"


GameState_Picking::GameState_Picking()
{
}


GameState_Picking::~GameState_Picking()
{
}

void GameState_Picking::Initialize()
{
	if ( grid == nullptr )
	{
		grid = new Grid;
		grid->Initialize();
	}	

	if ( objMap == nullptr )
	{
		objMap = new Obj();
		objMap->Initialize("build.obj", "build.obj");		
	}

	if ( cubeMan == nullptr )
	{
		cubeMan = new CubeMan_Walk;
		cubeMan->Initialize();
		cubeMan->SetPosition(D3DXVECTOR3(0, 0, 0));
		cubeMan->SetWalkingMap(objMap);
	}
	Reset();
}

void GameState_Picking::Destroy()
{
	SAFE_DELETE(cubeMan);
	SAFE_DELETE(objMap);
	SAFE_DELETE(grid);	
}

void GameState_Picking::Reset()
{
}

void GameState_Picking::Update()
{
	if ( cubeMan )
	{
		cubeMan->Update();
	}
}

void GameState_Picking::Render()
{
	if ( grid )
	{
		grid->Render();
	}

	if ( objMap )
	{
		objMap->Render();
	}

	if ( cubeMan )
	{
		cubeMan->Render();
	}
}

void GameState_Picking::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(5.0f);
	Initialize();
	GameManager::GetCamera()->SetLookTarget(cubeMan->GetPositionAdress());
	GameManager::GetCamera()->setCamRotY(cubeMan->getRotationAngleAddress());
}

void GameState_Picking::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_Picking::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	if ( result == 0 && cubeMan )
	{
		cubeMan->InputProc(hWnd, message, wParam, lParam);
	}
	return result;
}

#include "stdafx.h"
#include "GameState_OBJ_Load.h"


GameState_OBJ_Load::GameState_OBJ_Load()
{
}


GameState_OBJ_Load::~GameState_OBJ_Load()
{
}

void GameState_OBJ_Load::Initialize()
{
	if ( grid == nullptr )
	{
		grid = new Grid;
		grid->Initialize();
	}	

	if ( objMap == nullptr )
	{
		objMap = new Obj();
		objMap->Initialize("build.obj");		
	}
	Reset();
}

void GameState_OBJ_Load::Destroy()
{
	SAFE_DELETE(objMap);

	SAFE_DELETE(grid);	
}

void GameState_OBJ_Load::Reset()
{
}

void GameState_OBJ_Load::Update()
{

}

void GameState_OBJ_Load::Render()
{
	if ( grid )
	{
		grid->Render();
	}
	if ( objMap )
	{
		objMap->Render();
	}

}

void GameState_OBJ_Load::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(50.0f);
	Initialize();
}

void GameState_OBJ_Load::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_OBJ_Load::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	return result;
}

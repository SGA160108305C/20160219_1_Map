#include "stdafx.h"
#include "CubeMan_Walk.h"


CubeMan_Walk::CubeMan_Walk()
{
}


CubeMan_Walk::~CubeMan_Walk()
{
}

void CubeMan_Walk::Destroy()
{
	CubeMan_Light::Destroy();
	walkingMap = nullptr;
}

void CubeMan_Walk::Update()
{
	GameObject::Update();

	walkingMap->GroundCheck(position);
	
	float tick = (float)GameManager::GetTick();
	jumpTime += tick;

	if ((GetAsyncKeyState('A') & 0x8000) != 0)
	{
		rotationAngle -= (rotationSpeed * 0.015f);
	}
	else if ((GetAsyncKeyState('D') & 0x8000) != 0)
	{
		rotationAngle += (rotationSpeed * 0.015f);
	}

	if ((GetAsyncKeyState('W') & 0x8000) != 0)
	{
		position += (direction * moveSpeed * 0.015f);

		bool wasPressed = keyState[FORWARD];
		if (wasPressed == false)
		{
			//ó�� ������.
			AnimationOnOff(true);
		}
		keyState[FORWARD] = true;
	}
	else
	{
		bool wasPressed = keyState[FORWARD];
		if (wasPressed == true)
		{
			//Ű�� �� ���Ĵ�
			AnimationOnOff(false);
		}
		keyState[FORWARD] = false;
	}

	if ((GetAsyncKeyState('S') & 0x8000) != 0)
	{
		position -= (direction * moveSpeed * 0.015f);

		bool wasPressed = keyState[BACKWARD];
		if (wasPressed == false)
		{
			//ó�� ������.
			AnimationOnOff(true);
		}
		keyState[BACKWARD] = true;
	}
	else
	{
		bool wasPressed = keyState[BACKWARD];
		if (wasPressed == true)
		{
			//Ű�� �� ���Ĵ�
			AnimationOnOff(false);
		}
		keyState[BACKWARD] = false;
	}

	if ((GetAsyncKeyState('N') & 0x8000) != 0)
	{
		SetNextTexture();
	}
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0 && isJumping == false)
	{
		isJumping = true;
		jumpTime = 0.0f;
		jumpStart = position.y;
	}

	D3DXMATRIX scale, rotation, translation;
	D3DXMatrixScaling(&scale, scaleVector.x, scaleVector.y, scaleVector.z);
	D3DXMatrixRotationY(&rotation, rotationAngle);

	world = scale * rotation;

	D3DXVECTOR3 baseDirection(0, 0, 1);
	D3DXVec3TransformCoord(&direction, &baseDirection, &world);

	if (isJumping)
	{
		position.y = jumpStart + jumpPower * jumpTime - 4.9f/*0.5f * ( 9.8f ) */* jumpTime * jumpTime;

		if (position.y < jumpStart)
		{
			isJumping = false;
			jumpStart = position.y;
		}
	}

	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);
	world *= translation;

	root->Update(&world);
}

LRESULT CubeMan_Walk::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		//result = WM_LBUTTONDOWN;
	}
	break;
	case WM_LBUTTONUP:
	{
		//result = WM_LBUTTONUP;
	}
	break;
	case WM_MOUSEMOVE:
	{
		//result = WM_MOUSEMOVE;
	}
	break;
	}
	return result;
}
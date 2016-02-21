#include "stdafx.h"
#include "Obj.h"
#include "ObjLoader.h"

Obj::Obj()
{
}


Obj::~Obj()
{
}

void Obj::Initialize(char* mapFileName, char* groundFileName /*= nullptr*/)
{
	Destroy();
	ObjLoader::LoadObjFileData(objGroups, mapFileName);

	if (groundFileName)
	{
		ObjLoader::LoadObj_OnlyVertex(objGround, groundFileName);
	}
	printf_s("size: %d\n", objGround.size());
	D3DXMatrixIdentity(&world);

	D3DXMATRIXA16 scale, rotation, translation;

	D3DXMatrixScaling(&scale, 1, 1, 1);
	D3DXMatrixRotationY(&rotation, 0.0f);
	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);

	world = (scale * rotation * translation);
}

void Obj::Destroy()
{
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		delete *iter;
		*iter = nullptr;
	}
	objGroups.clear();
}

void Obj::Update()
{
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}

void Obj::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		( *iter )->Render();
	}
}


bool Obj::GroundCheck(IN OUT D3DXVECTOR3& groundPos) const
{
	bool find = false;
	D3DXVECTOR3 rayStart(groundPos.x, 1000.0f, groundPos.z);
	D3DXVECTOR3 rayDirection(0, -1, 0);
	for (size_t i = 0; i < objGround.size(); i += 3)
	{
		float u, v, distance;
		find = D3DXIntersectTri(
			&objGround[i],
			&objGround[i + 1],
			&objGround[i + 2],
			&rayStart,
			&rayDirection,
			&u, &v,
			&distance) != 0;
		if (find == true)
		{
			if (groundPos.y > distance)
			{
				printf_s("distance -> %.2f\n", distance);
				groundPos.y = 1000.0f - distance;
			}

			printf_s("positionY: %.2f / distance -> %.2f\n", groundPos.y, distance);
			//groundPos = objGround[i] + ( ( objGround[i + 1] - objGround[i] ) * u ) + ( ( objGround[i + 2] - objGround[i] ) * v );
			break;
		}
	}
	return find;
}
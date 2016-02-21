#pragma once
#include "ObjFileGroup.h"


class Obj
{
public:
	Obj();
	~Obj();

	void Initialize(char* mapFileName, char* groundFileName = nullptr);
	void Destroy();
	void Update();
	void Render();

	bool GroundCheck(IN OUT D3DXVECTOR3& groundPos) const;
	
	inline void SetPosition(D3DXVECTOR3& pos){ position = pos; }

private:
	
	D3DXMATRIX world;
	D3DXMATRIX rotation;

	D3DXVECTOR3 position = D3DXVECTOR3(-13.4f, 12.2f, 12.5f);

	std::vector<ObjFileGroup*> objGroups;
	std::vector<D3DXVECTOR3> objGround;
};


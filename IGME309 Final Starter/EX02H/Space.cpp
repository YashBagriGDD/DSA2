#include "Space.h"
using namespace Simplex;

uint Simplex::Space::m_uMaxLevel = 3;

void Simplex::Space::Resize(void)
{
	Release();

	// Calculate a box that surrounds all object's ARBB
	vector3 m_v3Max = ZERO_V3;
	vector3 m_v3Min = ZERO_V3;
	
	int eCount = m_pEntityMngr->GetEntityCount();

	//iterate through all the entities to get the max and min position
	for (int i = 0; i < eCount; i++)
	{
		MyEntity* e = m_pEntityMngr->GetEntity(i);
		vector3 pos = e->GetPosition();

		//Calculate the max and min by comparing each entity's position to them
		if (m_v3Max.x < pos.x) m_v3Max.x = pos.x;
		else if (m_v3Min.x > pos.x) m_v3Min.x = pos.x;

		if (m_v3Max.y < pos.y) m_v3Max.y = pos.y;
		else if (m_v3Min.y > pos.y) m_v3Min.y = pos.y;

		if (m_v3Max.z < pos.z) m_v3Max.z = pos.z;
		else if (m_v3Min.z > pos.z) m_v3Min.z = pos.z;
	}

	//set the size and center
	m_v3Size = (m_v3Max - m_v3Min) / 4;
	m_v3Center = (m_v3Max + m_v3Min) / 2;

	Subdivide();
}
void Simplex::Space::Subdivide()
{
	if (m_uLevel >= m_uMaxLevel)
		return;

	//Create the four children of the current space in the right place and size and subdivide 
	//if the maximum level has not being reach yet
	vector3 newSize = m_v3Size / 2;

	vector3 point = m_v3Center;
	point.x -= newSize.x;
	point.y -= newSize.y;

	//Sets the point to (min, min)
	m_pChild[0] = new Space(point, newSize*2);

	//(max, min)
	point.x += newSize.x * 2;
	m_pChild[1] = new Space(point, newSize*2);

	//(max, max)
	point.y += newSize.y * 2;
	m_pChild[2] = new Space(point, newSize*2);

	//(min, max)
	point.x -= newSize.x * 2;
	m_pChild[3] = new Space(point, newSize*2);

	for (int i = 0; i < 4; i++) {
		m_pChild[i]->m_uLevel = m_uLevel+1;
		m_pChild[i]->Subdivide();
	}
}

void Simplex::Space::Init(void)
{
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	m_v3Size = vector3(10.0f);
	m_v3Center = vector3(10.0f, 0.0f, 0.0f);
	m_uLevel = 0;

	for (uint i = 0; i < 4; i++)
	{
		m_pChild[i] = nullptr;
	}

}
Simplex::Space::~Space(void)
{
	Release();
}
Simplex::Space::Space(uint a_uMaxLevel)
{
	Init();
	m_uMaxLevel = a_uMaxLevel;
}
Simplex::Space::Space(vector3 a_v3Center, vector3 a_v3Size)
{
	Init();
	m_v3Center = a_v3Center;
	m_v3Size = a_v3Size;
}



void Simplex::Space::Display(void)
{
	for (uint i = 0; i < 4; i++)
	{
		if (m_pChild[i])
		{
			m_pChild[i]->Display();
		}
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(m_v3Size), C_RED);
}



void Simplex::Space::Release()
{
	for (uint i = 0; i < 4; i++)
		if (m_pChild[i])
			m_pChild[i]->Release();

	for (uint i = 0; i < 4; i++)
		SafeDelete(m_pChild[i]);
}
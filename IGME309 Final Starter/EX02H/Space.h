/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2018/12
----------------------------------------------*/
#ifndef __SPACE_H_
#define __SPACE_H_

#include "Definitions.h"
#include "MyEntityManager.h"

namespace Simplex
{
class Space
{
private:
	vector3 m_v3Center = vector3(0.0f); //Center of the Space
	vector3 m_v3Size = vector3(0.0f); //Size of the Space
	MyEntityManager* m_pEntityMngr = nullptr;
	MeshManager* m_pMeshMngr = nullptr;
	Space* m_pChild[4];
	uint m_uLevel = 0;
	static uint m_uMaxLevel;

public:
	void Init(void);
	Space(uint a_iMaxLevel); //Constructor
	Space(vector3 a_v3Center, vector3 a_v3Size); //Constructor
	~Space(void);
	void Display(void);
	void Resize(void);
	void Release(void);
	void Subdivide(void);
};
}
#endif //__SPACE_H_
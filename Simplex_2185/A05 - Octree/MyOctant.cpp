#include "MyOctant.h"
using namespace Simplex;

//static vars
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;

#pragma region Rule_Of_3
//Makes the root
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount) {
	Init();

	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uOctantCount = 0;
	m_uID = m_uOctantCount;
	m_pRoot = this;
	m_lChild.clear();
	std::vector<vector3> points;
	int num = m_pEntityMngr->GetEntityCount();

	for (int i = 0; i < num; i++) {
		MyEntity* e = m_pEntityMngr->GetEntity(i);
		MyRigidBody* rb = e->GetRigidBody();
		points.push_back(rb->GetMinGlobal());
		points.push_back(rb->GetMaxGlobal());
	}

	MyRigidBody* rb = new MyRigidBody(points);

	//get the min and max
	vector3 halfWidth = rb->GetHalfWidth();
	vector3 center = rb->GetCenterLocal();
	float dist = halfWidth.x;
	points.clear();
	SafeDelete(rb);

	for (int i = 1; i < 3; i++) {
		if (dist < halfWidth[i]) {
			dist = halfWidth[i];
		}
	}
	
	m_v3Max = m_v3Center + vector3(dist);
	m_v3Min = m_v3Center - vector3(dist);

	//set size
	m_fSize = dist * 2;

	//increment count
	m_uOctantCount++;

	//make the tree
	ConstructTree(m_uMaxLevel);
}

//constructor for branches
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) {
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	//adjust min and max
	m_v3Min = m_v3Center - vector3(m_fSize / 2.0f);
	m_v3Max = m_v3Center + vector3(m_fSize / 2.0f);

	m_uOctantCount++;
}

//Copy constructor
Simplex::MyOctant::MyOctant(MyOctant const & other) {
	m_uID = other.m_uID;
	m_pRoot = other.m_pRoot;
	m_pParent = other.m_pParent;
	m_uChildren = other.m_uChildren;
	m_uLevel = other.m_uLevel;
	m_fSize = other.m_fSize;
	m_v3Center = other.m_v3Center;
	m_v3Max = other.m_v3Max;
	m_v3Min = other.m_v3Min;
	m_lChild = other.m_lChild;

	for (uint i = 0; i < 8; i++)
		m_pChild[i] = other.m_pChild[i];

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

//Using the copy assignment operator (=)
MyOctant & Simplex::MyOctant::operator=(MyOctant const & other) {
	if (this != &other) {
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

Simplex::MyOctant::~MyOctant(void) {
	Release();
}
#pragma endregion

//Swaps the data of two Octants
void Simplex::MyOctant::Swap(MyOctant & other) {
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Min, other.m_v3Min);

	for (int i = 0; i < 8; i++)
		std::swap(m_pChild[i], other.m_pChild[i]);
}

#pragma region Accessors
float Simplex::MyOctant::GetSize(void) { return m_fSize; }
vector3 Simplex::MyOctant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Simplex::MyOctant::GetMinGlobal(void) { return m_v3Min; }
vector3 Simplex::MyOctant::GetMaxGlobal(void) { return m_v3Max; }
uint Simplex::MyOctant::GetOctantCount(void) { return m_uOctantCount; }
MyOctant * Simplex::MyOctant::GetParent(void) { return m_pParent; }
MyOctant * Simplex::MyOctant::GetChild(uint a_nChild) {
	if (a_nChild > 7)
		return nullptr;

	return m_pChild[a_nChild];
}
//Determines if this octant is a leaf. It's a leaf if it has no children.
bool Simplex::MyOctant::IsLeaf(void) { return m_uChildren == 0; }
#pragma endregion

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex) {
	int num = m_pEntityMngr->GetEntityCount();

	if (a_uRBIndex >= num)
		return false;

	//get data of entity
	MyEntity* e = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* rb = e->GetRigidBody();
	vector3 min = rb->GetMinGlobal();
	vector3 max = rb->GetMaxGlobal();

	//Check collisions
	if (min.x > m_v3Max.x)
		return false;
	if (max.x < m_v3Min.x)
		return false;
	if (min.y > m_v3Max.y)
		return false;
	if (max.y < m_v3Min.y)
		return false;
	if (min.z > m_v3Max.z)
		return false;
	if (max.z < m_v3Min.z)
		return false;

	//It is colliding
	return true;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color) {
	if (m_uID == a_nIndex) {
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}
	for (int i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_nIndex);
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color) {
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->Display(a_v3Color);

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color) {
	int numLeaf = m_lChild.size();
	for (int i = 0; i < numLeaf; i++)
		m_lChild[i]->DisplayLeafs(a_v3Color);

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void) {
	//Clear this list and the list on every child
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->ClearEntityList();

	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void) {
	//Cannot subdivivde if its over the max level
	if (m_uLevel >= m_uMaxLevel)
		return;

	if (m_uChildren != 0) //If it has children its already been divided
		return;

	m_uChildren = 8;

	float newSize = m_fSize / 4.0f; //divides into 1/4th of the previous size

	vector3 point = m_v3Center;
	point.x -= newSize;
	point.y -= newSize;
	point.z -= newSize;
	//Sets the point to (min, min, min)
	m_pChild[0] = new MyOctant(point, newSize * 2);

	//(min, min, max)
	point.z += newSize * 2;
	m_pChild[1] = new MyOctant(point, newSize * 2);

	//(max, min, max)
	point.x += newSize * 2;
	m_pChild[2] = new MyOctant(point, newSize * 2);

	//(max, min, min)
	point.z -= newSize * 2;
	m_pChild[3] = new MyOctant(point, newSize * 2);

	//(max, max, min)
	point.y += newSize * 2;
	m_pChild[4] = new MyOctant(point, newSize * 2);

	//(min, max, min)
	point.x -= newSize * 2;
	m_pChild[5] = new MyOctant(point, newSize * 2);

	//(min, max, max)
	point.z += newSize * 2;
	m_pChild[6] = new MyOctant(point, newSize * 2);

	//(max, max, max)
	point.x += newSize * 2;
	m_pChild[7] = new MyOctant(point, newSize * 2);

	for (int i = 0; i < 8; i++) {
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
}


bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities) {
	int count = 0;
	int objectCount = m_pEntityMngr->GetEntityCount();

	for (int i = 0; i < objectCount; i++) {
		if (IsColliding(i))
			count++;
		if (count > a_nEntities) //if the count exceeds the number we're checking return true
			return true;
	}
	return false;
}

void Simplex::MyOctant::KillBranches(void) {
	//delete pointers to the children
	for (int i = 0; i < m_uChildren; i++) {
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}

	m_uChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel) {
	//Only allow the root to construct the tree
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	//Clear all the current lists
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

	//If it had more than the ideal count, subdivide it until 
	if (ContainsMoreThan(m_uIdealEntityCount))
		Subdivide();

	AssignIDtoEntity();
	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void) {
	//recursive loop to assign id to all children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->AssignIDtoEntity();

	//If this is a leaf check for entities within it
	if (IsLeaf()) {
		int num = m_pEntityMngr->GetEntityCount();
		for (int i = 0; i < num; i++) {
			if (IsColliding(i)) {
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

//Releases data from the octree
void Simplex::MyOctant::Release(void) {
	//Clear everything from the root and reset values to their default
	if (m_uLevel == 0) {
		KillBranches();
	}

	m_fSize = 0.0f;
	m_lChild.clear();
	m_EntityList.clear();
	m_uChildren = 0;
}

//Initializes the default values for the octant
void Simplex::MyOctant::Init(void) {
	m_uID = m_uOctantCount;
	m_pRoot = nullptr;
	m_pParent = nullptr;
	m_uChildren = 0;
	m_uLevel = 0;
	m_fSize = 0.0f;
	m_v3Center = ZERO_V3;
	m_v3Max = ZERO_V3;
	m_v3Min = ZERO_V3;

	for (uint i = 0; i < 8; i++)
		m_pChild[i] = nullptr;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

void Simplex::MyOctant::ConstructList(void) {

	//Recursive loop to call ConstructList on every child
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ConstructList();
	}

	//If the child contains objects then add it to the list of child "branches"
	if (m_EntityList.size() > 0) {
		m_pRoot->m_lChild.push_back(this);
	}
}

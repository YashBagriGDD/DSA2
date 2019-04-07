#pragma once
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace std;
using namespace Simplex;

class Octant
{
private:
	//Singleton references
	MeshManager* m_meshMngr = nullptr;
	MyEntityManager* m_entityMngr = nullptr; 

	//vars
	int level = 1;
	int subdivisionLimit = 8;
	vector3 max;
	vector3 min;
	vector3 halfWidth = vector3(35.0f);
	vector3 center = vector3(0.0f);
	bool isSubdivided;
	vector<Octant*> subdivisions;
	vector<int> entityIndexes;

public:
	//constructors/destructors
	Octant(int level, vector3 halfWidth, vector3 center, int subdivisionLimit);
	~Octant();

	//Accessors
	int GetSubdivisionLimit();
	int GetLevel;
	vector3 GetHalfWidth();
	vector3 GetCenter();
	bool GetIsSubdivided();
	void SetIsSubdivided(bool b_subdivided);
	int GetEntityCount();
	vector<Octant*> GetSubdivisions();
	vector<int> GetEntityIndexes();

	//functions
	bool IsEntityColliding(int entityIndex);
	void IsColliding();
	void AddSubdivision(Octant* oct);
	void AddEntityIndex(int entityIndex);
	void Display(vector3 v3color);
};


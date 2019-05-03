#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 50.0f, 150.0f), //Position
		vector3(0.0f, 50.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

															 
	m_pEntityMngr = MyEntityManager::GetInstance(); //Initialize the entity manager
	m_pEntityMngr->AddEntity("Sorted\\Polycube.obj", "Cube");
	m_pEntityMngr->UsePhysicsSolver();

	for (int i = 0; i < 100; i++)
	{
		m_pEntityMngr->AddEntity("Sorted\\IronMan.obj", "IronMan_" + std::to_string(i));
		vector2 temp = glm::circularRand(12.0f);
		vector3 v3Position = vector3(0.0f, 12.0f, 0.0f) + vector3(temp.x, temp.y, 0.0f);
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(1.0f)) * glm::rotate(IDENTITY_M4, glm::radians(45.0f), AXIS_Y));
		m_pEntityMngr->UsePhysicsSolver(true);
		m_pEntityMngr->SetMass(1);
	}

	m_pSpace = new Space(3);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	static float fIncrement = 0.0f;
	for (int i = 1; i < 101; i++)
	{
		matrix4 m4Model = m_pEntityMngr->GetModelMatrix(i);
		m4Model = m4Model * glm::rotate(IDENTITY_M4, glm::radians(5.0f), AXIS_Y);
		m_pEntityMngr->SetModelMatrix(m4Model, i);
	}
	fIncrement += 1.0f;

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	//Resize the space
	m_pSpace->Resize();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//display the space
	m_pSpace->Display();
		
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();
	
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}

void Application::Release(void)
{
	SafeDelete(m_pSpace);

	//release GUI
	ShutdownGUI();
}
#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Model = IDENTITY_M4;

	//m4Model[3][2] = 1.0f; // by value
	/*
	vector4 v4Trans = m4Model[3];
	v4Trans = vector4(1.0f);
	m4Model[3] = v4Trans; //by column
	*/
	//m4Model = glm::translate(IDENTITY_M4, vector3(1.0f)); //using glm library

	matrix4 m4Trans = glm::translate(IDENTITY_M4, vector3(1.0f, 0.0f, 0.0f)); //using glm library
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f)); //using glm library

	m4Model = m4Trans * m4Scale; //applies both in order. ORDER MATTERS

	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	m_pMeshMngr->AddGridToRenderList();
	
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
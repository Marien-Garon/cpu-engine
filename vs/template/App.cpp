#include "pch.h"
#include <iostream>


App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// YOUR CODE HERE
	//m_EarthTexture.Load("earth.png");
	//m_EarthMaterial.pTexture = &m_EarthTexture;
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	track_mesh.CreateTube(0.1f, 4.f, 32);
	center_mesh.CreateSphere(0.5f, 16, 16);
	floor_mesh.CreateCylinder(0.001f, 6.f, 6, true, false);

	//m_pEarth = cpuEngine.CreateEntity();
	//m_pEarth->pMesh = &circle_mesh;
	//m_pEarth->pMaterial = &m_EarthMaterial;
	//m_pEarth->transform.pos.x = 3.0f;
	//m_pEarth->transform.pos.y = 3.0f;
	//m_pEarth->transform.pos.z = 5.0f;

	track_material.color = cpu::ToColor(200, 20, 60);
	basic_material.color = cpu::ToColor(200, 200, 200);

	center = cpuEngine.CreateEntity();
#ifdef _DEBUG
	center->pMesh = &center_mesh;
	center->pMaterial = &basic_material;
#endif // _DEBUG
	center->transform.pos.y = 0.f;
	center->transform.pos.x = 0.f;
	center->transform.pos.z = 0.f;

	track = cpuEngine.CreateEntity();
	track->pMesh = &track_mesh;
	track->pMaterial = &track_material;
	track->transform.pos.x = 0.f;
	track->transform.pos.y = -1.f;
	track->transform.pos.z = 0.f;
	track->transform.AddYPR(0.f, 0.f, 0.f);

	floor = cpuEngine.CreateEntity();
	floor->pMesh = &floor_mesh;
	floor->pMaterial = &basic_material;
	floor->transform.pos.x = 0.f;
	floor->transform.pos.y = -2.f;
	floor->transform.pos.z = 0.f;
}

void App::OnUpdate()
{
	// YOUR CODE NOT HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	cpu_camera* camera = cpuEngine.GetCamera();
	cpu_ray ray;
	cpuEngine.GetCursorRay(ray);

	if (cpuInput.IsRight())
		m_speed = -2.f;
	else if (cpuInput.IsLeft())
		m_speed = 2.f;
	else
		m_speed = 0.f;

	m_angle += m_speed * dt;

	//circle->transform.LookAt(cpuEngine.GetCamera()->transform.pos.x, cpuEngine.GetCamera()->transform.pos.y, cpuEngine.GetCamera()->transform.pos.z, CPU_VEC3_RIGHT);
	cpuEngine.GetCamera()->transform.OrbitAroundAxis(center->transform.pos, CPU_VEC3_UP, 5.f, m_angle);
	cpuEngine.GetCamera()->transform.LookAt(ray.dir.x * 5, ray.dir.y * 5, ray.dir.z * 5, CPU_VEC3_UP);

}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE

		switch (pass)
		{
		case CPU_PASS_PARTICLE_BEGIN:
		{
			// Blur particles
			//cpuEngine.SetRT(m_rts[0]);
			//cpuEngine.ClearColor();
			break;
		}
		case CPU_PASS_PARTICLE_END:
		{
			// Blur particles
			//cpuEngine.Blur(10);
			//cpuEngine.SetMainRT();
			//cpuEngine.AlphaBlend(m_rts[0]);
			break;
		}
		case CPU_PASS_UI_END:
		{
			// Debug
			cpu_stats& stats = *cpuEngine.GetStats();
			std::string info = CPU_STR(cpuTime.fps) + " fps, ";
			info += "Camera pos :\nx = " + CPU_STR(cpuEngine.GetCamera()->transform.pos.x);
			info += "\ny = " + CPU_STR(cpuEngine.GetCamera()->transform.pos.y);
			info += "\nz = " + CPU_STR(cpuEngine.GetCamera()->transform.pos.z);

			XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };
			cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);
			break;
		}
	}
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}

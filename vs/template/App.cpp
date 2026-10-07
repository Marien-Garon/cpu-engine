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

void App::CreateObstacle()
{

}

void App::OnStart()
{
	// YOUR CODE HERE

	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	track_mesh.CreateTube(0.1f, 5.f, 32);
	//track_mesh.CreateCircle(1.f, 16.f);
	center_mesh.CreateSphere(0.5f, 16, 16);
	floor_mesh.CreateCylinder(0.001f, 6.f, 6, true, false);

	player_mesh.CreateCube(0.5f);


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

	player = cpuEngine.CreateEntity();
	player->pMesh = &player_mesh;
	player->pMaterial = &basic_material;
	player->transform.pos.x = 0.f;
	player->transform.pos.y = 0.f;
	player->transform.pos.z = 0.f;

	cpuEngine.GetCamera()->transform.pos.x = 6.f;
	cpuEngine.GetCamera()->transform.pos.y = 10.f;
	cpuEngine.GetCamera()->transform.pos.z = 5.f;
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
	player->transform.OrbitAroundAxis(center->transform.pos, CPU_VEC3_UP, 5.f, m_angle);
	player->transform.LookAt(center->transform.pos.x, center->transform.pos.y, center->transform.pos.z, CPU_VEC3_UP);
	//cpuEngine.GetCamera()->transform.LookAt(ray.dir.x / 100000000, ray.dir.y / 10000000, ray.dir.z / 10000000, CPU_VEC3_UP);
	cpuEngine.GetCamera()->transform.LookAt(0.f, 0.f, 1.f, CPU_VEC3_UP);
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

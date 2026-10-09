#include "pch.h"
#include <iostream>
#include <random>

template<typename T>
T GetRandomNumber(T min, T max)
{
	static std::random_device rd;

	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

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
	cpu_entity* obs = cpuEngine.CreateEntity();
	obs->pMesh = &obstacle_mesh;
	obs->pMaterial = &basic_material;

	float a = GetRandomNumber(0.f, XM_2PI);

	obs->transform.pos.y = 10.f;
	obs->transform.pos.x = cos(a) * spawn_radius;
	obs->transform.pos.z = sin(a) * spawn_radius;
	obs->transform.dir.x = 0.f;
	obs->transform.dir.y = -1.f;
	obs->transform.dir.z = 0.f;
	obstacle_list.push_back(obs);
}

void App::OnStart()
{
	// YOUR CODE HERE

	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	track_mesh.CreateTube(0.1f, 5.f, 32);
	center_mesh.CreateSphere(0.5f, 16, 16);
	obstacle_mesh.CreateSphere();
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
	
	game_timer -= dt;

	spawn_timer -= dt;
	if (spawn_timer < 0.f)
	{
		spawn_timer = 10.f;
		CreateObstacle();
	}


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

	for (auto it = obstacle_list.begin(); it != obstacle_list.end(); ++it)
	{	
		cpu_entity* obs = *it;
		obs->transform.Move(dt * 2.f);
		if (obs->lifetime > 5.f)
		{
			cpuEngine.Release(obs);
			game_timer -= 10.f;
		}
		if (player->aabb.Contains(obs->transform.pos))
		{
			cpuEngine.Release(obs);
			score += 1;
		}
	}

	for (auto it = obstacle_list.begin(); it != obstacle_list.end();)
	{
		if ((*it)->dead)
			it = obstacle_list.erase(it);
		else
			++it;
	}

	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();
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
			info += "Score : " + CPU_STR(score);
			info += "\nTimer : " + CPU_STR(static_cast<int>(game_timer)) + "s";

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

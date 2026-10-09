#pragma once

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void CreateObstacle();

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

private:

	// Leeeeeeeeeeeeeeee
	cpu_mesh track_mesh;
	cpu_mesh center_mesh;
	cpu_mesh floor_mesh;
	cpu_mesh player_mesh;
	cpu_mesh obstacle_mesh;
	cpu_material track_material;
	cpu_material basic_material;
	cpu_entity* track = nullptr;

	cpu_entity* center = nullptr;
	cpu_entity* floor = nullptr;
	cpu_entity* player = nullptr;

	std::vector<cpu_entity*> obstacle_list;

	cpu_font m_font;

	float m_angle = 0.f;
	float m_speed = 0.f;
	float spawn_radius = 5.f;
	float spawn_timer = 0.f;
	float game_timer = 300.f;
	int score = 0;

	inline static App* s_pApp = nullptr;
};

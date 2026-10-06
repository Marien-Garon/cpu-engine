#pragma once

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

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
	cpu_material track_material;
	cpu_material basic_material;
	cpu_entity* track = nullptr;

	cpu_entity* center = nullptr;
	cpu_entity* floor = nullptr;

	cpu_font m_font;

	float m_angle = 0.f;
	float m_speed = 0.f;

	inline static App* s_pApp = nullptr;
};

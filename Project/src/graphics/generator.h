#pragma once
#include "noise_texture.h"
#include "framebuffer.h"
#include "eroder.h"

constexpr int s_select_noise_map = 0;
constexpr int s_apply_layers = 1;
constexpr int s_rasterization = 2;

class Shader_Program;
struct generator
{
	// Noiser
	noise_texture m_noise;

	// Layer
	raw_mesh m_layered_mesh;
	raw_mesh m_rasterized_mesh;
	raw_texture_rgb m_rasterized_texture;
	float m_blend_factor{ 0.75f };
	float m_terrain_slope{ 0.75f };
	float m_water_height{ 0.05f };

	// Water
	float m_reflect_factor{ 0.55f };
	float m_wcolor_factor{ 0.45f };
	vec3 m_wcolor{ 0.12f, 0.45f, 0.50f };
	framebuffer m_reflection;
	framebuffer m_refraction;

	// Eroder
	eroder m_eroder;

	struct level
	{
		vec3 color;
		float txt_height;
		float real_height;
	};
	std::vector<level> levels
	{
		{ vec3{ 0.941f, 0.941f, 0.471f }, 0.4f, 0.075f },
		{ vec3{ 0.118f, 0.471f, 0.235f }, 0.7f, 0.1f },
		{ vec3{ 0.275f, 0.234f, 0.078f }, 0.9f, 0.1f },
		{ vec3{ 0.196f, 0.196f, 0.196f }, 1.0f, 0.05f }
	};

	void init();
	void update();
	enum class e_shader {e_layer_color, e_layer_mesh, e_water};
	void set_uniforms(Shader_Program* shader_p, e_shader shader_type);
	void draw_gui();

	int step{ -1 };
private:
	void rasterize_mesh();

	void enter_step();
	void exit_step();
	void change_step(int next_step);
};
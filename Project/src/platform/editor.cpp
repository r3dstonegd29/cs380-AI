
#include "editor.h"
#include "window_manager.h"
#include "window.h"
#include <graphics/renderer.h>
#include <utils/generate_noise.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

c_editor* editor = new c_editor;

bool c_editor::init()
{
	// Start ImGui
	ImGui::CreateContext();
	if (!ImGui_ImplGlfw_InitForOpenGL(window_manager->m_window->m_window, true))
		return false;
	const char* glsl_version = "#version 440";
	if(!ImGui_ImplOpenGL3_Init(glsl_version))
		return false;

	// Set Own Style
	ImGui::StyleColorsLight();
	ImGui::GetStyle().FrameRounding = 12;
	ImGui::GetStyle().Colors[2] = ImVec4{ 1.0f, 1.0f, 1.0f, 0.6f };
	ImGui::GetStyle().Colors[10] = ImVec4{ 0.8f, 0.0f, 0.2f, 1.0f };
	ImGui::GetStyle().Colors[11] = ImVec4{ 0.8f, 0.0f, 0.2f, 1.0f };

	return true;
}

void c_editor::update()
{
	// Draw ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	draw_main_window();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void c_editor::shutdown()
{
	// Exit ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void c_editor::draw_main_window()
{
	ImGui::Begin("cs350 - editor", nullptr, ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2{ 0.0f, 0.0f });

	if (ImGui::TreeNode("Camera"))
	{
		ImGui::InputFloat3("Eye", &renderer->scene_cam.m_eye.x);
		ImGui::InputFloat3("Front", &renderer->scene_cam.m_front.x);
		ImGui::InputFloat3("Right", &renderer->scene_cam.m_right.x);
		ImGui::InputFloat3("Up", &renderer->scene_cam.m_up.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Noise"))
	{
		bool changed = false;
		int resolution = static_cast<int>(renderer->m_noise.resolution);
		if (ImGui::InputInt("Resolution", &resolution))
			renderer->m_noise.resolution = static_cast<size_t>(resolution), changed = true;
		if(ImGui::SliderFloat("Scale", &renderer->m_noise.scale, 0.01f, 5.0f))changed = true;
		if(ImGui::InputInt("Iterations", &renderer->m_noise.iterations))changed = true;
		if(ImGui::SliderFloat("Persistance", &renderer->m_noise.persistance, 0.01f, 5.0f))changed = true;
		if (ImGui::SliderFloat("Lacunarity", &renderer->m_noise.lacunarity, 0.01f, 5.0f))changed = true;
		if(ImGui::SliderFloat("Complexity", &renderer->m_noise.lacunarity, 0.01f, 5.0f))renderer->m_noise.persistance = 1/renderer->m_noise.lacunarity, changed = true;
		if (ImGui::SliderFloat("FallOff", &renderer->m_noise.falloff, 0.0f, 1.0f))changed = true;
		if (ImGui::Button("Random"))
			changed = true, randomize_noise();
		if (changed)
			renderer->m_noise.update();
		ImGui::SliderFloat("BlendFactor", &renderer->m_noise.blend_factor, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}
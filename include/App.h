#pragma once 
#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct CreationParams
{
	//CreationParams(const CreationParams& other) : width{ other.width }, height{ other.height }, window_name{ name } {}
	//CreationParams(uint32_t w, uint32_t h, std::string_view name) : width{ w }, height{ h }, window_name{ name } {}
		
	uint32_t width, height;

	std::string window_name;
};

class App
{
public:
	App(const CreationParams& params);
	virtual void Run() = 0;
	virtual ~App() = default;
	
private:
	bool Init();
	bool GladInit();
	bool GlfwInit();
	bool OpenGLInit();
	bool ImguiInit();
	virtual void OnKeyPress(int key, int scancode, int action, int mods) {}
	virtual void OnMouseMove(double xpos, double ypos) {}
protected:
	bool ShouldRun() const;
	void BeginScene();
	void EndScene();
protected:
	CreationParams m_params;
	GLFWwindow* m_window;
private:
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};
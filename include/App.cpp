#include "App.h"

App::App(const CreationParams& params) :  m_params { params }
{
	if (!Init())
	{
		abort();
	}
}

bool App::Init()
{
	bool glfw = GlfwInit();
	bool glad = GladInit();
	glViewport(0, 0, m_params.width, m_params.height);
	
	return glfw && glad;
}

bool App::GladInit()
{
	bool loaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return loaded;
}

bool App::GlfwInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_params.width, m_params.height, m_params.window_name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	auto resize_callback = [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	};


	return m_window != nullptr;
}

bool App::ShouldRun() const
{
	return !glfwWindowShouldClose(m_window);
}

void App::EndScene()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

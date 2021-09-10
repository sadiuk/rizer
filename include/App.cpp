#include "App.h"
#include <iostream>
#include <chrono>
#include <thread>
App::App(const CreationParams& params) : m_params{ params }
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5000ms);
	if (!Init())
	{
		abort();
	}
}

bool App::Init()
{
	bool glfw = GlfwInit();
	bool glad = GladInit();
	bool opengl = OpenGLInit();
	bool imgui = ImguiInit();
	glViewport(0, 0, m_params.width, m_params.height);

	return glfw && glad && opengl && imgui;
}

bool App::GladInit()
{
	bool loaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return loaded;
}

void App::resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};
void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	app->OnKeyPress(key, scancode, action, mods);
}
void App::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	App* app = (App*)glfwGetWindowUserPointer(window);
	app->OnMouseMove(xpos, ypos);
}

bool App::GlfwInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_params.width, m_params.height, m_params.window_name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSwapInterval(0);

	return m_window != nullptr;
}

bool App::OpenGLInit()
{
	// not my code XD
	static auto opengl_debug_callback = [](GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		const char* _source;
		const char* _type;
		const char* _severity;

 		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}

		std::cout << "a";
		printf("%d: %s of %s severity, raised from %s: %s\n",
			id, _type, _severity, _source, message);
	};
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(opengl_debug_callback, nullptr);
	return true;
}

bool App::ImguiInit()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	bool ok = true;
	ok |= ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	const char* glsl_version = "#version 450";
	ok |= ImGui_ImplOpenGL3_Init(glsl_version);
	return ok;
}

bool App::ShouldRun() const
{
	return !glfwWindowShouldClose(m_window);
}

void App::BeginScene()
{
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void App::EndScene()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
}


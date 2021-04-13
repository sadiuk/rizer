#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
protected:
	bool ShouldRun() const;
	void EndScene();
protected:
	CreationParams m_params;
	GLFWwindow* m_window;
};
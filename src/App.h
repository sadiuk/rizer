#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct CreationParams
{
	CreationParams(const CreationParams& other) {}
	union
	{
		struct
		{
			int	width, height;
		};
		glm::uvec2 size;
	};
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
protected:
	bool ShouldRun() const;
protected:
	CreationParams m_params;
	GLFWwindow* m_window;
};
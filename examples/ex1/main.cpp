#include <App.h>
#include <../src/fs/FileManager.h>
#include "../src/Rasterizer.h"
#include "../src/OpenGL/Framebuffer.h"
#include "../../src/fs/OBJLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../src/Debug.h"
#include "../src/FullScreenRenderer.h"

#include <wtypes.h>
#include <iostream>
#include <functional>


class RasterizerApp : App
{
	bool m_should_run = true;

	double m_prev_mouse_x, m_prev_mouse_y;
	float m_mouse_sensitivity = 0.1f;
	float m_movement_speed = 1;

	float m_yaw = 0, m_pitch = 0;
	bool m_reset_mouse_pos = true;


	glm::vec3 forward{ 0, 0, -1 };
	glm::vec3 center{ 0, 0, 0 };
	glm::vec3 up{ 0, 1, 0 };

public:
	RasterizerApp(const CreationParams& params) : App(params) {

		m_prev_mouse_x = params.width / 2;
		m_prev_mouse_y = params.height / 2;
		glfwSetCursorPos(m_window, m_prev_mouse_x, m_prev_mouse_y);
	}
	void OnKeyPress(int key, int scancode, int action, int mods) override
	{
		additionalOnKeyPress(key, scancode, action, mods);
	}
	std::function<void(int, int, int, int)> additionalOnKeyPress;
	//TODO make this function threadsafe XD
	void OnMouseMove(double xpos, double ypos) override
	{
		if (m_reset_mouse_pos)
		{
			m_reset_mouse_pos = false;
			return;
		}
		int winsize_x, winsize_y;
		glfwGetWindowSize(m_window, &winsize_x, &winsize_y);
		m_yaw += (xpos - m_prev_mouse_x) * m_mouse_sensitivity, m_pitch += (ypos - m_prev_mouse_y) * m_mouse_sensitivity;

		if (m_pitch > 89) m_pitch = 89;
		if (m_pitch < -89) m_pitch = -89;

		glm::vec3 fwd;
		fwd.x = sin(glm::radians(m_yaw)) * cos(glm::radians(-m_pitch));
		fwd.y = sin(glm::radians(-m_pitch));
		fwd.z = -cos(glm::radians(m_yaw)) * cos(glm::radians(-m_pitch));
		forward = glm::normalize(fwd);

		m_prev_mouse_x = xpos;
		m_prev_mouse_y = ypos;
		if (xpos < winsize_x / 3 || xpos > winsize_x - winsize_x / 3 || ypos < winsize_y / 3 || ypos > winsize_y - winsize_y / 3)
		{
			glfwSetCursorPos(m_window, winsize_x / 2, winsize_y / 2);
			m_prev_mouse_x = winsize_x / 2;
			m_prev_mouse_y = winsize_y / 2;
			m_reset_mouse_pos = true;
		}

	}

	void ExtractViewFrustumPlanesFromMVP(glm::mat4 mvp, ViewFrustumPlanes& p)
	{
		for (int i = 0; i < 4; i++)
			p.planes[0][i] = mvp[i][3] + mvp[i][0];
		for (int i = 0; i < 4; i++)
			p.planes[1][i] = mvp[i][3] - mvp[i][0];
		for (int i = 0; i < 4; i++)
			p.planes[2][i] = mvp[i][3] + mvp[i][1];
		for (int i = 0; i < 4; i++)
			p.planes[3][i] = mvp[i][3] - mvp[i][1];
		for (int i = 0; i < 4; i++)
			p.planes[4][i] = mvp[i][3] + mvp[i][2];
		for (int i = 0; i < 4; i++)
			p.planes[5][i] = mvp[i][3] - mvp[i][2];

		//normalize the plane normal
		for (auto& plane : p.planes)
		{
			plane = plane / glm::length(glm::vec3(plane.x, plane.y, plane.z));
		}
	}

	struct AABB
	{
		glm::vec4 center;
		//This is the positive half diagonal
		glm::vec4 halfDiadonal;
	};
	enum : uint32_t
	{
		AABB_OUTSIDE_FRUSTUM = 0,
		AABB_INSIDE_FRUSTUM = 1,
		AABB_INTERSECTS_FRUSTUM = 3
	};
	uint32_t testFrustumAgainstAABB(ViewFrustumPlanes& frustum, AABB& aabb)
	{
		uint32_t res = 0;
		for (int i = 0; i < 6; i++)
		{
			float projLen = glm::dot(glm::abs(glm::vec3(frustum.planes[i].x, frustum.planes[i].y, frustum.planes[i].z)), glm::vec3(aabb.halfDiadonal.x, aabb.halfDiadonal.y, aabb.halfDiadonal.z));
			float centerPlaneDistance = glm::dot(glm::vec3(aabb.center.x, aabb.center.y, aabb.center.z), glm::vec3(frustum.planes[i].x, frustum.planes[i].y, frustum.planes[i].z)) + frustum.planes[i].w;
			//TODO: replace with bitwise operations
			if (centerPlaneDistance + projLen < 0) return AABB_OUTSIDE_FRUSTUM;
			else if (centerPlaneDistance - projLen > 0) res |= AABB_INSIDE_FRUSTUM;
			else res = AABB_INTERSECTS_FRUSTUM;
		}
		return res;
	}

	void ProcessMovement(float dt)
	{
		int upState = glfwGetKey(m_window, GLFW_KEY_W);
		int downState = glfwGetKey(m_window, GLFW_KEY_S);
		int leftState = glfwGetKey(m_window, GLFW_KEY_A);
		int rightState = glfwGetKey(m_window, GLFW_KEY_D);
		int quitState = glfwGetKey(m_window, GLFW_KEY_Q);
		if (upState == GLFW_PRESS)
		{
			center += m_movement_speed * forward * dt;
		}
		if (downState == GLFW_PRESS)
		{
			center -= m_movement_speed * forward * dt;
		}
		if (leftState == GLFW_PRESS)
		{
			center -= m_movement_speed * glm::normalize(glm::cross(forward, up)) * dt;
		}
		if (rightState == GLFW_PRESS)
		{
			center += m_movement_speed * glm::normalize(glm::cross(forward, up)) * dt;
		}
		if (quitState == GLFW_PRESS)
		{
			m_should_run = false;
		}
	}
	
	//test
	glm::ivec2 getBinForCoord(glm::vec2 coord)
	{
		glm::vec2 binSize = glm::vec2(0.125, 0.125);
		glm::ivec2 res = glm::ivec2(coord / binSize);
		res += 8;
		return res;
	}

	void Run()
	{
		FullScreenRenderer fullScreenRenderer;
		auto context = GLContext::Get();

		RasterizationParams params;
		Rasterizer rasterizer;

		params.proj = glm::perspective(60.f, m_params.width / float(m_params.height), 0.99f, 100.f);
		//params.proj = glm::ortho(0.0f, 1024.f, 0.0f, 1024.f, -0.1f, -100.f);
		params.view = glm::mat4(1); 
		params.model = glm::mat4(1);

		auto fbo = Framebuffer::Create();

		glm::vec3 vertices[] = {
			glm::vec3{-1, 0.5, -2  },
			glm::vec3{-1, 1, -2   },
			glm::vec3{ -0.5, 1, -2   },
			glm::vec3{ -1, 0, -2 },
			glm::vec3{ -1, 0.5, -2   },
			glm::vec3{ -0.5, 0.5, -2  },
			glm::vec3{ -1, -0.5, -2 },
			glm::vec3{ -1, 0, -2   },
			glm::vec3{ -0.5, 0, -2  },
			glm::vec3{ -1, -1, -2 },
			glm::vec3{ -1, -0.5, -2   },
			glm::vec3{ -0.5, -0.5, -2  },

			glm::vec3{-0.5, 0.5, -2  },
			glm::vec3{-0.5, 1, -2   },
			glm::vec3{ 0, 1, -2   },
			glm::vec3{ -0.5, 0, -2 },
			glm::vec3{ -0.5, 0.5, -2   },
			glm::vec3{ 0, 0.5, -2  },
			glm::vec3{ -0.5, -0.5, -2 },
			glm::vec3{ -0.5, 0, -2   },
			glm::vec3{ 0, 0, -2  },
			glm::vec3{ -0.5, -1, -2 },
			glm::vec3{ -0.5, -0.5, -2   },
			glm::vec3{ 0, -0.5, -2  },

			glm::vec3{0, 0.5, -2  },
			glm::vec3{0, 1, -2   },
			glm::vec3{0.5, 1, -2   },
			glm::vec3{ 0, 0, -2 },
			glm::vec3{ 0, 0.5, -2   },
			glm::vec3{ 0.5, 0.5, -2  },
			glm::vec3{ 0,-0.5, -2 },
			glm::vec3{ 0, 0, -2   },
			glm::vec3{ 0.5, 0, -2  },
			glm::vec3{ 0, -1, -2 },
			glm::vec3{ 0, -0.5, -2   },
			glm::vec3{ 0.5, -0.5, -2  },
						
			//glm::vec3{1, 0.5, -2  },
			//glm::vec3{1, 1, -2   },
			//glm::vec3{ 0.5, 1, -2   },
			//glm::vec3{ 1, 0, -2 },
			//glm::vec3{ 1, 0.5, -2   },
			//glm::vec3{ 0.5, 0.5, -2  },
			//glm::vec3{ 1, -0.5, -2 },
			//glm::vec3{ 1, 0, -2   },
			//glm::vec3{ 0.5, 0, -2  },
			//glm::vec3{ 1, -1, -2 },
			//glm::vec3{ 1, -0.5, -2   },
			//glm::vec3{ 0.5, -0.5, -2  },
		};
		glm::uvec3 indices[] = { 
			glm::uvec3{ 0, 1, 2 },
			glm::uvec3{3, 4, 5 },
			glm::uvec3{6, 7, 8},
			glm::uvec3{9, 10, 11},
			glm::uvec3{12, 13, 14 },
			glm::uvec3{15, 16, 17 },
			glm::uvec3{18, 19, 20},
			glm::uvec3{21, 22, 23},
			glm::uvec3{ 0, 1, 2 } + glm::uvec3(24),
			glm::uvec3{3, 4, 5 } + glm::uvec3(24),
			glm::uvec3{6, 7, 8} + glm::uvec3(24),
			glm::uvec3{9, 10, 11} + glm::uvec3(24),
			//glm::uvec3{12, 13, 14 } + glm::uvec3(24),
			//glm::uvec3{15, 16, 17 } + glm::uvec3(24),
			//glm::uvec3{18, 19, 20} + glm::uvec3(24),
			//glm::uvec3{21, 22, 23} + glm::uvec3(24),
		};
		fs::OBJLoader objLoader;
		auto modelData = objLoader.LoadModel("D:/dev/rizer/media/cube.obj");

		//Rasterizer::InputParams inputParams((void*)modelData.objects[0].positions.data(), sizeof(float) * modelData.objects[0].positions.size(), (void*)modelData.objects[0].vtxIndices.data(), sizeof(uint32_t) * modelData.objects[0].vtxIndices.size(), params);
		Rasterizer::InputParams inputParams((void*)vertices, sizeof(vertices), indices, sizeof(indices), params);
		fbo->AttachTexture(inputParams.fineRasterizerOutTex.get());
		fbo->Bind();

		Texture2D* drawTex = inputParams.fineRasterizerOutTex.get();
		additionalOnKeyPress = [&](int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				switch (key)
				{
				case GLFW_KEY_E:
					drawTex = inputParams.binRasterizerOutTex.get();
					break;
				case GLFW_KEY_R:
					drawTex = inputParams.coarseRasterizerOutTex.get();
					break;
				case GLFW_KEY_T:
					drawTex = inputParams.fineRasterizerOutTex.get();
					break;
				}
			}
		};
		uint64_t start = std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
		while (ShouldRun() && m_should_run)
		{
			BeginScene();
			

			inputParams.rasterParams.view = glm::lookAt(center, center + forward, up);
			inputParams.rasterParams.model = glm::scale(glm::mat4(1), glm::vec3(0.05, 0.05, 0.05));
			
			TEST_TIMER_START("Uniform Update")
			inputParams.uniforms->Update((void*)&inputParams.rasterParams, sizeof(inputParams.rasterParams));
			TEST_TIMER_END()

			TEST_TIMER_START("Entire Rasterization")
			rasterizer.Rasterize(inputParams);
			TEST_TIMER_END()


			TEST_TIMER_START("Screen rendering")
			fullScreenRenderer.RenderTextureFullScreen(drawTex);
			TEST_TIMER_END()

			TEST_TIMER_START("Swap buffers")
			EndScene();
			TEST_TIMER_END()
			std::cout << "\n\n";
			uint64_t now = std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
			auto diff = now - start;
			ProcessMovement(diff / 1000.);
			start = now;
			SetWindowCaption(std::string("Framerate: ") + std::to_string(diff) + " ms, " + std::to_string(1000. / diff) + " FPS");
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 1024, 1024, "Demo App" });
	app.Run();
}

extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
#include <App.h>
#include <../src/fs/FileManager.h>
#include "../src/Rasterizer.h"
#include "../src/OpenGL/Framebuffer.h"
#include "../../src/fs/OBJLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../src/Debug.h"

#include <wtypes.h>
#include <iostream>
#include <functional>

class RasterizerApp : App
{
	bool m_should_run = true;

	double m_prev_mouse_x, m_prev_mouse_y;
	float m_mouse_sensitivity = 0.1f;
	float m_movement_speed = 0.1f;

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
		switch (key)
		{
			//WASD processing
		case GLFW_KEY_W: [[fallthrough]];
		case GLFW_KEY_UP:
		{
			center += m_movement_speed * forward;
			break;
		}
		case GLFW_KEY_A: [[fallthrough]];
		case GLFW_KEY_LEFT:
		{
			center -= m_movement_speed * glm::normalize(glm::cross(forward, up));
			break;
		}
		case GLFW_KEY_S: [[fallthrough]];
		case GLFW_KEY_DOWN:
		{
			center -= m_movement_speed * forward;
			break;
		}
		case GLFW_KEY_D: [[fallthrough]];
		case GLFW_KEY_RIGHT:
		{
			center += m_movement_speed * glm::normalize(glm::cross(forward, up));
			break;
		}
		case GLFW_KEY_Q:
		{
			m_should_run = false;
			break;
		}
		}
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
	/*void getTriangleAABB(glm::vec4 a, glm::vec4 b, glm::vec4 c, AABB& aabb)
	{
		glm::vec4 minV = glm::vec4(glm::min(glm::min(a, b), c));
		glm::vec4 maxV = glm::vec4(glm::max(glm::max(a, b), c));
		aabb.center = (minV + maxV) / 2;
		aabb.halfDiadonal = (maxV - minV) / 2;
	}*/
	
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
		constexpr uint32_t tex_size_x = 2048, tex_size_y = 2048;
		auto context = GLContext::Get();

		RasterizationParams params;
		Rasterizer rasterizer;

		//RasterizationDynamicParams dynamicParams;
		//dynamicParams.texture_width = m_params.width;
		//dynamicParams.texture_height = m_params.height;
		params.proj = glm::perspective(60.f, m_params.width / float(m_params.height), 0.01f, 100.f);
		params.view = glm::mat4(1); //glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		params.model = glm::mat4(1);

		auto fbo = Framebuffer::Create();

		glm::vec3 vertices[] = {
			glm::vec3{-10, 1, -20  },
			glm::vec3{-5, 10, -20   },
			glm::vec3{ 0, 1, -20   },
			glm::vec3{ -3, 1, -30 },
			glm::vec3{ 2, 10, -30   },
			glm::vec3{ 7, 1, -30  }
		};
		glm::uvec3 indices[2] = { glm::uvec3{ 0, 1, 2 }, glm::uvec3{3, 4, 5 } };


		Rasterizer::InputParams inputParams((void*)vertices, sizeof vertices, (void*)indices, sizeof indices, params);
		fbo->AttachTexture(inputParams.fineRasterizerOutTex.get());
		fbo->Bind();

		additionalOnKeyPress = [&](int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				switch (key)
				{
				case GLFW_KEY_E:
					fbo->AttachTexture(inputParams.binRasterizerOutTex.get());
					break;
				case GLFW_KEY_R:
					fbo->AttachTexture(inputParams.coarseRasterizerOutTex.get());
					break;
				case GLFW_KEY_T:
					fbo->AttachTexture(inputParams.fineRasterizerOutTex.get());
					break;
				}
			}
		};
		uint64_t start = std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
		while (ShouldRun() && m_should_run)
		{
			//inputParams.outTex = Texture2D::CreateEmptyR8G8B8A8_UNORM(2048, 2048);
			BeginScene();
			//ImGui::Begin("Rasterization Params");
			//ImGui::SetWindowSize(ImVec2((float)1024, (float)1024));
			//
			//ImGui::End();
			//dynamicParams.view = glm::rotate(dynamicParams.view, glm::radians(15.f), glm::vec3(0, -1, 0));
			//dynamicParams.view = glm::translate(dynamicParams.view, glm::vec3(0, 0, 0.01));
			inputParams.rasterParams.view = glm::lookAt(center, center + forward, up);
			//vertex_buffer->Update(vertices, sizeof vertices);
			//vertex_buffer->Update(vertices, sizeof vertices);
			//ExtractViewFrustumPlanesFromMVP(inputParams.rasterParams.proj * inputParams.rasterParams.view * inputParams.rasterParams.model, inputParams.rasterParams.planes);
			TEST_TIMER_START("Uniform Update")
			inputParams.uniforms->Update((void*)&inputParams.rasterParams, sizeof(inputParams.rasterParams));
			TEST_TIMER_END()

			TEST_TIMER_START("Entire Rasterization")
			rasterizer.Rasterize(inputParams);
			TEST_TIMER_END()

			TEST_TIMER_START("End scene")
			context->BlitFramebuffer(fbo.get());
			EndScene();

			TEST_TIMER_END()

			uint64_t now = std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
			auto diff = now - start;
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
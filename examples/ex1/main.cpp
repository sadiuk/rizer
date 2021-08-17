#include <App.h>
#include <../src/fs/FileManager.h>
#include "../src/Rasterizer.h"
#include "../src/OpenGL/Framebuffer.h"
#include <wtypes.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class RasterizerApp : App
{
	bool m_should_run = true;
	float m_yaw = 0, m_pitch = 0;
	
	double m_prev_mouse_x, m_prev_mouse_y;
	float m_mouse_sensitivity = 1.f;
	float m_movement_speed = 1.f;

	
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

			break;
		}
		case GLFW_KEY_A: [[fallthrough]];
		case GLFW_KEY_LEFT: 
		{

			break;
		}
		case GLFW_KEY_S: [[fallthrough]];
		case GLFW_KEY_DOWN: 
		{

			break;
		}
		case GLFW_KEY_D: [[fallthrough]];
		case GLFW_KEY_RIGHT:
		{

			break;
		}
		case GLFW_KEY_Q:
		{
			m_should_run = false;
			break;
		}
		}
	}
	void OnMouseMove(double xpos, double ypos) override
	{
		int winsize_x, winsize_y;
		glfwGetWindowSize(m_window, &winsize_x, &winsize_y);
		if (xpos < winsize_x / 4 || xpos > winsize_x - winsize_x / 4 || ypos < winsize_y / 4 || ypos > winsize_y - winsize_y / 4)
		{
			//glfwSetCursorPos(m_window, winsize_x / 2, winsize_y / 2);
		}
		double delta_x = xpos - m_prev_mouse_x, delta_y = ypos - m_prev_mouse_y;


		m_prev_mouse_x = xpos;
		m_prev_mouse_y = ypos;
		
	}

	void Run()
	{
		auto context = GLContext::Get();

		RasterizationParams params;
		params.vertex_buffer_layout = VertexBufferLayout::POSR8G8B8A8_COLR8G8B8A8;
		Rasterizer rasterizer(params);

		RasterizationDynamicParams dynamicParams;
		dynamicParams.texture_width = m_params.width;
		dynamicParams.texture_height = m_params.height;
		dynamicParams.proj = glm::perspective(60.f, m_params.width / float(m_params.height), 0.01f, 100.f);
		dynamicParams.view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		auto out_tex = Texture2D::CreateEmptyR8G8B8A8_UNORM(dynamicParams.texture_width, dynamicParams.texture_height);
		auto fbo = Framebuffer::Create();
		fbo->AttachTexture(out_tex.get());
		struct alignas(16) vec3
		{
			float x, y, z;
		};
		struct alignas(16) vec4
		{
			float x, y, z, w;
		};
		/*vec3 vertices[6] = {
			vec3{-0.5, -0.5, -0.1 },
			vec3{-0.4,  0.5, -0.5 },
			vec3{0.5, -0.5, -0.5 },
			vec3{0, -0.5, -0.9 },
			vec3{0.1,  0.5, -0.05 },
			vec3{0.2, -0.5, -0.2 }
		};*/
		struct vtx { vec3 pos; vec4 color; };
		vtx vertices[] = {
			{ vec3{-0.5, -0.5, -0.1 }, vec4{0, 0.5, 1, 1} },
			{ vec3{-0.4,  0.5, -0.5 }, vec4{0, 1, 0, 1} },
			{ vec3{ 0.5, -0.5, -0.5 }, vec4{0, 1, 1, 1} },
			{ vec3{ 0,	 -0.5, -0.9 }, vec4{1, 0, 1, 1} },
			{ vec3{ 0.1,  0.5, -0.05}, vec4{1, 0, 1, 1} },
			{ vec3{ 0.2, -0.5, -0.05}, vec4{1, 0, 1, 1} } 
		};
		//dynamicParams.view = glm::translate(dynamicParams.view, glm::vec3(0, 0, -1));
		//auto a = dynamicParams.proj * dynamicParams.view * glm::vec4(vertices[0].pos.x, vertices[0].pos.y, vertices[0].pos.z, 1);
		//dynamicParams.view = glm::translate(dynamicParams.view, glm::vec3(0, 0, 15));
		//auto b = dynamicParams.proj * dynamicParams.view * glm::vec4(vertices[0].pos.x, vertices[0].pos.y, vertices[0].pos.z, 1);
		struct alignas(16) uvec3
		{
			uint32_t x, y, z;
		};
		uvec3 indices[2] = { uvec3{ 0, 1, 2 }, uvec3{3, 4, 5 } };

		auto vertex_buffer = SSBO::Create(vertices, sizeof vertices);
		auto index_buffer = SSBO::Create(indices, sizeof indices);
		bool enable_depth_testing = true, update_depth_buffer = true;
		auto depth_buffer = SSBO::Create(nullptr, dynamicParams.texture_width * dynamicParams.texture_height * sizeof(float));
		while (ShouldRun() && m_should_run)
		{
			BeginScene();
			context->clearSSBO(depth_buffer.get());

			ImGui::Begin("Rasterization Params");
			ImGui::ColorEdit3("Clear Color", (float*)&dynamicParams.clear_color);
			ImGui::Checkbox("Enable Depth Test", &enable_depth_testing);
			ImGui::Checkbox("Update Depth Buffer", &update_depth_buffer);
			ImGui::SliderFloat("Z0", &vertices[0].pos.z, -1, 0);
			ImGui::SliderFloat("Z1", &vertices[1].pos.z, -1, 0);
			ImGui::SliderFloat("Z2", &vertices[2].pos.z, -1, 0);
			ImGui::SliderFloat("Z3", &vertices[3].pos.z, -1, 0);
			ImGui::SliderFloat("Z4", &vertices[4].pos.z, -1, 0);
			ImGui::SliderFloat("Z5", &vertices[5].pos.z, -1, 0);
			ImGui::End();
			dynamicParams.enable_depth_test = enable_depth_testing;
			dynamicParams.update_depth_buffer = update_depth_buffer;
			dynamicParams.view = glm::rotate(dynamicParams.view, glm::radians(10.f), glm::vec3(0, 1, 0));
			//dynamicParams.view = glm::translate(dynamicParams.view, glm::vec3(0, 0, 0.01));

			vertex_buffer->Update(vertices, sizeof vertices);
			rasterizer.Rasterize(vertex_buffer.get(), index_buffer.get(), out_tex.get(), dynamicParams, depth_buffer.get());
			context->BlitFramebuffer(fbo.get());
			EndScene();
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 1000, 700, "Demo App" });
	app.Run();
}

extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
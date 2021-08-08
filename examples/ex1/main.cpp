#include <App.h>
#include <../src/fs/FileManager.h>
#include "../src/Rasterizer.h"
#include "../src/OpenGL/Framebuffer.h"
#include <wtypes.h>

class RasterizerApp : App
{
public:
	RasterizerApp(const CreationParams& params) : App(params) {
	}
	void Run()
	{
		auto context = GLContext::Get();

		RasterizationParams params;
		Rasterizer rasterizer(params);

		RasterizationDynamicParams dynamicParams;
		dynamicParams.texture_width = m_params.width;
		dynamicParams.texture_height = m_params.height;
		dynamicParams.vertex_buffer_layout = VertexBufferLayout::POSR8G8B8A8;

		auto out_tex = Texture2D::CreateEmptyR8G8B8A8_UNORM(dynamicParams.texture_width, dynamicParams.texture_height);
		auto fbo = Framebuffer::Create();
		fbo->AttachTexture(out_tex.get());
		struct alignas(16) vec3
		{
			float x, y, z;
		};
		vec3 vertices[6] = {
			vec3{-0.5, -0.5, -0.1 },
			vec3{-0.4,  0.5, -0.5 },
			vec3{0.5, -0.5, -0.5 },
			vec3{0, -0.5, -0.9 },
			vec3{0.1,  0.5, -0.05 },
			vec3{0.2, -0.5, -0.2 }
		};
		struct alignas(16) uvec3
		{
			uint32_t x, y, z;
		};
		uvec3 indices[2] = { uvec3{ 0, 1, 2 }, uvec3{3, 4, 5 } };

		auto vertex_buffer = SSBO::Create(vertices, sizeof vertices);
		auto index_buffer = SSBO::Create(indices, sizeof indices);
		bool enable_depth_testing = false, update_depth_buffer = false;
		auto depth_buffer = SSBO::Create(nullptr, dynamicParams.texture_width * dynamicParams.texture_height * sizeof(float));
		while (ShouldRun())
		{
			BeginScene();
			context->clearSSBO(depth_buffer.get());

			ImGui::Begin("Rasterization Params");
			ImGui::ColorEdit3("Clear Color", (float*)&dynamicParams.clear_color);
			ImGui::Checkbox("Enable Depth Test", &enable_depth_testing);
			ImGui::Checkbox("Update Depth Buffer", &update_depth_buffer);
			ImGui::End();
			//enable_depth_testing = !enable_depth_testing;
			//update_depth_buffer = !update_depth_buffer;
			dynamicParams.enable_depth_test = enable_depth_testing;
			dynamicParams.update_depth_buffer = update_depth_buffer;

			rasterizer.Rasterize(vertex_buffer.get(), index_buffer.get(), out_tex.get(), dynamicParams, depth_buffer.get());
			context->BlitFramebuffer(fbo.get());
			EndScene();
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 1920, 1080, "Demo App" });
	app.Run();
}

extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
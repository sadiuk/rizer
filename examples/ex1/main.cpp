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
		params.vertex_buffer_layout = VertexBufferLayout::POSR8G8B8A8_COLR8G8B8A8;
		Rasterizer rasterizer(params);

		RasterizationDynamicParams dynamicParams;
		dynamicParams.texture_width = m_params.width;
		dynamicParams.texture_height = m_params.height;

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
		struct vtc { vec3 pos; vec4 color; };
		vtc vertices[6] = {
			{ vec3{-0.5, -0.5, -0.1 }, vec4{0, 0.5, 1, 1} },
			{ vec3{-0.4,  0.5, -0.5 }, vec4{0, 1, 0, 1} },
			{ vec3{ 0.5, -0.5, -0.5 }, vec4{0, 1, 1, 1} },
			{ vec3{ 0,	 -0.5, -0.9 }, vec4{1, 0, 1, 1} },
			{ vec3{ 0.1,  0.5, -0.05}, vec4{1, 0, 1, 1} },
			{ vec3{ 0.2, -0.5, -0.05 }, vec4{1, 0, 1, 1} } 
		};
		struct alignas(16) uvec3
		{
			uint32_t x, y, z;
		};
		uvec3 indices[2] = { uvec3{ 0, 1, 2 }, uvec3{3, 4, 5 } };

		auto vertex_buffer = SSBO::Create(vertices, sizeof vertices);
		auto index_buffer = SSBO::Create(indices, sizeof indices);
		bool enable_depth_testing = true, update_depth_buffer = true;
		auto depth_buffer = SSBO::Create(nullptr, dynamicParams.texture_width * dynamicParams.texture_height * sizeof(float));
		while (ShouldRun())
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

			vertex_buffer->Update(vertices, sizeof vertices);
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
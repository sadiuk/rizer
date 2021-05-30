#include <App.h>
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
		params.texture_width = m_params.width;
		params.texture_height = m_params.height;
		params.vertex_buffer_layout = VertexBufferLayout::POSR8G8B8A8;
		Rasterizer rasterizer(params);

		auto out_tex = Texture2D::CreateEmptyR8G8B8A8_UNORM(params.texture_width, params.texture_height);
		auto fbo = Framebuffer::Create();
		fbo->AttachTexture(out_tex.get());

		glm::vec2 vertices[4] = { glm::vec2{-0.5, -0.5}, glm::vec2{-0.5, 0.5}, glm::vec2{0.5, 0.5}, glm::vec2{0.5, -0.5} };
		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };

		auto vertex_buffer = SSBO::Create(vertices, sizeof vertices);
		auto index_buffer = SSBO::Create(indices, sizeof indices);
		while (ShouldRun())
		{
			rasterizer.Rasterize(vertex_buffer.get(), index_buffer.get(), out_tex.get());
			context->BlitFramebuffer(fbo.get());
			EndScene();
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 700, 400, "Demo App" });
	app.Run();
}

//extern "C" {  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
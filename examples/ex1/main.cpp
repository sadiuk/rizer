#include <App.h>
#include "../src/Rasterizer.h"
#include "../src/OpenGL/Framebuffer.h"

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
		Rasterizer rasterizer(params);
		auto out_tex = Texture2D::CreateEmptyR8G8B8A8_UNORM(params.texture_width, params.texture_height);
		auto fbo = Framebuffer::Create();
		fbo->AttachTexture(out_tex.get());
		while (ShouldRun())
		{
			rasterizer.Rasterize(out_tex.get());
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

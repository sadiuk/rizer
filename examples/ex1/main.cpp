#include <App.h>
#include "../src/Rasterizer.h"
class RasterizerApp : App
{
public:
	RasterizerApp(const CreationParams& params) : App(params) {
	}
	void Run()
	{
		RasterizationParams params;
		params.texture_width = 1000;
		params.texture_height = 1000;
		Rasterizer rasterizer(params);
		auto out_tex = Texture2D::CreateEmptyR8G8B8A8_UNORM(params.texture_width, params.texture_height);
		while (ShouldRun())
		{
			rasterizer.Rasterize(out_tex.get());
			EndScene();
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 700, 400, "Demo App" });
	app.Run();
}

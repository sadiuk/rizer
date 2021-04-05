#include <App.h>

class RasterizerApp : App
{
public:
	RasterizerApp(const CreationParams& params) : App(params) {}
	void Run()
	{
		while (ShouldRun())
		{
			EndScene();
		}
	}
};

int main()
{
	RasterizerApp app(CreationParams{ 700, 400, "Demo App" });
	app.Run();
}
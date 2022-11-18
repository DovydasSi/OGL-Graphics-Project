#include "../NCLGL/window.h"
#include "Renderer.h"

int main()	{
	string title = "Elden Ring 2";
	Window w(title, 1280, 720, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){

		float sec = w.GetTimer()->GetTimeDeltaSeconds();
		renderer.UpdateScene(sec);
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
		w.SetTitle(title + " (FPS: " + std::to_string(1/sec) + ")");
	}
	return 0;
}
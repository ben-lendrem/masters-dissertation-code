#include "Renderer.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

/*
  Feesh   /"*._         _
	  .-*'`    `*-.._.-'/
	< * ))     ,       (
	  `*-._`._(__.--*"`.\
*/



int main(void) {
	//filepath the results go to (when results are sent, mode is append)
	//const std::string filePath = "../Results/DumbParticlesResults.csv";
	srand(time(NULL));
	Window w("ParticleSystem", 1280, 720, false);

	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}
	bool started = false;
	float dTime = 0.0f;
	w.LockMouseToWindow(false);
	w.ShowOSPointer(true);
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (!started) {
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
				started = true;
				w.LockMouseToWindow(true);
				w.ShowOSPointer(false);
			}
			continue;
		}
		renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
				Shader::ReloadAllShaders();
		}
	}
	//renderer.SendResultsToCSV(filePath);

	return 0;
}
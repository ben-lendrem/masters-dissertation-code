#include "Renderer.h"
#include "Camera.h"
#include "ParticleSystem.h"

#include <chrono>
#include <fstream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	glEnable(GL_DEPTH_TEST);

	initShaders();
	(dumb) ? initDumbParticles() : initParticles();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);
	camera = new Camera(-3, 0.0f, Vector3(0,10,50));
	camera->SetSpeed(30.0f);

	init = true;
}

Renderer::~Renderer(void) {
	delete currentShader;
	delete particleShader;
	delete camera;
	delete pSystem;
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	currentShader = particleShader;
	BindShader(particleShader);
	UpdateShaderMatrices();
	pSystem->Draw(particleShader->GetProgram());
	currentShader = nullptr;
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	UpdatePositions(dt);
	viewMatrix = camera->BuildViewMatrix();
	//std::chrono::steady_clock::time_point updateStart = std::chrono::steady_clock::now();
	if (!dumb) {
		pSystem->UpdateParticles(dt);
	}
	/*
	This comment block (and the commented line above is the code for testing the system

	std::chrono::steady_clock::time_point updateEnd = std::chrono::steady_clock::now();
	float updateDuration = (float)
		((std::chrono::duration_cast<std::chrono::microseconds>(updateEnd - updateStart))
			.count()/1000000.0f); //gets time in seconds as a float with reasonable (~4-5 decimal places) precision*/
	//AddResult(pSystem->NumAliveParticles(), dt, updateDuration);
}

void Renderer::initShaders() {
	particleShader = new Shader("Coursework/particleVertex.glsl", "Coursework/particleFragment.glsl");
	currentShader = nullptr;
	if (!particleShader->LoadSuccess()) {
		return;
	}
}

void Renderer::initParticles() {
	pSystem = new ParticleSystem(Vector3(), 2000);
	glPointSize(5.0);
}

void Renderer::UpdatePositions(float dt) {
	Vector3 sysTransform;
	Vector3 emitTransform;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
		sysTransform.x += -100.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
		sysTransform.x += 100.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		sysTransform.y += -100.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
		sysTransform.y += 100.0f * dt;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD4)) {
		emitTransform.x += -10.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD6)) {
		emitTransform.x += 10.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD2)) {
		emitTransform.y += -10.0f * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD8)) {
		emitTransform.y += 10.0f * dt;
	}
	pSystem->UpdateSystemPosition(sysTransform);
	pSystem->UpdateEmitterPosition(emitTransform);
}

void Renderer::initDumbParticles() {
	pSystem = new ParticleSystem(Vector3(), 2000);
	glPointSize(5.0);
	pSystem->PlaceDumbParticles(13);
}

void Renderer::AddResult(int numParticles, float frameTime, float updateTime) {
	std::string line;
	line.append(std::to_string(numParticles) + "," + std::to_string(frameTime) + "," + std::to_string(updateTime) + "\n");
	results.emplace_back(line);
}

void Renderer::SendResultsToCSV(const std::string filePath) {
	std::ofstream output;
	output.open(filePath, std::ofstream::out | std::ofstream::app);
	while (!results.empty()) {
		std::string current = results.front();
		results.erase(results.begin());
		output << current;
	}
	output.close();
}
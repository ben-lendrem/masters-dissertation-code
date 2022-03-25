#pragma once

#include "OGLRenderer.h"
#include <vector>
#include <string>

class ParticleSystem;
class Camera;
class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;
	void SendResultsToCSV(const std::string filePath);
protected:
	Camera* camera;
	
	void initShaders();
	void initParticles();
	void initDumbParticles();

	void UpdatePositions(float dt);

	Shader* currentShader;
	Shader* particleShader;

	ParticleSystem* pSystem;

	bool dumb = false;

	std::vector<std::string> results;
	void AddResult(int numParticles, float frameTime, float updateTime);
	

};
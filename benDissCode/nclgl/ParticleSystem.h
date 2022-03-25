#pragma once

/*
Class: ParticleSystem
Author: Ben Lendrem, Student No. 170624723
Acts as a holder for an array/vector of particles. Provides member functions to update the
particles and the system as a whole (including emission of new particles, moving the system,
and providing a choice for updating of particles (compute shader/multithreading))


  Feesh   /"*._         _
	  .-*'`    `*-.._.-'/
	< * ))     ,       (
	  `*-._`._(__.--*"`.\
*/
#include "Vector3.h"
#include "Emitter.h"
#include <vector>
#include <glad/glad.h>
#include <thread>
class Particle;

class ParticleSystem {
public:
	ParticleSystem(Vector3 initPos, int particleMax);
	~ParticleSystem();
	void Draw(GLuint shader);
	void UpdateParticles(float dt);
	void UpdateSystemPosition(Vector3 sysTransform) { sysPos += sysTransform; }
	void UpdateEmitterPosition(Vector3 emitTransformDelta) { emitter.SetInitialTransform(emitter.GetInitialTransform() + emitTransformDelta); }
	void SetEmitterPosition(Vector3 emitTransform) { emitter.SetInitialTransform(emitTransform); }

	void PlaceDumbParticles(int sideLength);

	int NumAliveParticles() { return aliveParticles.size(); }
protected:
	
	//positions (whole system and emitter)
	Vector3 sysPos; //changing this moves all particles in the system
	Emitter emitter; //moving this changes the position new particles are created relative to the position of sysPos

	DeltaUpdate delta;
	
	void InitEmitter();
	void KillParticles();
	void UpdateLiveParticles(float dt);
	void UpdateLiveParticlesThreaded(float dt);
	void SpawnNewParticle();
	void GenerateDelta();

	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;

	int mode;
	const int NUM_THREADS = 4;
	std::thread updateThreads[4];
};
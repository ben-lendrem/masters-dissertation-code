#include "ParticleSystem.h"
#include "Emitter.h"
#include "Particle.h"
#include <cstdlib>
#include <functional>

enum UpdateMode {
	BASIC,
	THREADED
};

typedef std::vector<Particle*>::iterator ParticleVecIterator;

ParticleSystem::ParticleSystem(Vector3 initPos, int particleMax) {
	sysPos = initPos;
	InitEmitter();
	deadParticles.reserve(particleMax);
	for (int i = 0; i < particleMax; i++) {
		Particle* temp = new Particle();
		deadParticles.emplace_back(temp);
	}
	GenerateDelta();
	mode = UpdateMode::BASIC;
}

ParticleSystem::~ParticleSystem() {
	while (!aliveParticles.empty()) {
		Particle* temp = aliveParticles.back();
		aliveParticles.pop_back();
		delete temp;
	}

	while (!deadParticles.empty()) {
		Particle* temp = deadParticles.back();
		deadParticles.pop_back();
		delete temp;
	}
}

void ParticleSystem::Draw(GLuint shader) {
	for (ParticleVecIterator it = aliveParticles.begin();
		it != aliveParticles.end(); it++) {
		(*it)->DrawParticle(shader); //when draw is called, shader is bound and matrices have been sent
	}
}

void ParticleSystem::UpdateParticles(float dt) {
	//update existing particles
		//if particle has no more life, kill
	KillParticles();
		//if it does, update individual particle with new values
	if (mode == UpdateMode::BASIC) {
		UpdateLiveParticles(dt);
	}
	else if (mode == UpdateMode::THREADED) {
		UpdateLiveParticlesThreaded(dt);
	}
	else {
		//don't update I suppose
	}
	
	//Spawn new particles (check done here to avoid unnecessary function calls) 
	if (!deadParticles.empty() && rand() % 6 == 0) {
		SpawnNewParticle();
	}
	

}

void ParticleSystem::InitEmitter() {
	emitter.SetInitialVelocity(Vector3(0, 3, 0));
	emitter.SetInitialColour(Vector4(1, 0.5, 0, 1));
	emitter.SetInitialLifeSpan(8.0f);
	emitter.SetInitialTransform(sysPos);
}

void ParticleSystem::KillParticles() {
	ParticleVecIterator it = aliveParticles.begin();
	while (it != aliveParticles.end()) {
		if (!(*it)->pAlive) {
			deadParticles.emplace_back(*it);
			it = aliveParticles.erase(it);
		}
		else {
			it++;
		}
	}
}

void ParticleSystem::UpdateLiveParticles(float dt) {
	for (ParticleVecIterator it = aliveParticles.begin();
		it != aliveParticles.end(); it++) {
		GenerateDelta();
		(*it)->UpdateBasicParticle(dt, delta);
	}
}

void ParticleSystem::SpawnNewParticle() {
	aliveParticles.emplace_back(deadParticles.back());
	deadParticles.pop_back();
	emitter.EmitParticle(*aliveParticles.back());
}

void ParticleSystem::GenerateDelta() {
	float rand_half_halved = ((float)rand() / RAND_MAX - 0.5f) / 2.0f; //wordy variable name, can do better
	delta.deltaVel = Vector3(rand_half_halved, rand_half_halved, rand_half_halved);
	float rand_half_tenth = ((float)rand() / RAND_MAX - 0.5f) / 10.0f;
	delta.deltaCol = Vector4(rand_half_tenth, rand_half_tenth, rand_half_tenth, 0.0f);
	delta.deltaLife = rand_half_tenth;
}

void ParticleSystem::UpdateLiveParticlesThreaded(float dt) {
	
}

void ParticleSystem::PlaceDumbParticles(int sideLength) {
	for (int x = 0; x < sideLength; x++) {
		for (int y = 0; y < sideLength; y++) {
			for (int z = 0; z < sideLength; z++) {
				if (!deadParticles.empty()) {
					emitter.SetInitialTransform(Vector3((x * 4), (y * 4), (z * 4)));
					SpawnNewParticle();
				}
			}
		}
	}
}
#pragma once
#include "Vector3.h"
#include "Emitter.h"
#include <vector>
#include <glad/glad.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>

class Particle;

const int NUMTHREADS = 4;

enum class flags {
	A = 1 << 0,
	B = 1 << 1,
	C = 1 << 2,
	D = 1 << 3
}; //way to dynamically construct this at runtime (or some equivalent if not)?

static DeltaUpdate GenerateDelta() {
	DeltaUpdate val;
	float rand_half_halved = ((float)rand() / RAND_MAX - 0.5f) / 2.0f; //wordy variable name, can do better
	val.deltaVel = Vector3(rand_half_halved, rand_half_halved, rand_half_halved);
	float rand_half_tenth = ((float)rand() / RAND_MAX - 0.5f) / 10.0f;
	val.deltaCol = Vector4(rand_half_tenth, rand_half_tenth, rand_half_tenth, 0.0f);
	val.deltaLife = rand_half_tenth;
	
	return val;
};

class ThreadParticleSystem {
public:
	ThreadParticleSystem(Vector3 initPos, int particleMax);
	~ThreadParticleSystem();
	void Draw(GLuint shader);
	void UpdateParticles(float dt);
	void UpdateSystemPosition(Vector3 sysTransform) { sysPos += sysTransform; }

protected:
	Vector3 sysPos;
	Emitter emitter;

	DeltaUpdate delta;

	float currentDT; //dt of frame stored for threads to access
	int aliveSize;

	void InitEmitter();
	void KillParticles();
	void SpawnNewParticle();
	void ResetFlags();
	void FlagThreadsDone();
	void SetAliveSize();

	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;

	/*
	Above is functionality from ParticleSystem
	Below is added functionality for multithreading
	*/

	std::thread updateThreads[NUMTHREADS];


	std::shared_mutex aliveKey; //are 5 mutex's needed?
	std::shared_mutex deadKey;
	std::shared_mutex flagKey;
	std::shared_mutex dtKey;
	std::shared_mutex sizeKey; 
	

	std::condition_variable_any aliveCV; //are 5 CV's needed??
	std::condition_variable_any deadCV;

	std::condition_variable_any flagCV;
	std::condition_variable_any dtCV;
	std::condition_variable_any sizeCV;

	//variable to store thread flags
	int allThreadFlags;

	//worker function is friend
	friend void workerThreadFunc(flags thisFlag, ThreadParticleSystem* sys);


};
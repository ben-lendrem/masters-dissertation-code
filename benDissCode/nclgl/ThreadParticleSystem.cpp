#include "ThreadParticleSystem.h"
#include "Emitter.h"
#include "Particle.h"
#include "ThreadFunction.h"
#include <cstdlib>
#include <functional>

typedef std::vector<Particle*>::iterator ParticleVecIterator;

void threadFuncPlaceholder() {
	//do Nothing
}
void workerThreadFunc(flags thisFlag, ThreadParticleSystem* sys) {
	int myFlag = (int)thisFlag;
	int allFlags = 0;

	std::shared_lock<std::shared_mutex> flk(sys->flagKey); //these 4 lines need to be put in a function
	sys->flagCV.wait(flk);
	allFlags = sys->allThreadFlags;
	flk.unlock();

	while ([allFlags]()->bool { return (allFlags < (1 << NUMTHREADS)); }()) {


		/*
		while masterflag set to running*
			wait on flagkey until thread flag is set to run
			take subset of live particles
			update each particle:
				generate delta
				pass delta and dt (dtKey) to update particle function
			hold subset until live particles is empty
			return subset to live particles
			flip this threads flag on allThreadFlags to indicate it is complete
		*/

		//std::unique_lock<std::shared_mutex> lk(sys->flagKey);
		//threadFlagCV.wait(lk, ((~sys->allThreadFlags) & myFlag) == myFlag);

		sys->flagCV.wait(flk);
		allFlags = sys->allThreadFlags;
		flk.unlock();
	}
}


ThreadParticleSystem::ThreadParticleSystem(Vector3 initPos, int particleMax) {
	sysPos = initPos;
	InitEmitter();
	deadParticles.reserve(particleMax);
	for (int i = 0; i < particleMax; i++) {
		Particle* temp = new Particle();
		deadParticles.emplace_back(temp);
	}
	delta = GenerateDelta();

	//launch threads
	allThreadFlags = 0;
	for (int i = 0; i < NUMTHREADS; i++) {
		updateThreads[i] = std::thread(threadFuncPlaceholder);
	}
}

ThreadParticleSystem::~ThreadParticleSystem() {
	for (int i = 0; i < NUMTHREADS; i++) {
		updateThreads[i].join();
	}
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

void ThreadParticleSystem::InitEmitter() {
	emitter.SetInitialVelocity(Vector3(0, 3, 0));
	emitter.SetInitialColour(Vector4(1, 0.5, 0, 1));
	emitter.SetInitialLifeSpan(8.0f);
	emitter.SetInitialTransform(sysPos);
}

//below is all member functions that (may) require CV and mutex

void ThreadParticleSystem::FlagThreadsDone() {
	/*std::unique_lock<std::mutex> lk(flagKey);
	flagCV.wait(lk);
	allThreadFlags = allThreadFlags ^ (1 << NUMTHREADS);*/
}

void ThreadParticleSystem::ResetFlags() {
	/*std::unique_lock<std::mutex> lk(flagKey);
	flagCV.wait(lk);
	allThreadFlags = 0;*/
}

void ThreadParticleSystem::UpdateParticles(float dt) {
	/*
	This function needs to:
	- kill alive particles
	- set dt for current frame
	- notify
	*/
	KillParticles();
}

void ThreadParticleSystem::KillParticles() {

	std::vector<Particle*> temp;
	std::unique_lock<std::shared_mutex> alk(aliveKey);
	aliveCV.wait(alk);
	ParticleVecIterator it = aliveParticles.begin();
	while (it != aliveParticles.end()) {
		if (!(*it)->pAlive) {
			temp.emplace_back(*it);
			it = aliveParticles.erase(it);
		}
		else {
			it++;
		}
	}
	alk.unlock();
	aliveCV.notify_all();

	std::unique_lock<std::shared_mutex> dlk(deadKey);
	deadCV.wait(dlk);
	while (!temp.empty()) {
		deadParticles.emplace_back(temp.back());
		temp.pop_back();
	}
	dlk.unlock();
	deadCV.notify_all();

}
//DONE
void ThreadParticleSystem::SpawnNewParticle() {
	Particle* temp;
	std::unique_lock<std::shared_mutex> dlk(deadKey);

	deadCV.wait(dlk);
	temp = deadParticles.back();
	deadParticles.pop_back();
	dlk.unlock();

	std::unique_lock<std::shared_mutex> alk(aliveKey);
	aliveCV.wait(alk);
	aliveParticles.emplace_back(temp);
	emitter.EmitParticle(*aliveParticles.back());
	alk.unlock();
}
//DONE
void ThreadParticleSystem::Draw(GLuint shader) {
	std::shared_lock<std::shared_mutex> alk(aliveKey);
	aliveCV.wait(alk);
	for (ParticleVecIterator it = aliveParticles.begin();
		it != aliveParticles.end(); it++) {
		(*it)->DrawParticle(shader);
	}
	alk.unlock();
}
//DONE
void ThreadParticleSystem::SetAliveSize() {
	//lock aliveKey, get size, unlock
	//lock sizeKey, set size, unlock
	int currentSize = 0;
	std::unique_lock<std::shared_mutex> alk(aliveKey);
	aliveCV.wait(alk);
	currentSize = aliveParticles.size();
	alk.unlock();
	aliveCV.notify_all();
	
	std::unique_lock<std::shared_mutex> slk(sizeKey);
	sizeCV.wait(slk);
	aliveSize = currentSize;
	slk.unlock();
	sizeCV.notify_all();

}
//DONE

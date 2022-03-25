#include "Emitter.h"
#include "Particle.h"
#include <cstdlib>

static float RAND_HALF() { //this is a wordy function name, replace with something actually typeable
	return (float)rand() / RAND_MAX - 0.5f;
}

Emitter::Emitter() : initialPVelocity(Vector3(0,1,0)),
initialPColour(Vector4(1,0,0,1)), initialPTransform(Vector3(0,0,0)),
initialPLife(4.0f), velVariation(Vector3()), colVariation(Vector4()), lifeVariation(0.0f) {
}

void Emitter::EmitParticle(Particle& particle) {
	if (!particle.pAlive) { //shouldn't need this but just to be sure not resetting particles that are already alive
		GenerateVariation();
		particle.pVelocity = initialPVelocity + velVariation;
		particle.pColour = initialPColour + colVariation;
		particle.pTransform = initialPTransform;
		particle.pLife = initialPLife + lifeVariation;
		particle.pAlive = true;
	}
}

void Emitter::GenerateVariation() {
	velVariation = Vector3(RAND_HALF(), RAND_HALF(), RAND_HALF());
	colVariation = Vector4(RAND_HALF() / 5.0f, RAND_HALF() / 5.0f, RAND_HALF() / 5.0f, 0.0f);
	lifeVariation = RAND_HALF();
}
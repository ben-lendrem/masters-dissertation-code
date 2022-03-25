#pragma once
#include "Vector4.h"

//is this needed?
//No??
//Nevermind it can be used


/*
Class: Emitter
Author: Ben Lendrem, Student No. 170624723
Stores attributes that new particles should have.
Function called that takes in a particle by reference
and changes it's attributes to what is required.
Handling of switch from alive list to dead list is done
in the particle system itself, although the switching of
the pAlive attribute is done here.



  Feesh   /"*._         _
	  .-*'`    `*-.._.-'/
	< * ))     ,       (
	  `*-._`._(__.--*"`.\
*/
class Particle;
struct DeltaUpdate {
	Vector3 deltaVel;
	Vector4 deltaCol;
	float	deltaLife;
};
class Emitter {
public:
	Emitter();
	void SetInitialVelocity(Vector3 inVel) { initialPVelocity = inVel; }
	void SetInitialColour(Vector4 inCol) { initialPColour = inCol; }
	void SetInitialTransform(Vector3 inTrans) { initialPTransform = inTrans; }
	void SetInitialLifeSpan(float inLife) { initialPLife = inLife; }
	void EmitParticle(Particle& particle);

	Vector3 GetInitialTransform() { return initialPTransform; }

protected:
	Vector3 initialPVelocity;
	Vector4 initialPColour;
	Vector3 initialPTransform;
	float initialPLife;

	Vector3 velVariation;
	Vector4 colVariation;
	float lifeVariation;

	void GenerateVariation();

	const int updateMethod = 0; //0, basic. 1, Multithreaded. 2, Compute. (For now just change in code rather than variable)
};
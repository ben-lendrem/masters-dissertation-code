#pragma once
/*
Class: Particle
Author: Ben Lendrem, Student No. 170624723
Shares functionality with SceneNode.h from nclgl library.
Difference being there is no mesh stored in the class, as the
mesh for every particle is just a single point.
Additionally, particles aren't going to have children, as they are
each the "child" of the particle system, and won't have anything else 
attached to it (at least for this system).
Essentially it's a mesh with some of the functionality of scenenode so
that variable data (colour, position) doesn't have to be buffered and can
just be sent to the shader via uniforms.



  Feesh   /"*._         _
	  .-*'`    `*-.._.-'/
	< * ))     ,       (
	  `*-._`._(__.--*"`.\
*/

#include "Mesh.h"
#include "ParticleSystem.h"
#include "ThreadParticleSystem.h"
class Particle : Mesh {
friend class Emitter;
friend class ParticleSystem;
friend class ThreadParticleSystem;
public:
	Particle();
	~Particle() {};
protected:
	void UpdateBasicParticle(float dt, DeltaUpdate d);
	void DrawParticle(GLuint shader);

	Vector3 pVelocity;
	Vector4 pColour;
	Vector3 pTransform;
	float pLife;
	bool pAlive;

	
};
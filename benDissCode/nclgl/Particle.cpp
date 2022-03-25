#include "Particle.h"
#include "Vector3.h"

#include <algorithm>
Particle::Particle() {
	/*Particle constructor takes in 0 parameters, as the attributes of the particle 
	(e.g. colour) are set at emission.
	*/
	type = GL_POINTS;
	numVertices = 1;
	vertices = new Vector3[numVertices];
	colours = new Vector4[numVertices];

	vertices[0] = Vector3(0, 0, 0);
	colours[0] = Vector4(1, 0, 0, 1); //Default red

	BufferData(); //Can just do here?

	pVelocity = Vector3(0, 0, 0);
	pColour = Vector4(1, 0, 0, 1);
	pTransform = Vector3();
	pLife = 4.0f;
	pAlive = false;
}


void Particle::UpdateBasicParticle(float dt, DeltaUpdate d) {
	//not sure if scaling everything by dt works well, will have to assess visually.
	
	//Change position by velocity
	pTransform += (pVelocity * dt);
	//Change velocity and apply delta (base change + randomness)
	pVelocity -= ((Vector3(0.0f, 0.2f, 0.0f) + d.deltaVel) * dt);
	//change colour by deltaCol
	pColour += (d.deltaCol);
	//change life by dt + randomness;
	
	/*
	maybe possible for deltaLife to reverse dt and actually increase life, leading to infinite particle life.
	Add min to guard against this.
	Haven't thoroughly thought through the maths of changing the delta's so might be possible that the speed and colour
	of particles changes wildly, although it would lead to interesting visuals so not too worried at the minute.
	Talk about in dissertation (future work includes creating proper checks so that the desired effect is reached?)
	*/
	pLife -= std::min((dt), 0.0f);

	//finally, check life left, if <= 0, pAlive is false
	if (pLife <= 0.0f) {
		pAlive = false;
	}
}

void Particle::DrawParticle(GLuint shader) {
	/*
	Shader is bound
	matrices for mvp transformation have been passed in
	need to pass in position (pTransform), colour (pColour)
	*/
	
	Matrix4 model = Matrix4::Translation(pTransform);
	glUniformMatrix4fv(
		glGetUniformLocation(shader, "modelMatrix"), 1, false, model.values);
	glUniform4fv(
		glGetUniformLocation(shader, "particleColour"), 1, (float*)&pColour);
	Draw();

}
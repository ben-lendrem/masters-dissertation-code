#pragma once

//I'm aware I've spelt Frustum wrong, but I've come too far to change it now :(

#include "Plane.h"
class SceneNode;
class Matrix4;

class Frustrum {
public:
	Frustrum(void) {};
	~Frustrum(void) {};

	void FromMatrix(const Matrix4& mvp);
	bool InisdeFrustrum(SceneNode& n);
protected:
	Plane planes[6];
};
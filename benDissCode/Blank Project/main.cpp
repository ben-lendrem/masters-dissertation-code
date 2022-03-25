#include "../nclgl/ComputeShader.h"
#include <iostream>
int main() {
	float testFlArr[10] = { 0.0, 4.9, 0.1, 6.6, 2.9, 4.4, 1.6, 4.6, 8.0, 5.4 };
	GLuint ssboTest;
	glGenBuffers(1, &ssboTest);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTest);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(testFlArr), testFlArr, GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboTest);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}
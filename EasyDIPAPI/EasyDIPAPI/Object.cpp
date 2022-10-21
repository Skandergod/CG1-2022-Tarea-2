#include "Object.h"
#include "shaders/Shader.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/transform2.hpp"
//static Object* instance;
//static unsigned int VBO, VAO, EBO;

//Object* Object::instance = nullptr;
unsigned int Object::VBO = 0;
unsigned int Object::VAO = 0;


Object::Object() {
	scale = glm::vec3(1.0f);
	translate = glm::vec3(0.0f);
	rotate = glm::vec3(0.0f);
	origin = glm::vec3 (0.0f, 0.0f, -8.0f);
	destiny = glm::vec3 (0.0f, 0.0f, 0.0f);

	Rotation(rotate);
	Scale(scale);
	Translation(translate);
	modelMatrix();
	view(origin, destiny);
}

Object::~Object() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Object::Bind()
{
	glBindVertexArray(VAO);
}

void Object::Init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertex.size(), &vertex[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//Shader::setMat4("MatrixView", matrixView);
	glBindVertexArray(0);
	//glEnable(GL_LINE_SMOOTH);
	
}
void Object::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size());
}

void Object::DrawLines() {
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexLines.size(), &vertexLines[0], GL_STATIC_DRAW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size());
}

void Object::DrawPoints() {
	glPointSize(4.0f);
	glDrawArrays(GL_POINTS, 0, vertex.size());
}

void Object::drawNormals() {
	glBegin(GL_LINES);
	for (int i = 0; i < normalCenter.size()-1; i++) {
		glVertex3f(normalCenter[i].x, normalCenter[i].y, normalCenter[i].z);
		//glVertex3f(normalCenter[i+1].x, normalCenter[i+1].y, normalCenter[i+1].z);
	}
	glEnd();
}

void Object::drawAntialiasing() {

	glDisable(GL_MULTISAMPLE);

}

void Object::Rotation(glm::vec3 t) {
	rotate = t;
	rotateMatrix = glm::mat4(1.0f);
	rotateMatrix = glm::rotate(rotateMatrix, t.x, glm::vec3(1, 0, 0));
	rotateMatrix = glm::rotate(rotateMatrix, t.y, glm::vec3(0, 1, 0));
	rotateMatrix = glm::rotate(rotateMatrix, t.z, glm::vec3(0, 0, 1));
}

void Object::Scale(glm::vec3 t) {
	scale = t;
	scaleMatrix = glm::scale(t);
}

void Object::Translation(glm::vec3 t) {
	translate = t;
	translateMatrix = glm::translate(t);
}

void Object::modelMatrix() {

	matrixModel = translateMatrix * scaleMatrix * rotateMatrix;

}

void Object::view(glm::vec3 origin, glm::vec3 eye) {

	look = glm::lookAt(eye, origin, glm::vec3(0.0f, 1.0f, 0.0f));
	matrixView = look;

}

void Object::proyec(float ratio) {
	//ratio width/height
	orthoMatrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 1000.0f);
	perspectiveMatrix = glm::perspective(45.0f, ratio, 0.1f, 1000.0f);
	matrixProyec = perspectiveMatrix;

}
#pragma once

#include "EDpch.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>



class Object {
	//static Object* instance;
	
public:
	glm::vec3 scale;
	glm::vec3 rotate;
	glm::vec3 translate;
	glm::mat4 rotateMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translateMatrix;
	glm::mat4 look;
	glm::mat4 proyecMatrix;
	glm::mat4 orthoMatrix;
	glm::mat4 matrixModel;
	glm::mat4 matrixProyec;
	glm::mat4 matrixView;
	glm::mat4 perspectiveMatrix;
	glm::vec3 origin;
	glm::vec3 destiny;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vertexLines;
	std::vector<glm::vec3> normalCenter;
	std::map<int, std::vector<glm::vec3>> normalmap;
	static unsigned int VBO, VAO;
	Object();
	~Object();
	void Draw();
	void Bind();
	void Init();
	void Rotation(glm::vec3 t);
	void Translation(glm::vec3 t);
	void Scale(glm::vec3 t);
	void modelMatrix();
	void DrawLines();
	void DrawPoints();
	void view(glm::vec3 origin, glm::vec3 eye);
	void proyec(float ratio);
	void drawNormals();
	void drawAntialiasing();
	//void Object();
	//static Object* Instance();
};




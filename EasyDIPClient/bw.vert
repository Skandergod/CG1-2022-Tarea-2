#version 330 core

layout (location = 0) in vec3 pos;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 rotMat;
uniform mat4 transMat;
uniform mat4 scaleMat;
uniform mat4 proyecMatrix;
void main()
{
	gl_Position = proyecMatrix*viewMatrix* modelMatrix* vec4(pos, 1.0f);
}
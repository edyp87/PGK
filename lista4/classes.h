/**
	@name:     pgk_lista_4
	@author:   Marek Jenda
	@revision: 1.0
	@date:     10.12.2013
	@file:     classess.h
	@depends:  none
**/

#ifndef _CLASSES_H_
#define _CLASSES_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stack>
#include "GL/glew.h" // wymagane dla GLFW
#include "GL/glfw.h" // GLFW - biblioteka open source do tworzenia okien w z zawartoscia OpenGL, API
#include <glm-0.9.4.0/glm/glm.hpp>
#include <glm-0.9.4.0/glm/ext.hpp>
#include <shaders.hpp>


inline float DegToRad(float fAngDeg);
glm::mat3 RotateX(float fAngDeg);
glm::mat3 RotateY(float fAngDeg);
glm::mat3 RotateZ(float fAngDeg);

class MatrixStack
{
public:
	glm::mat4 m_currMat;
	std::stack<glm::mat4> m_matrices;

	MatrixStack();
	const glm::mat4 &Top();
	void RotateX(float fAngDeg);
	void RotateY(float fAngDeg);
	void RotateZ(float fAngDeg);
	void Scale(const glm::vec3 &scaleVec);
	void Translate(const glm::vec3 &offsetVec);
	void Push(); 
	void Pop(); 
};

class Cube {
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint &MatrixID;
	glm::mat4 MVP;
public:
	Cube(GLuint &mID);
	void draw(glm::mat4 aChange);
	void transform();
	~Cube();
};

class Human {
public:
	Cube cube;
	MatrixStack matrixStack;
	GLfloat move;
	GLfloat rotate;
	GLfloat initx, inity;
	GLfloat speed;
	GLfloat leftLegAngle, rightLegAngle;
	bool leftLegUp, leftLegDown, rightLegUp, rightLegDown;

	Human(GLuint &mID);
	void draw(GLfloat offset_x, GLfloat offset_y, glm::mat4 &MVPa);
};

class GLclass {
public:
	GLuint win_width;
	GLuint win_height;
	GLuint programID;
	GLuint VertexArrayID;
	GLuint MatrixID;
	static GLfloat offset_x, offset_y;
	GLfloat mouse_speed, mouse_change_x, mouse_change_y;
	GLdouble newtime, oldtime;
	static GLuint camera_type;

	GLclass(GLuint a_win_width, GLuint a_win_height);
	void opengl_init();
	int glfw_init();
	static void GLFWCALL key_press(int key, int action);
	void mouse();
};

class World {
public:
	glm::mat4 Camera;
	GLfloat fp_x, fp_y, fp_rotate, fp_offset;
	GLclass &glclass;
	static GLuint max_humans;
	static GLuint camera_human;

	World(GLclass &_glclass, GLuint _max_humans);
	void getCameraCoordinates(GLfloat x, GLfloat y, GLfloat rotate, GLfloat offset);
};



















const GLfloat g_vertex_buffer_floor[] = {
	-1.0f, 0.0f,  1.0f,
	 1.0f, 1.0f, -1.0f
};

const GLfloat g_color_buffer_floor[] = {
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
};


const GLfloat g_vertex_buffer_data[] = { 
	/*-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f*/
	
	-1.0f, 0.0f,-1.0f,
	-1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 0.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 0.0f, 1.0f,
	-1.0f, 0.0f,-1.0f,
	 1.0f, 0.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f, 0.0f,-1.0f,
	-1.0f, 0.0f,-1.0f,
	-1.0f, 0.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,
	-1.0f, 0.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f, 0.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
const GLfloat g_color_buffer_data[] = { 
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

#endif // _CLASSES_H_
/**
	@name:     pgk_lista_4
	@author:   Marek Jenda
	@revision: 1.0
	@date:     10.12.2013
	@file:     lista4.h
	@depends:  none
**/

#ifndef _LISTA4_H_
#define _LISTA4_H_

#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h" // wymagane dla GLFW
#include "GL/glfw.h" // GLFW - biblioteka open source do tworzenia okien w z zawartoscia OpenGL, API
#include <glm-0.9.4.0/glm/glm.hpp>
#include <glm-0.9.4.0/glm/ext.hpp>
#include <shaders.hpp>
#include <vector>


extern const GLfloat g_color_buffer_data[3*36];
extern const GLfloat g_vertex_buffer_data[3*36];

#endif //_LISTA4_H_
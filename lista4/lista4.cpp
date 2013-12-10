/**
	@name:     pgk_lista_4
	@author:   Marek Jenda
	@revision: 1.0
	@date:     10.12.2013
	@file:     lista4.cpp (main file)
	@depends:  lista4.h, classes.h, classes.cpp
**/


#include "lista4.h"
#include "classes.h"

int main( void )
{
	srand( static_cast<unsigned int>( time(NULL) ) );
	glm::mat4 MVP;
	std::vector<Human *> humans;

	// GLclass must be construct and init first (before we construct other classes)!
	GLclass glclass(1024, 896);
	glclass.glfw_init();
	glclass.opengl_init();

	World world(glclass, 50);
	Cube cube(glclass.MatrixID);
	for(unsigned int i = 0; i < world.max_humans; i++) 
		humans.push_back(new Human(glclass.MatrixID));

	do{
		world.getCameraCoordinates(
			humans[world.camera_human]->initx,
			humans[world.camera_human]->inity,
			humans[world.camera_human]->rotate,
			humans[world.camera_human]->move
		);
		glclass.mouse();
		glclass.newtime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(glclass.programID);

		if(glclass.newtime - glclass.oldtime > 0.01f)
			glclass.oldtime = glclass.newtime;
		
		// here we draw humans ...
		for(unsigned int i = 0; i < world.max_humans; i++) {
			if(glclass.camera_type != 1 && glclass.camera_type != 2)
				MVP =  world.Camera * glm::rotate(glclass.offset_x, 0.0f, 1.0f, 0.0f);
			else
				MVP =  world.Camera;
			humans[i]->draw(0, 0, MVP);
		}
		// ... here surrounding world
		cube.draw(MVP * glm::translate(0.0f, -6.0f, 0.0f) * glm::scale(300.0f, 300.0f, 300.0f));

		glfwSwapBuffers();
	} while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) ); //end:dowhile

	// tiding
	glDeleteProgram(glclass.programID);
	glDeleteVertexArrays(1, &glclass.VertexArrayID);
	glfwTerminate();

	for(unsigned int i = 0; i < world.max_humans; i++)
		delete humans[i];
		
	return 0;
}




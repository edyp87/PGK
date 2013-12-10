#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h" // wymagane dla GLFW
#include "GL/glfw.h" // GLFW - biblioteka open source do tworzenia okien w z zawartoscia OpenGL, API
#include <shaders.hpp>

int glfw_init();
void opengl_init();
void display();

GLuint programID;
GLuint VertexArrayID;
GLuint vertexbuffer;


int main() {
	if(!glfw_init()) return -1;
	opengl_init();
	do {
		display();
	} while(glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

}

int glfw_init() {
	//sprawdzamy, czy mozemy odpalic GLFW
	if( !glfwInit() ) { //inicjalizowanie glfw
		fprintf( stderr, "Nie udalo sie uruchomic GLFW\n" );
		return -1;
	}
	
	// void glfwOpenWindowHint( int target, int hint )
	// Funkcja ustawia wlasciwosci dla okna ktore ma by otwarte. Aby zostaly one
	// zarejestrowane, musza byc ustawione przed otwarciem okna glfwOpenWindow.
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // antialiasing = 4x
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // wersja OpenGL = 3.3
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // nie chcemy starego OpenGL

	// otwieramy okno
	// glfwOpenWindow ( int width, int height, 
	//                  int redbits, int greenbits, int bluebits, int alphabits,
	//			        int dephbits, int stencilbits, int mode )
	if( !glfwOpenWindow( 1900, 1000, 0,0,0,0, 32,0, GLFW_WINDOW ) ) {
		fprintf(stderr, "Nie udalo sie otworzyc okna GLFW\n" );
		glfwTerminate(); // zabija wszystkie watki zwiazane z glfw
		return -1;
	}

	// odpalamy glew
	glewExperimental=true;
	if( glewInit() != GLEW_OK ) {
		fprintf(stderr, "Nie udalo sie uruchomic GLEW\n" );
		return -1;
	}

	// nadajemy tytul belce glownej
	glfwSetWindowTitle( "Trojkat" );

	//  nasluchujemy czy zostal wcisniety jakis klawisz.
	// GLFW_STICKY_KEYS loguje wszystkie operacje wcisniecia i wysyla je do glfwGetKey (dodaje do jakiejs kolejki)
	glfwEnable( GLFW_STICKY_KEYS );
}

void opengl_init() {
	//ustawiamy kolor tla (pomaranczowy)
	// red, green, blue, alpha
	glClearColor(0.98f, 0.625f, 0.12f, 1.0f);

	// tworzymy obiekt VertexArray. Zawsze po otwarciu okna, ale przed rysowaniem zawartosci
	glGenVertexArrays(1, &VertexArrayID); // tworzymy
	glBindVertexArray(VertexArrayID);     // i ustawiamy jako aktualny

	//shadery
	programID = LoadShaders( "vertexshader", "fragmentshader" );

	// ustawiamy wierzcholki
	/*                   ^ (0,1)
	                     |
	  (-1,0)			 |(0,0)           (1,0)
	  <------------------------------------>
	  					 |
						 |
						 v (0,-1)
	*/

	static const GLfloat g_vertex_buffer_data[] = {
		-0.8f, -0.8f, 0.0f,// 1.0f,
		 0.8f, -0.8f, 0.0f,// 1.0f,
		 0.0f,  0.8f, 0.0f, //1.0f,
	};

	// tworzymy 1 bufor wierzcholkow
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void display() {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // czyscimy ekran

		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
			);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers();
}
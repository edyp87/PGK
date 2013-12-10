#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include "GL/glew.h" 
#include "GL/glfw.h"
#include <shaders.hpp>
#include <math.h>

// PROTOTYPY FUNKCJI 
int glfw_init();
void opengl_init();
void display();
void alocate_buffers();
void board();
void build_pegs(int);
void build_hints();
void cursor(int);
bool check_solution();
void GLFWCALL key_press(int, int);
void display_circle(char *emotion);
void display_face(char *emotion);

// ZMIENNE
GLuint programID;
GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint vertexcolor;
GLuint line;
GLuint pegsbuffer;
GLuint pegscolorbuffer;
GLuint hintsbuffer;
GLuint hintscolorbuffer;
GLuint cursorbuffer;
GLuint colorbuffer;
	
struct state_str {
	unsigned char cursor;
	unsigned char round;
	int points;
} state;
unsigned char solution[4];
extern GLfloat board_data[51];
extern GLfloat g_vertex_color[51];
extern GLfloat cursor_color[21];
GLfloat pegs_color[4*10*9*2];
GLfloat hints_data[4*10*9*2];
GLfloat hints_color[4*10*9*2];
GLfloat pegs_data[4*10*9*2];
GLfloat cursor_data[21];
char pegs_colored[40];
bool victory = false;

// ***************************************************************

int main() {
	state.points = 0;
	state.round = 1;
	state.cursor = 0;

	for(int i = 0; i < 720; i++) {
		pegs_color[i] = 1.0f;
		hints_color[i] = 1.0f;
	}

	srand (time(NULL));
	solution[0] = rand() % 6 + 1;
	solution[1] = rand() % 6 + 1;
	solution[2] = rand() % 6 + 1;
	solution[3] = rand() % 6 + 1;

	printf("Keys:\n"
		"1: Blue\n"
		"2: Green\n"
		"3: Yellow\n"
		"4: Orange\n"
		"5: Red\n"
		"6: Violet\n\n");

	printf("Solution is: %d, %d, %d, %d \n\n", 
				solution[0], 
				solution[1], 
				solution[2], 
				solution[3]
			);

	if(!glfw_init()) return -1;
	opengl_init();

	alocate_buffers();

	do {
		if(state.round == 11 || victory) {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glUseProgram(programID);
			if(victory)
				display_face("smile");
			else display_face("sad");
			glfwSwapBuffers();
		} else 
			display();
	} while(glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetKey( 'Q' ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

}

int glfw_init() {
	if( !glfwInit() ) {
		fprintf( stderr, "Nie udalo sie uruchomic GLFW\n" );
		return -1;
	}
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); 
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); 
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	if( !glfwOpenWindow( 768, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) ) {
		fprintf(stderr, "Nie udalo sie otworzyc okna GLFW\n" );
		glfwTerminate(); 
		return -1;
	}

	glewExperimental=true;
	if( glewInit() != GLEW_OK ) {
		fprintf(stderr, "Nie udalo sie uruchomic GLEW\n" );
		return -1;
	}
	glfwSetWindowTitle( "Mastermind" );

	//glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_KEY_REPEAT );
	glfwSetKeyCallback(key_press);
}

void opengl_init() {
	glClearColor(0.32f, 0.54f, 0.81f, 1.0f);
	glGenVertexArrays(1, &VertexArrayID); 
	glBindVertexArray(VertexArrayID);    
	programID = LoadShaders( "vertexshader", "fragmentshader" );

}

void alocate_buffers() {
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(board_data), board_data, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexcolor);
	glBindBuffer(GL_ARRAY_BUFFER, vertexcolor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_color), g_vertex_color, GL_STATIC_DRAW);

	glGenBuffers(1, &pegsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pegsbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pegs_data), pegs_data, GL_STREAM_DRAW);

	glGenBuffers(1, &pegscolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pegscolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pegs_color), pegs_color, GL_STREAM_DRAW);

	glGenBuffers(1, &hintsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,hintsbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hints_data), hints_data, GL_STREAM_DRAW);

	glGenBuffers(1, &hintscolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,hintscolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hints_color), hints_color, GL_STREAM_DRAW);

	glGenBuffers(1, &cursorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cursorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cursor_data), cursor_data, GL_STREAM_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cursor_color), cursor_color, GL_STREAM_DRAW);

}

void display() {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		glUseProgram(programID);

		board();
		build_pegs(state.round);
		build_hints();
		cursor(0);

		glfwSwapBuffers();
}

void board() {
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
			);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexcolor);
		glVertexAttribPointer(
			1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
			);

		glDrawArrays(GL_TRIANGLES, 0, 12);
		glDrawArrays(GL_LINE_STRIP, 12, 5);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
}

void build_pegs(int n) {
	GLfloat left, down = -0.95f;
	int index = -1;
	
	for(int i = 0; i < n; i++) { // dla kazdego rzedu
		left = -0.6f;
		for(int j = 0; j < 4; j++) { // dla kazdego pola
			pegs_data[++index]   = left + 0.02f; // 1szy trojkat
			pegs_data[++index] = down + 0.02f;
			pegs_data[++index] = 0.0f;
			pegs_data[++index] = left + 0.02f;
			pegs_data[++index] = down + 0.17f;
			pegs_data[++index] = 0.0f;
			pegs_data[++index] = left + 0.22f;
			pegs_data[++index] = down + 0.02f;
			pegs_data[++index] = 0.0f;

			pegs_data[++index]   = left + 0.22f; // 2gi trojkat
			pegs_data[++index] = down + 0.02f;
			pegs_data[++index] = 0.0f;
			pegs_data[++index] = left + 0.22f;
			pegs_data[++index] = down + 0.17f;
			pegs_data[++index] = 0.0f;
			pegs_data[++index] = left + 0.02f;
			pegs_data[++index] = down + 0.17f;
			pegs_data[++index] = 0.0f;
			left += 0.24;

		}
		down += 0.19f;
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pegsbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pegs_data), pegs_data);
	glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, pegscolorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pegs_color), pegs_color);
	glVertexAttribPointer(
			1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 360);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void build_hints() {
	GLfloat left, down = -0.95f;
	int index = -1;

	for(int i = 0; i < state.round - 1; i++) { // dla kazdego rzedu
		down = i * 0.19 + (-0.95);
		for(int j = 0; j < 2; j++) { // dla kazdego podrzedu
			left = 0.36f;
			for(int k = 0; k < 2; k++) { // dla kazdego pola
				hints_data[++index] = left + 0.01f; // 1szy trojkat
				hints_data[++index] = down + 0.01f;
				hints_data[++index] = 0.0f;
				hints_data[++index] = left + 0.01f;
				hints_data[++index] = down + 0.085f;
				hints_data[++index] = 0.0f;
				hints_data[++index] = left + 0.11f;
				hints_data[++index] = down + 0.01f;
				hints_data[++index] = 0.0f;

				hints_data[++index] = left + 0.11f; // 2gi trojkat
				hints_data[++index] = down + 0.01f;
				hints_data[++index] = 0.0f;
				hints_data[++index] = left + 0.11f;
				hints_data[++index] = down + 0.085f;
				hints_data[++index] = 0.0f;
				hints_data[++index] = left + 0.01f;
				hints_data[++index] = down + 0.085f;
				hints_data[++index] = 0.0f;
				left += 0.12;
			}
			down += 0.095f;
		}
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hintsbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(hints_data), hints_data);
	glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hintscolorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(hints_color), hints_color);
	glVertexAttribPointer(
			1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 360);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);



}

void cursor(int move) {
	state.cursor = (state.cursor + move) % 4;

	cursor_data[0]  = state.cursor * 0.24 - 0.59; cursor_data[1]  = state.round * 0.19 - 0.18 - 0.95; cursor_data[2]  = 0.0f;
	cursor_data[3]  = state.cursor * 0.24 - 0.59; cursor_data[4]  = state.round * 0.19 - 0.01 - 0.95; cursor_data[5]  = 0.0f;
	cursor_data[6]  = state.cursor * 0.24 - 0.37; cursor_data[7]  = state.round * 0.19 - 0.01 - 0.95; cursor_data[8]  = 0.0f;
	cursor_data[9]  = state.cursor * 0.24 - 0.37; cursor_data[10] = state.round * 0.19 - 0.18 - 0.95; cursor_data[11] = 0.0f;
	cursor_data[12] = state.cursor * 0.24 - 0.59; cursor_data[13] = state.round * 0.19 - 0.18 - 0.95; cursor_data[14] = 0.0f;

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cursorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cursor_data), cursor_data);
	glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cursor_color), cursor_color);
	glVertexAttribPointer(
			1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glDrawArrays(GL_LINE_STRIP, 0, 5);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
}

bool check_solution() {
	bool res;
	const int red = 2, black = 1;
	int red_c = 0, black_c = 0;
	int pos;
	int hint[4] = {0, 0, 0, 0};
	int blackened;

	res =   pegs_colored[(state.round - 1) * 4    ] &&
			pegs_colored[(state.round - 1) * 4 + 1] &&
			pegs_colored[(state.round - 1) * 4 + 2] &&
			pegs_colored[(state.round - 1) * 4 + 3];

	if(res) {
		for(int i = 0; i < 4; i++)  {
			blackened = -1;
			for(int j = 0; j < 4; j++) {
				if(pegs_colored[(state.round - 1) * 4 + i] == solution[j])
					if(i == j) { 
						hint[j] = red;
						if(blackened != -1)
							hint[blackened] = 0;
						break;
					}
					else if(!hint[j] && (blackened == -1) ) { 
						hint[j] = black;
						blackened = j;
					}
			}
		}

		for(int i = 0; i < 4; i++) {
			if(hint[i] == red) red_c++;
			else if(hint[i] == black) black_c++;
		}

		if( red_c == 4) victory = true;
		pos = (state.round - 1) * 4*18 - 1;
		while(red_c > 0) {
			for(int i = 0; i < 18; i += 3) {
				hints_color[++pos] = 0.94f;
				hints_color[++pos] = 0.19f;
				hints_color[++pos] = 0.039f;
			}
			red_c--;
		}
		while(black_c > 0) {
			for(int i = 0; i < 18; i += 3) {
				hints_color[++pos] = 0.0f;
				hints_color[++pos] = 0.0f;
				hints_color[++pos] = 0.0f;
			}
			black_c--;
		}

	}
	return res;
}

void GLFWCALL key_press(int key, int action) {
	int peg, j;
	peg = (state.round - 1) * 4 * 18 + state.cursor * 18 - 1;
	if(!victory && state.round < 11)
	switch(key) {
	
	case '1' : // niebieski
		pegs_colored[(state.round - 1) * 4 + state.cursor] = 1;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  0.47f;
			pegs_color[++peg] =  0.83f;
			pegs_color[++peg] =  0.94f;
		}
		break;
	case '2' : // zielony
		pegs_colored[(state.round - 1) * 4 +  state.cursor] = 2;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  0.47f;
			pegs_color[++peg] =  0.94f;
			pegs_color[++peg] =  0.59f;
		}
		break;
	case '3' : // zolty
		pegs_colored[(state.round - 1) * 4 +  state.cursor] = 3;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  0.95f;
			pegs_color[++peg] =  1.00f;
			pegs_color[++peg] =  0.50f;
		}
		break;
	case '4' : // pomaranczowy
		pegs_colored[(state.round - 1) * 4 +  state.cursor] = 4;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  1.00f;
			pegs_color[++peg] =  0.79f;
			pegs_color[++peg] =  0.50f;
		}
		break;
	case '5' : // czerwony
		pegs_colored[(state.round - 1) * 4 +  state.cursor] = 5;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  1.00f;
			pegs_color[++peg] =  0.54f;
			pegs_color[++peg] =  0.50f;
		}
		break;
	case '6' : // fioletowy
		pegs_colored[(state.round - 1) * 4 +  state.cursor] = 6;
		for(j = 0; j < 18; j += 3) {
			pegs_color[++peg] =  0.79f;
			pegs_color[++peg] =  0.50f;
			pegs_color[++peg] =  1.00f;
		}
		break;
	case GLFW_KEY_RIGHT:
		if(action) state.cursor++;
		break;
	case GLFW_KEY_LEFT:
		if(action) state.cursor--;
		break;
	case GLFW_KEY_ENTER:
		if(action & check_solution()) {
			state.round++;
			state.cursor = 0;
		}
		break;



	}
}


void display_face(char *emotion) {
	float mult, shift;
	if(emotion == "smile") {
		mult = -1;
		shift = 0;	
	} else {
		mult =  1;
		shift = 0.8;
	}
	GLfloat circle_data[130];
	GLuint circlebuffer;
	GLfloat x = -0.4f;
	int i = -1;

	while(x <= 0.5f) {
			circle_data[++i] = x; 
			circle_data[++i] = mult*sqrt(0.5f*0.5f - x*x) - shift;
			circle_data[++i] = 0.0f;
			x += 0.03f;
	}
	
	glGenBuffers(1, &circlebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle_data), circle_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
	glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 28);

	glDisableVertexAttribArray(0);

	GLfloat squares_data[] = {
		0.3f, 0.3f, 0.0f,
		0.3f, 0.5f, 0.0f,
		0.1f, 0.5f, 0.0f,

		0.1f, 0.5f, 0.0f,
		0.1f, 0.3f, 0.0f,
		0.3f, 0.3f, 0.0f,

		-0.3f, 0.3f, 0.0f,
		-0.3f, 0.5f, 0.0f,
		-0.1f, 0.5f, 0.0f,

		-0.1f, 0.5f, 0.0f,
		-0.1f, 0.3f, 0.0f,
		-0.3f, 0.3f, 0.0f,
	};
	GLuint squaresbuffer;

	glGenBuffers(1, &squaresbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, squaresbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squares_data), squares_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, squaresbuffer);
	glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(0);


}

GLfloat board_data[] = {
		// glowna ramka
		-0.6f, -0.95f, 0.0f,
		-0.6f,  0.95f, 0.0f,
		 0.36f,  0.95f, 0.0f,

		 0.36f,  0.95f, 0.0f,
		 0.36f, -0.95f, 0.0f,
	    -0.6f, -0.95f, 0.0f,

		// ramka z podpowiedziami
		 0.36f,  0.95f, 0.0f, 
		 0.6f,   0.95f, 0.0f,
		 0.36f, -0.95f, 0.0f,

		 0.36f, -0.95f, 0.0f,
		 0.6f,  -0.95f, 0.0f,
	     0.6f,  0.95f,  0.0f, 

		 // linia wokol glownej ramki
		 -0.6f, -0.95f, 0.0f,
		 -0.6f,  0.95f, 0.0f,
		  0.6f,  0.95f, 0.0f,
		  0.6f, -0.95f, 0.0f,
		 -0.6f, -0.95f, 0.0f,

		 // 1 linia
		// 0.55f, 


	};

	GLfloat g_vertex_color[] = {
		// glowna ramka
		0.32f,  0.90f, 0.35f,
		0.32f,  0.81f, 0.35f,
		0.32f,  0.81f, 0.35f,

		0.32f,  0.81f, 0.35f,
		0.32f,  0.81f, 0.35f,
		0.32f,  0.81f, 0.35f,

		// ramka z podpowiedziami
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,

		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f, 

		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
		0.69f,  0.90f, 0.52f,
	};

	GLfloat cursor_color[21] = {
		0.89f, 0.90f, 0.52f,
		0.89f, 0.90f, 0.52f,
		0.89f, 0.90f, 0.52f,
		0.89f, 0.90f, 0.52f,
		0.89f, 0.90f, 0.52f

	};
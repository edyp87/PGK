#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include "GL/glew.h" 
#include "GL/glfw.h"
#include <shaders.hpp>
#include <math.h>
#include "lista3.h"


int main() {
	if(!glfw_init()) return -1;
	opengl_init();
	init_objects();
	alocate_buffers();
	mouse_change = paddle_speed*(win_width - float(xpos));
	do {
		display();
	} while(glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetKey( 'Q' ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );
	delete_buffers();
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
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE ) ;

	if( !glfwOpenWindow( win_width, win_height, 0,0,0,0, 32,0, GLFW_WINDOW ) ) {
		fprintf(stderr, "Nie udalo sie otworzyc okna GLFW\n" );
		glfwTerminate(); 
		return -1;
	}

	glewExperimental=true;
	if( glewInit() != GLEW_OK ) {
		fprintf(stderr, "Nie udalo sie uruchomic GLEW\n" );
		return -1;
	}
	glfwSetWindowTitle( "Arkanoid" );

	//glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_KEY_REPEAT );
	glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(win_height/2, win_width/2);
	glfwGetMousePos(&xpos, &ypos);
}

void opengl_init() {
	glClearColor(0.32f, 0.54f, 0.81f, 1.0f);
	glGenVertexArrays(1, &VertexArrayID); 
	glBindVertexArray(VertexArrayID);    
	programID = LoadShaders( "vertexshader.shader", "fragmentshader.shader" );

}

void alocate_buffers() {
	offset = glGetUniformLocation(programID, "offset");
	uniform_color = glGetUniformLocation(programID, "ucolor");

	glGenBuffers(1, &hball_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, hball_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball_vertex), ball_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &hball_color);
	glBindBuffer(GL_ARRAY_BUFFER, hball_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball_color), ball_color, GL_STATIC_DRAW);

	glGenBuffers(1, &hhexagon_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, hhexagon_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagon_vertex), hexagon_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &hhexagon_color);
	glBindBuffer(GL_ARRAY_BUFFER, hhexagon_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagon_color), hexagon_color, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex), square_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_color);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_color), square_color, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_front_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_front_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_front_vertex), square_front_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_front_color);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_front_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_front_color), square_front_color, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_paddle_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_paddle_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_paddle_vertex), square_paddle_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &hsquare_paddle_color);
	glBindBuffer(GL_ARRAY_BUFFER, hsquare_paddle_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_paddle_color), square_paddle_color, GL_STATIC_DRAW);
}

void delete_buffers() {
	glDeleteProgram(programID);
	glDeleteBuffers(1, &hball_vertex);
	glDeleteBuffers(1, &hball_color);
	glDeleteBuffers(1, &hhexagon_vertex);
	glDeleteBuffers(1, &hhexagon_color);
	glDeleteBuffers(1, &hsquare_vertex);
	glDeleteBuffers(1, &hsquare_color);
	glDeleteBuffers(1, &hsquare_front_vertex);
	glDeleteBuffers(1, &hsquare_front_color);
	glDeleteBuffers(1, &hsquare_paddle_vertex);
	glDeleteBuffers(1, &hsquare_paddle_color);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void display() {
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glUseProgram(programID);
	if(play) {
		draw_background();
		draw_ball();
		blocks();
		draw_paddle();
		glfwSwapBuffers();
	} else {
		draw_background();
		display_face("sad");
		glfwSwapBuffers();
	}
}

void init_objects() {
	init_ball(0.02f, 0.0f, 0.0f);
	init_hexagon(0.1f, 0.0f, 0.0f);
	init_square(-1, 1, 0.05, 0.2, square_vertex); // back squares
	init_square(-0.999, 0.999, 0.048, 0.199, square_front_vertex); // front squares
	init_square(-1.0, -0.952, 0.048, 0.35, square_paddle_vertex); // paddle

	for(int i = 0; i < 4*10; i++) {
		block_exist[i] = 1;
	}
}

void blocks() {
	color_object(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < 10; i++) 
		if(block_exist[0 + i])
			draw_square(i*0.2f, 0, hsquare_vertex, hsquare_color);
	for(int i = 0; i < 10; i++) 
		if(block_exist[10 + i])
			draw_square(i*0.2f, -0.05f, hsquare_vertex, hsquare_color);
	for(int i = 0; i < 10; i++) 
		if(block_exist[20 + i])
			draw_square(i*0.2f, -0.1f, hsquare_vertex, hsquare_color);
	for(int i = 0; i < 10; i++) 
		if(block_exist[30 + i])
			draw_square(i*0.2f, -0.15f, hsquare_vertex, hsquare_color);

	color_object(0.57f, 0.96f, 0.47f);
	for(int i = 0; i < 10; i++) 
		if(block_exist[0 + i])
			draw_square(i*0.2f, 0, hsquare_front_vertex, hsquare_front_color);
	color_object(0.47f, 0.76f, 0.96f);
	for(int i = 0; i < 10; i++) 
		if(block_exist[10 + i])
			draw_square(i*0.2f, -0.05f, hsquare_front_vertex, hsquare_front_color);
	color_object(0.96f, 0.92f, 0.47f);
	for(int i = 0; i < 10; i++) 
		if(block_exist[20 + i])
			draw_square(i*0.2f, -0.1f, hsquare_front_vertex, hsquare_front_color);
		color_object(0.96f, 0.47f, 0.47f);
	for(int i = 0; i < 10; i++) 
		if(block_exist[30 + i])
			draw_square(i*0.2f, -0.15f, hsquare_front_vertex, hsquare_front_color);
	color_object(0.0f, 0.0f, 0.0f);
}

void init_ball(float r, float cx, float cy) {
	int num_segments = 30;
	int vert = 0;
	
	ball_color [vert  ] = 0.32f;
	ball_vertex[vert++] = cx;
	ball_color [vert  ] = 0.90f;
	ball_vertex[vert++] = cy;
	ball_color [vert  ] = 0.35f;
	ball_vertex[vert++] = 0.0f;

	for(int i = 0; i < 32; i++) 
	{ 
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); //get the current angle 

		float x = r * cosf(theta) * float(win_height)/float(win_width);
		float y = r * sinf(theta);

		ball_color [vert  ] = 1.0f;
		ball_vertex[vert++] = x + cx;
		ball_color [vert  ] = 1.0f;
		ball_vertex[vert++] = y + cy;
		ball_color [vert  ] = 1.0f;
		ball_vertex[vert++] = 0.0f;

	} 

}

void draw_ball() {
	double newtime = glfwGetTime();
	if(newtime - oldtime > 0.01f) {
		count_fps();
		add_offset();
		oldtime = newtime;
	}
	move_object(offset_ball_x, offset_ball_y);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hball_vertex);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hball_color);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 33);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	move_object(0, 0);
}

void init_square(GLfloat x, GLfloat y, GLfloat height, GLfloat width, GLfloat* vertex) {
	vertex[0] = x;
	vertex[1] = y;
	vertex[2] = 0;
	vertex[3] = x + width;
	vertex[4] = y;
	vertex[5] = 0;
	vertex[6] = x + width;
	vertex[7] = y - height;
	vertex[8] = 0;
	vertex[9] = x;
	vertex[10] = y - height;
	vertex[11] = 0;
}

void draw_square(GLfloat offsetx, GLfloat offsety, GLuint hvertex, GLuint hcolor) {
	move_object(offsetx, offsety);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hvertex);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hcolor);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	move_object(0, 0);
}

void draw_paddle() {
	int newxpos;
	glfwGetMousePos(&newxpos, &ypos);
	paddle_offset += mouse_change - paddle_speed*(win_width - float(newxpos));
	if(paddle_offset > 1.65) paddle_offset = 1.65;
	if(paddle_offset < 0.0) paddle_offset = 0.00;
	mouse_change = paddle_speed*(win_width - float(newxpos));

	color_object(0.94f, 0.47f, 0.96f);
	draw_square(paddle_offset, 0.0, hsquare_paddle_vertex, hsquare_paddle_color);
	color_object(0.0f, 0.0f, 0.0f);
}

void init_hexagon(float r, float cx, float cy) {
	int num_segments = 6;
	int vert = 0;
	for(int i = 0; i < 7; i++) 
	{ 
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); //get the current angle 

		float x = r * cosf(theta) * float(win_height)/float(win_width);
		float y = r * sinf(theta);

		hexagon_color [vert  ] = 0.41f;
		hexagon_vertex[vert++] = x + cx;
		hexagon_color [vert  ] = 0.63f;
		hexagon_vertex[vert++] = y + cy;
		hexagon_color [vert  ] = 0.97f;
		hexagon_vertex[vert++] = 0.0f;
	} 
}

void draw_hexagon(GLfloat offset_x, GLfloat offset_y) {
	double timecurr = glfwGetTime();
	move_object(offset_x, offset_y);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hhexagon_vertex);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hhexagon_color);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0
		);

	glDrawArrays(GL_LINE_STRIP, 0, 7);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	move_object(0, 0);
}

void draw_background() {
	int shift = 0;
	GLfloat move_y = 0;
	GLfloat move_x = 0;
	do {
		if(shift) {
			move_y = 0;
			shift = 0;
		} else {
			move_y = 0.085;
			shift = 1;
		}
		do  {
			draw_hexagon(move_x, move_y);
			draw_hexagon(move_x, -move_y);
			draw_hexagon(-move_x, move_y);
			draw_hexagon(-move_x, -move_y);
			move_y += 0.17;
		} while (move_y < 1.2f);
		move_x += 0.135f;
	} while (move_x < 1.2f);
}

void move_object(GLfloat x, GLfloat y) {
	glUniform2f(offset, x, y);
}

void color_object(GLfloat r, GLfloat g, GLfloat b) {
	glUniform3f(uniform_color, r, g, b);
}

void add_offset() {
	if(offset_ball_y <= -0.98f) play = 0;
	if(angle >= 90 && angle <= 270 && paddle_colide()) {
		GLfloat relative_intersect =  ((paddle_offset + (0.35 / 2)) - (offset_ball_x + 1)) / (0.35 / 2);
		if(relative_intersect < 0)
			relative_intersect *= (-1);
		
		if(angle >= 180)
			angle = 360 - relative_intersect * 70.0f;
		else 
			angle = relative_intersect * 70.0f;
	} 
	if(offset_ball_x >= 0.98f || offset_ball_x <= -0.98f || block_colide_side()) {
		if(angle <= 90) 
			angle = 360 - angle;
		else if(angle <= 180)
			angle = 360 - angle;
		else if(angle <= 270)
			angle = 180 - (angle - 180);
		else if(angle <= 360) 
			angle = 360 - angle;
	}
	if(offset_ball_y >= 0.98f || offset_ball_y <= -0.98f || block_colide()) {
		if(angle <= 90)
			angle = 180 - angle;
		else if(angle <= 180)
			angle = 180 - angle;
		else if(angle <= 270)
			angle = 360 - (angle - 180);
		else if(angle <= 360) 
			angle = 360 - (angle - 180);
	}

	offset_ball_x += sin(angle * 3.141523865 / 180) * offset_delta; 
	offset_ball_y += cos(angle * 3.141523865 / 180) * offset_delta; 

}

int paddle_colide() {
	if( angle > 90 && angle < 270 &&
		offset_ball_y <= -0.932 && offset_ball_x >= paddle_offset - 1.0 && offset_ball_x <= paddle_offset + 0.35 - 1.0) {
			return 1;
	}
	else 
		return 0;
}

int block_colide() {
	int ret = 0;
	//if( (angle > 0 && angle < 90) || (angle > 270 && angle < 360))
		for(int j = 0; j < 4; j++) {
			if( offset_ball_y >= 0.78 + j * 0.05 && offset_ball_y <= 0.79 + j * 0.05 + offset_delta)
				for(int i = 0; i < 10; i++) {
					if(offset_ball_x >= i * 0.2 - 1.0 && offset_ball_x <= i * 0.2 + 0.2 - 1.0) {
						if(block_exist[(3 - j) * 10 + i]) {
							block_exist[(3 - j) * 10 + i] = 0;
							ret = 1;
						}
					}
				}
		}
	//else {
		for(int j = 0; j < 4; j++) {
			if( offset_ball_y >= 0.87 + j * 0.05 && offset_ball_y <= 0.88 + j * 0.05 + offset_delta)
				for(int i = 0; i < 10; i++) {
					if(offset_ball_x >= i * 0.2 - 1.0 && offset_ball_x <= i * 0.2 + 0.2 - 1.0) {
						if(block_exist[(3 - j) * 10 + i]) {
							block_exist[(3 - j) * 10 + i] = 0;
							ret = 1;
						}
					}
				}
		}
	//}
	return ret;
	
}

int block_colide_side() {
	int ret = 0;

	if(angle > 180 && angle < 360)
		for(int j = 0; j < 10; j++) {
			if( offset_ball_x >= -0.78 + j * 0.2 && offset_ball_x <= -0.78 + j * 0.2 + offset_delta)
				for(int i = 0; i < 4; i++) {
					if(offset_ball_y <= 1.0 - i * 0.05 && offset_ball_y >= 1.0 - i * 0.05 - 0.05) {
						if(block_exist[j + 10 * i]) {
							block_exist[j + 10 * i] = 0;
							ret = 1;
						}
					}
				}
			}
	else {
		for(int j = 0; j < 10; j++) {
			if( offset_ball_x >= -0.82 + j * 0.2 && offset_ball_x <= -0.82 + j * 0.2 + offset_delta)
				for(int i = 0; i < 4; i++) {
					if(offset_ball_y <= 1.0 - i * 0.05 && offset_ball_y >= 1.0 - i * 0.05 - 0.05) {
						if(block_exist[j + 10 * i + 1]) {
							block_exist[j + 10 * i + 1] = 0;
							ret = 1;
						}
					}
				}
			}	
	}

	return ret;

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

void count_fps() {
	static double lastTime = glfwGetTime();
	static int nbFrames = 0;


	double currentTime = glfwGetTime();
	nbFrames++;
	if ( currentTime - lastTime >= 1.0 ){
         printf("%f frames\n", 1000 / (1000.0/double(nbFrames)));
         nbFrames = 0;
         lastTime += 1.0;
     }
}
/**
	@name:     pgk_lista_4
	@author:   Marek Jenda
	@revision: 1.0
	@date:     10.12.2013
	@file:     classess.cpp
	@depends:  classes.h
**/

#include "classes.h"

// Static members for keyhandler in GLclass class.
GLuint GLclass::camera_type;
GLfloat GLclass::offset_x;
GLfloat GLclass::offset_y;
GLuint World::max_humans;
GLuint World::camera_human;

////////////// outside functions ////////////// 

inline float DegToRad(float fAngDeg)
{
	const float fDegToRad = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad;
}

glm::mat3 RotateX(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[1].y = fCos; theMat[2].y = -fSin;
	theMat[1].z = fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateY(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[2].x = fSin;
	theMat[0].z = -fSin; theMat[2].z = fCos;
	return theMat;
}

glm::mat3 RotateZ(float fAngDeg)
{
	float fAngRad = DegToRad(fAngDeg);
	float fCos = cosf(fAngRad);
	float fSin = sinf(fAngRad);

	glm::mat3 theMat(1.0f);
	theMat[0].x = fCos; theMat[1].x = -fSin;
	theMat[0].y = fSin; theMat[1].y = fCos;
	return theMat;
}

////////////// Cube class methods ////////////// 

Cube::Cube(GLuint &mID) : MatrixID(mID) {
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void Cube::transform() {
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void Cube::draw(glm::mat4 aChange)  {
		MVP        =  aChange;
		transform();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
}

Cube::~Cube() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
}

////////////// Human class methods ////////////// 

Human::Human(GLuint &mID) : 
	cube         (mID), 
	move         (0),
	leftLegAngle (0.0f),
	rightLegAngle(0.0f),
	torsoOffset  (0.0f),
	rightLegUp   (false),
	rightLegDown (false),
	leftLegUp    (false),
	leftLegDown  (false),
	torsoUp      (false),
	torsoDown    (false) {	
	
	rotate = static_cast<float>(rand() % 360);// rotate = 0;
	initx  = static_cast<float>(rand() % 100); 
	inity  = static_cast<float>(rand() % 100); 
	speed  = static_cast<float>(rand() % 10 + 1); 

	glm::mat4 Projection  = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::mat4 View        = glm::lookAt(
							glm::vec3(0,5,-30), 
							glm::vec3(0,0,0), 
							glm::vec3(0,1,0) 
								);
	glm::mat4 Model		 = glm::mat4(1.0f); 
};

void Human::draw(GLfloat offset_x, GLfloat offset_y, glm::mat4 &MVPa) {
	move += speed*0.1f;
	if(inity - move < -200){
		move = 0;
		rotate = static_cast<float>(rand() % 360); 
	}
	// Rysuj Tors
	matrixStack.m_currMat = MVPa;
	matrixStack.RotateY( rotate);
	if(leftLegAngle >= 40 || leftLegAngle <= -40) { // legs dictate torso vertical position
			torsoUp   = false;
			torsoDown = true;
		} else if(leftLegAngle >= -0.01 && leftLegAngle <= 0.01) {
			torsoUp   = true;
			torsoDown = false;
		}
		if(!torsoUp)        torsoOffset += speed * 0.01f;
		else if(!torsoDown) torsoOffset -= speed * 0.01f;

	matrixStack.Translate(glm::vec3(initx, 0.0f + torsoOffset, inity));

	matrixStack.Translate(glm::vec3(0.0f, 0.0f, -1.5f*move));
	
	matrixStack.Push(); // poprzednia klatka
	matrixStack.Push();
	matrixStack.Scale(glm::vec3(2.0f, 6.0f, 1.0f));
	cube.draw(matrixStack.Top());
	matrixStack.Pop();
	
	{ // szyja
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(0.0f, 6.0f, 0.0f));

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.6f, 0.3f, 0.6f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
	} matrixStack.Pop();

	{ // glowa
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(0.0f, 6.3f, 0.0f));

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(1.0f, 1.5f, 1.0f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
	} matrixStack.Pop();

	{ // lewa noga
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(1.0f, 0.7f - torsoOffset, 0.0f));
		if(leftLegAngle >= 40) {
			leftLegUp = true;
			leftLegDown = false;
		} else if(leftLegAngle <= -40) {
			leftLegUp = false;
			leftLegDown = true;
		}
		if(!leftLegUp) leftLegAngle += speed * 1.0f;
		else if(!leftLegDown) leftLegAngle -= speed * 1.0f;
		matrixStack.RotateX(180.0f+leftLegAngle);

		//matrixStack.RotateX(180.0f+offset_y+5*move);

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.8f, 3.7f, 0.70f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		{ // lewa piszczel
			matrixStack.Push();
			matrixStack.Translate(glm::vec3(0.0f, 3.0f, 0.0f));
			if(leftLegAngle >= 0) matrixStack.RotateX(0);
			else matrixStack.RotateX(leftLegAngle);

			matrixStack.Push();
			matrixStack.Scale(glm::vec3(0.7f, 3.7f, 0.6f));
			cube.draw(matrixStack.Top());
			matrixStack.Pop();
			{ // lewa stopa
				matrixStack.Push();
				matrixStack.Translate(glm::vec3(0.0f, 3.5f, 0.0f));
				matrixStack.RotateX(90);

				matrixStack.Push();
				matrixStack.Scale(glm::vec3(0.6f, 2.0f, 0.2f));
				cube.draw(matrixStack.Top());
				matrixStack.Pop();
				matrixStack.Pop(); matrixStack.Pop();
			}
		}
		matrixStack.Pop();
	}
	
	{ // prawa noga
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(-1.0f, 0.7f - torsoOffset, 0.0f));
		
		if(rightLegAngle >= 40) {
			rightLegUp = true;
			rightLegDown = false;
		} else if(rightLegAngle <= -40) {
			rightLegUp = false;
			rightLegDown = true;
		}
		if(!rightLegDown) rightLegAngle -= speed * 1.0f;
		else if(!rightLegUp) rightLegAngle += speed * 1.0f;
		matrixStack.RotateX(180.0f+rightLegAngle);

		//matrixStack.RotateX(180.0f+offset_y+5*move);

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.8f, 3.7f, 0.70f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		{ // prawa piszczel
			matrixStack.Push();
			matrixStack.Translate(glm::vec3(0.0f, 3.0f, 0.0f));
			if(rightLegAngle >= 0) matrixStack.RotateX(0);
			else matrixStack.RotateX(rightLegAngle);

			matrixStack.Push();
			matrixStack.Scale(glm::vec3(0.7f, 3.7f, 0.6f));
			cube.draw(matrixStack.Top());
			matrixStack.Pop();
			{ // prawa stopa
				matrixStack.Push();
				matrixStack.Translate(glm::vec3(0.0f, 3.5f, 0.0f));
				matrixStack.RotateX(90);

				matrixStack.Push();
				matrixStack.Scale(glm::vec3(0.6f, 2.0f, 0.2f));
				cube.draw(matrixStack.Top());
				matrixStack.Pop();
				matrixStack.Pop(); matrixStack.Pop();
			}
		}
		matrixStack.Pop();
	}
	
	{ // lewe ramie
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(2.8f, 5.0f, 0.0f));

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.8f, 1.0f, 0.8f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		matrixStack.Pop();
	}

	{ // lewa reka
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(2.8f, 5.5f, 0.0f));
		matrixStack.RotateX(180.0f+rightLegAngle);

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.5f, 3.0f, 0.5f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		{ // lewe przedramie
			matrixStack.Push();
			matrixStack.Translate(glm::vec3(0.0f, 2.5f, 0.0f));
			if(leftLegAngle >= 0) matrixStack.RotateX(0);
			else matrixStack.RotateX(-leftLegAngle);

			matrixStack.Push();
			matrixStack.Scale(glm::vec3(0.4f, 3.0f, 0.4f));
			cube.draw(matrixStack.Top());
			matrixStack.Pop();

		} matrixStack.Pop();
	} matrixStack.Pop();
	
		{ // prawe ramie
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(-2.8f, 5.0f, 0.0f));

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.8f, 1.0f, 0.8f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		matrixStack.Pop();
	}

	{ // lewa reka
		matrixStack.Push();
		matrixStack.Translate(glm::vec3(-2.8f, 5.5f, 0.0f));
		matrixStack.RotateX(180.0f-rightLegAngle);

		matrixStack.Push();
		matrixStack.Scale(glm::vec3(0.5f, 3.0f, 0.5f));
		cube.draw(matrixStack.Top());
		matrixStack.Pop();
		{ // lewe przedramie
			matrixStack.Push();
			matrixStack.Translate(glm::vec3(0.0f, 2.5f, 0.0f));
			if(rightLegAngle >= 0) matrixStack.RotateX(0);
			else matrixStack.RotateX(-rightLegAngle);

			matrixStack.Push();
			matrixStack.Scale(glm::vec3(0.4f, 3.0f, 0.4f));
			cube.draw(matrixStack.Top());
			matrixStack.Pop();

		} matrixStack.Pop();
	} matrixStack.Pop();


	
	matrixStack.Pop();


}

////////////// World class methods ////////////// 

World::World(GLclass &_glclass, GLuint _max_humans) : glclass(_glclass) {
	max_humans = _max_humans;
	camera_human = rand() % max_humans;
	// camera perspective
	// Comment:: This is default camera which is situated high above models.  
	glm::mat4 Projection  = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::mat4 View        = glm::lookAt(
										glm::vec3(0, 20, -100), 
										glm::vec3(0, 0 ,  0  ), 
										glm::vec3(0, 1 ,  0  ) 
									);
	glm::mat4 Model		 = glm::mat4(1.0f); 

	Camera = Projection * View * Model;
}

void World::getCameraCoordinates(GLfloat x, GLfloat y, GLfloat rotate, GLfloat offset, GLfloat torsoOffset) {
	fp_x = x;
	fp_y = y;
	fp_rotate = rotate;
	fp_offset = offset;

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);;
	glm::mat4 	View;
	glm::mat4 	Model = glm::mat4(1.0f);

	switch(glclass.camera_type) {
		case 0 : // camera perspective
				 // Comment:: This is default camera which is situated high above models.  
			View        = glm::lookAt(
										glm::vec3(0, 20, -100), 
										glm::vec3(0, 0 ,  0  ), 
										glm::vec3(0, 1 ,  0  ) 
									);
			Camera = Projection * View * Model;
			break;
		case 1 : // third person camera
				 // Comment:: Here and in subsequent camera case we restore translates and rotations made by the model. 
				 //           This results in camera coordinates equal (or nearly equal) to this model.
			View        = glm::lookAt(
										glm::vec3(fp_x, 10, fp_y - 1.5f*fp_offset + 50), 
										glm::vec3(fp_x, 5 , fp_y - 1.5f*fp_offset - 50), 
										glm::vec3(0,    1 , 0                    ) 
									);
			Camera = Projection * View * Model;
			Camera = Camera * glm::mat4(RotateY(360 - fp_rotate));
			break;
		case 2 : // first person camera
			View        = glm::lookAt(
										glm::vec3(fp_x, 7 + torsoOffset, fp_y - 1.5f*fp_offset - 4 ), 
										glm::vec3(fp_x, 7 + torsoOffset, fp_y - 1.5f*fp_offset - 50), 
										glm::vec3(0,    1, 0                    ) 
									);
			Camera = Projection * View * Model;
			Camera = Camera * glm::mat4(RotateY(360 - fp_rotate));
			break;
		default  : // again perspective camera as default
			View        = glm::lookAt(
										glm::vec3(0, 20, -100), 
										glm::vec3(0, 0 , 0   ), 
										glm::vec3(0, 1 , 0   ) 
									); 
			Camera = Projection * View * Model;
			break;
	} // end:switch
}

//////////////  Glclass class methods ////////////// 

GLclass::GLclass(GLuint a_win_width, GLuint a_win_height) : win_width(a_win_width), win_height(a_win_height), mouse_speed(1.0f) {
	offset_x = 0;
	offset_y = 0;
	camera_type = 0;
	interframe = 1;
	{

	};
}

void GLclass::opengl_init() {
	glClearColor(0.32f, 0.54f, 0.81f, 1.0f);
	glGenVertexArrays(1, &VertexArrayID); 
	glBindVertexArray(VertexArrayID);    
	programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
	MatrixID = glGetUniformLocation(programID, "MVP");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

int GLclass::glfw_init() {
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

	glfwSetKeyCallback(key_press);
	glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_KEY_REPEAT );
	//glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(win_height/2, win_width/2);
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);
	mouse_change_x = mouse_speed*(win_width - float(xpos));
	mouse_change_y = mouse_speed*(win_width - float(ypos));
	oldtime = glfwGetTime();
	return 0;
}

void GLclass::mouse() {
	int newxpos, newypos;
	glfwGetMousePos(&newxpos, &newypos);
	offset_x += mouse_change_x - mouse_speed*(win_width - float(newxpos));
	offset_y -= mouse_change_y - mouse_speed*(win_width - float(newypos));
	mouse_change_x = mouse_speed*(win_width - float(newxpos));
	mouse_change_y = mouse_speed*(win_width - float(newypos));
}

void GLFWCALL GLclass::key_press(int key, int action) {
	switch(key) {
	
	case 'V' :
		if(action) camera_type = (camera_type + 1) % 3;
		if(camera_type == 0)  World::camera_human = rand() % World::max_humans;
		break;
	case GLFW_KEY_RIGHT:
		if(action) offset_x += 1;
		break;
	case GLFW_KEY_LEFT:
		if(action) offset_x -= 1;
		break;
	case GLFW_KEY_UP:
		if(action) offset_y += 1;
		break;
	case GLFW_KEY_DOWN:
		if(action) offset_y -= 1;
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		offset_x += 10;
		break;
	}
}

//////////////  MatrixStack class methods ////////////// 

MatrixStack::MatrixStack() : m_currMat(1.0f) {
}

const glm::mat4& MatrixStack::Top() {
		return m_currMat;
	}

void MatrixStack::RotateX(float fAngDeg) {
	m_currMat = m_currMat * glm::mat4(::RotateX(fAngDeg));
}

void MatrixStack::RotateY(float fAngDeg) {
	m_currMat = m_currMat * glm::mat4(::RotateY(fAngDeg));
}

void MatrixStack::RotateZ(float fAngDeg) {
	m_currMat = m_currMat * glm::mat4(::RotateZ(fAngDeg));
}

void MatrixStack::Scale(const glm::vec3 &scaleVec) {
	m_currMat = m_currMat * glm::scale(scaleVec.x, scaleVec.y, scaleVec.z);
		/*glm::mat4 scaleMat(1.0f);
		scaleMat[0].x = scaleVec.x;
		scaleMat[1].y = scaleVec.y;
		scaleMat[2].z = scaleVec.z;

		m_currMat = m_currMat * scaleMat;*/
}

void MatrixStack::Translate(const glm::vec3 &offsetVec) {
	glm::mat4 translateMat(1.0f);
	translateMat[3] = glm::vec4(offsetVec, 1.0f);

	m_currMat = m_currMat * translateMat;
}

void MatrixStack::Push() {
	m_matrices.push(m_currMat);
}

void MatrixStack::Pop() {
	m_currMat = m_matrices.top();
	m_matrices.pop();
}


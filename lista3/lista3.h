// ************************** PROTOTYPY FUNKCJI ************************************
  // podstawy
int glfw_init();
void opengl_init();
void display();
void alocate_buffers();
void delete_buffers();
void init_objects();
  // statyczne rysowanie obiektow
void init_ball(float r, float cx, float cy);
void draw_ball();
void init_square(GLfloat x, GLfloat y, GLfloat height, GLfloat width,  GLfloat* square);
void draw_square(GLfloat offsetx, GLfloat offsety, GLuint hvertex, GLuint hcolor);
void init_hexagon(float r, float cx, float cy);
void draw_hexagon(GLfloat offset_x, GLfloat offset_y);
void draw_background();
void blocks();
void color_object(GLfloat r, GLfloat g, GLfloat b);
  // przesuwanie obiektow
void move_object(GLfloat x, GLfloat y);
void add_offset();
void draw_paddle();
void move_paddle();
  // model kolizji
int paddle_colide();
int block_colide();
int block_colide_side();
  // inne
void display_face(char *emotion);
void count_fps();

// ZMIENNE
GLuint win_width  = 800;
GLuint win_height = 700;
GLuint programID;
GLuint VertexArrayID;
// Handles
GLuint hball_vertex;
GLuint hball_color;
GLuint hhexagon_vertex;
GLuint hhexagon_color;
GLuint hsquare_vertex;
GLuint hsquare_color;
GLuint hsquare_front_vertex;
GLuint hsquare_front_color;
GLuint hsquare_paddle_vertex;
GLuint hsquare_paddle_color;
GLuint offset;
GLuint uniform_color;
GLfloat paddle_offset = 0.825f;
// pileczka
GLfloat offset_delta  = 0.04f;
GLfloat offset_ball_x = 0.0f;
GLfloat offset_ball_y = -0.922f;
GLfloat ball_dir_x = 1;
GLfloat ball_dir_y = 1;
GLfloat angle = 44.0f;
// vertex arrays
GLfloat ball_vertex[100];
GLfloat ball_color [100];
GLfloat hexagon_vertex[100];
GLfloat hexagon_color [100];
GLfloat square_vertex[100];
GLfloat square_color [100];
GLfloat square_front_vertex[100];
GLfloat square_front_color [100];
GLfloat square_paddle_vertex[100];
GLfloat square_paddle_color [100];
int xpos, ypos;
GLfloat mouse_change = 0.0;
GLfloat paddle_speed = 0.002;
int block_exist[4*10];
GLuint play = 1;
GLfloat oldtime = 0, newtime = 0;
// ***************************************************************
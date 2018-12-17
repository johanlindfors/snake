// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <memory>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

using namespace glm;

#include <shader.h>


const int SPRITE_SIZE = 20;
const int SCREEN_SIZE = 20;
const int FRAMES_PER_SECOND = 15;
const int INITIAL_TAIL = 5;

const int SCREEN_WIDTH = SPRITE_SIZE * SCREEN_SIZE;
const int SCREEN_HEIGHT = SPRITE_SIZE * SCREEN_SIZE;

enum DIRECTION {
	Left,
	Right,
	Up,
	Down
};

struct Point {
public:
  int X;
  int Y;

  Point(int x, int y) : X(x), Y(y) {}
};

class Snake {
private:
  int x;
  int y;
  std::list<Point> trail;

public:
  int tail;
  int dx;
  int dy;

  Snake() {
    trail = std::list<Point>();
    tail = INITIAL_TAIL;
    x = 10;
    y = 10;
    dx = 0;
    dy = 1;
  }

  void Initialize() {

  }

  bool CheckCollision(int x, int y) {
    for (Point point : trail) {
      if (point.X == x && point.Y == y) {
        return true;
      }
    }
    return false;
  }

  void Update() {
    x = (x + dx + SCREEN_SIZE) % SCREEN_SIZE;
    y = (y + dy + SCREEN_SIZE) % SCREEN_SIZE;

    if (CheckCollision(x, y)) {
      x = y = 10;
      dx = dy = 0;
      tail = INITIAL_TAIL;
    }

    trail.push_back(Point(x, y));
    while ((int)trail.size() > tail) {
      trail.pop_front();
    }
  }

  void Draw() {
    // Render green filled quad
    for (Point point : trail) {
      auto left = point.X * SPRITE_SIZE + 1;
      auto top = point.Y * SPRITE_SIZE + 1;
      auto right = left + SPRITE_SIZE - 1;
      auto bottom = top + SPRITE_SIZE - 1;

    }
  }
};

class Apple {

public:
  int x;
  int y;

  Apple() {
    x = 3;
    y = 3;
    std::srand(std::time(nullptr));
  }

  void Initialize() {

  }

  void Reposition(int x, int y) {
    this->x = x;
    this->y = y;
  }

  void Draw() {
    auto left = this->x * SPRITE_SIZE + 1;
    auto top = this->y * SPRITE_SIZE + 1;
    auto right = left + SPRITE_SIZE - 1;
    auto bottom = top + SPRITE_SIZE - 1;
  }
};

class Game {
private:
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Apple> apple;

public:
  Game() 
  	: snake(std::make_unique<Snake>())
	, apple(std::make_unique<Apple>()) {
  }

  ~Game() { }

  void Tick() {
	Update();
    Draw();
  }

  bool Initialize() {
    apple->Initialize();
    snake->Initialize();

    return true;
  }

  void HandleInput(DIRECTION direction) {
    switch (direction) {
    case Left:
      if (snake->dx == 0) {
        snake->dx = -1;
        snake->dy = 0;
      }
      break;

    case Right:
      if (snake->dx == 0) {
        snake->dx = 1;
        snake->dy = 0;
      }
      break;

    case Up:
      if (snake->dy == 0) {
        snake->dx = 0;
        snake->dy = -1;
      }
      break;

    case Down:
      if (snake->dy == 0) {
        snake->dx = 0;
        snake->dy = 1;
      }
      break;

	default:
		break;
    }
  }

  void Update() {
    snake->Update();
    if (snake->CheckCollision(apple->x, apple->y)) {
      snake->tail++;
      int x = 0;
      int y = 0;
      do {
        x = std::rand() % SCREEN_SIZE;
        y = std::rand() % SCREEN_SIZE;
      } while (snake->CheckCollision(x, y));
      apple->Reposition(x, y);
    }
  }

  void Draw() {
    // Clear the window
    apple->Draw();
    snake->Draw();
  }
};


int main( void )
{
	auto game = std::make_unique<Game>();
	game->Initialize();

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 400, 400, "Snake", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/SimpleShader.vert", "shaders/SimpleShader.frag" );

	static const GLfloat g_vertex_buffer_data[] = { 
		0.0f, 0.0f, 0.0f,
		0.0f, 100.0f, 0.0f,
		100.0f,  0.0f, 0.0f,
		100.0f,  0.0f, 0.0f,
		.0f, 100.0f, 0.0f,
		100.0f, 100.0f, 0.0f,
		};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);
		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		glm::mat4 mvp = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 0.0f));
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

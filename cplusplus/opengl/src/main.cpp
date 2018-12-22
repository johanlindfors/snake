// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <memory>
#include <windows.h>
#include <functional>

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


glm::mat4 CreateOrthoMatrix() {
  glm::mat4 orthomatrix;

  auto right = 400;
  auto left = 0;
  auto top = 0;
  auto bottom = 400;
  auto Zfar = 10;
  auto Znear = 0;

  orthomatrix[0].x = 2.f/(right-left);
  orthomatrix[0].y = 0;
  orthomatrix[0].z = 0;
  orthomatrix[0].w = 0;

  orthomatrix[1].x = 0;
  orthomatrix[1].y = 2.f/(top-bottom);
  orthomatrix[1].z = 0;
  orthomatrix[1].w = 0;

  orthomatrix[2].x = 0;
  orthomatrix[2].y = 0;
  orthomatrix[2].z = 2.f/(Zfar-Znear);
  orthomatrix[2].w = 0;

  orthomatrix[3].x = static_cast<float>(-(right+left)/(right-left));
  orthomatrix[3].y = static_cast<float>(-(top+bottom)/(top-bottom));
  orthomatrix[3].z = static_cast<float>(-(Zfar+Znear)/(Zfar-Znear));
  orthomatrix[3].w = 1.f;

  return orthomatrix;
}

class Snake {
private:
  int x;
  int y;
  std::list<Point> trail;
  GLuint matrixId;
  GLuint vertexbuffer;
  GLuint VertexArrayID;
  GLuint programId;

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

  void Initialize(GLuint programID) {
    glGenVertexArrays(2, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = { 
       1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
       1.0f, 20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      20.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      20.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
       1.0f, 20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      20.0f, 20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };

    glGenBuffers(2, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    matrixId = glGetUniformLocation(programID, "MVP");
  }

  bool CheckCollision(int objx, int objy) {
    for (Point point : trail) {
      if (point.X == objx && point.Y == objy) {
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

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		glVertexAttribPointer(
		  0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			7 * sizeof(float),  // stride
			(void*)0            // array buffer offset
		);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(
      1, 
      4, 
      GL_FLOAT, 
      GL_FALSE, 
      7 * sizeof(float), 
      (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Render green filled quad
    auto orthomatrix = CreateOrthoMatrix();

    for (Point point : trail) {
      auto left = point.X * SPRITE_SIZE + 1;
      auto top = point.Y * SPRITE_SIZE + 1;

      glm::mat4 mvp = glm::translate(orthomatrix, glm::vec3(left, top, 0.0f));

      // Send our transformation to the currently bound shader, in the "MVP" uniform
      // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
      glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

      // Draw the triangle !
      glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
  }
};

class Apple {
private:
  GLuint matrixId;
  GLuint vertexbuffer;
  GLuint VertexArrayID;
  GLuint programId;

public:
  int x;
  int y;

  Apple() {
    x = 3;
    y = 3;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
  }

  ~Apple() {
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
  }

  void Initialize(GLuint programID) {
    programId = programID;

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = { 
       1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       1.0f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      20.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      20.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       1.0f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      20.0f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    matrixId = glGetUniformLocation(programId, "MVP");
  }

  void Reposition(int newx, int newy) {
    x = newx;
    y = newy;
  }

  void Draw() {
 		// Use our shader
		glUseProgram(programId);
		// 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			7 * sizeof(float),  // stride
			(void*)0            // array buffer offset
		);

    // color attribute
    glVertexAttribPointer(
      1, 
      4, 
      GL_FLOAT, 
      GL_FALSE, 
      7 * sizeof(float), 
      (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    auto left = this->x * SPRITE_SIZE + 1.f;
    auto top = this->y * SPRITE_SIZE + 1.f;
    
    auto orthomatrix = CreateOrthoMatrix();

    glm::mat4 mvp = glm::translate(orthomatrix, glm::vec3(left, top, 0.0f));

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }
};

class Timer {
private:
  double lastTime;
  LARGE_INTEGER frequency;
  double targetElapsedMilliseconds;

  double GetTime() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return static_cast<double>(time.QuadPart) / frequency.QuadPart;
  }

public:
  Timer() { 
    QueryPerformanceFrequency(&frequency);
  }

  void SetTarget(double target) {
    targetElapsedMilliseconds = target;
    lastTime = GetTime();
  }

  void Tick(const std::function<void()>& tickCallback) {
    auto now = GetTime();
    auto delta = now - lastTime;
      if(delta >= targetElapsedMilliseconds) { 
      tickCallback();
      lastTime = now;
    }
  }
};


class Game {
private:
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Apple> apple;
  Timer timer;

public:
  Game() 
  	: snake(std::make_unique<Snake>())
	  , apple(std::make_unique<Apple>()) 
  {
  }

  ~Game() { }

  void Tick() {
    timer.Tick([&]() {
      Update();
    });
    Draw();
  }

  bool Initialize(GLuint programID) {
    apple->Initialize(programID);
    snake->Initialize(programID);

    timer.SetTarget(1.f / FRAMES_PER_SECOND);

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

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/SimpleShader.vert", "shaders/SimpleShader.frag" );

  game->Initialize(programID);

	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

    game->Tick();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) {
      game->HandleInput(DIRECTION::Left);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      game->HandleInput(DIRECTION::Right);
    }
    if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS) {
      game->HandleInput(DIRECTION::Up);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) {
      game->HandleInput(DIRECTION::Down);
    }

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

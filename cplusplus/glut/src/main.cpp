#include <windows.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <list>
#include <ctime>
#include <memory>
// #include "timer.hpp"

const int SPRITE_SIZE = 20;
const int SCREEN_SIZE = 20;
const int FRAMES_PER_SECOND = 15;
const int INITIAL_TAIL = 5;

const int SCREEN_WIDTH  = SPRITE_SIZE * SCREEN_SIZE;
const int SCREEN_HEIGHT = SPRITE_SIZE * SCREEN_SIZE;

// Timer fps;

struct Point {
public:
   int X;
   int Y;

   Point(int x, int y) 
      : X(x)
      , Y(y)
   {

   }
};

void draw_square_at(int x, int y, int width, int height) {
   auto x1 = (-200.0f + x)/200.0f;
   auto y1 = (200.0f - y)/200.0f;
   auto x2 = (-200.0f + x+width)/200.0f;
   auto y2 = (200.0f - (y+height))/200.0f;
   glVertex2f( x1, y1);
   glVertex2f( x2, y1);
   glVertex2f( x2, y2);
   glVertex2f( x1, y2);
}

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

   bool checkCollision(int obj_x, int obj_y) {
      for (Point point : trail) {
         if(point.X == obj_x && point.Y == obj_y) {
            return true;
         }
      }
      return false;
   }

   void update() {
      x = (x + dx + SCREEN_SIZE) % SCREEN_SIZE;
      y = (y + dy + SCREEN_SIZE) % SCREEN_SIZE;

		if (checkCollision(x, y)) {
			x = y = 10;
			dx = dy = 0;
			tail = INITIAL_TAIL;
		}

      trail.push_back(Point(x,y));
      while((int)trail.size() > tail) {
         trail.pop_front();
      }
   }

   void draw() {
      //Render green filled quad for each trail element
		glColor3f(0.0f, 1.0f, 0.0f); // Red
      for (Point point : trail) {
         draw_square_at(point.X * SPRITE_SIZE + 1, point.Y * SPRITE_SIZE + 1, SPRITE_SIZE - 1, SPRITE_SIZE - 1);
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
      std::srand((unsigned int)std::time(nullptr));
   }

   void reposition(std::shared_ptr<Snake> snake){
      do {
         x = std::rand() % SCREEN_SIZE;
         y = std::rand() % SCREEN_SIZE;
      } while(snake->checkCollision(x,y));
   }

   void draw() {
      //Render red filled quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      draw_square_at(x * SPRITE_SIZE + 1, y * SPRITE_SIZE + 1, SPRITE_SIZE - 1, SPRITE_SIZE - 1);
   }
};

class Game {
private:
   std::shared_ptr<Snake> snake;
   std::unique_ptr<Apple> apple;
   static Game* currentInstance;

public:
   Game() 
      : snake(std::make_shared<Snake>())
      , apple(std::make_unique<Apple>())
   {
      currentInstance = this;
   }

   ~Game() {
      apple.reset();
      snake.reset();
   }

   void init(int argc, char** argv) {
      glutInit(&argc, argv);                 // Initialize GLUT
      glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
      glutInitWindowSize(400, 400);   // Set the window's initial width & height
      //glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   }

   static void handle_input_callback(int key, int, int) {
      currentInstance->handle_input(key);
   }

   void handle_input(int key) {
      switch(key)
      {
         case GLUT_KEY_UP:
            if(snake->dy == 0){
               snake->dx = 0;
               snake->dy = -1;
            }
            break; 
         case GLUT_KEY_DOWN:
            if(snake->dy == 0){
               snake->dx = 0;
               snake->dy = 1;
            }
            break;
         case GLUT_KEY_LEFT:
            if(snake->dx == 0){
               snake->dx = -1;
               snake->dy = 0;
            }
            break;
         case GLUT_KEY_RIGHT:
            if(snake->dx == 0){
               snake->dx = 1;
               snake->dy = 0;
            }
            break;
      }
   }

   static void update_callback(int) {
      currentInstance->update();
   }

   void update() {
      snake->update();
      if(snake->checkCollision(apple->x, apple->y)) {
         snake->tail++;
         apple->reposition(snake);
      }

      glutPostRedisplay();
      glutTimerFunc(1000/FRAMES_PER_SECOND, update_callback, 0);
   }

   static void draw_callback() {
      currentInstance->draw();
   }

   void draw() {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
      glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
   
      // Draw a Red 1x1 Square centered at origin
      glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      apple->draw();
      snake->draw();
      glEnd();
   
      glFlush();  // Render now
   }

   void run() {
      glutSpecialFunc(handle_input_callback);
      glutDisplayFunc(draw_callback);  // Register display callback handler for window re-paint
      update();                        // Enter the event-processing loop
      glutMainLoop();
   }
};

Game* Game::currentInstance = nullptr;

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    //fps.start();
   Game game;
   game.init(argc, argv);
   game.run(); 
   return 0;
}
#include <windows.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
// #include "timer.hpp"

const int SPRITE_SIZE = 20;
const int SCREEN_SIZE = 20;
const int FRAMES_PER_SECOND = 15;
const int INITIAL_TAIL = 5;

const int SCREEN_WIDTH  = SPRITE_SIZE * SCREEN_SIZE;
const int SCREEN_HEIGHT = SPRITE_SIZE * SCREEN_SIZE;

// Timer fps;

void handle_input() {

}

void update() {
    glutPostRedisplay();
}

void add_square_at(int x, int y){
      glVertex2f(-0.5f, -0.5f);    // x, y
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);
}

void draw() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
 
   // Draw a Red 1x1 Square centered at origin
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      
      glVertex2f(-0.5f, -0.5f);    // x, y
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);

      glColor3f(0.0f, 1.0f, 0.0f); // Green
      
      glVertex2f(-1.0f, -1.0f);    // x, y
      glVertex2f(-0.5f, -1.0f);
      glVertex2f(-0.5f, -0.5f);
      glVertex2f(-1.0f, -0.5f);
   glEnd();
 
   glFlush();  // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    //fps.start();
    
    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
    glutInitWindowSize(400, 400);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutIdleFunc(update);
    glutDisplayFunc(draw); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the event-processing loop
    return 0;
}
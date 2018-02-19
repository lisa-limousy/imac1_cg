#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.1415927
#define CIRCLE 300

static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

static const unsigned int BIT_PER_PIXEL = 32;

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resize() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
}

void setVideoMode() {
  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  
  resize();
  
  glClear(GL_COLOR_BUFFER_BIT);
}

void drawSquare(SDL_Event e){
    glBegin(GL_QUADS);   
                glVertex2d(- 0.5, 0.5);
                glVertex2d(0.5, 0.5);
                glVertex2d(0.5, - 0.5);
                glVertex2d(- 0.5, - 0.5);
            glEnd();
           SDL_GL_SwapBuffers();   
}

void drawLandMark(SDL_Event e){
    glBegin(GL_LINES);
                glColor3ub(0, 255, 0);
                glVertex2d(0, 0);
                glVertex2d(0, 1);
                glColor3ub(255, 0, 0);
                glVertex2d(0, 0);
                glVertex2d(1, 0);
            glEnd();
           SDL_GL_SwapBuffers();   
}

void drawCircle(){
    glBegin(GL_LINE_LOOP);
        for(int i =0; i <= CIRCLE; i++){
            double angle = 2 * PI * i / CIRCLE;
            double x = cos(angle);
            double y = sin(angle);
            glVertex2d(x,y);
        }
        glEnd();
       SDL_GL_SwapBuffers();   
}

int main(int argc, char** argv) {
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
    
  //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
  
  setVideoMode();

  SDL_WM_SetCaption("Paint", NULL);
  
  int loop = 1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {
        case SDL_MOUSEBUTTONDOWN:
    
            drawSquare(e);
            drawLandMark(e);
            drawCircle(e);
        break;

        case SDL_VIDEORESIZE:
          WINDOW_WIDTH = e.resize.w;
          WINDOW_HEIGHT = e.resize.h;
          setVideoMode();
          break;
          
        default:
          break;
      }
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
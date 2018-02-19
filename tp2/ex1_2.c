#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Point {
  float x, y; 
  unsigned char r, g, b;
  struct point* next;
} Point, *PointList;

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
  SDL_GL_SwapBuffers();
}

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
  Point* point = (Point*) malloc(sizeof(Point));
  if(!point) {
    return NULL;
  }
  point->x = x;
  point->y = y;
  point->r = r;
  point->g = g;
  point->b = b;
  point->next = NULL;
  return point;
}

void addPointToList(Point* point, PointList *list) {
  if(*list == NULL) {
    *list = point;
  } 
  else {
    addPointToList(point, &(*list)->next);
  }
}

void drawPoints(PointList list) {
  while(list) {
    glColor3ub(list->r, list->g, list->b);
    glVertex2f(list->x, list->y);
    list = list->next;
  }
}

PointList list = NULL;

int main(int argc, char** argv) {
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
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
            
          if (e.button.button == SDL_BUTTON_RIGHT){
              addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT), 255, 0, 0), &list);
              addPointToList(allocPoint(list->x, list->y, 255, 0, 0), &list);
              glBegin(GL_LINE_STRIP);
                    drawPoints(list);
                glEnd();
                SDL_GL_SwapBuffers();
              
            }
            else {
                addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT), 255, 0, 0), &list);
                glBegin(GL_LINE_STRIP);
                    drawPoints(list);
                glEnd();
                SDL_GL_SwapBuffers();
            }
        
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
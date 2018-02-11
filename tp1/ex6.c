#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "primitives.h"

static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

static const unsigned int BIT_PER_PIXEL = 32;

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static const unsigned int COLORS[] = {
     0, 0, 0 ,
     255, 0, 0 ,
     0, 255, 0 ,
     0, 0, 255 ,
     255, 255, 0 ,
     255, 0, 255 ,
     0, 255, 255 ,
     255, 255, 255 
};

// = 8 couleurs disponibles dans la palette */
static const unsigned int RVB_COLORS = sizeof(COLORS) / (3 * sizeof(unsigned char));

int main(int argc, char** argv) {
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
  setVideoMode();
  SDL_WM_SetCaption("Paint", NULL);
  
  unsigned int color = 6; 
  int mode = 0;
  PrimitiveList primitives = NULL;
  addPrimitive(allocPrimitive(GL_POINTS), &primitives);
  
  int loop = 1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    if(mode == 1) {
      /* Mode palette */
      palette();
    } else {
      /* Mode dessin */
      drawPrimitives(primitives);
    }
    
    SDL_GL_SwapBuffers();
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {
        case SDL_MOUSEBUTTONUP:
          if(mode == 1) {
            //color = e.button.x * RVB_COLORS / WINDOW_WIDTH;
          } 
          else {
            addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT), COLORS[color * 3], COLORS[color * 3 + 1], COLORS[color * 3 + 2]), &primitives->points);
          }
        break;
        
        case SDL_KEYDOWN:
          if(e.key.keysym.sym == ' ') {
            mode = 1;
          }
        break;
        
        case SDL_KEYUP:
          if(e.key.keysym.sym == ' ') {
            mode = 0;
          } else {
            switch(e.key.keysym.sym) {
              case 'p':
                addPrimitive(allocPrimitive(GL_POINTS), &primitives);
                break;
              case 'l':
                addPrimitive(allocPrimitive(GL_LINES), &primitives);
                break;
              case 't':
                addPrimitive(allocPrimitive(GL_TRIANGLES), &primitives);
                break;
              default:
                break;
            }
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

  deletePrimitives(&primitives);
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}

void reshape() {
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
  reshape();
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

void deletePoints(PointList *list) {
  while(*list) {
    Point *next = (*list)->next;
    free(*list);
    *list = next;
  }
}


Primitive* allocPrimitive(GLenum primitiveType) {
  Primitive* primitive = (Primitive*) malloc(sizeof(Primitive));
  if(!primitive) {
    return NULL;
  }
  primitive->primitiveType = primitiveType;
  primitive->points = NULL;
  primitive->next = NULL;
  return primitive;
}

void addPrimitive(Primitive* primitive, PrimitiveList *list) {
  primitive->next = *list;
  *list = primitive;
}

void deletePrimitives(PrimitiveList* list) {
  while(*list) {
    Primitive* next = (*list)->next;
    deletePoints(&(*list)->points);
    free(*list);
    *list = next;
  }
}

void drawPrimitives(PrimitiveList list) {
  while(list) {
    glBegin(list->primitiveType);
    drawPoints(list->points);
    glEnd();
    list = list->next;
  }
}

void palette() {
  int i;
  glBegin(GL_QUADS);
    for(i = 0; i < RVB_COLORS; ++i) {
      /*glColor3ubv(i * 3);
      glVertex2f();*/
    }
  glEnd();
}
#ifndef PRIMITIVES
#define PRIMITIVES

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

void reshape();
void setVideoMode();
void palette();

typedef struct Point {
  float x, y; 
  unsigned char r, g, b;
  struct point* next;
} Point, *PointList;

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b);
void addPointToList(Point* point, PointList* list);
void drawPoints(PointList list);
void deletePoints(PointList* list);


typedef struct Primitive {
  GLenum primitiveType; 
  PointList points;
  struct Primitive* next;
} Primitive, *PrimitiveList;

Primitive* allocPrimitive(GLenum primitiveType);
void addPrimitive(Primitive* link, PrimitiveList* list);
void deletePrimitives(PrimitiveList* list);
void drawPrimitives(PrimitiveList list);


#endif
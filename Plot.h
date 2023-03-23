#ifndef PLOT_H
#define PLOT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <applogic.h>
#include <QPainter>

#define VALUES_Y_SIZE 11
#define EMPTY 00
#define S_Y 350
#define S_X 50
#define LEN_X 1000
#define LEN_Y 300
#define MARGIN 10
#define PIX_MAP_W 400
#define PIX_MAP_H 1100
#define MAX_VALUE_INDEX 0
#define MID_VALUE_INDEX 1
#define MIN_VALUE_INDEX 2

void drawPoints(FuncReturningValue *frv, QPainter* paint);
void drawAxis(FuncReturningValue *plt, QPainter* paint);
double charToDouble(double*, char***, size_t, size_t);
double* charToDoublePlot(double*, char***, size_t, size_t);
QPixmap* createPlot(FuncReturningValue* plt);

#endif // PLOT_H

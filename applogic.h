#ifndef APPLOGIC_H
#define APPLOGIC_H

#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "struct.h"

#define MAX_VALUE_INDEX 0
#define MID_VALUE_INDEX 1
#define MIN_VALUE_INDEX 2
#define YEAR_FROM_INDEX 0
#define YEAR_TO_INDEX 1
#define VALUES_Y_SIZE 11
#define BUFF_SIZE 64
#define EMPTY 00
#define LEN_X 1000
#define LEN_Y 300


enum FuncType
{
    getData,
    calculateData,
    getNeededDdta,
    cleanData
};

typedef struct
{
    char* filename;
    char* region;
    double year[2];
    char*** data;
    char** headers;
    double solution[3];
    size_t field;
    size_t len;
    size_t fields_num;
} FuncArgument;

typedef struct
{
    char* region;
    double year[2];
    char*** data;
    char** headers;
    double solution[3];
    double* valuesX;
    int valuesXSize;
    double* valuesY;
    int valuesYSize;
    double* plotValues;
    int plotValuesSize;
    double vertStep;
    double horStep;
    double lenUsingY;
    double plotStep;
    size_t len;
    size_t fields_num;
} FuncReturningValue;

FuncReturningValue* entryPoint(FuncType ft, FuncArgument* fa);
#endif // APPLOGIC_H

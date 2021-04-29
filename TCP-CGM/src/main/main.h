/**
 * @file main.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef MAIN_H_
#define MAIN_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

int algorithmId, tcp, maxNumber, partitioningTechniqueId;
char algorithmName[100], partitioningName[100];

double startTime, tmpDouble, endTime, calculTime, comTime, comRate, calculRate, totalTime, idlenessTime, idlenessRate;
int sizeDataCom, nbMessageExchange, mcop;

int maxProcess, rank, processorNameSize;
char processorName[MPI_MAX_PROCESSOR_NAME];

clock_t currentTime;

int *tabDim;
#endif
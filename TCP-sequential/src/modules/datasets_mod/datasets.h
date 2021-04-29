/**
 * @file datasets.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 1.1
 * @date 2019-11-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#ifndef DATASETS_H_
#define DATASETS_H_

#include "main.h"
#include "utils.h"

#define MAX_NUMBER_MATRIX getMaxNumberOfMatrixDatasets()

extern int maxNumber;
FILE *file;

void initDataSetsMod();
void createDatasets();
void readFileData(int *tabDim);
#endif
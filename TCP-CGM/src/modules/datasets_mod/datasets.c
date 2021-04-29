/**
 * @file datasets.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 1.1
 * @date 2019-11-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#include "datasets.h"
#include "config.h"
#include "clogger.h"

void initDataSetsMod()
{
    tabDim = (int *)malloc((maxNumber + 1) * sizeof *tabDim);
}

void createDatasets()
{
    int i, alea;
    srand(time(0));
    file = fopen(getFilenameCreateDatasets(), "w");
    if (file == NULL)
    {
        logE("error: file %s not exist\n", getFilenameCreateDatasets());
        exit(EXIT_FAILURE);
    }
    printf("\n");
    for (i = 0; i <= MAX_NUMBER_MATRIX; i++)
    {
        alea = 1 + (int)99 * (rand() / (double)RAND_MAX);
        fprintf(file, "%d\t", alea);
    }
    fclose(file);
}

void readFileData(int *tabDim)
{
    int i;
    file = fopen(getFilenameInputDatasets(), "r");
    if (file == NULL)
    {
        logE("error: file %s not exist\n", getFilenameInputDatasets());
        exit(EXIT_FAILURE);
    }
    for (i = 0; i <= maxNumber; i++)
    {
        fscanf(file, "%d", tabDim + i);
    }
    fclose(file);
}
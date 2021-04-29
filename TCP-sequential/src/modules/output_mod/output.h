/**
 * @file output.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-12-14
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "main.h"

#define MAX_SIZE_STRING 1024
#define CONVERT_TO_MEGABYTE 1048576.0

typedef struct GeneralInfo_T GeneralInfo;
struct GeneralInfo_T
{
    char softName[MAX_SIZE_STRING];
    char softVersion[MAX_SIZE_STRING];
    char dataset[MAX_SIZE_STRING];
    char algorithm[MAX_SIZE_STRING];
    int algorithmId;
};

typedef struct InputInfo_T InputInfo;
struct InputInfo_T
{
    int maxNumberOfMatrix;
};

typedef struct OutputInfo_T OutputInfo;
struct OutputInfo_T
{
    int tcp;
    double totalTime;
};

void saveStatistic(char *filename, GeneralInfo genInfo, InputInfo inInfo, OutputInfo outInfo);

#endif
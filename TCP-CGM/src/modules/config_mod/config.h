/**
 * @file config.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsmn.h"

#define JSON_FILE_PATH "resources/config/config.json"
#define BUFFER_SIZE 5000
#define MAX_TOKEN_COUNT 128

void readfile(char *fileContent);
char *getValueByKey(char *key);
int jsoneq(const char *json, jsmntok_t *tok, const char *s);

char *getSoftName();
char *getSoftVersion();
char *getInputDataSetId();

char *getFileNamePrimarySurvey(int number);
char *getFileNameSecondarySurvey(int number);

char *getFilenameInputDatasets();
char *getFilenameCreateDatasets();

int getMaxNumberOfMatrixDatasets();
int getAlgorithmInputDatasets();
int getMaxNumberOfMatrix();
int getPartitioningTechnique();

#endif
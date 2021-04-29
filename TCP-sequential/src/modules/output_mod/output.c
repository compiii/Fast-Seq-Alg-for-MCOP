/**
 * @file output.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-12-14
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#include "output.h"
#include "clogger.h"

void saveStatistic(char *filename, GeneralInfo genInfo, InputInfo inInfo, OutputInfo outInfo)
{
	FILE *file = fopen(filename, "a");
	if (file == NULL)
	{
		logW("Error: no such file %s\r\n", filename);
		return;
	}
	fseek(file, 0L, SEEK_END);
	if (ftell(file) == 0)
	{

		fprintf(file, "%s;", "algortihm-id");
		fprintf(file, "%s;", "max-number-matrix");
		fprintf(file, "%s;", "total-time");
		fprintf(file, "%s;", "tcp");
		fprintf(file, "\n");
	}

	fprintf(file, "%d;", genInfo.algorithmId);
	fprintf(file, "%d;", inInfo.maxNumberOfMatrix);
	fprintf(file, "%f;", outInfo.totalTime);
	fprintf(file, "%d;", outInfo.tcp);
	fprintf(file, "\n");

	fclose(file);
}
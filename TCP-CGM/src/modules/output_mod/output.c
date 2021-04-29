/**
 * @file output.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
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
		fprintf(file, "%s;", "paritioning-technique");
		fprintf(file, "%s;", "max-process");
		fprintf(file, "%s;", "rank");

		fprintf(file, "%s;", "max-number-matrix");

		fprintf(file, "%s;", "total-time");
		fprintf(file, "%s;", "calcul-time");
		fprintf(file, "%s;", "com-time");
		fprintf(file, "%s;", "idleness-time");
		fprintf(file, "%s;", "calcul-rate");
		fprintf(file, "%s;", "com-rate");
		fprintf(file, "%s;", "idleness-rate");
		fprintf(file, "%s;", "nb-message-exchange");
		fprintf(file, "%s;", "size-data-com");
		fprintf(file, "%s;", "size-data-com-in-mb");
		fprintf(file, "%s;", "tcp");
		fprintf(file, "\n");
	}

	fprintf(file, "%d;", genInfo.algorithmId);
	fprintf(file, "%d;", genInfo.partitioningTechniqueId);
	fprintf(file, "%d;", genInfo.maxProcess);
	fprintf(file, "%d;", genInfo.rank);

	fprintf(file, "%d;", inInfo.maxNumberOfMatrix);

	fprintf(file, "%f;", outInfo.totalTime);
	fprintf(file, "%f;", outInfo.calculTime);
	fprintf(file, "%f;", outInfo.comTime);
	fprintf(file, "%f;", outInfo.idlenessTime);
	fprintf(file, "%f;", outInfo.calculTime / outInfo.totalTime * 100);
	fprintf(file, "%f;", outInfo.comTime / outInfo.totalTime * 100);
	fprintf(file, "%f;", outInfo.idlenessTime / outInfo.totalTime * 100);
	fprintf(file, "%d;", outInfo.nbMessageExchange);
	fprintf(file, "%d;", outInfo.sizeDataCom);
	fprintf(file, "%f;", outInfo.sizeDataCom / CONVERT_TO_MEGABYTE);
	fprintf(file, "%d;", outInfo.tcp);
	fprintf(file, "\n");

	fclose(file);
}
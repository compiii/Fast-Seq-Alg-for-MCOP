/**
 * @file main.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-12-14
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#include "clogger.h"
#include "config.h"
#include "datasets.h"
#include "tcp.h"
#include "output.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        maxNumber = atol(argv[1]);
        algorithmId = atoi(argv[2]);
    }
    else
    {
        maxNumber = getMaxNumberOfMatrix();
        algorithmId = getAlgorithmInputDatasets();
    }

    char *print = malloc(sizeof(char));
    char tmp[1024];
    int verbose = 1;
    sprintf(tmp, "%s", "");
    GeneralInfo generalInfo;
    InputInfo inputInfo;
    OutputInfo outputInfo;
    char fileName[MAX_SIZE_STRING];

    if (algorithmId == 0)
        sprintf(algorithmName, "%s", "Godbole's sequential algorithm");
    else if (algorithmId == 1)
        sprintf(algorithmName, "%s", "Yao's sequential algorithm");
    else if (algorithmId == 2)
        sprintf(algorithmName, "%s", "Yao's sequential algorithm (Kengne et Myoupo'14)");
    else if (algorithmId == 3)
        sprintf(algorithmName, "%s", "Yao's sequential algorithm (new improved)");
    else
    {
        logE("check the value of algorithm use in config.json file");
        exit(EXIT_FAILURE);
    }

    print = concat(2, algorithmName, " input datas\n------------------------------\n");

    strcpy(generalInfo.softName, getSoftName());
    strcpy(generalInfo.softVersion, getSoftVersion());
    strcpy(generalInfo.algorithm, algorithmName);
    strcpy(generalInfo.dataset, getInputDataSetId());
    generalInfo.algorithmId = algorithmId;

    inputInfo.maxNumberOfMatrix = maxNumber;

    sprintf(tmp, "max number of matrix: %d\n", maxNumber);
    print = concat(2, print, tmp);
    logI(print);
    print = concat(1, "");

    initDataSetsMod();
    //createDatasets();
    readFileData(tabDim);
    initTCPMod();

    if (verbose)
    {
        print = concat(2, "starting computation of ", algorithmName);
        logI(print);
        print = concat(1, "");
    }

    if (algorithmId == 0 || algorithmId == 1 || algorithmId == 2 || algorithmId == 3)
    {
        currentTime = clock();
        tcp = tcpAlgorithm();
        totalTime = ((float)(clock() - currentTime)) / CLOCKS_PER_SEC;

        outputInfo.tcp = tcp;
        outputInfo.totalTime = totalTime;

        strcpy(fileName, getFileNameSurvey());
        saveStatistic(fileName, generalInfo, inputInfo, outputInfo);

        sprintf(tmp, " of %s ", algorithmName);
        print = concat(2, "ending computation", tmp);
        logI(print);
        print = concat(1, "");
        sprintf(tmp, "Output datas for %s", algorithmName);
        print = concat(2, tmp, "\n------------------------------\n");
        sprintf(tmp, "mcop: %d\n", tcp);
        print = concat(2, print, tmp);
        sprintf(tmp, "total time: %f\n------------------------------\n", totalTime);
        print = concat(2, print, tmp);
        logI(print);
    }

    free(tabCone);
    free(tabDim);
    free(tabPos);
    free(tabRank);
    free(tabSegment);
    return 0;
}

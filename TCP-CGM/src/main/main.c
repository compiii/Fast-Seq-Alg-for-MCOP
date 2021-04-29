/**
 * @file main.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#include "clogger.h"
#include "config.h"
#include "datasets.h"
#include "tcp.h"
#include "output.h"
#include "kengne_tmax.h"
#include "kengne_tint.h"
#include "kechid.h"

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &maxProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processorName, &processorNameSize);

    if (argc > 1)
    {
        maxNumber = atol(argv[1]);
        algorithmId = atoi(argv[2]);
        partitioningTechniqueId = atoi(argv[3]);
    }
    else
    {
        maxNumber = getMaxNumberOfMatrix();
        algorithmId = getAlgorithmInputDatasets();
        partitioningTechniqueId = getPartitioningTechnique();
    }

    char *print = malloc(sizeof(char));
    char tmp[1024];
    int verbose = 1;
    sprintf(tmp, "%s", "");
    GeneralInfo generalInfo;
    InputInfo inputInfo;
    OutputInfo outputInfo;
    char fileName[MAX_SIZE_STRING];

    if (algorithmId == 1)
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

    if (partitioningTechniqueId == 1)
        sprintf(partitioningName, "%s", "the DAG is divided by Tmax/p\n------------------------------\n");
    else if (partitioningTechniqueId == 2)
        sprintf(partitioningName, "%s", "the DAG is divided by Tint/p\n------------------------------\n");
    else if (partitioningTechniqueId == 3)
        sprintf(partitioningName, "%s", "the DAG is divided by Tcb_optimal\n------------------------------\n");
    else
    {
        logE("check the value of partitioning technique use in config.json file");
        exit(EXIT_FAILURE);
    }
    print = concat(2, print, partitioningName);

    strcpy(generalInfo.softName, getSoftName());
    strcpy(generalInfo.softVersion, getSoftVersion());
    strcpy(generalInfo.algorithm, algorithmName);
    strcpy(generalInfo.dataset, getInputDataSetId());
    generalInfo.algorithmId = algorithmId;
    generalInfo.partitioningTechniqueId = partitioningTechniqueId;
    generalInfo.rank = rank;
    generalInfo.maxProcess = maxProcess;

    inputInfo.maxNumberOfMatrix = maxNumber;

    if (rank == 0)
    {
        sprintf(tmp, "max number of matrix: %d\n", maxNumber);
        print = concat(2, print, tmp);
        if (algorithmId != 0)
        {
            sprintf(tmp, "max number of process: %d\n", maxProcess);
            print = concat(2, print, tmp);
        }
        logI(print);
        print = concat(1, "");
    }

    initDataSetsMod();
    //createDatasets();
    readFileData(tabDim);
    initTCPMod();

    startTime = tmpDouble = endTime = calculTime = comTime = comRate = calculRate = totalTime = idlenessTime = idlenessRate = sizeDataCom = nbMessageExchange = 0;

    if (verbose && rank == 0)
    {
        print = concat(2, "starting computation of ", algorithmName);
        logI(print);
        print = concat(1, "");
    }

    if (partitioningTechniqueId >= 1 && partitioningTechniqueId <= 3)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        startTime = MPI_Wtime();
        if (partitioningTechniqueId == 1)
        {
            tcp = kengneTmax();
        }
        else if (partitioningTechniqueId == 2)
        {
            tcp = kengneTint();
        }
        else if (partitioningTechniqueId == 3)
        {
            tcp = kechid();
        }

        endTime = MPI_Wtime();

        idlenessTime = endTime - startTime - (calculTime + comTime);
        totalTime = calculTime + comTime + idlenessTime;

        calculRate = calculTime / totalTime * 100;
        comRate = comTime / totalTime * 100;
        idlenessRate = idlenessTime / totalTime * 100;

        outputInfo.tcp = tcp;
        outputInfo.totalTime = totalTime;
        outputInfo.calculTime = calculTime;
        outputInfo.comTime = comTime;
        outputInfo.idlenessTime = idlenessTime;
        outputInfo.nbMessageExchange = nbMessageExchange;
        outputInfo.sizeDataCom = sizeDataCom * sizeof(unsigned short);

        if (tcp > 0)
        {
            strcpy(fileName, getFileNamePrimarySurvey(rank));
            saveStatistic(fileName, generalInfo, inputInfo, outputInfo);
        }
        else
        {
            strcpy(fileName, getFileNameSecondarySurvey(rank));
            saveStatistic(fileName, generalInfo, inputInfo, outputInfo);
        }

        if (verbose)
        {
            sprintf(tmp, " of %s (process %d)", algorithmName, rank);
            print = concat(3, processorName, " ends computation", tmp);
            logI(print);
            print = concat(1, "");
            sprintf(tmp, "Output datas for %s", algorithmName);
            print = concat(2, tmp, "\n------------------------------\n");
            sprintf(tmp, "mcop: %d\n", tcp);
            print = concat(2, print, tmp);
            sprintf(tmp, "total time: %f\n------------------------------\n", totalTime);
            print = concat(2, print, tmp);
            sprintf(tmp, "calcul time: %f\n", calculTime);
            print = concat(2, print, tmp);
            sprintf(tmp, "comunication time: %f\n", comTime);
            print = concat(2, print, tmp);
            sprintf(tmp, "idleness time: %f\n", idlenessTime);
            print = concat(2, print, tmp);
            sprintf(tmp, "calcul rate: %f\n", calculRate);
            print = concat(2, print, tmp);
            sprintf(tmp, "comunication rate: %f\n", comRate);
            print = concat(2, print, tmp);
            sprintf(tmp, "idleness rate: %f\n", idlenessRate);
            print = concat(2, print, tmp);
            sprintf(tmp, "number of message exchange: %d\n", nbMessageExchange);
            print = concat(2, print, tmp);
            sprintf(tmp, "size of data communicate: %f MB\n", sizeDataCom * sizeof(unsigned short) / 1048576.0);
            print = concat(2, print, tmp);
            logI(print);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}

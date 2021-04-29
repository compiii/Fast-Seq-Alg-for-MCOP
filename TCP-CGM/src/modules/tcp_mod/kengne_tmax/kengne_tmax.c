/**
 * @file kengne_tmax.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#include "kengne_tmax.h"
#include "clogger.h"
#include "stack.h"

int kengneTmax()
{
    markBridges();
    updateIdSon();
    partitionningTmax();

    if (algorithmId == 2)
    {
        for (int i = 1; i < (maxNumber + 2); i++)
        {
            tabRank[i] = tabPos[i - 1];
        }
        quicksort(tabRank, 1, maxNumber + 1);
    }

    unsigned int(*tabMCOP)[maxNumber];
    tabMCOP = malloc(sizeof(unsigned int[2 * maxNumber][maxNumber + 1]));
    if (tabMCOP == NULL)
    {
        logE("insuffisant memory for allocate tabMCOP\n");
        exit(EXIT_FAILURE);
    }

    return cgmAlgorithmKengneTmax(tabMCOP);
}
void partitionningTmax()
{
    int tc, tmax, tcj;
    int minIK, minKJ;
    tc = tmax = 0, totalLoadBlock = 0, totalNumberCone = 0;
    idBlock = 0;
    idBand = 1;
    idLastConeBand = -1;
    for (int i = 0; i < 2 * maxNumber; i = i + 2)
    {
        minIK = min(tabSegment[i].i.w, tabSegment[i].j.w);
        minKJ = min(tabSegment[i + 1].i.w, tabSegment[i + 1].j.w);
        tc = tc + (minIK + minKJ);
        if (tabSegment[i].type == 0)
        {
            tmax += minIK;
        }
        if (tabSegment[i + 1].type == 0)
        {
            tmax += minKJ;
        }
        if (tabSegment[i].type == 0 && tabSegment[i + 1].type == 0)
        {
            tmax += min(tabSegment[i].i.w, tabSegment[i + 1].j.w);
        }
    }
    tcj = floor(tmax / (1.0 * maxProcess));

    currentDAG = createSegmentList();
    for (int i = 2 * maxNumber - 1; i >= 0; i--)
    {
        currentDAG = addSegmentToSegmentList(currentDAG, tabSegment[i]);
    }
    blockInfoList = createBlockInfoList();

    while (!isEmptySegmentList(currentDAG))
    {
        constructBandTmax(tcj);
        idBand++;
    }

    if (!isEmptyBlockInfoList(blockInfoList) && 1 == 2)
    {
        BlockInfoList *path = blockInfoList;
        while (path != NULL)
        {
            path = path->next;
        }
    }

    free(currentDAG);
}
void constructBandTmax(int tcb)
{
    int endBand = 0, pivot = 1;
    BlockInfo blockInfo;
    int processor;
    while (endBand == 0)
    {
        processor = idBlock % maxProcess;
        blockInfo = constructBlock(tcb, processor, &endBand, &pivot);
        if (blockInfo.load == 0 && blockInfo.nbCone == 0)
        {
            idBlock--;
        }
        else
        {
            if (processor == rank)
                addBlockInfoToBlockInfoList(blockInfoList, blockInfo);
        }
        totalLoadBlock += blockInfo.load;
        totalNumberCone += blockInfo.nbCone;
        idBlock++;
    }
}

int cgmAlgorithmKengneTmax(unsigned int (*tabMCOP)[maxNumber + 1])
{
    BlockInfo blockInfo;
    BlockInfoList *path = blockInfoList;
    int nbCone = 1, currentBand = 0, currentBlock;
    DataList *sendList;
    Cone c;
    MPI_Request request;
    while (path != NULL)
    {
        blockInfo = path->blockInfo;
        currentBand = blockInfo.cgmInfo.idBand;
        currentBlock = blockInfo.cgmInfo.idBlock;
        nbCone = 1;
        sendList = createDataList();
        for (int i = 0; i < 2 * maxNumber && nbCone <= blockInfo.nbCone; i++)
        {
            c = tabCone[i];
            if (c.cgmInfo.rank == rank && currentBand == c.cgmInfo.idBand && currentBlock == c.cgmInfo.idBlock)
            {
                if (tabSegment[c.idSegment].type == 1)
                {
                    if (tabCone[c.idSegmentSonLeft].cgmInfo.rank != rank)
                    {
                        tmpDouble = MPI_Wtime();
                        MPI_Recv(tabMCOP[c.idSegmentSonLeft], maxNumber + 1, MPI_INT, tabCone[c.idSegmentSonLeft].cgmInfo.rank, c.idSegmentSonLeft, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        comTime += (MPI_Wtime() - tmpDouble);
                    }

                    if (tabCone[c.idSegmentSonRight].cgmInfo.rank != rank)
                    {
                        tmpDouble = MPI_Wtime();
                        MPI_Recv(tabMCOP[c.idSegmentSonRight], maxNumber + 1, MPI_INT, tabCone[c.idSegmentSonRight].cgmInfo.rank, c.idSegmentSonRight, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        comTime += (MPI_Wtime() - tmpDouble);
                    }
                }

                tmpDouble = MPI_Wtime();
                if (algorithmId == 1)
                {
                    computeCone_1(c, tabMCOP);
                }
                else if (algorithmId == 2)
                {
                    computeCone_2(c, tabMCOP);
                }
                calculTime += (MPI_Wtime() - tmpDouble);

                nbCone++;

                if (c.idSegmentParent > 0 && tabCone[c.idSegmentParent].cgmInfo.rank != rank)
                {
                    sendList = addDataToDataList(sendList, c.idSegment);
                }
            }
        }

        if (!isEmptyDataList(sendList))
        {
            while (sendList != NULL)
            {
                tmpDouble = MPI_Wtime();
                MPI_Isend(tabMCOP[sendList->data], maxNumber + 1, MPI_INT, tabCone[tabCone[sendList->data].idSegmentParent].cgmInfo.rank, sendList->data, MPI_COMM_WORLD, &request);
                comTime += (MPI_Wtime() - tmpDouble);

                sendList = sendList->next;
            }
        }

        path = path->next;
    }

    if (currentBand == idBand - 1)
    {
        c = tabCone[2 * maxNumber - 2];
        if (c.cgmInfo.rank != rank)
        {
            tmpDouble = MPI_Wtime();
            MPI_Recv(tabMCOP[c.idSegment], 2, MPI_INT, c.cgmInfo.rank, c.idSegment, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            comTime += (MPI_Wtime() - tmpDouble);
        }
        c = tabCone[2 * maxNumber - 1];
        if (c.cgmInfo.rank != rank)
        {
            tmpDouble = MPI_Wtime();
            MPI_Recv(tabMCOP[c.idSegment], 2, MPI_INT, c.cgmInfo.rank, c.idSegment, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            comTime += (MPI_Wtime() - tmpDouble);
        }

        return getMCOP(2 * maxNumber - 2, 1, tabMCOP) + getMCOP(2 * maxNumber - 1, 1, tabMCOP);
    }
    else
    {
        c = tabCone[2 * maxNumber - 2];
        if (c.cgmInfo.rank == rank)
        {
            tmpDouble = MPI_Wtime();
            MPI_Isend(tabMCOP[c.idSegment], 2, MPI_INT, tabCone[c.idSegment + 1].cgmInfo.rank, c.idSegment, MPI_COMM_WORLD, &request);
            comTime += (MPI_Wtime() - tmpDouble);
        }
        c = tabCone[2 * maxNumber - 1];
        if (c.cgmInfo.rank == rank)
        {
            tmpDouble = MPI_Wtime();
            MPI_Isend(tabMCOP[c.idSegment], 2, MPI_INT, tabCone[c.idSegment - 1].cgmInfo.rank, c.idSegment, MPI_COMM_WORLD, &request);
            comTime += (MPI_Wtime() - tmpDouble);
        }
        return -1;
    }
}
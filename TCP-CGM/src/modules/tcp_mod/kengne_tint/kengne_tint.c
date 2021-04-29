/**
 * @file kengne_tint.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#include "kengne_tint.h"
#include "clogger.h"
#include "stack.h"

int kengneTint()
{
    markBridges();
    updateIdSon();
    partitionningTint();

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

    return cgmAlgorithmKengneTint(tabMCOP);
}
void partitionningTint()
{
    int tc, tcj;
    int minIK, minKJ;
    tc = 0, totalLoadBlock = 0, totalNumberCone = 0;
    idBlock = 0;
    idBand = 1;
    idLastConeBand = -1;
    for (int i = 0; i < 2 * maxNumber; i = i + 2)
    {
        minIK = min(tabSegment[i].i.w, tabSegment[i].j.w);
        minKJ = min(tabSegment[i + 1].i.w, tabSegment[i + 1].j.w);
        tc = tc + (minIK + minKJ);
    }

    tcj = constructFirstBandTint(tc);

    while (!isEmptySegmentList(currentDAG))
    {
        constructBandTint(tcj);
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
void constructBandTint(int tcb)
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

int constructFirstBandTint(int tc)
{
    int tcj, inf, sup, gap;
    int endBand = 0, pivot = 1, step = 0;
    BlockInfo blockInfo;
    int processor;
    sup = floor(tc / (1.0 * maxProcess));
    inf = floor(tc / (2.0 * maxProcess));
    gap = sup - inf;

    tcj = sup;

    while (gap > floor(tc / (1.0 * pow(maxProcess, 2))))
    {
        step++;
        if (!isEmptySegmentList(currentDAG))
        {
            free(currentDAG);
        }
        currentDAG = createSegmentList();
        for (int i = 2 * maxNumber - 1; i >= 0; i--)
        {
            if (tabCone[i].cgmInfo.color != 0)
            {
                tabCone[i].cgmInfo.color = 0;
                tabCone[i].cgmInfo.idBand = -1;
                tabCone[i].cgmInfo.idBlock = -1;
                tabCone[i].cgmInfo.rank = -1;
            }
            currentDAG = addSegmentToSegmentList(currentDAG, tabSegment[i]);
        }
        if (!isEmptyBlockInfoList(blockInfoList))
        {
            free(blockInfoList);
        }
        blockInfoList = createBlockInfoList();

        endBand = 0,
        pivot = 1;
        idBlock = 0;
        totalLoadBlock = 0, totalNumberCone = 0;
        while (endBand == 0)
        {
            processor = idBlock % maxProcess;
            blockInfo = constructBlock(tcj, processor, &endBand, &pivot);
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
            if (rank == 1000)
            {
                printBlockInfo(blockInfo);
            }
            idBlock++;
        }

        if (idBlock >= maxProcess)
        {
            inf = tcj;
        }
        else
        {
            sup = tcj;
        }
        gap = sup - inf;
        tcj = ceil((sup + inf) / 2.0);
    }

    if (step != 1)
    {
        if (!isEmptySegmentList(currentDAG))
        {
            free(currentDAG);
        }
        currentDAG = createSegmentList();
        for (int i = 2 * maxNumber - 1; i >= 0; i--)
        {
            if (tabCone[i].cgmInfo.color != 0)
            {
                tabCone[i].cgmInfo.color = 0;
                tabCone[i].cgmInfo.idBand = -1;
                tabCone[i].cgmInfo.idBlock = -1;
                tabCone[i].cgmInfo.rank = -1;
            }
            currentDAG = addSegmentToSegmentList(currentDAG, tabSegment[i]);
        }
        if (!isEmptyBlockInfoList(blockInfoList))
        {
            free(blockInfoList);
        }
        blockInfoList = createBlockInfoList();

        endBand = 0,
        pivot = 1;
        idBlock = 0;
        totalLoadBlock = 0, totalNumberCone = 0;
        while (endBand == 0)
        {
            processor = idBlock % maxProcess;
            blockInfo = constructBlock(inf, processor, &endBand, &pivot);
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
            if (rank == 1000)
            {
                printBlockInfo(blockInfo);
            }
            idBlock++;
        }
    }

    idBand++;
    return inf;
}

int cgmAlgorithmKengneTint(unsigned int (*tabMCOP)[maxNumber + 1])
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

    return -1;
}
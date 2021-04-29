/**
 * @file tcp.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-05
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#include "tcp.h"
#include "clogger.h"
#include "stack.h"

void initTCPMod()
{
    int i, j, pos, lower = 0;

    cyclicRotation(tabDim);

    tabPos = (Node *)malloc((maxNumber + 1) * sizeof *tabPos);
    if (tabPos == NULL)
    {
        logE("insuffisant memory for allocate tabPos\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < (maxNumber + 1); i++)
    {
        tabPos[i].v = i;
        tabPos[i].weight = tabDim[i];
        pos = i + 1;
        lower = 0;
        for (j = i + 1; pos != i; j++)
        {
            pos = j % (maxNumber + 1);
            if (tabDim[i] > tabDim[pos] || (tabDim[i] == tabDim[pos] && i > pos))
                lower++;
        }
        tabPos[i].w = lower + 1;
    }

    tabRank = (Node *)malloc((maxNumber + 2) * sizeof *tabRank);
    if (tabRank == NULL)
    {
        logE("insuffisant memory for allocate tabRank\n");
        exit(EXIT_FAILURE);
    }

    tabSegment = (Segment *)malloc(2 * maxNumber * sizeof *tabSegment);
    tabCone = (Cone *)malloc(2 * maxNumber * sizeof *tabCone);
    if (tabSegment == NULL || tabCone == NULL)
    {
        logE("insuffisant memory for allocate tabSegment or tabCone\n");
        exit(EXIT_FAILURE);
    }
}

int getMCOP(int i, int j, unsigned int (*tabMCOP)[maxNumber + 1])
{
    return tabMCOP[i][j];
}

int max(int m, int n)
{
    return (m >= n ? m : n);
}

int min(int m, int n)
{
    return (m <= n ? m : n);
}

// Function to swap two pointers
void swap(Node *a, Node *b)
{
    Node temp = *a;
    *a = *b;
    *b = temp;
}

// Function to run quicksort on an array of integers
// l is the leftmost starting index, which begins at 0
// r is the rightmost starting index, which begins at array length - 1
void quicksort(Node arr[], int l, int r)
{
    // Base case: No need to sort arrays of length <= 1
    if (l >= r)
    {
        return;
    }

    // Choose pivot to be the last element in the subarray
    int pivot = arr[r].w;

    // Index indicating the "split" between elements smaller than pivot and
    // elements greater than pivot
    int cnt = l;

    // Traverse through array from l to r
    for (int i = l; i <= r; i++)
    {
        // If an element less than or equal to the pivot is found...
        if (arr[i].w <= pivot)
        {
            // Then swap arr[cnt] and arr[i] so that the smaller element arr[i]
            // is to the left of all elements greater than pivot
            swap(&arr[cnt], &arr[i]);

            // Make sure to increment cnt so we can keep track of what to swap
            // arr[i] with
            cnt++;
        }
    }

    // NOTE: cnt is currently at one plus the pivot's index
    // (Hence, the cnt-2 when recursively sorting the left side of pivot)
    quicksort(arr, l, cnt - 2); // Recursively sort the left side of pivot
    quicksort(arr, cnt, r);     // Recursively sort the right side of pivot
}

void cyclicRotation(int *tabDim)
{
    int i, pos = 0;
    for (i = 1; i <= maxNumber; i++)
        if (tabDim[pos] > tabDim[i])
            pos = i;

    if (pos != 0)
    {
        int *tmp = malloc((maxNumber + 1) * sizeof(int));
        for (i = 0; i <= maxNumber; i++)
            tmp[i] = tabDim[i];
        for (i = 0; i <= maxNumber; i++)
            tabDim[i] = tmp[(i + pos) % (maxNumber + 1)];
    }
}

void markBridges()
{
    Stack *s = createStack();
    Node currentNode = tabPos[0], tmp;
    Segment segment;
    Cone cone;
    int i = 0, idSegment = 0;
    while (i <= maxNumber)
    {
        i++;
        push(&s, &currentNode, sizeof(Node));
        currentNode = tabPos[i % (maxNumber + 1)];
        while (((Node *)peek(s))->w > currentNode.w)
        {
            tmp = *(Node *)peek(s);
            s = pop(s);
            segment.id = idSegment++;
            segment.i = *(Node *)peek(s);
            segment.j = tmp;
            segment.type = (segment.i.v == segment.j.v - 1 || segment.i.v % maxNumber == segment.j.v ? 0 : 1);
            tabSegment[segment.id] = segment;

            cone.idSegment = segment.id;
            cone.w = (segment.i.w < segment.j.w ? segment.i : segment.j);
            cone.idSegmentSonLeft = -1;
            cone.idSegmentSonRight = -1;

            cone.nodeList = createNodeList();
            tabCone[cone.idSegment] = cone;

            segment.id = idSegment++;
            segment.i = tmp;
            segment.j = currentNode;
            segment.type = (segment.i.v == segment.j.v - 1 || segment.i.v % maxNumber == segment.j.v ? 0 : 1);
            tabSegment[segment.id] = segment;

            cone.idSegment = segment.id;
            cone.w = (segment.i.w < segment.j.w ? segment.i : segment.j);
            cone.idSegmentSonLeft = -1;
            cone.idSegmentSonRight = -1;

            cone.nodeList = createNodeList();
            tabCone[cone.idSegment] = cone;
        }
    }

    for (int i = 0; i < idSegment && 1 == 0; i++)
    {
        printCone(tabCone[i]);
    }
}

void updateIdSon()
{
    int wI, wJ;
    tabCone[2 * maxNumber - 1].idSegmentParent = tabCone[2 * maxNumber - 2].idSegmentParent = -1;
    for (int a = 0; a < 2 * maxNumber - 2; a = a + 2)
    {
        wI = tabSegment[a].i.w;
        wJ = tabSegment[a + 1].j.w;
        if (!(wI == 1 && wJ == 2) && !(wI == 2 && wJ == 1))
        {
            for (int b = a + 2; b < 2 * maxNumber - 2; b++)
            {
                if (tabSegment[b].i.w == wI && tabSegment[b].j.w == wJ)
                {
                    tabCone[b].idSegmentSonLeft = a;
                    tabCone[b].idSegmentSonRight = a + 1;

                    tabCone[a].idSegmentParent = tabCone[a + 1].idSegmentParent = b;
                    break;
                }
            }
        }
        else
        {
            if (wI == 1 && wJ == 2)
            {
                tabCone[2 * maxNumber - 2].idSegmentSonLeft = a;
                tabCone[2 * maxNumber - 2].idSegmentSonRight = a + 1;

                tabCone[a].idSegmentParent = tabCone[a + 1].idSegmentParent = 2 * maxNumber - 2;
            }
            else if (wI == 2 && wJ == 1)
            {
                tabCone[2 * maxNumber - 1].idSegmentSonLeft = a;
                tabCone[2 * maxNumber - 1].idSegmentSonRight = a + 1;

                tabCone[a].idSegmentParent = tabCone[a + 1].idSegmentParent = 2 * maxNumber - 1;
            }
        }
    }
}

void updateNodeList()
{
    NodeList *nodeList = createNodeList();
    nodeList = addNodeToNodeList(nodeList, tabPos[0]);
    tabCone[2 * maxNumber - 1].nodeList = tabCone[2 * maxNumber - 2].nodeList = nodeList;
    for (int i = 2 * maxNumber - 1; i > 1; i--)
    {
        if (tabSegment[tabCone[i].idSegment].type == 1)
        {
            tabCone[tabCone[i].idSegmentSonLeft].nodeList = tabCone[i].nodeList;
            if (tabSegment[tabCone[i].idSegmentSonLeft].type == 1)
            {
                if (tabCone[tabCone[i].idSegmentSonLeft].w.w != tabCone[tabCone[i].idSegmentSonLeft].nodeList->node.w)
                    tabCone[tabCone[i].idSegmentSonLeft].nodeList = addNodeToNodeList(tabCone[tabCone[i].idSegmentSonLeft].nodeList, tabCone[tabCone[i].idSegmentSonLeft].w);
            }

            tabCone[tabCone[i].idSegmentSonRight].nodeList = tabCone[i].nodeList;
            if (tabSegment[tabCone[i].idSegmentSonRight].type == 1)
            {
                if (tabCone[tabCone[i].idSegmentSonRight].w.w != tabCone[tabCone[i].idSegmentSonRight].nodeList->node.w)
                    tabCone[tabCone[i].idSegmentSonRight].nodeList = addNodeToNodeList(tabCone[tabCone[i].idSegmentSonRight].nodeList, tabCone[tabCone[i].idSegmentSonRight].w);
            }
        }
    }
}

void printCone(Cone c)
{
    printf("\nid segment : %d\n", c.idSegment);
    printf("\tNode i: (w = %d, v = %d, weight = %d)\n", tabSegment[c.idSegment].i.w, tabSegment[c.idSegment].i.v, tabSegment[c.idSegment].i.weight);
    printf("\tNode j: (w = %d, v = %d, weight = %d)\n", tabSegment[c.idSegment].j.w, tabSegment[c.idSegment].j.v, tabSegment[c.idSegment].j.weight);
    printf("\ttype = %d\n", tabSegment[c.idSegment].type);
    printf("\tw : (w = %d, v = %d, weight = %d)\n", c.w.w, c.w.v, c.w.weight);
    if (!isEmptyNodeList(c.nodeList))
    {
        NodeList *path = c.nodeList;
        while (path != NULL)
        {
            printf("\t\tNode List : (w = %d, v = %d, weight = %d)\n", path->node.w, path->node.v, path->node.weight);
            path = path->next;
        }
    }
    printf("\tid parent = %d\n", c.idSegmentParent);
    printf("\tid left son = %d\n", c.idSegmentSonLeft);
    printf("\tid right son = %d\n", c.idSegmentSonRight);
    printf("\tid block = %d\n", c.cgmInfo.idBlock);
    printf("\t\tid band = %d\n", c.cgmInfo.idBand);
    printf("\t\trank = %d\n", c.cgmInfo.rank);
    printf("\t\tcolor = %d\n", c.cgmInfo.color);
}

void printBlockInfo(BlockInfo blockInfo)
{
    printf("\nid block = %d\n", blockInfo.cgmInfo.idBlock);
    printf("\tid band = %d\n", blockInfo.cgmInfo.idBand);
    printf("\trank = %d\n", blockInfo.cgmInfo.rank);
    printf("\tload : %d\n", blockInfo.load);
    printf("\tnbCone : %d\n", blockInfo.nbCone);
}

void computeCone_1(Cone c, unsigned int (*tabMCOP)[maxNumber + 1])
{
    int nbConeToEvaluate = 0, nbConeEvaluated, path;
    if (tabSegment[c.idSegment].type == 0)
    {
        tabMCOP[c.idSegment][c.w.w] = 0;
        nbConeToEvaluate = c.w.w - 1;
        if (nbConeToEvaluate != 0)
        {
            nbConeEvaluated = 0;
            path = c.w.v + 1;
            while (nbConeEvaluated != nbConeToEvaluate)
            {
                if (tabPos[path % (maxNumber + 1)].w < c.w.w)
                {
                    tabMCOP[c.idSegment][tabPos[path % (maxNumber + 1)].w] = tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabPos[path % (maxNumber + 1)].weight;
                    nbConeEvaluated++;
                }
                path++;
            }
        }
    }
    else
    {
        tabMCOP[c.idSegment][c.w.w] = getMCOP(c.idSegmentSonLeft, c.w.w, tabMCOP) + getMCOP(c.idSegmentSonRight, c.w.w, tabMCOP);
        nbConeToEvaluate = c.w.w - 1;
        if (nbConeToEvaluate != 0)
        {
            nbConeEvaluated = 0;
            path = c.w.v + 1;
            while (nbConeEvaluated != nbConeToEvaluate)
            {
                if (tabPos[path % (maxNumber + 1)].w < c.w.w)
                {
                    tabMCOP[c.idSegment][tabPos[path % (maxNumber + 1)].w] = min(getMCOP(c.idSegment, c.w.w, tabMCOP) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabPos[path % (maxNumber + 1)].weight, getMCOP(c.idSegmentSonLeft, tabPos[path % (maxNumber + 1)].w, tabMCOP) + getMCOP(c.idSegmentSonRight, tabPos[path % (maxNumber + 1)].w, tabMCOP));
                    nbConeEvaluated++;
                }
                path++;
            }
        }
    }
}
void computeCone_2(Cone c, unsigned int (*tabMCOP)[maxNumber + 1])
{
    int path;

    if (tabSegment[c.idSegment].type == 0)
    {
        tabMCOP[c.idSegment][c.w.w] = 0;
        for (path = c.w.w - 1; path > 0; path--)
        {
            tabMCOP[c.idSegment][path] = tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabRank[path].weight;
        }
    }
    else
    {
        tabMCOP[c.idSegment][c.w.w] = getMCOP(c.idSegmentSonLeft, c.w.w, tabMCOP) + getMCOP(c.idSegmentSonRight, c.w.w, tabMCOP);
        for (path = c.w.w - 1; path > 0; path--)
        {
            tabMCOP[c.idSegment][path] = min(getMCOP(c.idSegment, c.w.w, tabMCOP) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabRank[path].weight, getMCOP(c.idSegmentSonLeft, tabRank[path].w, tabMCOP) + getMCOP(c.idSegmentSonRight, tabRank[path].w, tabMCOP));
        }
    }
}
int tcpAlgorithm_3(unsigned int (*tabMCOP)[maxNumber])
{
    Cone c;
    NodeList *paths;
    Node node;
    for (int i = 0; i < 2 * maxNumber; i++)
    {
        c = tabCone[i];
        if (tabSegment[c.idSegment].type == 0)
        {
            paths = c.nodeList;
            if (c.w.w == paths->node.w)
            {
                tabMCOP[c.idSegment][paths->node.w] = 0;
                paths = paths->next;
            }
            while (paths != NULL)
            {
                tabMCOP[c.idSegment][paths->node.w] = tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * paths->node.weight;
                paths = paths->next;
            }
        }
        else
        {
            paths = c.nodeList;
            tabMCOP[c.idSegment][paths->node.w] = getMCOP(c.idSegmentSonLeft, paths->node.w, tabMCOP) + getMCOP(c.idSegmentSonRight, paths->node.w, tabMCOP);
            node = paths->node;
            paths = paths->next;
            while (paths != NULL)
            {
                tabMCOP[c.idSegment][paths->node.w] = min(getMCOP(c.idSegment, node.w, tabMCOP) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * paths->node.weight, getMCOP(c.idSegmentSonLeft, paths->node.w, tabMCOP) + getMCOP(c.idSegmentSonRight, paths->node.w, tabMCOP));
                paths = paths->next;
            }
        }
    }
    return getMCOP(2 * maxNumber - 2, 1, tabMCOP) + getMCOP(2 * maxNumber - 1, 1, tabMCOP);
}

NodeList *createNodeList()
{
    NodeList *nodeList = (NodeList *)malloc(sizeof *nodeList);
    if (nodeList == NULL)
    {
        logE("insuffisant memory for allocate nodeList\n");
        exit(EXIT_FAILURE);
    }
    nodeList->node.v = nodeList->node.w = nodeList->node.weight = -1;
    nodeList->next = NULL;
    return nodeList;
}

int isEmptyNodeList(NodeList *nodeList)
{
    return nodeList == NULL || (nodeList->node.v == -1 && nodeList->node.w == -1 && nodeList->node.weight == -1);
}

NodeList *addNodeToNodeList(NodeList *nodeList, Node node)
{
    if (isEmptyNodeList(nodeList))
    {
        nodeList->node = node;
        return nodeList;
    }
    else
    {
        NodeList *newHead = createNodeList();
        newHead->node = node;
        newHead->next = nodeList;
        return newHead;
    }
}

BlockInfo constructBlock(int tcb, int processor, int *endBand, int *pivot)
{
    int endBlock = 0, nbCone = 0, load = 0, idCone = -1, incPivot = 1, isLastBlockOfBand = 0;
    Cone c;

    CGMInfo cgmInfo;
    cgmInfo.color = 0;
    cgmInfo.idBand = idBand;
    cgmInfo.idBlock = idBlock;
    cgmInfo.rank = processor;

    BlockInfo blockInfo;
    blockInfo.cgmInfo = cgmInfo;
    blockInfo.load = 0;
    blockInfo.nbCone = 0;

    idCone = findLeftMostNoColorLeaf(pivot);
    if (idCone == -1 || (idBand == 1 && *pivot > (maxNumber + 1)))
    {
        *endBand = 1;
        endBlock = 1;
    }
    else
    {

        while (endBlock == 0)
        {
            if (isLastBlockOfBand == 0 && idLastConeBand == idCone)
            {
                isLastBlockOfBand = 1;
            }
            tabCone[idCone].cgmInfo = cgmInfo;
            tabCone[idCone].cgmInfo.color = 2;
            c = tabCone[idCone];

            load += c.w.w;
            nbCone++;
            if (incPivot)
                *pivot = *pivot + 1;

            if (load >= tcb || (idBand > 1 && isRoot(c) == 1))
            {
                if (idBand == 1 && *pivot > (maxNumber + 1))
                {
                    *endBand = 1;
                    idLastConeBand = tabCone[idCone].idSegmentParent;
                }
                else if (idBand > 1)
                {
                    if (isRoot(c) == 1)
                    {
                        *endBand = 1;
                    }
                    else if (isLastBlockOfBand == 1)
                    {
                        *endBand = 1;
                        idLastConeBand = tabCone[idCone].idSegmentParent;
                    }
                }

                endBlock = 1;
            }

            else
            {
                if (c.idSegmentParent > 0 && tabCone[c.idSegmentParent].cgmInfo.color == 1)
                {
                    idCone = c.idSegmentParent;
                    currentDAG = removeSegmentToSegmentList(currentDAG, tabSegment[c.idSegmentParent]);
                    incPivot = 0;
                }
                else
                {
                    incPivot = 1;
                    tabCone[c.idSegmentParent].cgmInfo.color = 1;
                    if (idBand == 1)
                    {
                        if (*pivot <= (maxNumber + 1))
                        {
                            idCone = findLeftMostNoColorLeaf(pivot);
                        }
                        else
                        {
                            idLastConeBand = tabCone[idCone].idSegmentParent;
                            idCone = -1;
                        }
                    }
                    else
                        idCone = findLeftMostNoColorLeaf(pivot);

                    if (idCone == -1)
                    {
                        *endBand = 1;
                        endBlock = 1;
                    }
                }
            }
        }
    }
    blockInfo.load = load;
    blockInfo.nbCone = nbCone;
    return blockInfo;
}

int findLeftMostNoColorLeaf(int *pivot)
{
    int result = -1, find = 0;
    if (idBand == 1)
    {
        int wI, wJ;
        wI = tabPos[*pivot - 1].w;
        wJ = tabPos[*pivot % (maxNumber + 1)].w;
        SegmentList *path = currentDAG;
        Segment s;
        while (path != NULL && find == 0)
        {
            if (path->segment.i.w == wI && path->segment.j.w == wJ)
            {
                s = path->segment;
                find = 1;
            }
            path = path->next;
        }
        result = s.id;
        currentDAG = removeSegmentToSegmentList(currentDAG, s);
        return result;
    }
    else
    {
        SegmentList *path = currentDAG;
        Segment s;
        Cone c;
        while (path != NULL && find == 0)
        {
            s = path->segment;
            c = tabCone[s.id];
            if (tabCone[c.idSegmentSonLeft].cgmInfo.color == 2 && tabCone[c.idSegmentSonRight].cgmInfo.color == 2)
            {
                find = 1;
                if (tabCone[c.idSegmentSonLeft].cgmInfo.idBand == tabCone[c.idSegmentSonRight].cgmInfo.idBand)
                {
                    if (tabCone[c.idSegmentSonLeft].cgmInfo.idBand == idBand)
                    {
                        if (tabCone[c.idSegmentSonLeft].cgmInfo.idBlock != tabCone[c.idSegmentSonRight].cgmInfo.idBlock)
                        {
                            find = 0;
                        }
                    }
                }
                else if (tabCone[c.idSegmentSonLeft].cgmInfo.idBand == idBand)
                {
                    if (tabCone[c.idSegmentSonLeft].cgmInfo.idBlock != idBlock)
                    {
                        find = 0;
                    }
                }
                else if (tabCone[c.idSegmentSonRight].cgmInfo.idBand == idBand)
                {
                    if (tabCone[c.idSegmentSonRight].cgmInfo.idBlock != idBlock)
                    {
                        find = 0;
                    }
                }
            }

            path = path->next;
        }
        if (find == 1)
        {
            result = s.id;
            currentDAG = removeSegmentToSegmentList(currentDAG, s);
        }
        else
        {
            result = -1;
        }

        return result;
    }
}

int isRoot(Cone c)
{
    if (c.idSegmentParent == -1)
    {
        if (tabCone[2 * maxNumber - 2].cgmInfo.color == 2 && tabCone[2 * maxNumber - 1].cgmInfo.color == 2)
            return 1;
    }
    return 0;
}

SegmentList *createSegmentList()
{
    SegmentList *segmentList = (SegmentList *)malloc(sizeof *segmentList);
    if (segmentList == NULL)
    {
        logE("insuffisant memory for allocate segmentList\n");
        exit(EXIT_FAILURE);
    }
    segmentList->segment.id = -1;
    segmentList->segment.i.v = segmentList->segment.i.w = segmentList->segment.i.weight = -1;
    segmentList->segment.j.v = segmentList->segment.j.w = segmentList->segment.j.weight = -1;
    segmentList->next = NULL;
    return segmentList;
}

int isEmptySegmentList(SegmentList *list)
{
    return list == NULL || list->segment.id == -1 || (list->segment.i.v == -1 && list->segment.i.w == -1 && list->segment.i.weight == -1) || (list->segment.j.v == -1 && list->segment.j.w == -1 && list->segment.j.weight == -1);
}

SegmentList *addSegmentToSegmentList(SegmentList *list, Segment segment)
{
    if (isEmptySegmentList(list))
    {
        list->segment = segment;
        return list;
    }
    else
    {
        SegmentList *newHead = createSegmentList();
        newHead->segment = segment;
        newHead->next = list;
        return newHead;
    }
}

SegmentList *removeSegmentToSegmentList(SegmentList *list, Segment segment)
{
    int first = 1, find = 0;
    if (isEmptySegmentList(list))
        return NULL;
    else
    {
        SegmentList *path = list;
        SegmentList *del = path;
        SegmentList *prec = path;
        while (path != NULL && find == 0)
        {
            if (path->segment.id == segment.id)
            {
                find = 1;
            }
            else
            {
                first = 0;
                prec = path;
                path = path->next;
            }
        }

        if (first == 1)
        {
            del = list;
            list = list->next;
            free(del);
            path = list;
            prec = list;
        }
        else
        {
            del = path;
            prec->next = path->next;
            free(del);
            path = prec->next;
        }
        return list;
    }
}

BlockInfoList *createBlockInfoList()
{
    BlockInfoList *blockInfoList = (BlockInfoList *)malloc(sizeof *blockInfoList);
    if (blockInfoList == NULL)
    {
        logE("insuffisant memory for allocate blockInfoList\n");
        exit(EXIT_FAILURE);
    }
    blockInfoList->blockInfo.load = blockInfoList->blockInfo.nbCone = -1;
    blockInfoList->next = NULL;
    return blockInfoList;
}

int isEmptyBlockInfoList(BlockInfoList *list)
{
    return list == NULL || (list->blockInfo.load == -1 && list->blockInfo.nbCone == -1);
}

void addBlockInfoToBlockInfoList(BlockInfoList *list, BlockInfo blockInfo)
{
    if (isEmptyBlockInfoList(list))
    {
        list->blockInfo = blockInfo;
    }
    else
    {
        BlockInfoList *path = list;
        while (path->next != NULL)
        {
            path = path->next;
        }

        BlockInfoList *newEnd = createBlockInfoList();
        newEnd->blockInfo = blockInfo;
        path->next = newEnd;
    }
}

DataList *createDataList()
{
    DataList *dataList = (DataList *)malloc(sizeof *dataList);
    if (dataList == NULL)
    {
        logE("insuffisant memory for allocate DataList\n");
        exit(EXIT_FAILURE);
    }
    dataList->data = -1;
    dataList->next = NULL;
    return dataList;
}

int isEmptyDataList(DataList *list)
{
    return (list == NULL || list->data == -1);
}

DataList *addDataToDataList(DataList *list, int data)
{
    if (isEmptyDataList(list))
    {
        list->data = data;
        return list;
    }
    else
    {
        DataList *newHead = createDataList();
        newHead->data = data;
        newHead->next = list;
        return newHead;
    }
}
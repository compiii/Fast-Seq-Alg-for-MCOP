/**
 * @file tcp.c
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-12-14
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#include "tcp.h"
#include "clogger.h"
#include "stack.h"

void initTCPMod()
{
    int i, j, pos, lower = 0;

    if (tabMCOP != NULL)
    {
        free(tabMCOP[0]);
        free(tabMCOP);
    }

    if (algorithmId == 0)
    {
        tabMCOP = (int **)malloc((maxNumber + 1) * sizeof *tabMCOP);
        if (tabMCOP == NULL)
        {
            logE("insuffisant memory for allocate tabMCOP\n");
            exit(EXIT_FAILURE);
        }

        tabMCOP[0] = (int *)malloc((maxNumber + 1) * (maxNumber + 1) * sizeof *tabMCOP[0]);
        if (tabMCOP[0] == NULL)
        {
            logE("insuffisant memory for allocate tabMCOP[0]\n");
            exit(EXIT_FAILURE);
        }

        for (i = 1; i < (maxNumber + 1); i++)
        {
            tabMCOP[i] = tabMCOP[0] + i * (maxNumber + 1);
        }
    }
    else
    {
        tabMCOP = (int **)malloc(2 * maxNumber * sizeof *tabMCOP);
        if (tabMCOP == NULL)
        {
            logE("insuffisant memory for allocate tabMCOP\n");
            exit(EXIT_FAILURE);
        }

        tabMCOP[0] = (int *)malloc(2 * maxNumber * (maxNumber + 1) * sizeof *tabMCOP[0]);
        if (tabMCOP[0] == NULL)
        {
            logE("insuffisant memory for allocate tabMCOP[0]\n");
            exit(EXIT_FAILURE);
        }

        for (i = 1; i < 2 * maxNumber; i++)
        {
            tabMCOP[i] = tabMCOP[0] + i * (maxNumber + 1);
        }

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

        if (algorithmId == 2)
        {
            for (i = 1; i < (maxNumber + 2); i++)
            {
                tabRank[i] = tabPos[i - 1];
            }
        }
        else if (algorithmId == 3)
        {
            for (i = 0; i < (maxNumber + 1); i++)
            {
                tabRank[tabPos[i].w] = tabPos[i];
            }
        }

        tabSegment = (Segment *)malloc(2 * maxNumber * sizeof *tabSegment);
        tabCone = (Cone *)malloc(2 * maxNumber * sizeof *tabCone);
        if (tabSegment == NULL || tabCone == NULL)
        {
            logE("insuffisant memory for allocate tabSegment or tabCone\n");
            exit(EXIT_FAILURE);
        }
    }
}

int godboleAlgorithm()
{
    int d, i, k, tmp;
    for (d = 1; d <= maxNumber; d++)
    {
        for (i = 1; i <= (maxNumber - d + 1); i++)
        {
            tabMCOP[i][i + d - 1] = (i == (i + d - 1)) ? 0 : INT_MAX;
            for (k = i; k <= i + d - 2; k++)
            {
                tmp = getMCOP(i, k) + getMCOP(k + 1, i + d - 1) + tabDim[i - 1] * tabDim[k] * tabDim[i + d - 1];
                if (tmp < tabMCOP[i][i + d - 1])
                {
                    tabMCOP[i][i + d - 1] = tmp;
                }
            }
        }
    }
    return getMCOP(1, maxNumber);
}

int getMCOP(int i, int j)
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
    Node currentNode = tabPos[0], tmp, pivot;
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
            segment.type = (segment.i.v == segment.j.v - 1 || segment.i.v + segment.j.v == maxNumber ? 0 : 1);
            tabSegment[segment.id] = segment;
            if (algorithmId == 3)
            {
                pivot = (segment.i.w < segment.j.w ? segment.i : segment.j);
            }

            cone.idSegment = segment.id;
            cone.w = (segment.i.w < segment.j.w ? segment.i : segment.j);
            cone.idSegmentSonLeft = -1;
            cone.idSegmentSonRight = -1;
            cone.nodeList = createNodeList();
            tabCone[cone.idSegment] = cone;

            segment.id = idSegment++;
            segment.i = tmp;
            segment.j = currentNode;
            segment.type = (segment.i.v == segment.j.v - 1 || segment.i.v + segment.j.v == maxNumber ? 0 : 1);
            tabSegment[segment.id] = segment;
            if (algorithmId == 3)
            {
                tmp = (segment.i.w < segment.j.w ? segment.i : segment.j);
                pivot = (pivot.w < tmp.w ? pivot : tmp);
            }

            cone.idSegment = segment.id;
            cone.w = (segment.i.w < segment.j.w ? segment.i : segment.j);
            cone.idSegmentSonLeft = -1;
            cone.idSegmentSonRight = -1;
            cone.nodeList = createNodeList();
            tabCone[cone.idSegment] = cone;

            if (algorithmId == 3000)
            {
                Stack *path = s;
                Node node;
                while (path->next != NULL)
                {
                    node = *(Node *)path->data;
                    if (node.w < pivot.w)
                    {
                        tabCone[cone.idSegment - 1].nodeList = addNodeToNodeList(tabCone[cone.idSegment - 1].nodeList, node);
                        tabCone[cone.idSegment].nodeList = addNodeToNodeList(tabCone[cone.idSegment].nodeList, node);
                    }
                    path = path->next;
                }
                tabCone[cone.idSegment - 1].nodeList = addNodeToNodeList(tabCone[cone.idSegment - 1].nodeList, pivot);
                tabCone[cone.idSegment].nodeList = addNodeToNodeList(tabCone[cone.idSegment].nodeList, pivot);

                if (tabSegment[tabCone[cone.idSegment - 1].idSegment].type == 1)
                {
                    segment = tabSegment[tabCone[cone.idSegment - 1].idSegment];
                    tmp = (segment.i.w < segment.j.w ? segment.i : segment.j);
                    if (tmp.w != tabCone[cone.idSegment - 1].nodeList->node.w)
                        tabCone[cone.idSegment - 1].nodeList = addNodeToNodeList(tabCone[cone.idSegment - 1].nodeList, tmp);
                }

                if (tabSegment[tabCone[cone.idSegment].idSegment].type == 1)
                {
                    segment = tabSegment[tabCone[cone.idSegment].idSegment];
                    tmp = (segment.i.w < segment.j.w ? segment.i : segment.j);
                    if (tmp.w != tabCone[cone.idSegment].nodeList->node.w)
                        tabCone[cone.idSegment].nodeList = addNodeToNodeList(tabCone[cone.idSegment].nodeList, tmp);
                }
            }
        }
    }
}

void updateIdSon()
{
    int wI, wJ;
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
            }
            else if (wI == 2 && wJ == 1)
            {
                tabCone[2 * maxNumber - 1].idSegmentSonLeft = a;
                tabCone[2 * maxNumber - 1].idSegmentSonRight = a + 1;
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
    printf("\tid left son = %d\n", c.idSegmentSonLeft);
    printf("\tid right son = %d\n", c.idSegmentSonRight);
}

int tcpAlgorithm_1()
{
    Cone c;
    int nbConeToEvaluate = 0, nbConeEvaluated, path;
    for (int i = 0; i < 2 * maxNumber; i++)
    {
        c = tabCone[i];
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
            tabMCOP[c.idSegment][c.w.w] = getMCOP(c.idSegmentSonLeft, c.w.w) + getMCOP(c.idSegmentSonRight, c.w.w);
            nbConeToEvaluate = c.w.w - 1;
            if (nbConeToEvaluate != 0)
            {
                nbConeEvaluated = 0;
                path = c.w.v + 1;
                while (nbConeEvaluated != nbConeToEvaluate)
                {
                    if (tabPos[path % (maxNumber + 1)].w < c.w.w)
                    {
                        tabMCOP[c.idSegment][tabPos[path % (maxNumber + 1)].w] = min(getMCOP(c.idSegment, c.w.w) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabPos[path % (maxNumber + 1)].weight, getMCOP(c.idSegmentSonLeft, tabPos[path % (maxNumber + 1)].w) + getMCOP(c.idSegmentSonRight, tabPos[path % (maxNumber + 1)].w));
                        nbConeEvaluated++;
                    }
                    path++;
                }
            }
        }
    }

    return getMCOP(2 * maxNumber - 2, 1) + getMCOP(2 * maxNumber - 1, 1);
}
int tcpAlgorithm_2()
{
    Cone c;
    int path;
    quicksort(tabRank, 1, maxNumber + 1);
    for (int i = 0; i < 2 * maxNumber; i++)
    {
        c = tabCone[i];
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
            tabMCOP[c.idSegment][c.w.w] = getMCOP(c.idSegmentSonLeft, c.w.w) + getMCOP(c.idSegmentSonRight, c.w.w);
            for (path = c.w.w - 1; path > 0; path--)
            {
                tabMCOP[c.idSegment][path] = min(getMCOP(c.idSegment, c.w.w) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * tabRank[path].weight, getMCOP(c.idSegmentSonLeft, tabRank[path].w) + getMCOP(c.idSegmentSonRight, tabRank[path].w));
            }
        }
    }
    return getMCOP(2 * maxNumber - 2, 1) + getMCOP(2 * maxNumber - 1, 1);
}

int tcpAlgorithm_3()
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
            tabMCOP[c.idSegment][paths->node.w] = getMCOP(c.idSegmentSonLeft, paths->node.w) + getMCOP(c.idSegmentSonRight, paths->node.w);
            node = paths->node;
            paths = paths->next;
            while (paths != NULL)
            {
                tabMCOP[c.idSegment][paths->node.w] = min(getMCOP(c.idSegment, node.w) + tabSegment[c.idSegment].i.weight * tabSegment[c.idSegment].j.weight * paths->node.weight, getMCOP(c.idSegmentSonLeft, paths->node.w) + getMCOP(c.idSegmentSonRight, paths->node.w));
                paths = paths->next;
            }
        }
    }
    return getMCOP(2 * maxNumber - 2, 1) + getMCOP(2 * maxNumber - 1, 1);
}

int tcpAlgorithm()
{
    if (algorithmId == 0)
    {
        return godboleAlgorithm();
    }
    else
    {
        markBridges();
        updateIdSon();
        if (algorithmId == 1)
        {
            return tcpAlgorithm_1();
        }
        else if (algorithmId == 2)
        {
            return tcpAlgorithm_2();
        }
        else if (algorithmId == 3)
        {
            updateNodeList();
            return tcpAlgorithm_3();
        }
    }
    return -1;
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
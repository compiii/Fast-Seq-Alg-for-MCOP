/**
 * @file tcp.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-06
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef TCP_H_
#define TCP_H_

#include "main.h"

typedef struct Node_T Node;
struct Node_T
{
    int w;                // rank following the nodes of a polygon
    int v;                // position in the polygon, in a clockwise order
    unsigned char weight; // weight of vertice
};

typedef struct Segment_T Segment;
struct Segment_T
{
    int id;
    Node i;
    Node j;
    char type; // 0 = side segment && 1 = diagonal segment
};

typedef struct NodeList_T NodeList;
struct NodeList_T
{
    Node node;
    NodeList *next;
};

typedef struct CGMInfo_T CGMInfo;
struct CGMInfo_T
{
    int rank;
    int idBlock;
    int idBand;
    int color; // 0 = no color; 1 = gray color; 2 = black color
};

typedef struct Cone_T Cone;
struct Cone_T
{
    int idSegment;
    int idSegmentParent;
    Node w;
    NodeList *nodeList;
    int idSegmentSonLeft;
    int idSegmentSonRight;
    CGMInfo cgmInfo;
};

typedef struct DataList_T DataList;
struct DataList_T
{
    int data;
    DataList *next;
};

typedef struct BlockInfo_T BlockInfo;
struct BlockInfo_T
{
    CGMInfo cgmInfo;
    int nbCone;
    int load;
    DataList *dataList;
};

typedef struct BlockInfoList_T BlockInfoList;
struct BlockInfoList_T
{
    BlockInfo blockInfo;
    BlockInfoList *next;
};

typedef struct SegmentList_T SegmentList;
struct SegmentList_T
{
    Segment segment;
    SegmentList *next;
};

typedef struct Bound_T Bound;
struct Bound_T
{
    int begin;
    int end;
};

Node *tabRank,
    *tabPos;

Segment *tabSegment;
Cone *tabCone;

int sizeBlock;

SegmentList *currentDAG;
BlockInfoList *blockInfoList, *allBlockInfoList;

int idBlock, idBand, idLastConeBand, totalLoadBlock, totalNumberCone;

int nbBlock;

NodeList **tabNodeList;

void initTCPMod();
int getMCOP(int i, int j, unsigned int (*tabMCOP)[maxNumber]);
int max(int n, int m);
int min(int m, int n);
void cyclicRotation(int *tabDim);

void markBridges();
void updateIdSon();
void updateNodeList();
void computeCone_1(Cone c, unsigned int (*tabMCOP)[maxNumber]);
void computeCone_2(Cone c, unsigned int (*tabMCOP)[maxNumber]);
int tcpAlgorithm_3(unsigned int (*tabMCOP)[maxNumber]);
void printCone(Cone c);

void swap(Node *a, Node *b);
void quicksort(Node arr[], int l, int r);

NodeList *createNodeList();
int isEmptyNodeList(NodeList *list);
NodeList *addNodeToNodeList(NodeList *list, Node node);

SegmentList *createSegmentList();
int isEmptySegmentList(SegmentList *list);
SegmentList *addSegmentToSegmentList(SegmentList *list, Segment segment);
SegmentList *removeSegmentToSegmentList(SegmentList *list, Segment segment);

BlockInfoList *createBlockInfoList();
int isEmptyBlockInfoList(BlockInfoList *list);
void addBlockInfoToBlockInfoList(BlockInfoList *list, BlockInfo blockInfo);

DataList *createDataList();
int isEmptyDataList(DataList *list);
DataList *addDataToDataList(DataList *list, int data);

BlockInfo constructBlock(int tcb, int processor, int *endBand, int *pivot);
int findLeftMostNoColorLeaf(int *pivot);
int isRoot(Cone c);

void printBlockInfo(BlockInfo blockInfo);
#endif

/**
 * @file tcp.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-12-14
 * 
 * @copyright Compiii Thesis Copyright (c) 2019
 * 
 */
#ifndef TCP_H_
#define TCP_H_

#include "main.h"

typedef struct Node_T Node;
struct Node_T
{
    int w;      // rank following the nodes of a polygon
    int v;      // position in the polygon, in a clockwise order
    int weight; // weight of vertice
};

typedef struct Segment_T Segment;
struct Segment_T
{
    int id;
    Node i;
    Node j;
    int type; // 0 = side segment && 1 = diagonal segment
};

typedef struct NodeList_T NodeList;
struct NodeList_T
{
    Node node;
    NodeList *next;
};

typedef struct Cone_T Cone;
struct Cone_T
{
    int idSegment;
    Node w;
    NodeList *nodeList;
    int idSegmentSonLeft;
    int idSegmentSonRight;
};

Node *tabRank, *tabPos;

Segment *tabSegment;
Cone *tabCone;

int **tabMCOP;

void initTCPMod();
int getMCOP(int i, int j);
int godboleAlgorithm();
int tcpAlgorithm_1();
int tcpAlgorithm_2();
int tcpAlgorithm_3();
int tcpAlgorithm();
int max(int n, int m);
int min(int m, int n);
void cyclicRotation(int *tabDim);

void markBridges();
void updateIdSon();
void updateNodeList();
void printCone(Cone c);

void swap(Node *a, Node *b);
void quicksort(Node arr[], int l, int r);

NodeList *createNodeList();
int isEmptyNodeList(NodeList *list);
NodeList *addNodeToNodeList(NodeList *list, Node node);
#endif

/**
 * @file kechid.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef KECHID_H_
#define KECHID_H_
#include "tcp.h"

int kechid();
void partitionningKechid();
void constructBandKechid(int tcb);

int cgmAlgorithmKechid(unsigned int (*tabMCOP)[maxNumber + 1]);
#endif
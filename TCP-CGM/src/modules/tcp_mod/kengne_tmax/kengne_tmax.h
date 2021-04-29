/**
 * @file kengne_tmax.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef KENGNE_TMAX_H_
#define KENGNE_TMAX_H_
#include "tcp.h"

int kengneTmax();
void partitionningTmax();
void constructBandTmax(int tcb);

int cgmAlgorithmKengneTmax(unsigned int (*tabMCOP)[maxNumber + 1]);

#endif
/**
 * @file kengne_tint.h
 * @author Jerry Lacmou (jerrylacmou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Compiii Thesis Copyright (c) 2020
 * 
 */
#ifndef KENGNE_TINT_H_
#define KENGNE_TINT_H_
#include "tcp.h"

int kengneTint();
void partitionningTint();
void constructBandTint(int tcb);
int constructFirstBandTint(int tc);

int cgmAlgorithmKengneTint(unsigned int (*tabMCOP)[maxNumber + 1]);
#endif
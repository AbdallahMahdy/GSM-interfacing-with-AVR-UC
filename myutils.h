/*
 * myutils.h
 *
 *  Created on: Jan 25, 2021
 *      Author: bedoa
 */

#ifndef MYUTILS_H_
#define MYUTILS_H_

#include"DIO.h"

 #define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)


#endif /* MYUTILS_H_ */

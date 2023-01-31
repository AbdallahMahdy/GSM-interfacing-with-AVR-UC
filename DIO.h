/*
 * DIO.h
 *
 *  Created on: Jan 12, 2019
 *      Author: lenovo
 */

#ifndef DIO_H_
#define DIO_H_
#include "stdint.h"
#define a 0
#define b 1
#define c 2
#define d 3
#define output 1
#define input 0
#define pin0 0
#define pin1 1
#define pin2 2
#define pin3 3
#define pin4 4
#define pin5 5
#define pin6 6
#define pin7 7
#define high 1
#define low 0
extern void DIO_voidSetPinDirection(uint8_t Port,uint8_t PinNum,uint8_t Dir);
extern void DIO_voidSetPinValue(uint8_t Port,uint8_t PinNum,uint8_t value);
extern uint8_t DIO_uint8_tGetPinValue(uint8_t Port ,uint8_t PinNum);
extern void DIO_voidSetPortDir(uint8_t Port, uint8_t Dir);
extern void DIO_voidSetPortValue(uint8_t Port,uint8_t Value);


#endif /* DIO_H_ */

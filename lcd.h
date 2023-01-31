#include <avr/io.h>
#include <util/delay.h>
#include "myutils.h"
#include "DIO.h"

#ifndef _LCD_H
#define _LCD_H

#define LCD_data b
#define LCD_control c
#define RSpin pin0
#define RWpin pin1
#define Epin pin2

extern void LCD_vidExcuteCommand(uint8_t Command);
extern void LCD_vidDisplayData(uint8_t Data);
extern void  LCD_vidINT(void);
extern void LCD_vidGoToposition(uint8_t x,uint8_t y);
extern void LCD_vidDisplaystring(uint8_t *p);
extern void LCD_vidDisplayINT(uint16_t num);


#endif

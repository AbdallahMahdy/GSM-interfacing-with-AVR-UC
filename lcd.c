#include<avr/delay.h>
#include<stdint.h>
#include "lcd.h"



 extern void LCD_vidExcuteCommand(uint8_t Command){
	DIO_voidSetPinValue(LCD_control,RSpin,low);
   DIO_voidSetPinValue(LCD_control,RWpin,low);
	DIO_voidSetPortValue(LCD_data,Command);
	DIO_voidSetPinValue(LCD_control,Epin,high);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_control,Epin,low);
}

extern void LCD_vidDisplayData(uint8_t Data){
	DIO_voidSetPinValue(LCD_control,RSpin,high);
	DIO_voidSetPinValue(LCD_control,RWpin,low);
	DIO_voidSetPortValue(LCD_data,Data);
	DIO_voidSetPinValue(LCD_control,Epin,high);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_control,Epin,low);
}
extern void  LCD_vidINT(void){
	DIO_voidSetPortDir(LCD_data,0b11111111);
	DIO_voidSetPinDirection(LCD_control,RSpin,high);
	DIO_voidSetPinDirection(LCD_control,RWpin,high);
	DIO_voidSetPinDirection(LCD_control,Epin,high);
_delay_ms(50);
   LCD_vidExcuteCommand(0b00111100);
   _delay_ms(1);
   LCD_vidExcuteCommand(0b00001100);
   _delay_ms(1);
   LCD_vidExcuteCommand(0b00000001);

}
extern void LCD_vidDisplaystring(uint8_t *p){
	while(*p)
	{
		LCD_vidDisplayData(*p++);
	}
}




extern void LCD_vidGoToposition(uint8_t x,uint8_t y){
	if(y==0 && x<=0xf && x>=0x0){
		LCD_vidExcuteCommand(0x80 +x);
	}
	else if(y==1 && x<=0xf && x>=0x0){
		LCD_vidExcuteCommand(0xc0 +x);
	}
	else{
		LCD_vidExcuteCommand(0x83);
		LCD_vidDisplaystring("Wrong address");
	}
}

extern void LCD_vidDisplayINT(uint16_t num)
{

	uint16_t y=1;
	if (num==0)
		LCD_vidDisplayData('0');
	if(num<0){
		LCD_vidDisplayData('-');
		num=num*-1;
	}
	while(num !=0){
		y=((y*10)+(num%10));
		num=num/10;
	}
    while(y!=1){
    	LCD_vidDisplayData((y%10)+'0');
    	y=y/10;
    }
}

#include<avr/delay.h>
#include "utils.h"
#include<avr/io.h>
extern void DIO_voidSetPinDirection(uint8_t Port,uint8_t PinNum,uint8_t Dir)
{
	switch(Port)
	{
	case 0:
		if(Dir==0)
			clr_bit(DDRA,PinNum);
		if(Dir==1)
			set_bit(DDRA,PinNum);
			break;
	case 1:
			if(Dir==0)
				clr_bit(DDRB,PinNum);
			if(Dir==1)
				set_bit(DDRB,PinNum);
				break;
	case 2:
			if(Dir==0)
				clr_bit(DDRC,PinNum);
			if(Dir==1)
				set_bit(DDRC,PinNum);
				break;
	case 3:
			if(Dir==0)
				clr_bit(DDRD,PinNum);
			if(Dir==1)
				set_bit(DDRD,PinNum);
				break;

	}
	}
extern void DIO_voidSetPinValue(uint8_t Port,uint8_t PinNum,uint8_t value)
{
	switch(Port)
	{
	case 0:
		if(value==0)
			clr_bit(PORTA,PinNum);
		if(value==1)
			set_bit(PORTA,PinNum);
			break;
	case 1:
			if(value==0)
				clr_bit(PORTB,PinNum);
			if(value==1)
				set_bit(PORTB,PinNum);
				break;
	case 2:
			if(value==0)
				clr_bit(PORTC,PinNum);
			if(value==1)
				set_bit(PORTC,PinNum);
				break;
	case 3:
			if(value==0)
				clr_bit(PORTD,PinNum);
			if(value==1)
				set_bit(PORTD,PinNum);
				break;

	}
	}
extern uint8_t DIO_uint8_tGetPinValue(uint8_t Port ,uint8_t PinNum)
		{
	switch(Port)

	{
	case 0:
		return get_bit(PINA,PinNum);
		break;
	case 1:
			return get_bit(PINB,PinNum);
			break;
	case 2:
			return get_bit(PINC,PinNum);
			break;
	case 3:
			return get_bit(PIND,PinNum);
			break;
	}
		}
	extern void DIO_voidSetPortDir(uint8_t Port, uint8_t Dir)
	{
		switch(Port)
		{
		case 0 :
			DDRA=Dir;
			break;
		case 1 :
			DDRB=Dir;
			break;
		case 2 :
			DDRC=Dir;
			break;
		case 3 :
			DDRD=Dir;
			break;
		}
	}
extern void DIO_voidSetPortValue(uint8_t Port,uint8_t Value)
	{
		switch(Port)
		{
	case 0 :
			PORTA=Value;
			break;
		case 1 :
			PORTB=Value;
			break;
		case 2 :
			PORTC=Value;
			break;
		case 3 :
			PORTD=Value;
			break;
		}
	}


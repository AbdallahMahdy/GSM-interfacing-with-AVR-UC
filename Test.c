#define F_CPU 8000000UL		/* define Clock Frequency */
#include<avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"	/* include 16x2 LCD Header file */
#include "USART_Interrupt.h"	/* include USART Header file */

#define SREG   _SFR_IO8(0x3F)

void GSM_Begin();
void GSM_Response();
void GSM_Response_Display();
void GSM_Msg_Delete(unsigned int);
void GSM_Send_Msg(char* , char*);
void GSM_Delete_All_Msg();

char buff[160];			/* buffer to store responses and messages */
volatile int buffer_pointer;
char Mobile_no[14];		/* store mobile no. of received message */
int position = 0;		/* save location of current message */


int main(void)
{
	buffer_pointer = 0;
	int is_msg_arrived;
	char number[14]={'0','0','2','0','1','0','9','1','5','7','7','3','7','6'};
	char msg[]={"harry up","I am in Tired"
	memset(message_received, 0, 60);
        LCD_vidINT();
	USART_Init(19200);	/* initialize USART */
	sei();
	LCD_vidGoToposition(0,0);
	LCD_vidDisplaystring("GSM Initializing");
	_delay_ms(3000);
	LCD_vidExcuteCommand(0b00000001);
	LCD_vidGoToposition(1,0);
	LCD_vidDisplaystring("AT");
	GSM_Begin();		/* Start GSM with AT*/
	LCD_vidExcuteCommand(0b00000001);

	while (1){







	}


}





void GSM_Begin()
{

	while(1)
	{
		LCD_vidExcuteCommand(0xc0);
		USART_SendString("ATE0\r"); /* send ATE0 to check module is ready or not */ //AT+CCLK? to get RTC
		_delay_ms(500);

		if(strstr(buff,"OK"))
		{
			GSM_Response();
			memset(buff,0,160);
			break;
		}
		else
		{
	        LCD_vidGoToposition(1,0);
	   	   LCD_vidDisplaystring("Erorr");
		}
	}
	_delay_ms(1000);

	LCD_vidExcuteCommand(0b00000001);
    LCD_vidGoToposition(1,0);
    LCD_vidDisplaystring("Text Mode");
	LCD_vidExcuteCommand(0xc0);
	USART_SendString("AT+CMGF=1\r"); /* select message format as text */
	GSM_Response();
	_delay_ms(1000);
}

void GSM_Msg_Delete(unsigned int position)
{
	buffer_pointer=0;
	char delete_cmd[20];

      /* delete message at specified position */
	sprintf(delete_cmd,"AT+CMGD=%d\r",position);
	USART_SendString(delete_cmd);
}

void GSM_Delete_All_Msg()
{
   USART_SendString("AT+CMGDA=\"DEL ALL\"\r"); /* delete all messages of SIM */
}



/* ISR routine to save responses/new message */
ISR(USART_RXC_vect)
{
   buff[buffer_pointer] = UDR;	/* copy UDR (received value) to buffer */
   buffer_pointer++;
   status_flag = 1;		/* flag for new message arrival */
}


void GSM_Send_Msg(char *num,char *sms)
{
	char sms_buffer[35];
	buffer_pointer=0;
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	USART_SendString(sms_buffer); /*send command AT+CMGS="Mobile No."\r */
	_delay_ms(200);
	while(1)
	{
		if(buff[buffer_pointer]==0x3e) /* wait for '>' character*/
		{
		   buffer_pointer = 0;
		   memset(buff,0,strlen(buff));
		   USART_SendString(sms); /* send msg to given no. */
		   USART_TxChar(0x1a); /* send Ctrl+Z */
		   break;
		}
		buffer_pointer++;
	}
	_delay_ms(300);
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
	memset(sms_buffer,0,strlen(sms_buffer));
}




void GSM_Response()
{
	unsigned int timeout=0;
	int CRLF_Found=0;
	char CRLF_buff[2];
	int Response_Length=0;
	while(1)
	{
		if(timeout>=60000)								/*if timeout occur then return */
		return;
		Response_Length = strlen(buff);
		if(Response_Length)
		{
			_delay_ms(2);
			timeout++;
			if(Response_Length==strlen(buff))
			{
				for(int i=0;i<Response_Length;i++)
				{
					memmove(CRLF_buff,CRLF_buff+1,1);
					CRLF_buff[1]=buff[i];
					if(strncmp(CRLF_buff,"\r\n",2))
					{
					    if(CRLF_Found++==2)				                                    /* search for \r\n in string */
					     {
						GSM_Response_Display();
						return;
					     }
					}

				}
				CRLF_Found = 0;

			}

		}
		_delay_ms(1);
		timeout++;
	}
	status_flag=0;
}

void GSM_Response_Display()
{
	buffer_pointer = 0;
	int lcd_pointer = 0;
	while(1)
	{
          /* search for \r\n in string */
	  if(buff[buffer_pointer]== '\r' || buff[buffer_pointer]== '\n')
		{
			buffer_pointer++;
		}
		else
			break;
	}


	LCD_vidExcuteCommand(0xc0);
	while(buff[buffer_pointer]!='\r')								   /* display response till "\r" */
	{
		LCD_vidDisplayData(buff[buffer_pointer]);
		buffer_pointer++;
		lcd_pointer++;
		if(lcd_pointer==15)	/* check for end of LCD line */
		  LCD_vidExcuteCommand(0x80);
	}
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
}





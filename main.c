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
void GSM_Calling(char *);
void GSM_HangCall();
void GSM_Response();
void GSM_Response_Display();
void GSM_Msg_Read(int);
bool GSM_Wait_for_Msg();
void GSM_Msg_Display();
void GSM_Msg_Delete(unsigned int);
void GSM_Send_Msg(char* , char*);
void GSM_Delete_All_Msg();

char buff[160];			/* buffer to store responses and messages */
char status_flag = 0;		/* for checking any new message */
volatile int buffer_pointer;
char Mobile_no[14];		/* store mobile no. of received message */
char message_received[60];	/* save received message */
int position = 0;		/* save location of current message */


int main(void)
{
	buffer_pointer = 0;
	int is_msg_arrived;
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


		/*check if any new message received */
		if(status_flag==1){
			is_msg_arrived = GSM_Wait_for_Msg(); /*check for message arrival*/
			if(is_msg_arrived== true)
			{
			   LCD_vidExcuteCommand(0b00000001);
			   LCD_vidGoToposition(1,0);
			   LCD_vidDisplaystring("new message");
			   _delay_ms(1000);
			   LCD_vidExcuteCommand(0b00000001);
			   GSM_Msg_Read(position); /* read arrived message */
			   _delay_ms(3000);

			/*check if received message is "call me" */
				if(strstr( message_received,"call me")){

				    GSM_Calling(Mobile_no); /* call sender of message */
			            LCD_vidExcuteCommand(0b00000001);
			            LCD_vidGoToposition(1,0);
			       	   LCD_vidDisplaystring("calling...");
				    _delay_ms(15000);
				    GSM_HangCall(); /* hang call */
		            LCD_vidGoToposition(1,0);
		       	   LCD_vidDisplaystring("Hang Call");
				    _delay_ms(1000);
				}


			LCD_vidExcuteCommand(0b00000001);
			GSM_Msg_Delete(position); /* to save SIM memory delete current message */
            LCD_vidGoToposition(1,0);
       	   LCD_vidDisplaystring("clear msg");
			GSM_Response();
			_delay_ms(1000);

			}


			is_msg_arrived=0;
			status_flag=0;
			LCD_vidExcuteCommand(0b00000001);
			}
        LCD_vidGoToposition(1,0);
   	   LCD_vidDisplaystring("waiting for msg");

		memset(Mobile_no, 0, 14);
		memset(message_received, 0, 60);


	}
	char number[];
	GSM_Send_Msg(number,number);

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

bool GSM_Wait_for_Msg()
{
	char msg_location[4];
	int i;
	_delay_ms(500);
	buffer_pointer=0;

	while(1)
	{
           /*eliminate "\r \n" which is start of string */

	   if(buff[buffer_pointer]=='\r' || buff[buffer_pointer]== '\n'){
			buffer_pointer++;
		}
		else
			break;
	}

	/* "CMTI:" to check if any new message received */

	if(strstr(buff,"CMTI:")){
		while(buff[buffer_pointer]!= ',')
		{
			buffer_pointer++;
		}
		buffer_pointer++;

		i=0;
		while(buff[buffer_pointer]!= '\r')
		{
			msg_location[i]=buff[buffer_pointer];				      /* copy location of received message where it is stored */
			buffer_pointer++;
			i++;
		}

		/* convert string of position to integer value */
		position = atoi(msg_location);

		memset(buff,0,strlen(buff));
		buffer_pointer=0;

		return true;
	}
	else
	{
		return false;
	}
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

void GSM_Calling(char *Mob_no)
{
   char call[20];
   sprintf(call,"ATD%s;\r",Mob_no);
   USART_SendString(call);	/* send command ATD<Mobile_No>; for calling*/
}

void GSM_HangCall()
{
   LCD_vidExcuteCommand(0b00000001);
   USART_SendString("ATH\r");	/*send command ATH\r to hang call*/
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

void GSM_Msg_Read(int position)
{
	char read_cmd[10];
	sprintf(read_cmd,"AT+CMGR=%d\r",position);
	USART_SendString(read_cmd);	/* read message at specified location/position */
	GSM_Msg_Display();	/* display message */
}

void GSM_Msg_Display()
{
	_delay_ms(500);
	if(!(strstr(buff,"+CMGR")))	/*check for +CMGR response */
	{
		 LCD_vidGoToposition(1,0);
	     LCD_vidDisplaystring("No message");
	}
	else
	{
		buffer_pointer = 0;

		while(1)
		{
                        /*wait till \r\n not over*/

			if(buff[buffer_pointer]=='\r' || buff[buffer_pointer]== 'n') 			                {
				buffer_pointer++;
			}
			else
			        break;
		}

		/* search for 1st ',' to get mobile no.*/
		while(buff[buffer_pointer]!=',')
		{
			buffer_pointer++;
		}
		buffer_pointer = buffer_pointer+2;

		/* extract mobile no. of message sender */
		for(int i=0;i<=12;i++)
		{
			Mobile_no[i] = buff[buffer_pointer];
			buffer_pointer++;
		}

		do
		{
			buffer_pointer++;
		}while(buff[buffer_pointer-1]!= '\n');

		LCD_vidExcuteCommand(0xC0);
		int i=0;

		/* display and save message */
		while(buff[buffer_pointer]!= '\r' && i<31)
		{
				LCD_vidDisplayData(buff[buffer_pointer]);
				message_received[i]=buff[buffer_pointer];

				buffer_pointer++;
				i++;
				if(i==16)
					LCD_vidExcuteCommand(0x80); /* display on 1st line */
		}

		buffer_pointer = 0;
		memset(buff,0,strlen(buff));
	}
	status_flag = 0;
}


/********************************************************************************
 Written by: Mehadi Seid, Swanand Kulkarni, Rajesh Kaushik
 AVR Studio Version 4.17, Build 666

 Starting Date: 20 September 2010
 End Date: 8 November 2010
*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, ERTS Lab.                       -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/


/*
*	Code For Scout Robot
*
*/

#include "firebird_winavr.h"

unsigned int position = 0; // Record the current postion of Robot using block count.
unsigned char command = 0;
unsigned int stopDistance = 200;
unsigned int maxWhite = 40;
unsigned int fastSpeed = 120;
unsigned int mediumSpeed = 100;
unsigned int slowSpeed = 80;
unsigned int RIGHT_WHITELINE_VALUE;
unsigned int MIDDLE_WHITELINE_VALUE;
unsigned int LEFT_WHITELINE_VALUE;
unsigned int FRONT_IR_VALUE;

unsigned int startBlack = 0;
unsigned int completeBlack = 0;

unsigned int ShaftCountLeft = 0;
unsigned int ShaftCountRight = 0;
unsigned int ShaftCountMax = 10;

unsigned int prevLeftSpeed = 0;
unsigned int prevRightSpeed = 0;
unsigned int prevState = 0; 	//1 - white, 0 - black

unsigned int sendOnce = 0;		//1 - send when stoping, 0 - already sent

ISR(INT4_vect)
{
	ShaftCountLeft++; //increment left shaft position count
	if(ShaftCountLeft > ShaftCountMax)
	{
		completeBlack = 1;
	}
}
ISR(INT5_vect)
{
	ShaftCountRight++; //increment right shaft position count
	if(ShaftCountRight > ShaftCountMax)
	{
		completeBlack = 1;
	}
}

//!ISR for receive complete interrupt
SIGNAL(SIG_USART0_RECV)
{
	command = UDR0; //making copy of data from UDR0 in 'data' variable
	//LCD_DISPLAY_INT_1(4);
	if(command == 0x38) //ASCII value of 8
		{
			BUZZER_ON();
			prevState = 1;
			sendOnce = 1;
			//UCSR0B = UCSR0B | 0xbf;
		} 

}

	
//!Moves the scout robot to scan the field.
void main(void)
{
	init_devices();	//defined in firebird_winavr.h.  It initialize all necessary ports for LCD, Buzzer, Motion control and IR sensor.
	position = 0;
	init_dist_measure();

	UCSR0B = UCSR0B | 0x10; //set bit4 = rx enables
	UCSR0B = UCSR0B | 0x08; //set bit3 = tx enable
	UCSR0B = UCSR0B | 0x80; 	//bit-7 enable rx-complet interrupt


	while(1)
	{
		RIGHT_WHITELINE_VALUE = LIGHT_RIGHT;	//LIGHT_RIGHT is a macro defines in firebird_winavr.h which returns the right whiteline sensor value.
		MIDDLE_WHITELINE_VALUE = LIGHT_MIDDLE;	// Similar
		LEFT_WHITELINE_VALUE = LIGHT_LEFT;	// Similar
		FRONT_IR_VALUE = FRONT_IR;		// Similar


	//	LCD_DISPLAY_INT_1(FRONT_IR_VALUE);	// Displays IR sensor value on first line of LCD

		LCD_DISPLAY_INT_1(position);

/*		if(FRONT_IR_VALUE < stopDistance)
		{
			MOTOR_LEFT_SPEED(0);
			MOTOR_RIGHT_SPEED(0);
			LCD_DISPLAY_INT_2(8);
			BUZZER_ON();
		}
		else
		{
			BUZZER_OFF();*/

			if(RIGHT_WHITELINE_VALUE < maxWhite && MIDDLE_WHITELINE_VALUE < maxWhite && LEFT_WHITELINE_VALUE < maxWhite && prevState == 1)
			{
				MOTOR_LEFT_SPEED(fastSpeed);
				MOTOR_RIGHT_SPEED(fastSpeed);
				MOVE_FWD();
				LCD_DISPLAY_INT_2(1);
				
				startBlack = 1;
				prevState = 1;
				prevLeftSpeed = fastSpeed;
				prevRightSpeed = fastSpeed;
			}
			else
			{
				if(RIGHT_WHITELINE_VALUE < maxWhite && MIDDLE_WHITELINE_VALUE < maxWhite  && prevState == 1)
				{
					MOTOR_LEFT_SPEED(fastSpeed);
					MOTOR_RIGHT_SPEED(mediumSpeed);
					MOVE_FWD();
					LCD_DISPLAY_INT_2(2);

					startBlack = 1;
					prevState = 1;
					prevLeftSpeed = fastSpeed;
					prevRightSpeed = mediumSpeed;
				}
				else
				{
					if(MIDDLE_WHITELINE_VALUE < maxWhite && LEFT_WHITELINE_VALUE < maxWhite  && prevState == 1)
					{
						MOTOR_LEFT_SPEED(mediumSpeed);
						MOTOR_RIGHT_SPEED(fastSpeed);
						MOVE_FWD();
						LCD_DISPLAY_INT_2(3);

						startBlack = 1;
						prevState = 1;
						prevLeftSpeed = mediumSpeed;
						prevRightSpeed = fastSpeed;
					}
					else
					{
						if(RIGHT_WHITELINE_VALUE < maxWhite  && prevState == 1)
						{
							MOTOR_LEFT_SPEED(fastSpeed);
							MOTOR_RIGHT_SPEED(slowSpeed);
							MOVE_FWD();
							LCD_DISPLAY_INT_2(4);

							startBlack = 1;
							prevState = 1;
							prevL//!ISR for receive complete interrupteftSpeed = fastSpeed;
							prevRightSpeed = slowSpeed;
						}
						else
						{
							if(LEFT_WHITELINE_VALUE < maxWhite  && prevState == 1)
							{
								MOTOR_LEFT_SPEED(slowSpeed);
								MOTOR_RIGHT_SPEED(fastSpeed);
								MOVE_FWD();
								LCD_DISPLAY_INT_2(5);

								startBlack = 1;
								prevState = 1;
								prevLeftSpeed = slowSpeed;
								prevRightSpeed = fastSpeed;
							}
							else
							{
								if(MIDDLE_WHITELINE_VALUE < maxWhite  && prevState == 1)
								{
									
									MOTOR_LEFT_SPEED(fastSpeed);
									MOTOR_RIGHT_SPEED(fastSpeed);
								//!ISR for receive complete interrupt	MOVE_FWD();
									LCD_DISPLAY_INT_2(6);

									startBlack = 1;
									prevState = 1;
									prevLeftSpeed = fastSpeed;
									prevRightSpeed = fastSpeed;
								}
								else		// Here we need to increment the position counter, capture an image, transmit the image to centralized server along with position counter and move the robot to some precise distance so that it will cross the Black mark and proceed further.
								{
									
									if(startBlack ==1)
									{
										position++;
										startBlack = 0;
										completeBlack = 0;
										
										//start shaft counter by enabling inturupt 4 and 5
										ShaftCountLeft = 0;
										ShaftCountRight = 0;

										prevState = 0;	//set prev state to black
									}
									else if(completeBlack == 1)		//last stop
									{
										MOTOR_LEFT_SPEED(0);
										MOTOR_RIGHT_SPEED(0);
										 
										position = 99;//only for test
									}
									//if prevstate is black and u detect white line stop
									else if(prevState == 0 && (RIGHT_WHITELINE_VALUE < maxWhite || MIDDLE_WHITELINE_VALUE < maxWhite || LEFT_WHITELINE_VALUE < maxWhite))
									{
										MOTOR_LEFT_SPEED(0);
										MOTOR_RIGHT_SPEED(0);

										
										//stop and wait for 'continue scouting command from server (for testing use button)
										if(sendOnce == 1)
										{
											UDR0 = (0x0f & position);	//sending position count
										//	UCSR0B = UCSR0B | 0x08; //set bit3 = tx enable
											//UDR0 = 0x0D;			// 'CR' signifies the terminator in matlab
										//	UCSR0B = UCSR0B | 0x08; //set bit3 = tx enable
											sendOnce = 0;
										}
										/*if(command == 'z')//	if Char c recieved
										{
											prevState = 1;
											sendOnce = 1;
											command = 0;
											LCD_DISPLAY_INT_2(72);
											
										}*/
									//	LCD_DISPLAY_INT_1(command);
										
										//BUZZER_ON();
										
										//disable inturupt 4  and 5
									}
									LCD_DISPLAY_INT_2(7);
								}
							}
						}
					}
				}
			}
		}
	//}
}

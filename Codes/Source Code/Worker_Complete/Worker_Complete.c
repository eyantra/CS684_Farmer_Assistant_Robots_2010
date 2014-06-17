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
*	Code For Worker Robot
*
*/

#include "firebird_winavr.h"

unsigned int position = 0; // Record the current postion of Robot using block count.
unsigned int locations[10];
unsigned int location_cnt = 0;
unsigned int locations_scanned = 0;
unsigned char data = 0;
unsigned int stopDistance = 200;
unsigned int maxWhite = 40;
unsigned int fastSpeed = 120;
unsigned int mediumSpeed = 100;
unsigned int slowSpeed = 80;
unsigned int RIGHT_WHITELINE_VALUE;
unsigned int MIDDLE_WHITELINE_VALUE;
unsigned int LEFT_WHITELINE_VALUE;
unsigned int FRONT_IR_VALUE;

unsigned char command;
unsigned int startBlack = 0;



unsigned int prevLeftSpeed = 0;
unsigned int prevRightSpeed = 0;
unsigned int prevState = -1; 	//1 - white, 0 - black

unsigned int sendOnce = 0;		//1 - send when stoping, 0 - already sent
unsigned int rotate = 1;

//
unsigned int recievedHeight = 0;
unsigned int armHeight = 0;

int dist;


//!ISR for receive complete interrupt
SIGNAL(SIG_USART0_RECV) 
{
	data = UDR0; //making copy of data from UDR0 in 'data' variable
	//LCD_DISPLAY_INT_1(4);
	if(data == (int)'f')
	{
		BUZZER_ON();
		prevState = 1;
		sendOnce = 1;
		//UCSR0B = UCSR0B | 0xbf;
	}
	else if(data == (int)'l')
	{
		command = 'l';	//tells the worker robot that the next data to be sent a location data
	}
	else if(data == (int)'h')
	{
		command = 'h';	//tells the worker robot that the next data to be sent a height data
	}
	else if(data == (int)'d')
	{
		command = 'd';
	}


	switch(data)
	{
		case 0x31:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 1;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 10;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		case 0x32:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 2;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 20;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		case 0x33:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 3;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 30;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		case 0x34:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 4;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 40;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		case 0x35:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 5;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 50;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		
		case 0x36:
		{
			BUZZER_ON();
			if(command == 'l')
			{
				locations[location_cnt] = 6;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 60;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		case 0x37:
		{
			if(command == 'l')
			{
				locations[location_cnt] = 7;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 70;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
		
		case 0x38:
		{
			if(command == 'l')
			{
				locations[location_cnt] = 8;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 80;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}

		
		case 0x39:
		{
			if(command == 'l')
			{
				locations[location_cnt] = 9;
				location_cnt++;
				break;
			}
			else if(command == 'h')
			{
				armHeight = 90;
				recievedHeight = 1;
				break;
			}
			else if(command == 'd')
			{
				break;
			} 
		}
	}

}

//!Calculate the exact distance of an object from the front of the robot in mm
int frontDist()
{
	int objDist = front_dist_mm();

	float readDist = (float) objDist;
	float actualDist =  readDist/2.75;	//calbiration value

	return (int) actualDist;
}

//!Moves the worker Robot towards the object to be collected depending on how far the object is.
void moveToObj()
{
	int constArmDist = 0; 
	int objDist = frontDist();

	dist = 0;
	while(objDist > 40)
	{
		forward_mm(10);
		objDist = frontDist();
		dist= dist +10;
	}
	
	forward_mm(10);
	dist = dist + 10;
}

//!After grabbing the object move the robot backward by the distance specified 
void backOffObj(int mm)
{
	back_mm(mm);
}


//!This function is used to rotate the robot back to the white line
void turnToLine()
{
	unsigned int whiteSensor = LIGHT_RIGHT;
	
	while(whiteSensor > maxWhite)
	{
		LEFT_TURN_BY(10);
		_delay_ms(100);
		whiteSensor = LIGHT_RIGHT;
	}
}



//!Once the worker robot reaches the specifed location where the object was detected by the scout robot then this function is colled to implement the logic which enables the robot to collect the object
void CollectFruit()
{

	int forwardMoved;

	int objDist;
	
	int temp =  120;
	
	BUZZER_ON();
	STOP();
	//set new speed

	MOTOR_LEFT_SPEED(200);
	MOTOR_RIGHT_SPEED(200);

	//turn right
	RIGHT_TURN_BY(83);
	_delay_ms(2000);

	
	
	CameraMiddle();
	_delay_ms(2000);

	//send IR reading to server
	UDR0 = (0x0f & 1);	
	
	//server does DIP


	//wait for height info from server
	while(1)
	{
		if(recievedHeight == 1)
		{
			recievedHeight = 0;
			break;
		}
	}

	//
	_delay_ms(2000);
	Release();

		
	_delay_ms(2000);
	Lift(armHeight);
	
	objDist = frontDist();

	moveToObj();

	LCD_DISPLAY_INT_2(objDist);
	
	_delay_ms(2000);
	Grab();

	_delay_ms(2000);
	Lift(90);

	
	back_mm(objDist);
//	backOffObj(forwardMoved);

	_delay_ms(2000);
	Release();
	
	//Tutn to original place
	_delay_ms(5000);
	//LEFT_TURN_BY(83);
	turnToLine();	

	_delay_ms(2000);
	Lower();

	
	 _delay_ms(2000);
	CameraRight();
	


	 _delay_ms(2000);
	 servo_1_free();
	 servo_2_free();
	 servo_3_free();

	 
	 prevState = 1;
	 sendOnce = 1;
}

//! Execution begin here
void main(void)
{
	init_devices();	//defined in firebird_winavr.h.  It initialize all necessary ports for LCD, Buzzer, Motion control and IR sensor.
	position = 0;
	init_dist_measure();


	UCSR0B = UCSR0B | 0x10; //set bit4 = rx enables
	UCSR0B = UCSR0B | 0x08; //set bit3 = tx enable
	UCSR0B = UCSR0B | 0x80; 	//bit-7 enable rx-complet interrupt
	
	CameraRight();
	_delay_ms(2000);
	
	Lower();
	_delay_ms(2000);


	while(1)
	{
		RIGHT_WHITELINE_VALUE = LIGHT_RIGHT;	//LIGHT_RIGHT is a macro defines in firebird_winavr.h which returns the right whiteline sensor value.
		MIDDLE_WHITELINE_VALUE = LIGHT_MIDDLE;	// Similar
		LEFT_WHITELINE_VALUE = LIGHT_LEFT;	// Similar
			
		FRONT_IR_VALUE = frontDist();
		

		LCD_DISPLAY_INT_2(FRONT_IR_VALUE);	// Displays IR sensor value on first line of LCD
		LCD_DISPLAY_INT_1(locations[location_cnt - 1]);


		if(RIGHT_WHITELINE_VALUE < maxWhite && MIDDLE_WHITELINE_VALUE < maxWhite && LEFT_WHITELINE_VALUE < maxWhite && prevState == 1 && position < locations[locations_scanned])
		{
			MOTOR_LEFT_SPEED(fastSpeed);
			MOTOR_RIGHT_SPEED(fastSpeed);
			MOVE_FWD();
			
			startBlack = 1;
			prevState = 1;
			prevLeftSpeed = fastSpeed;
			prevRightSpeed = fastSpeed;
		}
		else
		{
			if(RIGHT_WHITELINE_VALUE < maxWhite && MIDDLE_WHITELINE_VALUE < maxWhite  && prevState == 1 && position < locations[locations_scanned])
			{
				MOTOR_LEFT_SPEED(fastSpeed);
				MOTOR_RIGHT_SPEED(mediumSpeed);
				MOVE_FWD();

				startBlack = 1;
				prevState = 1;
				prevLeftSpeed = fastSpeed;
				prevRightSpeed = mediumSpeed;
			}
			else
			{
				if(MIDDLE_WHITELINE_VALUE < maxWhite && LEFT_WHITELINE_VALUE < maxWhite  && prevState == 1 && position < locations[locations_scanned])
				{
					MOTOR_LEFT_SPEED(mediumSpeed);
					MOTOR_RIGHT_SPEED(fastSpeed);
					MOVE_FWD();

					startBlack = 1;
					prevState = 1;
					prevLeftSpeed = mediumSpeed;
					prevRightSpeed = fastSpeed;
				}
				else
				{
					if(RIGHT_WHITELINE_VALUE < maxWhite  && prevState == 1 && position < locations[locations_scanned])
					{
						MOTOR_LEFT_SPEED(fastSpeed);
						MOTOR_RIGHT_SPEED(slowSpeed);
						MOVE_FWD();

						startBlack = 1;
						prevState = 1;
						prevLeftSpeed = fastSpeed;
						prevRightSpeed = slowSpeed;
					}
					else
					{
						if(LEFT_WHITELINE_VALUE < maxWhite  && prevState == 1 && position < locations[locations_scanned])/////////new
						{
							MOTOR_LEFT_SPEED(slowSpeed);
							MOTOR_RIGHT_SPEED(fastSpeed);
							MOVE_FWD();

							startBlack = 1;
							prevState = 1;
							prevLeftSpeed = slowSpeed;
							prevRightSpeed = fastSpeed;
						}
						else
						{
							if(MIDDLE_WHITELINE_VALUE < maxWhite  && prevState == 1 && position < locations[locations_scanned])
							{
								
								MOTOR_LEFT_SPEED(fastSpeed);
								MOTOR_RIGHT_SPEED(fastSpeed);
								MOVE_FWD();

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
									if(position == locations[locations_scanned])
									{
										locations_scanned++;
										prevState = 0;	//set prev state to black
									}	
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
									BUZZER_ON();
									CollectFruit();
								}
								
							}
						}
					}
				}
			}
		}
	}
}

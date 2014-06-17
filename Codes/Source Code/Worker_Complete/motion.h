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

/*//////////////////////////////////////////////////////////////////////// */

// motion functions


unsigned int ShaftCountLeft = 0;
unsigned int ShaftCountRight = 0;
unsigned int ShaftCountMax = 10;
unsigned int Degrees; //to accept angle in degrees for turning

unsigned int completeBlack = 0;



//SR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++; //increment left shaft position count
	if(ShaftCountLeft > ShaftCountMax)
	{
		completeBlack = 1;
	}
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++; //increment right shaft position count
	if(ShaftCountRight > ShaftCountMax)
	{
		completeBlack = 1;
	}
}


void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x10; //Enable internal pullup for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x20; //Enable internal pullup for PORTE 4 pin
}

void init_ports()
{
 motion_pin_config(); //robot motion pins config
 left_encoder_pin_config(); //left encoder pin config
 right_encoder_pin_config(); //right encoder pin config	
}


void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
 EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
 sei();   // Enables the global interrupt 
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
 EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
 sei();   // Enables the global interrupt 
}


//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 			// removing upper nibbel as it is not needed
 PortARestore = PORTA; 			// reading the PORTA's original status
 PortARestore &= 0xF0; 			// setting lower direction nibbel to 0
 PortARestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTA status
 PORTA = PortARestore; 			// setting the command to the port

}


/*#define MOVE_FWD forward()
#define MOVE_REV back()
#define MOVE_LEFT left()
#define MOVE_RIGHT right()
#define MOVE_SOFT_RIGHT soft_right()
#define MOVE_SOFT_LEFT soft_left()
#define MOVE_INPLACE_RIGHT inplace_right()
#define MOVE_INPLACE_LEFT inplace_left()*/


void MOVE_FWD(void) //both wheels forward
{
  motion_set(0x06);
}

void MOVE_REV(void) //both wheels backward
{
  motion_set(0x09);
}

void MOVE_LEFT(void) //Left wheel backward, Right wheel forward
{
  motion_set(0x05);
}

void MOVE_RIGHT(void) //Left wheel forward, Right wheel backward
{
  motion_set(0x0A);
}

void MOVE_SOFT_LEFT (void) //Left wheel stationary, Right wheel forward
{
 motion_set(0x04);
}

void MOVE_SOFT_RIGHT (void) //Left wheel forward, Right wheel is stationary
{
 motion_set(0x02);
}

void MOVE_INPLACE_LEFT(void) //Left wheel backward, right wheel stationary
{
 motion_set(0x01);
}

void MOVE_INPLACE_RIGHT (void) //Left wheel stationary, Right wheel backward
{
 motion_set(0x08);
}

void STOP (void)
{
  motion_set(0x00);
}

void angle_rotate(unsigned int Degrees)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
 ShaftCountRight = 0; 
 ShaftCountLeft = 0; 

 while (1)
 {
  if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
  break;
 }
 STOP(); //Stop action
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) DistanceInMM / 5.338; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
  
 ShaftCountRight = 0; 
 ShaftCountLeft = 0;
 while(1)
 {
  if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
  {
  	break;
  }
 } 
 STOP(); //Stop action
}


void forward_mm(unsigned int DistanceInMM)
{
 MOVE_FWD();
 linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
 MOVE_REV();
 linear_distance_mm(DistanceInMM);
}



void RIGHT_TURN_BY(int Degree)
{
	MOVE_RIGHT();
	angle_rotate(Degree);
	STOP();
}

void LEFT_TURN_BY(int Degree)
{
	MOVE_LEFT();
	angle_rotate(Degree);
	STOP();
}




// Timer 5 initialised in PWM mode for velocity control
// Prescale:64
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:674.988Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionalit to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

// Function for robot velocity control
// Make sure that you pass only unsigned char value.


void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}


void MOTOR_LEFT_SPEED(int val)
{
    if ( val > 254 )   //limiting the max velocity
	   val = 255;	
    OCR5AL = (unsigned char) val;
}

void MOTOR_RIGHT_SPEED(int val)
{
  if ( val > 254 )     //limiting the max velocity
	  val = 255;
  OCR5BL = (unsigned char) val;
}



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

//camera is servo 3

unsigned const int cameraMiddle = 82;		//camera face front at 82 degree
unsigned const int cameraRight = 0;			//camera face right at 0 degree
unsigned const int cameraLeft = 168;		//camera face right at 0 degree

//lifter is servo 1
unsigned const int maxDegree = 0;			//lifter max height in degree
unsigned const int minDegree = 150;			//lifter min height in degree
unsigned int currentDegree = 150;


//grabber is servo 2

unsigned const int grabberOpen = 180;
unsigned const int grabberClose = 122;


//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}

//Initialize the ports
void arm_port_init(void)
{
 servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
 servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation
 servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation
}

//TIMER1 initialization in 10 bit fast PWM mode
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 42.187Hz
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;        //Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;        //Output compare eegister high value for servo 1
 OCR1AL = 0xFF;        //Output Compare Register low Value For servo 1
 OCR1BH = 0x03;        //Output compare eegister high value for servo 2
 OCR1BL = 0xFF;        //Output Compare Register low Value For servo 2
 OCR1CH = 0x03;        ///Output compare eegister high value for servo 3
 OCR1CL = 0xFF;        //Output Compare Register low Value For servo 3
 ICR1H  = 0x03;
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
                                         For Overriding normal port functionalit to OCRnA outputs.
                                  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


//Function to initialize all the peripherals
void arm_init_devices(void)
{
 cli(); //disable all interrupts
 arm_port_init();
 timer1_init();
 sei(); //re-enable interrupts
}


//Function to rotate Servo 1 by a specified angle in the multiples of 2.25 degrees
void servo_1(unsigned char degrees)
{
 float PositionPanServo = 0;
 PositionPanServo = ((float)degrees / 2.25) + 21.0;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 2.25 degrees
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees / 2.25) + 21.0;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 2.25 degrees
void servo_3(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees / 2.25) + 21.0;
 OCR1CH = 0x00;
 OCR1CL = (unsigned char) PositionTiltServo;
}

//servo_free functions unlocks the servo motors from the any angle
//and make them free by giving 100% duty cycle at the PWM. This function can be used to
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03;
 OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
}

//!Function to rotate Camera to face front
void CameraMiddle()
{
	servo_3(cameraMiddle);
	_delay_ms(50);
}

//!Function to rotate Camera to face Right
void CameraRight()
{
	servo_3(cameraRight);
	_delay_ms(50);

}

//!Function to rotate Camera to face left
void CameraLeft()
{
	servo_3(cameraLeft);
	_delay_ms(50);

}

//!Function to lift the arm gripper to the specified height. 
//!Height - should be in mm and between 0 - 90mm.
void Lift(float Height)
{
	//CONVERT Degree TO DEGREE
	float tempD = minDegree - maxDegree;


	float temp2 = (Height * 10 * tempD )/ 1010;			//this number needs to be optimized
	
	int Degree = minDegree - (int) temp2;
	
	int i = 0;
	for (i = currentDegree; i > Degree; i--)
	{
		 servo_1(i);
		 _delay_ms(20);
	}
	currentDegree = Degree;
}


//!Function to lower the arm gripper to position 0
void Lower()
{
	int i = 0;

	 for (i = currentDegree; i <minDegree; i++)	//change max to current degree
	 {
	  servo_1(i);
	  _delay_ms(20);
	 }
}

//!Function to grab object with the grabber. You can modify the values of grabberClose to specifie the size of the object.
void Grab()
{
	int i=0;
	for (i = grabberOpen; i > grabberClose; i--)
	 {
		  servo_2(i);
		  _delay_ms(20);
	 }	
}

//!Function to release object from the grabber.
void Release()
{
	int i=0;
	for (i = grabberClose; i < grabberOpen; i++)
	{
		servo_2(i);
		_delay_ms(20);
	}
}

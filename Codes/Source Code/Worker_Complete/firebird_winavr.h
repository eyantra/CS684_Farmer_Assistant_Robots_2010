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


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "motion.h"
#include "whiteline.h"
#include "ir.h"
#include "lcd.h"
#include "buzzer.h"
#include "Arm_Control.h"


//initialise the ports used for distance measurment ISR
init_dist_measure()
{
	DDRE = 0x00;
	PORTE = PORTE | 0x30;
}

//init uart 0 for zegbee
void uart0_init(void)
{
UCSR0B = 0x00; //disable while setting baud rate
UCSR0A = 0x00;
UCSR0C = 0x06;
UBRR0L = 0x47; //set baud rate lo set to 9600
UBRR0H = 0x00; //set baud rate hi
UCSR0B = 0x90;	//98
}

//Function to initialize ports
void init_devices()
{
 cli(); //Clears the global interrupts
 motion_pin_config();

 port_init();
 init_adc();
 timer5_init();
 init_lcd();
 lcd_init();
 lcd_set_4bit();

 init_buzzer();
 uart0_init();

 left_encoder_pin_config(); //left encoder pin config
 right_encoder_pin_config(); //right encoder pin config
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();

 arm_init_devices();	

 sei(); //Enables the global interrupts
}








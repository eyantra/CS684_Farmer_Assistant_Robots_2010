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


/*  ////////////////////////////////////////////    */

// BUZZER Functions

/*****Function to Initialize PORTS*****/
void init_buzzer()
{
	DDRC = DDRC | 0x08;				//Setting PORTC pin3 as outpt
	PORTC = 0x00;				//Initialize Values of PORTC to 0.
}

/***** Buzzer Delay Function*****/
void buzzer_delay()
{
	unsigned int i;
	for(i=0; i<10;i++)
	{
		_delay_us(100);
	}
}

void BUZZER_ON()
{

int i=0; 
   unsigned char port_C_copy;
   port_C_copy = PORTC;
   port_C_copy = port_C_copy | 0x08;
   PORTC = port_C_copy;	
   //for(i=0;i<10;i++)
	//tick_timer();
	buzzer_delay();
   port_C_copy = PORTC;
   port_C_copy = port_C_copy & 0xF7;
   PORTC = port_C_copy;	



//PORTC = 0x08;			//Set pin3 to turn the "buzzer ON"
//buzzer_delay();			//delay


}


void BUZZER_OFF()
{

PORTC = 0x00;			//Clear pin3 to turn the "buzzer OFF"
buzzer_delay();			//delay

}






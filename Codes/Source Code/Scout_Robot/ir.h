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


/***********************************************************************************
 This Function calculates the actual distance in millimeters(mm) from the input
 analog value of Sharp Sensor. 
 ***********************************************************************************/

/***** Function to Initialize PORTS *****/
void init_ir()
{
	DDRF = 0x00; 
	PORTF = 0x00;
	DDRA = 0x0F;
	PORTA = 0x00;
	DDRL = 0x18;
	PORTL = 0x18;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRJ = 0x00;
	PORTJ = 0xFF;
}


unsigned int Sharp_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}


int front_dist_mm(void) 
{
  //float front_distance;
  int front_distance_mm;
  unsigned char analog_val;
  analog_val = ADC_Conversion(11);
 

 front_distance_mm=Sharp_estimation(analog_val);
  return front_distance_mm; 	

  //return 100;
}



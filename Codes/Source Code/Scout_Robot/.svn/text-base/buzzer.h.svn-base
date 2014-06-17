

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






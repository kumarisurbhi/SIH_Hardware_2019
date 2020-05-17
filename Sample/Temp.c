/*
* Author: ERTS Lab, IIT Bombay
* File name: buzzer_for_1_second.c
* Description: This experiment demonstrates simple Input and Output operation. 
* Functions: Buzzer_Delay(unsigned int), Init_Buzzer_Pin(void), Init_Peripherals(void), Init_Ports(void)
* Concepts covered: Input and Output operations
  
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 done for proper operation of the code
  Microcontroller: LPC2148
 Frequency: 12 MHz								   
 2. Buzzer is connected to the 25th pin of Port0 i.e P0.25, to turn it 
    on we have to set it as output and give logic 1 to it.
 3. Coding Standards followed(For further reference on Coding Standards go to portal link).
*********************************************************************************/
#include "../../utilities/ARM_Header_Files/LPC214x.h"


/***************************************************************************** 
* Function name - Buzer_Delay
* Input - time in milli seconds 
* Outut - gives delay for the spcified time before executing next instruction
* Logic - delay can be given in the program by introducing loops
******************************************************************************/
void Buzzer_Delay(unsigned int ms)
{
   unsigned int i,j;
   for(j=0;j<(ms/10);j++)
   {
   for(i=0; i<60000; i++);
   }
}

void BUZZER_ON()
{
   IO0SET=(1<<25);
}

void BUZZER_OFF()
{
   IO0CLR=(1<<25);
}
/************************************************************
Function : Init_Buzzer_Pin
Return type : None
Parameters : None
Description : Initialises Buzzer pin
************************************************************/
void Init_Buzzer_Pin(void)
{
   PINSEL1&=0xFFF3FFFF;
   PINSEL1|=0x00000000; //Set P0.25 as GPIO
   IO0DIR&=0xFDFFFFFF;
   IO0DIR|= (1<<25); //Set P0.25 as Output
   BUZZER_OFF(); //Initially turn OFF buzzer
}

/*********************************************************************
* Function Name	: main
* Input			: None
* Output		: int to inform the caller that the program exited 
                  correctly or incorrectly (C code standard)
* Logic			: blows and stops the buzzer for specified time delays 
* Example Call	: Called automatically by the Operating System
*
**********************************************************************/
#ifndef TEST
int main(void)
{
   Init_Buzzer_Pin(); // Init Port pins and Peripherals
   while(1)
   {
      BUZZER_ON(); //Turn ON buzzer
      Buzzer_Delay(1000); //Wait
      BUZZER_OFF(); //Turn OFF Buzzer
      Buzzer_Delay(1000); //Wait
   } 
} //main ends here

#endif
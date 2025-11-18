/*****************************************************************
file: embedded_skeleton.c

Sample embedded program to demonstrate communication with PC. 
Reads from the USB COMPORT using C232HM

Author: 
Ivan Ling, 2021

Licence: GPL version 2

Based on the code made available 
by Steve Gunn and Klaus-Peter Zauner (2014) 

Use turboAVR for a more streamlined uploading experience.
https://sotonac.sharepoint.com/teams/UoSM-ELEC1201/SitePages/TurboAVR-Version-3-released.aspx

*****************************************************************/


#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h" //<-- Need this for UART
#define BUFFSIZE 200

int main() {
	
	//This should be self-explanatory
	char cmd[BUFFSIZE];	
	int prm;
	int res;
	init_debug_uart0();
	printf("START");
	while(1)
	{
		//Reads from your hosted program 
		res = scanf("%s %u", cmd, &prm);
		//Hosted program always sends a character and an integer
		//printf("%s", cmd); //<< To debug and view raw command send from host
		
		
		if(res !=2 )
		{
			scanf("%*s");  /* Clear input buffer */
		}
		else
		{
			if(!strcmp( "R", cmd))
			{
				printf("I am reading");
			}
			else if(!strcmp( "C", cmd))
			{
				int i = 0;
				for(i = 0; i < prm; i++)
				{
					printf("\nMultiple reads: %i\n", i);
				}
			}
			else if(!strcmp( "B", cmd))
			{
				printf("Bye bye! Just Kidding! Still here!");
			}
			else
			{
				printf("NO MESSAGE!");
			}
		
		}
	}
    
}




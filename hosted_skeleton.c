/*****************************************************************

file: hosted_skeleton.c

Sample host program to demonstrate communication with IlMatto. 
Reads from the Il-Matto's printf() on UART

Author: 
Ivan Ling, 2021

Licence: GPL version 2

Based on the code made available 
by Teunis van Beelen under GPL2 at:
   http://www.teuniz.net/RS-232/
and
   http://www.teuniz.net/Timer_code/

by Klaus-Peter Zauner (2014) 



To compile on Linux:
  gcc hosted_skeleton.c timer.c rs232_d1.c -Wall -s -o2 -o host 

To compile on Windows:
  gcc hosted_skeleton.c timer.c rs232_d1.c -Wall -s -o2 -o host.exe
  
Alternatively, just run MakeHost.bat
Makehost is a script that will automatically build host.exe

*****************************************************************/
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "rs232_d1.h"
#include "timer.h"


/******DEFINITIONS******/
#define TIMER_MS 300 //<<<<<<----- For timer in UI if you choose to use it.
#define KEY_IN_BUFLEN   256 //<<<<<<< -----Buffer size for user input in the UI
#define COMPORT 4 //<<<<<<<------CHANGE THIS TO YOUR COMPORT
#define SER_IN_BUFLEN   4096  //<<<<-----Buffer size for UART incoming string from Il Matto
#define CMD_BUFFSIZE 50 //<<<<<<<-----Buffer size for outgoing command string to be send to Il matto


/******Variables and function prototype******/
void print_serial(void);
char *get_line (char *s, size_t n, FILE *f);
void send_command(const char *cmd, int val);
char *p;
int comport_num= COMPORT;
unsigned char serbuf[SER_IN_BUFLEN];



int main()
{
  int bdrate=9600;       /* 9600 baud */
  char line[KEY_IN_BUFLEN];


  if(OpenComport(comport_num, bdrate))//Check if comport is available
  {
    printf("Can not open comport\n");
    fflush (stdout);
    return(1); //Exit with error if can't access COMPORT
  }

  //Your setup codes here
  /****   SAMPLE CODES *****/
  
  printf("\nEnter 'start' to read from serial port.\n");
  
  
  /****   SAMPLE CODES END *****/
  
  
  
  
  
  for(;;){
//Your loop codes here
/****   SAMPLE CODES *****/
  
      if (!get_line(line, KEY_IN_BUFLEN, stdin)) continue; //Stores user input and puts it it "line"
	  //!get_line() will be true if get_line is NULL. Hence the loop will terminate due to CONTINUE.
      
      printf( "You typed: \"%s\" --> ", line ); //<-- "line" is what you typed into the CLI
	  if(!strcmp( "start", line)) //strcmp returns 0 if strings are iddentical
	  {
		//Here we do something if user input is "start"
		printf( "Il Matto Started! Next Command: ");
	  }
	  else if(!strcmp( "read", line))
	  {
		//Here we do something if user input is "read"
		printf( "Read --> ");
		send_command( "R", 1); 
	  //Sends a command character "R" followed by a value "1" to Il Matto
      //It is up to you if you wish to follow this format. On the Il Matto side, you will need to edit the logic.
      //In my sample code, I've coded R to mean Read, and I ignored the number, so '1' is just an arbitrary value. So Il Matto will read one value and send it back.
      
		start_timer(TIMER_MS, &print_serial);  //Waits 300ms (to avoid errors) then print message from Il Matto
	  }
	  else if(!strcmp( "multi", line))
	  {
		printf( "Read --> ");
		send_command( "C", 10); 
		start_timer(TIMER_MS, &print_serial);  //Waits 300ms (to avoid errors) then print message from Il Matto
	  }
	  else if(!strcmp( "bye", line))
	  {
		printf( "Shut down --> ");
		send_command( "B", 10); 
		start_timer(TIMER_MS, &print_serial);  //Waits 300ms (to avoid errors) then print message from Il Matto
	  }
	  else if(!strcmp( "exit", line))
	  {
		printf( "Shut down... for real this time! ");
		return 0;
	  }
	  
/****   SAMPLE CODES END *****/

/***ADDITIONAL EXPLANATION***/
/** 
start_timer(TIMER_MS, &print_serial); 
	-Starts timer and do print_serial() everytime timer triggers
	-Use this function if you want to activate something like an auto read
	-Timer period can be changed by changing #define TIMER_MS at the top
stop_timer();
	-Use this function to stop the timer after you've started it. 
send_command( "some message", 0);
	-Sends "some message" to your Il matto. Usually this is not needed.
	-IF YOU CHOOSE TO DO THIS, YOU WILL NEED TO THINK ABOUT WHAT TO DO WHEN YOU RECEIVE THE MESSAGE ON IL MATTO.
	-But could be useful if you wish to implement some special features in your design.
	-E.g. Password
**/
      
      
  }
  
  
  return(0);
}


void send_command(const char *cmd, int val) 
{
    char cmdstring[CMD_BUFFSIZE];
    snprintf(cmdstring,CMD_BUFFSIZE, "%s %u\r\n", cmd, val);
    cprintf(comport_num, cmdstring );	    

}


void print_serial(void)
{
    int i, n = 0;
	
	n = PollComport(comport_num, serbuf, SER_IN_BUFLEN-1);
    if(n > 0)
    {
	  serbuf[n] = '\0';   /* always put a "null" at the end of a string! */
    
      for(i=0; i < n; i++)
      {
        if(serbuf[i] < 32 && serbuf[i] != '\n')  /* replace control-codes by dots */
        {
          serbuf[i] = '.';
        }
      }
    
      /* printf("received %i bytes: %s\n", n, (char *)serbuf); */
      printf("%s\n", (char *)serbuf);
      fflush (stdout);
	  stop_timer(); //Message received, stop timer here. 
	  printf( "Done! Enter next command: ");
    }

    /* printf("received %i bytes\n", n); */
    
}



/* Read a line from stdin 

   Thomas Wolf at
   http://home.datacomm.ch/t_wolf/tw/c/getting_input.html

 */
char *get_line (char *s, size_t n, FILE *f)
{
  char *p = fgets (s, n, f);

  if (p != NULL) {
    size_t last = strlen (s) - 1;

    if (s[last] == '\n') s[last] = '\0'; 
  }
  return p;
}





/*
volatile char *e_vram = (char*)0x900000;
volatile int  *e_time = (int *)0x80010c;
volatile char *e_gp1  = (char*)0x8001f0;
volatile char *e_gp2  = (char*)0x8001f1;
volatile char *e_sw1  = (char*)0x8001fc;
volatile char *e_sw2  = (char*)0x8001fd;
volatile char *e_sw3  = (char*)0x8001fe;
volatile char *e_gin  = (char*)0x8001ff;
*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main( void ) {
	int x = 57;
	int y = 55;
	int cury = y;
	int tmp_t;	
	int e_time = 0;
	int dt;
	int v;
	int is_jumping = 0;
 	while(1){

		if ( !is_jumping ) {
			cury = y;
			tmp_t = e_time;
		}
			
		is_jumping = 1;

		if ( is_jumping ){
			dt = e_time - tmp_t;
			v = (20000 - 49 * dt)/1000;
			y = y - (v * dt)/1000 ;	
			//y = (y > cury) ? cury : y;		
			
			printf("e_time: %d,dt: %d ,v: %d, y: %d\n",e_time, dt, v, y);
			
			if ( y > cury ) {
				return 0;
			}
		}
		
       //sleep(10); // sleep 10 msec
       	
       e_time += 10;	
       
       /*
       if(!is_jumping){
       	return;
       	}
       	*/
    }
    
}

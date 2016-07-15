/***** Sample Program for MieruEMB System v1.1                    *****/
/**********************************************************************/
volatile char *e_vram = (char*)0x900000;
volatile int  *e_time = (int *)0x80010c;
volatile char *e_gp1  = (char*)0x8001f0;
volatile char *e_gp2  = (char*)0x8001f1;
volatile char *e_sw1  = (char*)0x8001fc;
volatile char *e_sw2  = (char*)0x8001fd;
volatile char *e_sw3  = (char*)0x8001fe;
volatile char *e_gin  = (char*)0x8001ff;

#define LIMIT 10
#include "cfont.h" // include character & number font
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

char ConvNum(int num){
   switch(num){
    	case 0: return '0';
    	case 1: return '1';
    	case 2: return '2';
    	case 3: return '3';
   		case 4: return '4';
    	case 5: return '5';
    	case 6: return '6';
    	case 7: return '7';
    	case 8: return '8';
    	case 9: return '9';
    	default: return 0;
    }
    return 0;
}
/**********************************************************************/
void mylib_putpic(int x, int y, int width, int height,  
                  const unsigned int data[][width]){
    int i,j;

    for(i=0; i<height; i++)
        for(j=0; j<width; j++)
            e_vram[(x+j) + (y+i)*128] =  data[i][j];
}

/**********************************************************************/
void mylib_putc(int x, int y, char c, int color){
    int i, j;
    
    for(i=0; i<16; i++){
        for(j=0; j<8; j++){
            if(e_char[(int)(c-'A')][i][j]) 
                e_vram[(x+j)+(y+i)*128] = color;
        }
    }
}
/**********************************************************************/
void mylib_putnum(int x, int y, int num, int color){
    int i, j;
    
    for(i=0; i<16; i++){
        for(j=0; j<8; j++){
            if(e_number[num][i][j]) e_vram[(x+j)+(y+i)*128] = color;
        }
    }
}

/**********************************************************************/
void mylib_putuint(int x, int y, int num, int color){
    x = x - 7;

    if (x >= 128 || x <= -8 || y >= 128 || y <= -16)
        return;

    if (num < 0)
        return;
    else if (num == 0)
        mylib_putnum(x, y, num, color);

    for(; (num/10 != 0) || (num%10 != 0);){
        mylib_putnum(x, y, num % 10, color);
        x = x - 8;
        num = num / 10;
    }

}

/**********************************************************************/
void mylib_msleep(unsigned int tm){
    unsigned int end = (unsigned int) *e_time + tm;
    while (*e_time < end);
}

/**********************************************************************/
void mylib_clear(int color){
    int i;
    for(i=0; i<128*128; i++) e_vram[i] = color;
}

/**********************************************************************/
unsigned int fib(int n){
    if(n<=2) return 1;
    else return (fib(n-1) + fib(n-2));
}
/**********************************************************************/
int main(){
	int i,j, answer;
	int x = 57;
	int y = 55;
	 /*rをクロックの値にするんだよ*/
	unsigned long int r = 54786;
	unsigned long int a = 1664525;
	unsigned long int c = 1013904223;
	unsigned long int pr ;
	unsigned long int pow = 1 << 30;
	unsigned long int divp = 1 << 24;
	char randnum[2] = {'0', '0'};
	char randchar[2] = {'0', '0'};
	
    i = 1;
    while(1){
        answer = fib(i);
		if(*e_sw1==0 && x>10){
			mylib_clear(7);
			
			r = (unsigned long int)*e_time;
			
			for(j = 0; j < LIMIT; j++){
				r += (a*r + c)%pow;
			}	
			pr = r/(divp * LIMIT); 
			//printf("num:%u \n",(unsigned int)pr);	
			randnum[0] = pr/ 10;
			randnum[1] = pr -(randnum[0]*10);  
			
			randchar[0] = ConvNum(randnum[0]);
			randchar[1] = ConvNum(randnum[1]);
			
			mylib_putc( 48, 0, randchar[0], 1);
        	mylib_putc( 48, 0, randchar[1], 1);
			
		}

        mylib_putuint(120, 40, i,      0);
        mylib_putuint(120, 70, answer, 0);

        mylib_msleep(1000);
        i++;
    }
}
/**********************************************************************/

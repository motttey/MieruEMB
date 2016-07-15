#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define LIMIT 1

/*線形合同法による乱数発生プログラム*/
int main(void){
	/*rをクロックの値にするんだよ*/
	unsigned long int r = 1;
	unsigned long int a = 1664525;
	unsigned long int c = 1013904223;
	unsigned long int pow = 1 << 31;
	int i;
	
	for(i = 0; i < LIMIT; i++){
		r = (a*r + c)%pow;
		printf("num:%u \n",(unsigned int)r);		
	}	

	return 0;
}


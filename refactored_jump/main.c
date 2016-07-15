// Last Edit: 20160203 by rdn
volatile char *e_vram = (char*)0x900000;
volatile int  *e_time = (int *)0x80010c;
volatile char *e_gp1  = (char*)0x8001f0;
volatile char *e_gp2  = (char*)0x8001f1;
volatile char *e_sw1  = (char*)0x8001fc;
volatile char *e_sw2  = (char*)0x8001fd;
volatile char *e_sw3  = (char*)0x8001fe;
volatile char *e_gin  = (char*)0x8001ff;

// Inverted Button Status
#define L_PUSHED !*e_sw1
#define C_PUSHED !*e_sw2
#define R_PUSHED !*e_sw3

// include character & number font
#include "cfont.h"

// Colors
#define BLACK   0
#define BLUE    1
#define GREEN   2
#define CYAN    3
#define RED     4
#define MAGENTA 5
#define YELLOW  6
#define WHITE   7
#define NOT_USED_COLOR 8
#define TRANSPARENT    9

/**********************************************************************/
#define D_WIDTH  128
#define D_HEIGHT 128

#define P_WIDTH  32
#define P_HEIGHT 32

#include "pic.h"

char DISPLAY[D_HEIGHT * D_WIDTH];

/**********************************************************************/
// 画像を描画
void putpic( int x, int y, int width, int height, 
							const unsigned char data[][width], int is_inverted ) {
	int i, j;

	for ( i = 0; i < height; ++i ) {
		for ( j = 0; j < width; ++j ) {
			char pixel = data[i][is_inverted ? width-j-1 : j];
			
			if ( pixel < 8 ) {
				DISPLAY[ (x + j) + (y + i)*D_WIDTH ] = pixel;
			}
		}
	}
}

/**********************************************************************/
// 文字を描画
void putc( int x, int y, char c, int color ) {
	int i, j;
	
	for ( i = 0; i < 16; ++i ) {
		for ( j = 0; j < 8; ++j ) {
			if ( e_char[ (int)(c-'A') ][i][j] ) {
				DISPLAY[ (x+j) + (y+i)*128 ] = color;
			}
		}
	}
}
/**********************************************************************/
// 数字を描画
void putnum( int x, int y, int num, int color ) {
	int i, j;
	
	for ( i = 0; i < 16; ++i ) {
		for ( j = 0; j < 8; ++j ) {
			if ( e_number[num][i][j] ) {
				DISPLAY[ (x+j) + (y+i)*128 ] = color;
			}
		}
	}
}

/**********************************************************************/
// ミリ秒単位でスリープ
void msleep( unsigned int sleep_time ) {
	unsigned int finish_time = (unsigned int) *e_time + sleep_time;
	
	while ( *e_time < finish_time );
}

/**********************************************************************/
// バッファをフラッシュ
void flush() {
	int i;
	
	for ( i = 0; i < D_HEIGHT * D_WIDTH; ++i ) {
		DISPLAY[i] = 7;
	}
}

// バッファからレンダリング
void render() {
	int i;
	
	for ( i = 0; i < D_HEIGHT * D_WIDTH; ++i ) {
		e_vram[i] = DISPLAY[i];
	}
}

/**********************************************************************/
// Set Walking Picture
void set_pic_walk( const unsigned char (**p)[P_WIDTH] ) {
	switch ( (*e_time / 100) % 4 ) {
		case 0:
			*p = pic_br_walk_0;
			break;
			
		case 1:
		case 3:
			*p = pic_br_walk_1;
			break;
			
		case 2:
			*p = pic_br_walk_2;
			break;
	}
}

/**********************************************************************/
int main( void ) {
	int x = 57;
	int y = 55;
	
	int wall_thickness = 8;
	int y_ceil       = wall_thickness;
	int y_floor      = D_HEIGHT - wall_thickness - P_HEIGHT + 1;
	int x_left_wall  = wall_thickness;
	int x_right_wall = D_WIDTH  - wall_thickness - P_WIDTH;
	
	int v_x        = 8;
	int v_y        = 0;
	int v_y_0_jump = -24;
	int a_y        = 8;
	
	char is_left = 0;
	
	const unsigned char (*p_br)[P_WIDTH];
	
	// MAIN LOOP
	for ( ; ; ) {
		//putpic( 0, 0, D_WIDTH, D_HEIGHT, pic_stage );
		flush();
		
		p_br = pic_br_stop;
		
		// *****BUTTON*****
		// left
		if (  L_PUSHED && !C_PUSHED && !R_PUSHED ) {
			x -= v_x;
			x = x < x_left_wall ? x_left_wall : x;
			
			set_pic_walk( &p_br );
			is_left = 1;
		}
		
		// center
		if ( !L_PUSHED &&  C_PUSHED && !R_PUSHED ) {
			p_br = pic_br_stop_shoot;
		}
		
		// right
		if ( !L_PUSHED && !C_PUSHED &&  R_PUSHED ) {
			x += v_x;
			x = x > x_right_wall ? x_right_wall : x;
			
			set_pic_walk( &p_br );
			is_left = 0;
		}

		// left + right
		if (  L_PUSHED && !C_PUSHED &&  R_PUSHED ) {
			if ( y == y_floor ) {
				v_y = v_y_0_jump;
			}
		}
		
		if ( v_y < 0 || y < y_floor ) {
			v_y += a_y;
			y   += v_y;
		}
		
		if ( y <= y_ceil ) {
			v_y = 0;
			y = y_ceil;
		}
		
		if ( y >= y_floor ) {
			v_y = 0;
			y = y_floor;
		}
		
		if ( y != y_floor ) {
			p_br = p_br == pic_br_stop_shoot ? pic_br_jump_shoot : pic_br_jump;
		}
		
		putpic( x, y, P_WIDTH, P_HEIGHT, p_br, is_left );
		
		render();
		
		/*
		// sleep 10 msec
		msleep( 10 );
		*/
	}
	
	return 0;
}
/**********************************************************************/

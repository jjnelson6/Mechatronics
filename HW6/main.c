#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h> 	//for sine wave plotting
#include<string.h>
#include<stdio.h>
#include "ili9341.h"


// DEVCFG0
#pragma config DEBUG = OFF // no debugging

#pragma config JTAGEN = OFF // no jtag

#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1

#pragma config PWP = OFF // no write protect

#pragma config BWP = OFF // no boot write protect

#pragma config CP = OFF // no code protect



// DEVCFG1

#pragma config FNOSC = PRIPLL // use primary oscillator with pll

#pragma config FSOSCEN = OFF // turn off secondary oscillator

#pragma config IESO = OFF // no switching clocks

#pragma config POSCMOD = HS // high speed crystal mode

#pragma config OSCIOFNC = OFF // disable secondary osc

#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock

#pragma config FCKSM = CSDCMD // do not enable clock switch

#pragma config WDTPS = PS1048576 // use slowest wdt

#pragma config WINDIS = OFF // wdt no window mode

#pragma config FWDTEN =  OFF // wdt disabled

#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%



// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal

#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz

#pragma config FPLLMUL =  MUL_24// multiply clock after FPLLIDIV

#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB

#pragma config UPLLEN = ON // USB clock on



// DEVCFG3

#pragma config USERID = 00000000 // some 16bit userid, doesn't matter what

#pragma config PMDL1WAY = OFF // allow multiple reconfigurations

#pragma config IOL1WAY = OFF // allow multiple reconfigurations

#pragma config FUSBIDIO = ON // USB pins controlled by USB module

#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module


void draw_letter(char letter , unsigned short x, unsigned short y , unsigned short text_color, unsigned short bkg_color)
{
    
    int ascii_letter = (int) letter; //type casting 
    int i;// x index
    int j;//y index
    for ( i = 0; i <= 4; i++) 
    {    char col = ASCII[ascii_letter - 32][i]; //ASCII values
    
        for ( j = 0; j <= 7; j++)
        {
            char byte = ( col >>(7-j) ) & 1 ; //Check for 0's or 1's
     
            if ( byte ==1) //turn on pixel
            { 
                LCD_drawPixel(x + i, y - j, text_color);
            }
            
            
            else{ LCD_drawPixel(x + i, y - j, bkg_color); } //turn off pixel
            
         
        }
   }
}
    
void draw_string(char* m, unsigned short x, unsigned short y, unsigned short text_color, unsigned short bkg_color)
{
  
    int i = 0; 
    while(m[i]) // write string
    { 
        draw_letter(m[i],x+i*7, y, text_color, bkg_color); 
        i++;
    }    
};






int main() {



    __builtin_disable_interrupts();



    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)

    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);



    // 0 data RAM access wait states

    BMXCONbits.BMXWSDRM = 0x0;



    // enable multi vector interrupts

    INTCONbits.MVEC = 0x1;



    // disable JTAG to get pins back

    DDPCONbits.JTAGEN = 0;



    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;//set switch as input
    LATBbits.LATB4 = 1; //Set high
    TRISAbits.TRISA4 = 0; // output A4 (LED))
    
    __builtin_enable_interrupts();

    SPI1_init();
    LCD_init();
    LCD_clearScreen(0x03E0);
    //draw_letter('F', 20, 20, 0xFFFF, 0x03E0);

    char m[100];
    sprintf(m," ");
    
    draw_string(m,20,20,0xFFFF, 0xFFFF);
    
    
    //while(1) {}


}
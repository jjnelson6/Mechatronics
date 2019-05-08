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
    SPI2_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLUE);

    short x;
    short y;
    int z; 
    int I=0;
    int Release = 0;
    char I_val[10];
    char I_plus[10];
    char I_minus[10];
    
    
    LCD_touchbox(200, 100, 20, 20, ILI9341_BLACK);
    LCD_touchbox(200, 150, 20, 20, ILI9341_BLACK);
        
    sprintf(I_plus,"I+");
    LCD_drawstring(I_plus, 210, 110,ILI9341_WHITE, ILI9341_BLACK);
    sprintf(I_minus,"I-");
    LCD_drawstring(I_minus, 210, 160,ILI9341_WHITE, ILI9341_BLACK);
    
    while(1) {
        
        char x_val[10];
        char y_val[10];
        char z_val[10];
        
        XPT2046_read(&x, &y, &z);
            
        
        x = (short) ( ((double)(x - 450)/((double)(3855 - 450))) * ILI9341_TFTWIDTH ); // conversion into pixels
        y = (short) ( (1- (double)(y - 466)/((double)(3880 - 466))) * ILI9341_TFTHEIGHT );
        
        sprintf(x_val,"X = %d", x);
        LCD_drawstring(x_val,28,35,ILI9341_WHITE, ILI9341_BLUE);
        
        sprintf(y_val,"Y = %d", y);
        LCD_drawstring(y_val,28,50,ILI9341_WHITE, ILI9341_BLUE);
        
        sprintf(z_val,"Z = %d", z);
        LCD_drawstring(z_val,28,70,ILI9341_WHITE, ILI9341_BLUE);        
        
        
        if ( (z > 1000)  & (Release == 0) )
        { 
            Release = 1;
            
            if ( (x < 220) & (x > 200 ))
                
            {
            
                if ( (y < 120) & (y >100 ) )
                { I++;   
                }
                
                else if ( (y < 170) & (y > 150 ))
                { I--;
                }
                
                
            }
            
            
            
            
        }
        
        
        
        
        else if (z < 1000) {Release = 0;}
        
        sprintf(I_val,"I = %d", I);
        LCD_drawstring(I_val,150,125,ILI9341_WHITE, ILI9341_BLACK);
        
        
        
       _CP0_SET_COUNT(0);
            
       LATAbits.LATA4 = 1; // high A4 (LED on))
      
       while (_CP0_GET_COUNT() < 11500000){
           //do nothing
      }
      
      LATAbits.LATA4 = 0; // Low A4 (LED off))
      
       while (_CP0_GET_COUNT() < 23000000){
           
           
           //do nothing
      }
      
      LCD_drawstring(x_val,28,35,ILI9341_BLUE, ILI9341_BLUE);  
      LCD_drawstring(y_val,28,50,ILI9341_BLUE, ILI9341_BLUE);
      LCD_drawstring(z_val,28,70,ILI9341_BLUE, ILI9341_BLUE);
      LCD_drawstring(I_val,150,125,ILI9341_BLACK, ILI9341_BLACK);
        
    }
        
    
    
    


}

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include "i2c_master_noint.h"
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

#pragma config PMDL1WAY = ON // allow multiple reconfigurations

#pragma config IOL1WAY = ON // allow multiple reconfigurations

#pragma config FUSBIDIO = ON // USB pins controlled by USB module

#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module


void int_motors()
{
    TRISBbits.TRISB2 = 0;//set B2 as digital output pin
    RPB3Rbits.RPB3R = 0b0101;//set B3 as OC1 pin 
    
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR2 = 2399; // PR = PBCLK / N / desiredF - 1

    TMR2 = 0; // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC1RS = 0; // duty cycle

    OC1R = 0; // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1; // turn on Timer2

    OC1CONbits.ON = 1; // turn on OC1
    
    IEC0bits.T3IE = 1;
    
    
    

}


void __ISR(12, IPL5SOFT) Timer3ISR(void) {

IFS0bits.T3IF = 0;

//WriteTimer3ISR(0);// how many times has the interrupt occurred?
//while (ReadTimer3ISR() < 10000000);

OC1RS = 20; 
LATBbits.LATB2 = 1; // set the duty cycle and direction pin

}




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
    LATBbits.LATB2 = 0;  // set motor output pin
    __builtin_enable_interrupts();
    
    
    SPI1_init();
    SPI2_init();
    LCD_init();
    int_motors();
    LCD_clearScreen(ILI9341_BLUE);
 
    int k=0;
    int Release = 0;
    //unsigned char R_val[240];
    //unsigned char G_val[240];
    //unsigned char B_val[240];
    short R_val[240];
    short G_val[240];
    short B_val[240];
    
    char type[5];
    char test[20];
    
    for (k=0; k < 240; k++){
        R_val[k]= rand() %255;
        G_val[k] = rand() %255 ;
        B_val[k] = (rand() % 255);
    
    }
    
    sprintf(type,"R");
    
    LCD_Plot_RGB(R_val, type, 20, 120, 180, 100, ILI9341_BLUE);
    
    
    while(1) {
        
      //Timer3ISR();
          
        
        
       
        
        
        
       _CP0_SET_COUNT(0);
            
       LATAbits.LATA4 = 1; // high A4 (LED on))
      
       while (_CP0_GET_COUNT() < 11500000){
           //do nothing
      }
      
      LATAbits.LATA4 = 0; // Low A4 (LED off))
      
       while (_CP0_GET_COUNT() < 23000000){
           
           
           //do nothing
      }
      
     
        
    }
        
    
    
    


}

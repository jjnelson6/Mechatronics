#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<string.h>
#include<stdio.h>
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


void setIMU(char pin, char level)
{
    i2c_master_start(); // start
    
    i2c_master_send(0b1101011 << 1|0); //address of device
    
    i2c_master_send(pin); //pin register
    
    i2c_master_send(level); //value
    
    i2c_master_stop(); //stop
}

void initIMU()
    {
    ANSELBbits.ANSB2 = 0; // turn off analog
    ANSELBbits.ANSB3 = 0;
    
    i2c_master_setup();
    
    
    }



 unsigned char getIMU(char address ,char reg)
{
    i2c_master_start(); // start
    
    i2c_master_send(address << 1|0); //address of device 
    
    i2c_master_send(reg); //register 
    
    i2c_master_restart(); //restart
    
    i2c_master_send(address << 1|1); // read
    
    unsigned char r = i2c_master_recv(); // receive
    
    i2c_master_ack(1); // acknowledge message to device that message was received
    
    i2c_master_stop(); //stop
    
    return r;
    
}
 
 
 void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length)
 {
  
  i2c_master_start(); // start
  
  i2c_master_send(address << 1|0); //address of device 
    
  i2c_master_send(reg); //register 
    
  i2c_master_restart(); //restart
    
  i2c_master_send(address << 1|1); // read
     
  int k=0;
  while ( k < length-1) 
  {  
    data[k] = i2c_master_recv();
  
    //if (k == length-1){i2c_master_ack(1);//done reading
    
    //}
    i2c_master_ack(0); // keep reading}
    k++; 
  
  }  
  
  data[k]=i2c_master_recv();
  i2c_master_ack(1);
  i2c_master_stop(); //stop
 }
 
 
 
 
 
 
 
 
 
 void LCD_drawletter(char letter , unsigned short x, unsigned short y , unsigned short text_color, unsigned short bkg_color)
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
    
void LCD_drawstring(char* m, unsigned short x, unsigned short y, unsigned short text_color, unsigned short bkg_color)
{
  
    int i = 0; 
    while(m[i]) // write string
    { 
        LCD_drawletter(m[i],x+i*7, y, text_color, bkg_color); 
        i++;
    }    
}

void LCD_vertbar(unsigned short x,  unsigned short y, int len, int wid, unsigned short color)
{

    int i; 
    int j;
    
    for (j =0; j< wid/2 ; j++)
    {
        
        for(i=0; i<len/2; i++) 
        { 
            LCD_drawPixel(x + i, y + j, color); 
      
        }    
    }
   
    for (j = 0; j< wid/2 ; j++)
    {
        
        for(i=0; i<len/2; i++) 
        { 
            LCD_drawPixel(x + i, y - j, color); 
      
        }    
    } 
}

void LCD_horzbar(unsigned short x,  unsigned short y, int len, int wid, unsigned short color)
{

    int i; 
    int j;
    
    for (j =0; j< wid/2 ; j++)
    {
        
        for(i=0; i<len/2; i++) 
        { 
            LCD_drawPixel(x + i, y + j, color); 
      
        }    
    }
   
    for (j =0; j< wid/2 ; j++)
    {
        
        for(i=0; i<len/2; i++) 
        { 
            LCD_drawPixel(x - i, y + j, color); 
      
        }    
    } 
}


void LCD_horzfill(unsigned short x,  unsigned short y, signed short x_accl_n , int len, int wid, unsigned short color_on, unsigned short color_off)
{

    int dy; 
    int dx;
    int max_wid = wid/2; 
    
    
    for(dy = 0; dy <wid/2; dy++) 
    { 
        
         
        if (x_accl_n > 0)
        {
            
            for (dx= 0 ; dx <x_accl_n ; dx++)
            {
            LCD_drawPixel(x + dx, y + dy, color_on);
            }
            
            for (dx = 1; dx < (len/2 - x_accl_n); dx++ )
            {
                LCD_drawPixel(x + x_accl_n + dx, y + dy, color_off);
                LCD_drawPixel(x - dx, y + dy, color_off);
            }
            
        }
        
        else if ( x_accl_n < 0)
        { 
            
            for (dx=0 ; dx< abs(x_accl_n); dx++ )
            {
            LCD_drawPixel(x - dx, y + dy, color_on);
            }   
            for (dx = 1; dx < (len/2 + x_accl_n) ; dx++ )
            {
                LCD_drawPixel(x + x_accl_n - dx, y + dy, color_off);
                LCD_drawPixel(x + dx, y + dy, color_off);
            }
        
        }
    }
        
    
}



void LCD_vertfill(unsigned short x,  unsigned short y, signed short y_accl_n , int len, int wid, unsigned short color_on, unsigned short color_off)
{

    int dy; 
    int dx; 
    
    
    for(dx = 0; dx <len/2; dx++) 
    { 
        
         
        if (y_accl_n > 0)
        {
            
            for (dy= 0 ; dy <y_accl_n ; dy++)
            {
            LCD_drawPixel(x + dx, y + dy, color_on);
            }
            
            for (dy = 1; dy < (wid/2 - y_accl_n); dy++ )
            {
                LCD_drawPixel(x + dx, y + y_accl_n + dy, color_off);
                LCD_drawPixel(x + dx, y - dy, color_off);
                
            }
            
        }
        
        else if ( y_accl_n < 0)
        { 
            
            for (dy =0 ; dy < abs(y_accl_n); dy++ )
            {
            LCD_drawPixel(x + dx, y - dy, color_on);
            }   
            for (dy = 1; dy < (wid/2 + y_accl_n) ; dy++ )
            {
                LCD_drawPixel(x + dx, y + y_accl_n - dy, color_off);
                LCD_drawPixel(x + dx, y + dy, color_off);
            }
        
        }
    }
        
    
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
    
    __builtin_enable_interrupts();
    
    initIMU();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_PURPLE);
    
    setIMU(0x10,0b10000010); //Set Linear Acceleration 
    setIMU(0x11,0b10001000); // Set Angular rate
    setIMU(0x12,0b00000100); // Control Register 
    
    unsigned short origin_Y = 170;
    unsigned short origin_X = 120;
    int inter_x = 0;
    
    LCD_horzbar(origin_X,origin_Y,200,20,ILI9341_WHITE ); // horz bar
    LCD_vertbar(origin_X,origin_Y,20,200,ILI9341_WHITE ); //vert bar
    
    /*
    char check[10]; 
    int value = (int) getIMU(0b1101011,0x0F); 
    sprintf(check,"Reading: %d", value);
    LCD_drawstring(check,28,35,0x0000,0x780F);
    */

    
    
    
    while(1) {

	// use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing

	// remember the core timer runs at half the sysclk

   unsigned char data[14];     
   signed short comb_data[7];  
   char checkY[50];
   char checkX[50]; 
        
    _CP0_SET_COUNT(0);
     
  
    I2C_read_multiple(0b1101011,0x20,data,14);
    
    int i=0;
    short high;
    short low;
    while ( i < 7){
        high = data[i*2+1];
        low= data[i*2];
        comb_data[i] =  ( high << 8)| low  ; 
        i++;
    }
    signed short Yaccl = comb_data[5];
    signed short Xaccl = comb_data[4];
    
    //sprintf(checkY,"Y= %d", Yaccl);
    //LCD_drawstring(checkY,28,35,ILI9341_WHITE ,ILI9341_PURPLE ); 
    //sprintf(checkX,"X= %d", Xaccl);
    //LCD_drawstring(checkX,28,50,ILI9341_WHITE,ILI9341_PURPLE);
    
    
    short X_accl_n =  - Xaccl/100;
    short Y_accl_n = - Yaccl/100;
   
    
    
    char checkvx[20];
    char checkvy[20];
    
    sprintf(checkvx,"dX = %d", X_accl_n);
    LCD_drawstring(checkvx,28,35,ILI9341_WHITE, ILI9341_PURPLE);
    sprintf(checkvy,"dY = %d", Y_accl_n);
    LCD_drawstring(checkvy,28,50,ILI9341_WHITE, ILI9341_PURPLE);
    
    
    LCD_horzfill(origin_X, origin_Y, X_accl_n, 200, 20, ILI9341_RED, ILI9341_WHITE) ;
    LCD_vertfill(origin_X, origin_Y, Y_accl_n, 20, 200, ILI9341_RED, ILI9341_WHITE) ;
    
    
      LATAbits.LATA4 = 1; // high A4 (LED on))
      
       while (_CP0_GET_COUNT() < 11500000){
           //do nothing
      }
      
      LATAbits.LATA4 = 0; // Low A4 (LED off))
      
       while (_CP0_GET_COUNT() < 23000000){
           
           
           //do nothing
      }
      
   //LCD_drawstring(checkX, 28,50, ILI9341_PURPLE, ILI9341_PURPLE  );
   //LCD_drawstring(checkY, 28,35, ILI9341_PURPLE, ILI9341_PURPLE  );
   LCD_drawstring(checkvx,28,35,ILI9341_PURPLE, ILI9341_PURPLE);
   LCD_drawstring(checkvy,28,50,ILI9341_PURPLE, ILI9341_PURPLE);
     

    }
}
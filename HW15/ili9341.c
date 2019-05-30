#include <xc.h>
#include "ili9341.h"



void LCD_init() {
    int time = 0;
    
    CS = 0; // CS
   
    LCD_command(ILI9341_SWRESET);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 7200000) {} // 300ms

    LCD_command(0xEF);
  	LCD_data(0x03);
	LCD_data(0x80);
	LCD_data(0x02);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xCF);
  	LCD_data(0x00);
	LCD_data(0xC1);
	LCD_data(0x30);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xED);
  	LCD_data(0x64);
	LCD_data(0x03);
	LCD_data(0x12);
    LCD_data(0x81);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xE8);
  	LCD_data(0x85);
	LCD_data(0x00);
	LCD_data(0x78);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xCB);
  	LCD_data(0x39);
	LCD_data(0x2C);
	LCD_data(0x00);
    LCD_data(0x34);
    LCD_data(0x02);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xF7);
  	LCD_data(0x20);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xEA);
  	LCD_data(0x00);
	LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_PWCTR1);
  	LCD_data(0x23);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_PWCTR2);
  	LCD_data(0x10);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_VMCTR1 );
  	LCD_data(0x3e);
    LCD_data(0x28);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_VMCTR2);
  	LCD_data(0x86);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_MADCTL);
  	LCD_data(0x48);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
/*    
    LCD_command(ILI9341_VSCRSADD);
  	LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
 */   
    LCD_command(ILI9341_PIXFMT);
  	LCD_data(0x55);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_FRMCTR1);
  	LCD_data(0x00);
    LCD_data(0x18);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command( ILI9341_DFUNCTR);
  	LCD_data(0x08);
    LCD_data(0x82);
    LCD_data(0x27);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xF2);
  	LCD_data(0); // 1
    LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GAMMASET);
  	LCD_data(0x01);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GMCTRP1);
  	LCD_data(0x0F);
    LCD_data(0x31);
    LCD_data(0x2B);
    LCD_data(0x0C);
    LCD_data(0x0E);
    LCD_data(0x08);
    LCD_data(0x4E);
    LCD_data(0xF1);
    LCD_data(0x37);
    LCD_data(0x07);
    LCD_data(0x10);
    LCD_data(0x03);
    LCD_data(0x0E);
    LCD_data(0x09);
    LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GMCTRN1);
  	LCD_data(0x00);
    LCD_data(0x0E);
    LCD_data(0x14);
    LCD_data(0x03);
    LCD_data(0x11);
    LCD_data(0x07);
    LCD_data(0x31);
    LCD_data(0xC1);
    LCD_data(0x48);
    LCD_data(0x08);
    LCD_data(0x0F);
    LCD_data(0x0C);
    LCD_data(0x31);
    LCD_data(0x36);
    LCD_data(0x0F);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xB1);
  	LCD_data(0x00);
    LCD_data(0x10);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_SLPOUT);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_DISPON);
    
    CS = 1; // CS
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    
    CS = 0; // CS
    
    LCD_command(ILI9341_MADCTL);
    LCD_data(MADCTL_MX | MADCTL_BGR); // rotation
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    CS = 1; // CS
}

void SPI1_init() {
  SDI1Rbits.SDI1R = 0b0100; // B8 is SDI1
  RPA1Rbits.RPA1R = 0b0011; // A1 is SDO1
  TRISBbits.TRISB7 = 0; // CS is B7
  CS = 1; // CS starts high
   
  // DC pin
  TRISBbits.TRISB9 = 0;
  DC = 1;
  
  SPI1CON = 0; // turn off the spi module and reset it
  SPI1BUF; // clear the rx buffer by reading from it
  SPI1BRG = 0; // baud rate to 12 MHz [SPI1BRG = (48000000/(2*desired))-1]
  SPI1STATbits.SPIROV = 0; // clear the overflow bit
  SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
  SPI1CONbits.MSTEN = 1; // master operation
  SPI1CONbits.ON = 1; // turn on spi1
}



void SPI2_init() {
  SDI2Rbits.SDI2R = 0b0011; // B13 is SDI2
  RPB11Rbits.RPB11R= 0b0100 ; //B11 is SDO2
  TRISBbits.TRISB5 = 0; // TCS is B5
  TCS = 1; // CS starts high
  ANSELBbits.ANSB13 = 0;
   
  SPI2CON = 0; // turn off the spi module and reset it
  SPI2BUF; // clear the rx buffer by reading from it
  SPI2BRG = 4; // baud rate to 12 MHz [SPI1BRG = (48000000/(2*desired))-1]
  SPI2STATbits.SPIROV = 0; // clear the overflow bit
  SPI2CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
  SPI2CONbits.MSTEN = 1; // master operation
  SPI2CONbits.ON = 1; // turn on spi2
}

unsigned char Tspi_io(unsigned char o) {
  SPI2BUF = o;
  while(!SPI2STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI2BUF;
}



unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z) {
    
    unsigned char zero_byte = 0x00;
    unsigned char low_byte;
    unsigned char high_byte;
    unsigned char cmd_byte;
    
    short x_comb=0;
    short y_comb=0;
    short z1_comb=0;
    short z2_comb=0;
  
    
    // X Value
    // Start (1), address (101), Mode(0), SER(0), PD (01)
    cmd_byte = 0b11010011;
    
    TCS =0; //ready to talk
    //1st transaction
    Tspi_io(cmd_byte);
    //2nd transaction 
    high_byte = Tspi_io(zero_byte);
    //3rd transaction
    low_byte = Tspi_io(zero_byte);
    
    TCS = 1; // done talking
    
    x_comb= (high_byte << 8) | low_byte ;
    *x = x_comb >> 3;
    
     // Y Value
    // Start (1), address (001), Mode(0), SER(0), PD (01)
    cmd_byte = 0b10010001;
    
    TCS =0; //ready to talk
    //1st transaction
    Tspi_io(cmd_byte);
    //2nd transaction 
    high_byte = Tspi_io(zero_byte);
    //3rd transaction
    low_byte = Tspi_io(zero_byte);
    
    TCS = 1; // done talking
    
    y_comb= (high_byte << 8) | low_byte;
    *y = y_comb >> 3;
    
    
     // Z1 Value
    // Start (1), address (011), Mode(0), SER(0), PD (01)
    cmd_byte = 0b10110001;
    
    TCS =0; //ready to talk
    //1st transaction
    Tspi_io(cmd_byte);
    //2nd transaction 
    high_byte = Tspi_io(zero_byte);
    //3rd transaction
    low_byte = Tspi_io(zero_byte);
    
    TCS = 1; // done talking
    
    z1_comb = (high_byte << 8) | low_byte;
    z1_comb = z1_comb >> 3;
    
    
    // Z2 Value
    // Start (1), address (100), Mode(0), SER(0), PD (01)
    cmd_byte = 0b11000001;
    
    TCS =0; //ready to talk
    //1st transaction
    Tspi_io(cmd_byte);
    //2nd transaction 
    high_byte = Tspi_io(zero_byte);
    //3rd transaction
    low_byte = Tspi_io(zero_byte);
    
    TCS = 1; // done talking
    
    z2_comb = (high_byte << 8) | low_byte;
    z2_comb = z2_comb >> 3;
    
    *z = z1_comb - z2_comb + 4095;
       
    
}

void LCD_command(unsigned char com) {
    DC = 0; // DC
    spi_io(com);
    DC = 1; // DC
}

void LCD_data(unsigned char dat) {
    spi_io(dat);
}

void LCD_data16(unsigned short dat) {
    spi_io(dat>>8);
    spi_io(dat);
}

void LCD_setAddr(unsigned short x, unsigned short y, unsigned short w, unsigned short h) {
    LCD_command(ILI9341_CASET); // Column
    LCD_data16(x);
	LCD_data16(x+w-1);

	LCD_command(ILI9341_PASET); // Page
	LCD_data16(y);
	LCD_data16(y+h-1);

	LCD_command(ILI9341_RAMWR); // Into RAM
}

void LCD_drawPixel(unsigned short x, unsigned short y, unsigned short color) {
  // check boundary
    
    CS = 0; // CS
    
    LCD_setAddr(x,y,1,1);
    LCD_data16(color);
    
    CS = 1; // CS
}

void LCD_clearScreen(unsigned short color) {
    int i;
    
    CS = 0; // CS
    
    LCD_setAddr(0,0,ILI9341_TFTWIDTH,ILI9341_TFTHEIGHT);
	for (i = 0;i < ILI9341_TFTWIDTH*ILI9341_TFTHEIGHT; i++){
		LCD_data16(color);
	}
    
    CS = 1; // CS
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


void LCD_touchbox (unsigned short x_cor, unsigned short y_cor, int wid, int len ,unsigned short color_on)
{
    
    int i;
    int k;
     for(i=0; i<=wid; i++) {
         
         for (k =0; k<=len; k++)
         {
         LCD_drawPixel(x_cor+i, y_cor+k,color_on);
         }
     }
    
}


void LCD_Plot_RGB(unsigned char color_val, char title ,unsigned short x_cor, unsigned short y_cor, int y_axis, unsigned short bkg_color )
{

    int x_axis = 80;
    int i;
    unsigned short color = ILI9341_BLACK;
    char max_x = x_axis;
    char max_y = y_axis;
    short value;
    
    
    for(i=0; i< x_axis; i++) {
         
         LCD_drawPixel(x_cor+i, y_cor,color);
         
         }
    for ( i = 0; i< y_axis; i++){
        LCD_drawPixel(x_cor, y_cor+i, color);
    }
    
    for (i =0; i < ILI9341_TFTWIDTH - 2 ; i++ )
    {
        value = short(  double( color_val[i+2]*double(y_axis/255) )  );
        
        LCD_drawPixel(x_cor+i+2, y_cor+value);
    }
    
    
    LCD_drawletter("Pixel in X (0-255)", x_cor+ (x_axis/2), y_cor + 4 ,color, bkg_color);
    LCD_drawletter(title, x_cor - 4, y_cor + (y_axis/2),color, bkg_color);
    
    LCD_drawletter(max_y, x_cor - 4, y_cor + y_axis,color, bkg_color);
    LCD_drawletter(max_x, x_cor + x_axis, y_cor + 4 ,color, bkg_color);
     
}

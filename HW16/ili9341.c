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
  //SDI1Rbits.SDI1R = 0b0000; // A0 is supposed to be SDI1 but that's not an option, oops
  RPA1Rbits.RPA1R = 0b0011; // A1 is SDO1
  TRISBbits.TRISB7 = 0; // CS is B7
  CS = 1; // CS starts high
  TRISBbits.TRISB5 = 0; // CST is B5
  CST = 1; // CS starts high

  // DC pin
  TRISAbits.TRISA9 = 0;
  DC = 1;
  
  SPI1CON = 0; // turn off the spi module and reset it
  SPI1BUF; // clear the rx buffer by reading from it
  SPI1BRG = 3; // baud rate to 12 MHz [SPI1BRG = (48000000/(2*desired))-1]
  SPI1STATbits.SPIROV = 0; // clear the overflow bit
  SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
  SPI1CONbits.MSTEN = 1; // master operation
  SPI1CONbits.ON = 1; // turn on spi1
}

unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
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

void XPT2046_read(int *z, unsigned short *x, unsigned short *y){
    unsigned short z1, z2;
    
    unsigned char r1, r2;
    unsigned short t1, t2;
    
    CST = 0;
    spi_io(0b10110001); // Z1
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    z1 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b11000001); // Z2
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    z2 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b10010001); // Y
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    t1 = ((r1<<8)|r2)>>3;
    
    CST = 0;
    spi_io(0b11010001); // X
    r1 = spi_io(0x00);
    r2 = spi_io(0x00);
    CST = 1;
    t2 = ((r1<<8)|r2)>>3;
    
    
    *z = z1 - z2 + 4095;
    *y = t1;
    *x = t2;
    
}

void drawChar(unsigned short x,unsigned short y, char c){
    int i = 0;
    int j = 0;
    for(i=0;i<5;i++){
        for(j=0;j<8;j++){
            if((ASCII[c-0x20][i]>>j)&1==1){
                LCD_drawPixel(x+i,y+j,ILI9341_WHITE);
            }
            else{
                LCD_drawPixel(x+i,y+j,ILI9341_BLACK);
            }
        }
    }
}

void drawString(unsigned short x,unsigned short y, char * a){
    int i=0;
    while(a[i]){
        drawChar(x+i*5,y,a[i]);
        i++;
    }
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


void LCD_Plot_RGB(short *color_val, char *title ,unsigned short x_cor, unsigned short y_cor, int x_axis, int y_axis, unsigned short bkg_color )
{

    
    int i;
    unsigned short color = ILI9341_BLACK;
    char max_x[20];
    char max_y[20];
    char x_title[20];
    short y_value;
    short x_value;
    
    
    for(i=0; i< x_axis; i++) {
         
         LCD_drawPixel(x_cor+i, y_cor,color);
         
         }
    for ( i = 0; i< y_axis; i++){
        LCD_drawPixel(x_cor, y_cor-i, color);
    }
    
    for (i =0; i < ILI9341_TFTWIDTH - 2 ; i++ )
    {
        
        y_value = (short) (color_val[i] * (double)(y_axis/255.0) )  ;
        x_value = (short)( ( i*(double)(x_axis/240.0) )  );
        
        LCD_drawPixel(x_cor + x_value, y_cor-y_value, color);
    }
    
    
    sprintf(x_title,"Pixel");
    sprintf(max_x,"240");
    sprintf(max_y,"255");
    
    LCD_drawstring(x_title, x_cor + (x_axis/3), y_cor + 10 ,color, bkg_color);
    LCD_drawstring(title, x_cor - 10, y_cor - (y_axis/2),color, bkg_color);
    
    LCD_drawstring(max_y, x_cor - 4, y_cor - y_axis,color, bkg_color);
    LCD_drawstring(max_x, x_cor + x_axis, y_cor + 4 ,color, bkg_color);
     
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
/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "i2c_master_noint.h"
#include "ili9341.h"
#include<string.h>
#include<stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
     //Initializations 
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
    
    
    
    TRISBbits.TRISB4 = 1;//set switch as input
    LATBbits.LATB4 = 1; //Set high
    TRISAbits.TRISA4 = 0; // output A4 (LED))
    
    
    
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        
        
        
        case APP_STATE_SERVICE_TASKS:
        {
            unsigned short origin_Y = 170;
            unsigned short origin_X = 120;
    
            
            
            
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

            short X_accl_n =  - Xaccl/100;
            short Y_accl_n = - Yaccl/100;

            char checkvx[20];
            char checkvy[20];

            sprintf(checkvx,"dX = %d", X_accl_n);
            LCD_drawstring(checkvx,28,35,ILI9341_WHITE, ILI9341_PURPLE);
            sprintf(checkvy,"dY = %d", Y_accl_n);
            LCD_drawstring(checkvy,28,50,ILI9341_WHITE, ILI9341_PURPLE);


            LCD_horzfill(origin_X, origin_Y, X_accl_n, 200, 20, ILI9341_RED, ILI9341_WHITE) ;// progress bar for x
            LCD_vertfill(origin_X, origin_Y, Y_accl_n, 20, 200, ILI9341_RED, ILI9341_WHITE) ; // progress bar for y        

         while (PORTBbits.RB4 == 0  ) // check voltage on pin RB4
        {
          LATAbits.LATA4 = 0; //turn off LED
        }  
        
        _CP0_SET_COUNT(0);
   
         LATAbits.LATA4 = 1; // high A4 (LED on))
      
        while (_CP0_GET_COUNT() < 11500){
           //do nothing
        }
      
        LATAbits.LATA4 = 0; // Low A4 (LED off))
      
        while (_CP0_GET_COUNT() < 23000){
           //do nothing
        }
        LCD_drawstring(checkvx,28,35,ILI9341_PURPLE, ILI9341_PURPLE);// clear line
        LCD_drawstring(checkvy,28,50,ILI9341_PURPLE, ILI9341_PURPLE);// clear line
        
        
            break;
        }

        /* TODO: implement your application state machine.*/
       

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */

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
#include <stdio.h>
#include <xc.h>
#include "i2c_master_noint.h"
#include "ili9341.h"
#include<string.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
           
uint8_t APP_MAKE_BUFFER_DMA_READY dataOut[APP_READ_BUFFER_SIZE];
uint8_t APP_MAKE_BUFFER_DMA_READY readBuffer[APP_READ_BUFFER_SIZE];
int len, i = 0;
int output = 0;
int startTime = 0; // to remember the loop time
unsigned short origin_Y = 170;
unsigned short origin_X = 120;


#define MAF_N 6
#define FIR_N 6

short MAF_buffer[MAF_N];
float MAF_temp =0;
float MAF_div = 1.0/MAF_N;
short MAF;
int MAF_end = 0;

short FIR_buffer[FIR_N];
float FIR_temp = 0;
short FIR;
float FIR_coef[FIR_N];
float FIR_div = 1/FIR_N;
int FIR_end = 0; 

int count= 0;

 
float IIR_A = .5;
float IIR_B = .5;
short IIR_old; 
short IIR;

 

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

/*******************************************************
 * USB CDC Device Events - Application Event Handler
 *******************************************************/

USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
        USB_DEVICE_CDC_INDEX index,
        USB_DEVICE_CDC_EVENT event,
        void * pData,
        uintptr_t userData
        ) {
    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *) userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;

    switch (event) {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    &appDataObject->getLineCodingData, sizeof (USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    &appDataObject->setLineCodingData, sizeof (USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *) pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            appDataObject->breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *) pData)->breakDuration;

            /* Complete the control transfer by sending a ZLP  */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            appDataObject->isReadComplete = true;
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We dont
             * do much with this data in this demo. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */

            appDataObject->isWriteComplete = true;
            break;

        default:
            break;
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

/***********************************************
 * Application USB Device Layer Event Handler.
 ***********************************************/
void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * eventData, uintptr_t context) {
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;

    switch (event) {
        case USB_DEVICE_EVENT_SOF:

            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
            break;

        case USB_DEVICE_EVENT_RESET:

            /* Update LED to show reset state */

            appData.isConfigured = false;

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuratio. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*) eventData;
            if (configuredEventData->configurationValue == 1) {
                /* Update LED to show configured state */

                /* Register the CDC Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */

                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t) & appData);

                /* Mark that the device is now configured */
                appData.isConfigured = true;

            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            /* Switch LED to show suspended state */
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/*****************************************************
 * This function is called in every step of the
 * application state machine.
 *****************************************************/

bool APP_StateReset(void) {
    /* This function returns true if the device
     * was reset  */

    bool retVal;

    if (appData.isConfigured == false) {
        appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
        appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.isReadComplete = true;
        appData.isWriteComplete = true;
        retVal = true;
    } else {
        retVal = false;
    }

    return (retVal);
}

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



void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    /* Device Layer Handle  */
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;

    /* Device configured status */
    appData.isConfigured = false;

    /* Initial get line coding state */
    appData.getLineCodingData.dwDTERate = 9600;
    appData.getLineCodingData.bParityType = 0;
    appData.getLineCodingData.bParityType = 0;
    appData.getLineCodingData.bDataBits = 8;

    /* Read Transfer Handle */
    appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Intialize the read complete flag */
    appData.isReadComplete = true;

    /*Initialize the write complete flag*/
    appData.isWriteComplete = true;

    /* Reset other flags */
    appData.sofEventHasOccurred = false;
    //appData.isSwitchPressed = false;

    /* Set up the read buffer */
    appData.readBuffer = &readBuffer[0];

    /* PUT YOUR LCD, IMU, AND PIN INITIALIZATIONS HERE */
    
    //Initializations 
    initIMU();
    SPI1_init();
    LCD_init();
    //LCD_clearScreen(ILI9341_PURPLE);
    
  
    
    //LCD_horzbar(origin_X,origin_Y,200,20,ILI9341_WHITE ); // horz bar
    //LCD_vertbar(origin_X,origin_Y,20,200,ILI9341_WHITE ); //vert bar
       
    TRISBbits.TRISB4 = 1;//set switch as input
    LATBbits.LATB4 = 1; //Set high
    TRISAbits.TRISA4 = 0; // output A4 (LED)
    
    
    

    startTime = _CP0_GET_COUNT();
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )
  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {
    /* Update the application state machine based
     * on the current state */
    char data[14];
    
    

    switch (appData.state) {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);

            if (appData.deviceHandle != USB_DEVICE_HANDLE_INVALID) {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            } else {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device was configured */
            if (appData.isConfigured) {
                /* If the device is configured then lets start reading */
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            break;

        case APP_STATE_SCHEDULE_READ:

            if (APP_StateReset()) {
                break;
            }

            /* If a read is complete, then schedule a read
             * else wait for the current read to complete */

            appData.state = APP_STATE_WAIT_FOR_READ_COMPLETE;
            if (appData.isReadComplete == true) {
                appData.isReadComplete = false;
                appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

                USB_DEVICE_CDC_Read(USB_DEVICE_CDC_INDEX_0,
                        &appData.readTransferHandle, appData.readBuffer,
                        APP_READ_BUFFER_SIZE);

                        /* AT THIS POINT, appData.readBuffer[0] CONTAINS A LETTER
                        THAT WAS SENT FROM THE COMPUTER */
                        /* YOU COULD PUT AN IF STATEMENT HERE TO DETERMINE WHICH LETTER
                        WAS RECEIVED (USUALLY IT IS THE NULL CHARACTER BECAUSE NOTHING WAS
                      TYPED) */

                if (appData.readTransferHandle == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID) {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }

            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:
        case APP_STATE_CHECK_TIMER:

            if (APP_StateReset()) {
                break;
            }

            /* Check if a character was received or a switch was pressed.
             * The isReadComplete flag gets updated in the CDC event handler. */

             /* WAIT FOR 5HZ TO PASS OR UNTIL A LETTER IS RECEIVED */
            if (appData.isReadComplete || _CP0_GET_COUNT() - startTime > (48000000 / 2 / 100)) {
                appData.state = APP_STATE_SCHEDULE_WRITE;
            }


            break;


        case APP_STATE_SCHEDULE_WRITE:

            if (APP_StateReset()) {
                break;
            }

            /* Setup the write */

            appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            appData.isWriteComplete = false;
            appData.state = APP_STATE_WAIT_FOR_WRITE_COMPLETE;

            /* PUT THE TEXT YOU WANT TO SEND TO THE COMPUTER IN dataOut
            AND REMEMBER THE NUMBER OF CHARACTERS IN len */
            /* THIS IS WHERE YOU CAN READ YOUR IMU, PRINT TO THE LCD, ETC */
            
        
            
            signed short Xaccl, Yaccl, Zaccl, Xgyro, Ygyro, Zgyro;
            int k, w, f;
            //signed short old_Z, old_Y, old_X;
            signed short new_Z =0; //, new_Y, new_X;
            
            while(abs(new_Z) < 500)
            { 
            I2C_read_multiple(0b1101011,0x20,data,14);
           
            Xgyro = (data[3]<<8) | data[2];
            Ygyro = (data[5] <<8) | data[4] ;
            Zgyro = (data[7] <<8) | data[6];
            
            Xaccl = ( ((short)data[9]) << 8) | data[8] ;
            Yaccl = ( ((short)data[11]) <<8) | data[10] ;
            Zaccl = ( ((short)data[13]) <<8 ) | data[12] ;
            new_Z = Zaccl;
            
            
            
            }
            
            
            
            
            
            
          
            
            
            
            //short X_accl_n =  - Xaccl/100;
            //short Y_accl_n = - Yaccl/100;
            
            //LCD_horzfill(origin_X, origin_Y, X_accl_n, 200, 20, ILI9341_RED, ILI9341_WHITE) ;// progress bar for x
            //LCD_vertfill(origin_X, origin_Y, Y_accl_n, 20, 200, ILI9341_RED, ILI9341_WHITE) ; // progress bar for y
             
            
           // char checkvx[20];
            //char checkvy[20];
    
            //sprintf(checkvx,"dX = %d", Xaccl);
            //LCD_drawstring(checkvx,28,35,ILI9341_WHITE, ILI9341_PURPLE);
            //sprintf(checkvy,"dY = %d", Yaccl);
            //LCD_drawstring(checkvy,28,50,ILI9341_WHITE, ILI9341_PURPLE);
            
            
           //MAF
            
            if (i <  MAF_N)
                
            {
            
            MAF_buffer[MAF_end] = Zaccl;
            
            MAF = MAF_buffer[MAF_end] ;            
            }
            
            else{
                
                for(w=0; w< MAF_N; w++)
                {
                MAF_temp += MAF_buffer[w];
             
                }
                MAF_buffer[MAF_end] = Zaccl;
                
                MAF = (MAF_temp*MAF_div);
                MAF_temp = 0;
                
                
            }
            
            
            
          //  
       
         //FIR
            
           
         FIR_coef[0] = .0264;
         FIR_coef[1] = .1405;
         FIR_coef[2] = .3331;
         FIR_coef[3] = .3331;
         FIR_coef[4] = .1405;
         FIR_coef[5] = .0264;
           
            
         /*   
         FIR_coef[0] = -.0084;
         FIR_coef[1] = -.0428;
         FIR_coef[2] = -.1235;
         FIR_coef[3] = .8218;
         FIR_coef[4] = -.1235;
         FIR_coef[5] = -.0428;
         FIR_coef[6] = -.0084;
         */   
            
            
            if (i <  FIR_N)
                
            {
            
            FIR_buffer[FIR_end] = Zaccl*FIR_coef[FIR_end];
            FIR = Zaccl;            
            }
            
            else {
                for(f=0; f< FIR_N; f++)
                {
                FIR_temp += FIR_buffer[f];
             
                }
                FIR_buffer[FIR_end] = Zaccl*FIR_coef[FIR_end];
                FIR = FIR_temp;
                FIR_temp = 0;
                
            }
            
            
           //
            
           //IIR
            
            if (i < 1)
            {
                IIR = Zaccl;
                IIR_old = Zaccl;
            }
            else{
                IIR = (short)(IIR_A*IIR_old + IIR_B*Zaccl);
                IIR_old = Zaccl;
            }
            //.1 inch, ,121 inch for d gear
        
        
         
         
         
        
            len = sprintf(dataOut, "%i     %d     %d     %d     %d\r\n ", i, Zaccl, MAF, IIR, FIR );
            
           
            
            //len = sprintf(dataOut, "%i    %d    %d    %d    %f\r\n ", i, Zaccl,-1*abs(MAF), MAF_buffer[MAF_end], MAF_temp/6);
            //len = sprintf(dataOut, "%i  %d  %d  %d \r\n ", i, Xaccl, Yaccl, Zaccl);
            
            //len = sprintf(dataOut," \r\n");
            
            /* IF A LETTER WAS RECEIVED, ECHO IT BACK SO THE USER CAN SEE IT */
            if (appData.isReadComplete) {
                /*USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle,
                        appData.readBuffer, 1,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);*/
                
                if (appData.readBuffer[0] == 'r')
                {
                    output= 1;
                    //appData.readBuffer = "r";
                        len= 1;
                        dataOut[0]=0;
                    
                    
                    USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle, dataOut, len,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                    startTime = _CP0_GET_COUNT(); // reset the timer for acurate delays
                }
                    
                else
                { 
                   len = sprintf(dataOut, "Incorrect key. Use 'r' to start or 's' to stop");
                   
                    USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle, dataOut, len,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                
                }
                    
                
                
                
            }
            /* ELSE SEND THE MESSAGE YOU WANTED TO SEND */
            else {
                
                if (output==1)
                    
                {
                USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle, dataOut, len,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                 i++;
                 MAF_end = (MAF_end + 1) % MAF_N;
                 FIR_end = (FIR_end + 1) % FIR_N;
                 
                 
                }
                
                
                else {
                
                    dataOut[0]=0;
                    len=1;
                
                USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle, dataOut, len,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                
                
                }
                startTime = _CP0_GET_COUNT(); // reset the timer for acurate delays
                
               
                if (i > 100) {
                    output = 0;
                    i = 0;
                    MAF_end = 0;
                    FIR_end=0;
                }
                
            }
            break;

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            if (APP_StateReset()) {
                break;
            }

            /* Check if a character was sent. The isWriteComplete
             * flag gets updated in the CDC event handler */

            if (appData.isWriteComplete == true) {
                appData.state = APP_STATE_SCHEDULE_READ;
            }

            break;

        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}



/*******************************************************************************
 End of File
 */
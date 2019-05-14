// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Change I2C1 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h> 


void i2c_master_setup(void) {
  I2C2BRG = 53;            //53 I2CBRG = [1/(2*Fsck) - PGD]*Pblck - 2 
                                    // look up PGD for your PIC32
  I2C2CONbits.ON = 1;               // turn on the I2C1 module
  ANSELBbits.ANSB2 = 0; // turn off analog
  ANSELBbits.ANSB3 = 0;

}



// Start a transmission on the I2C bus
void i2c_master_start(void) {
    I2C2CONbits.SEN = 1;            // send the start bit
    while(I2C2CONbits.SEN) { ; }    // wait for the start bit to be sent
}

void i2c_master_restart(void) {     
    I2C2CONbits.RSEN = 1;           // send a restart 
    while(I2C2CONbits.RSEN) { ; }   // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to device
  I2C2TRN = byte;                   // if an address, bit 0 = 0 for write, 1 for read
  while(I2C2STATbits.TRSTAT) { ; }  // wait for the transmission to finish
  if(I2C2STATbits.ACKSTAT) {  while(1){}      // if this is high, device has not acknowledged
    // ("I2C2 Master: failed to receive ACK\r\n");
  }
}

unsigned char i2c_master_recv(void) { // receive a byte from the device
    I2C2CONbits.RCEN = 1;             // start receiving data
    while(!I2C2STATbits.RBF) { ; }    // wait to receive the data
    return I2C2RCV;                   // read and return the data
}

void i2c_master_ack(int val) {        // sends ACK = 0 (device should send another byte)
                                      // or NACK = 1 (no more bytes requested from device)
    I2C2CONbits.ACKDT = val;          // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1;            // send ACKDT
    while(I2C2CONbits.ACKEN) { ; }    // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) {          // send a STOP:
  I2C2CONbits.PEN = 1;                // comm is complete and master relinquishes bus
  while(I2C2CONbits.PEN) { ; }        // wait for STOP to complete
}

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

    
    i2c_master_setup();
    setIMU(0x10,0b10000010); //Set Linear Acceleration 
    setIMU(0x11,0b10001000); // Set Angular rate
    setIMU(0x12,0b00000100); // Control Register
    
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
     
     
     
     
     
 
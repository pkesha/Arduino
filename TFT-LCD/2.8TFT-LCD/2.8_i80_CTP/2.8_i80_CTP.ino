
// The 8 bit data bus is connected to PORTD of the Seeduino (Arduino Uno)
// The 3.3V option has been selected for the switch in the Seeduino (Arduino Uno)
//CTP touchscreen.  This code also supports Gestures

#include <Wire.h>


#define CS 	8
#define RS  9   // RS signal connected to Arduino digital pin 9
#define WR  10  // /WR signal connected to Arduino digital pin 10
#define RD  11  // /RD signal connected to Arduino digital pin 11
#define RES 12  // /RES signal connected to Arduino digital pin 12

// /CS signal tied to GND
// IM0 signal tied to VDD

//Touch screen Pin and variable declarations
#define RESTOUCH  8   //Touch reset
#define INT       13  //Interupt pin
//Pin SDA is define as SDA on ard
//Same SCL

//Variables needed for touchscreen
unsigned char dummy, gesture, numpoints, t1xh, t1xl, t1yh, t1yl, readValue, x, read1, read2, read3, read4;
unsigned int t1x, t1y;

//This might be for 2nd touch. Two touches at the sametime.
unsigned char t2xh, t2xl, t2yh, t2yl, read5, read6, read7, read8, dummy1, gesture1, numpoints1, readValue1, x1;
unsigned int t2x, t2y;

const char slave = 0x38;
const char slave_read = 0x39;

int dispstop = 0;


//------------------------------------------------------------------------initialize CTP code
void init_CTP() 
{
  //digitalWrite(RESTOUCH, LOW);
  //delay(1);
  digitalWrite(RESTOUCH, HIGH);
  delay(10);
}

//--------------------------------------------------------------------------------------Writing via I2C
void i2c_write(char addr, unsigned char reg, unsigned char i2cwrite)
{
  Wire.beginTransmission(addr); //Start transmission at address
  Wire.write(reg);  //Write register value
  Wire.write(i2cwrite);
  Wire.endTransmission(); //End transmission
}

void i2c_read4times(char addr, unsigned char reg) //-----------------------function for reading 4 registers in a row, starting from specified register
{
  unsigned char x;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 4);
  while(Wire.available())
  {
    read1 = Wire.read();
    read2 = Wire.read();
    read3 = Wire.read();
    read4 = Wire.read();
  }
}

void comm_out(unsigned char c)
{
  digitalWrite(RS, LOW);
  PORTD = c;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

void data_out(unsigned char d)
{
  digitalWrite(RS, HIGH);
  PORTD = d;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}


void disp()
{
  unsigned int i; 
  comm_out(0x2C);              //command to begin writing to frame memory
  for(i=0;i<38400;i++)         //fill screen with blue pixels
  {
    data_out(0x00);
    data_out(0x1F);
    data_out(0x00);
    data_out(0x1F);
  }

  //Printing a white line
  for(i=0;i<240;i++)
  {
   data_out(0xFF);
   data_out(0xFF);
  }
//----------------------------------------------------------------------  
  //Setting addresses locations for top white bar
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  data_out(0xEF);
  
  comm_out(0x2B);  //Y address set
  data_out(0x01);
  data_out(0x3F);
  data_out(0x01);
  data_out(0x3F);
  
  comm_out(0x2C);
  //Printing a white line
  for(i=0;i<240;i++)
  {
   data_out(0xFF);
   data_out(0xFF);
  }
//----------------------------------------------------------------------
  //left White vertical lines
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  
  comm_out(0x2B);  //Y address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x01);
  data_out(0x3F);
  
  comm_out(0x2C);
  for(i=0;i<320;i++)
  {
   data_out(0xFF);
   data_out(0xFF);
  }
//----------------------------------------------------------------------
  // right White vertical lines
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0xEF);
  data_out(0x00);
  data_out(0xEF);
  
  comm_out(0x2B);  //Y address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x01);
  data_out(0x3F);
  
  comm_out(0x2C);
  for(i=0;i<320;i++)
  {
   data_out(0xFF);
   data_out(0xFF);
  }
}

void testdisp()
{
  //Initialize x, y coordinates 
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  data_out(0xEF);

  comm_out(0x2B);  //Y address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x01);
  data_out(0x3F);
  delay(10);
  
  //if code is being run correctly, this function will produce a black screen
  unsigned int i; 
  comm_out(0x2C);              //command to begin writing to frame memory
  for(i=0;i<38400;i++)         //fill screen with blue pixels
  {
    data_out(0x00);
    data_out(0x00);
    data_out(0x00);
    data_out(0x00);
  }
}

void setup()
{  
  DDRD = 0xFF;
  PORTD = 0x00;

  //Initializing Control pins
  pinMode(RS, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(RES, OUTPUT);
  pinMode(CS, OUTPUT);
  
  digitalWrite(RD, HIGH);
  digitalWrite(WR, LOW);
  digitalWrite(RES, LOW);
  delay(250);
  digitalWrite(RES, HIGH);
  delay(250);
  digitalWrite(CS, LOW);

  comm_out(0x28);  //display off
  comm_out(0x11);  //exit SLEEP mode
  delay(100);
  
  comm_out(0x36);  //MADCTL: memory data access control
  data_out(0x80);  //RGB order
  
  //comm_out(0x3A); //COLMOD: Interface Pixel format *** 262K-colors in 18bit/pixel format when using 8-bit interface to allow 3-bytes per pixel
  //data_out(0x66);
  
  comm_out(0x3A);  //COLMOD: Interface Pixel format  *** 65K-colors in 16bit/pixel (5-6-5) format when using 16-bit interface to allow 1-byte per pixel
  data_out(0x55);
  
  comm_out(0xB2);  //PORCTRK: Porch setting
  data_out(0x0C);
  data_out(0x0C);
  data_out(0x00);
  data_out(0x33);
  data_out(0x33);
  
  comm_out(0xB7);  //GCTRL: Gate Control
  data_out(0x35);
  
  comm_out(0xBB);  //VCOMS: VCOM setting
  data_out(0x2B);
 
  comm_out(0xC0);  //LCMCTRL: LCM Control
  data_out(0x2C);
  
  comm_out(0xC2);  //VDVVRHEN: VDV and VRH Command Enable
  data_out(0x01);
  data_out(0xFF);
  
  comm_out(0xC3);  //VRHS: VRH Set
  data_out(0x11);
  
  comm_out(0xC4);  //VDVS: VDV Set
  data_out(0x20);
  
  comm_out(0xC6);  //FRCTRL2: Frame Rate control in normal mode
  data_out(0x0F);
  
  comm_out(0xD0);  //PWCTRL1: Power Control 1
  data_out(0xA4);
  data_out(0xA1);
  
  comm_out(0xE0);  //PVGAMCTRL: Positive Voltage Gamma control  
  data_out(0xD0);
  data_out(0x00);
  data_out(0x05);
  data_out(0x0E);
  data_out(0x15);
  data_out(0x0D);
  data_out(0x37);
  data_out(0x43);
  data_out(0x47);
  data_out(0x09);
  data_out(0x15);
  data_out(0x12);
  data_out(0x16);
  data_out(0x19);
  
  comm_out(0xE1);  //NVGAMCTRL: Negative Voltage Gamma control  
  data_out(0xD0);
  data_out(0x00);
  data_out(0x05);
  data_out(0x0D);
  data_out(0x0C);
  data_out(0x06);
  data_out(0x2D);
  data_out(0x44);
  data_out(0x40);
  data_out(0x0E);
  data_out(0x1C);
  data_out(0x18);
  data_out(0x16);
  data_out(0x19);
  
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  data_out(0xEF);

  comm_out(0x2B);  //Y address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x01);
  data_out(0x3F);
  delay(10);

  comm_out(0x21); //invert display
  comm_out(0x29);  //display ON
  delay(10);

  //Touchscreen setup
  pinMode(RESTOUCH, OUTPUT);
  pinMode(INT, INPUT);
  init_CTP();
  Wire.begin();
  //Serial.begin(9600);
}

void loop()
{
  //Problem with statements here, it doesn't run
  //Make sure screen border and blue background stay static
  if (dispstop == 0)
  {
      disp();
      dispstop = 1;
  }
  delay(100);

  while(1)
  {
  
  //White line on right is removed after this
  //unsigned char t2xh, t2xl, t2yh, t2yl;
  //unsigned int t2x, t2y;
  while((digitalRead(INT)) == LOW)
    {
        i2c_read4times(slave, 0x00);
        dummy = read1;                               //dummy read
        gesture = read2;                             //which gesture register read
        numpoints = read3;                           //how many touch points register read
        t1xh = read4;                                //upper 8 bits of X-axis touch location
        i2c_read4times(slave, 0x04);
        t1xl = read1;                                //lower 8 bits of X-axis touch location
        t1yh = read2;                                //upper 8 bits of Y-axis touch location
        t1yl = read3;                                //lower 8 bits of X-axis touch location
        dummy = read4;                               //dummy read
        t1x = t1xl | (t1xh << 8);                    //get the 16 bit X-axis touch location
        t1y = t1yl | (t1yh << 8);                    //get the 16 bit Y-axis touch location*/
        t1x&=0x0FFF;
        t1y&=0x0FFF;

        //Second touch point
        i2c_read4times(slave, 0x00);
        dummy1 = read5;                               //dummy read
        gesture1 = read6;                             //which gesture register read
        numpoints1 = read7;                           //how many touch points register read
        t2xh = read8;                                 //upper 8 bits of X-axis touch location
        
        //Double check this function to include second point
        i2c_read4times(slave, 0x04);
        t1xl = read1;                                //lower 8 bits of X-axis touch location
        t1yh = read2;                                //upper 8 bits of Y-axis touch location
        t1yl = read3;                                //lower 8 bits of X-axis touch location
        dummy = read4;                               //dummy read
        t1x = t1xl | (t1xh << 8);                    //get the 16 bit X-axis touch location
        t1y = t1yl | (t1yh << 8);                    //get the 16 bit Y-axis touch location*/
        t1x&=0x0FFF;
        t1y&=0x0FFF;

        //To have same xy plane/origin used 
        t1y = 319-t1y;

        //Serial.println(t1x, DEC);delay(25);
      //---------------------------------------
        //Writing to x and y line touch locations
        comm_out(0x2A);  //X address set
        data_out(t1x>>8);
        data_out(t1x);
        data_out(t1x>>8);
        data_out(t1x);
  
        comm_out(0x2B);  //Y address set
        data_out(t1y>>8);
        data_out(t1y);
        data_out(t1y>>8);
        data_out(t1y);

        //Printing white pixel
        comm_out(0x2C);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);
        data_out(0xFF);


        //Gesture block of code
        switch (gesture) 
        {
          //Reset display
          case 0x10:
          //reset display xy addresses
          comm_out(0x2A);  //X address set
          data_out(0x00);
          data_out(0x00);
          data_out(0x00);
          data_out(0xEF);

          comm_out(0x2B);  //Y address set
          data_out(0x00);
          data_out(0x00);
          data_out(0x01);
          data_out(0x3F);
          delay(10);
          
          disp();
          break;

          //Turns screen black
          case 0x18:
          testdisp();
          break;
          
        }
        
      //----------------------------------------
    }
  }
   //testdisp();
}

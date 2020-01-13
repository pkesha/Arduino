/*Created by Parham K.
8 bit i80 mode. 3.3 volt logic. ST7789Vi
Purpose: Cycle SD card images*/
#include <SPI.h>
#include <SD.h>

File pic;

#define CS 	8   //Controller TFT
#define DC  9   // RS signal connected to Arduino digital pin 9
#define WR  A1  // /WR signal connected to Arduino digital pin 10
#define RD  A2  // /RD signal connected to Arduino digital pin 11
#define RES A3  // /RES signal connected to Arduino digital pin 12
#define CS_SD A0 //SD card chip select

//----------------------------------Command function
void comm_out(unsigned char c)
{
  digitalWrite(DC, LOW);  //Setting for command
  digitalWrite(WR, LOW);  //Writing to TFT is an active low for 'write' pin
  digitalWrite(CS, LOW);  //Enabling TFT controller
  PORTD = c;
  digitalWrite(WR, HIGH); //Clocking it in
  digitalWrite(CS, HIGH); //Disabling the TFT controller
}

//-----------------------------------------------------------------------------------Data function
void data_out(unsigned char d)
{
  digitalWrite(DC, HIGH); //Setting for data
  digitalWrite(WR, LOW);  //Writing to TFT is an active low for 'write' pin
  digitalWrite(CS, LOW);  //Enabling TFT controller
  PORTD = d;
  digitalWrite(WR, HIGH); //Clocking it in
  digitalWrite(CS, HIGH); //Disabling the TFT controller
}

//---------------------------------------------------------------------------------Display function
void disp()
{
  unsigned long int i; 
  comm_out(0x2C);              //command to begin writing to frame memory
  for(i=0;i<76800;i++)         //fill screen with green pixels
  {
    data_out(0x00);
    data_out(0xFF);
    data_out(0x00);
  }
}

//----------------------------------------------------------------------------------PrintBMP()

void PrintBMP() {
  //Enabling SD card
  digitalWrite(CS_SD, LOW);

  //Dummy reading BMP header files
  uint32_t dummy;
  //54 Bytes of garbage data from SD card header
  for (unsigned int i = 0; i < 54; i++)
  {
    dummy = pic.read();
  }

  //Command to write color data to frame memory 
  //- SD card CS needs to be disabled when using controller CS
  digitalWrite(CS_SD, HIGH);  //disabling SD card
  comm_out(0x2C);
  digitalWrite(CS_SD, LOW);   //Enabling SD card

  //Read until there is no more data to read from BMP file
  while (pic.available())
  {
    uint32_t readBMP = pic.read();
    //- SD card CS needs to be disabled when using controller CS
    digitalWrite(CS_SD, HIGH);  //disabling SD card
    data_out(readBMP);
    digitalWrite(CS_SD, LOW); //renables SD card
  }
  pic.close();  //Closes files from SD card
  digitalWrite(CS_SD, HIGH);  //disables SD card
}

//----------------------------------------------Setup
void setup()
{  
  DDRD = 0xFF;
  PORTD = 0x00;

  //Initializing Control pins
  pinMode(DC, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(RES, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CS_SD, OUTPUT);

  //Initializng pins to avoid fuzz
  digitalWrite(RD, HIGH);
  digitalWrite(WR, LOW);
  digitalWrite(RES, LOW);
  digitalWrite(RES, HIGH);
  digitalWrite(CS, LOW);

  comm_out(0x28);  //display off
  comm_out(0x11);  //exit SLEEP mode

  comm_out(0x36);  //MADCTL: memory data access control
  data_out(0x88);  //RGB order
  
  comm_out(0x3A); //COLMOD: Interface Pixel format *** 262K-colors in 18bit/pixel format when using 8-bit interface to allow 3-bytes per pixel
  data_out(0x66);
  
//  comm_out(0x3A);  //COLMOD: Interface Pixel format  *** 65K-colors in 16bit/pixel (5-6-5) format when using 16-bit interface to allow 1-byte per pixel
//  data_out(0x55);
  
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
}

void loop()
{
  disp();
  SD.begin(CS_SD); 
  
  //Printing BMP functions
  pic = SD.open("2_4-1.bmp", FILE_READ);  //Open SD card file
  PrintBMP(); //Printing function
  pic = SD.open("2_4-2.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("2_4-3.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("2_4-4.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("2_4-5.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("2_4-6.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("2_4-7.bmp", FILE_READ);
  PrintBMP();
}

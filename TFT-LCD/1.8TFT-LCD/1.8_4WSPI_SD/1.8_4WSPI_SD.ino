/* Created by Parham K.
1.8 EF TFT model with ILI9163V Controller and Arduino SPI library
Purpose is to test the display by cycling through images*/

#include <SPI.h>
#include <SD.h>
#include <TFT.h>

#define CS_Controller 0   // /RST signal connected to Arduino digital pin 9
#define DC 1 // D/C signal connected to Arduino digital pin 10
#define CS_SD 10


File pic;
//----------------------------------------------------------------------------------Commands
void comm_out(unsigned char c)
{
  digitalWrite(DC, LOW);//Data/command pin is for command
  digitalWrite(CS_Controller, LOW);
  SPI.transfer(c); 
  digitalWrite(CS_Controller, HIGH);
}

//-------------------------------------------------------------------------------------Data
void data_out(unsigned char d)
{
  digitalWrite(DC, HIGH);	//Data/command pin is for data
  digitalWrite(CS_Controller, LOW);
  SPI.transfer(d);
  digitalWrite(CS_Controller, HIGH);
}

//--------------------------------------------------------------------------------display
void disp()
{
  unsigned int i; //number of pixels is too large, so unsigned number
  comm_out(0x2C);              //command to begin writing to frame memory
  for (i = 0; i < 20480; i++)  //fill screen with blue pixels
  {
    data_out(0x00);
    data_out(0x00);
    data_out(0xFF);
  }
}
//------------------------------------------------------------Displaytest
void dispTest()
{
  unsigned int i; //number of pixels is too large, so unsigned number
  comm_out(0x2C);              //command to begin writing to frame memory
  for (i = 0; i < 20480; i++)  //fill screen with blue pixels
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

//-----------------------------------------------------------------------------Setup/Initialization code
void setup()
{
  pinMode(CS_Controller, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(CS_SD, OUTPUT);
      
  SPI.begin();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));

  comm_out(0x11);              //exit SLEEP mode

  //comm_out(0x28);              //display off

  comm_out(0x26);              //select gamma curve
  data_out(0x04);

  comm_out(0xB1);              //frame rate control
  data_out(0x0A);
  data_out(0x14);

  comm_out(0xC0);              //power control 1
  data_out(0x0A);
  data_out(0x00);

  comm_out(0xC1);              //power control 2
  data_out(0x02);

  comm_out(0xC5);              //VCOM control 1
  data_out(0x2F);
  data_out(0x3E);

  comm_out(0xC7);              //VCOM control 2
  data_out(0x40);

  comm_out(0x2A);              //column address set
  data_out(0x00);
  data_out(0x00);              //start 0x0000
  data_out(0x00);
  data_out(0x7F);              //end 0x007F

  comm_out(0x2B);              //page address set
  data_out(0x00);
  data_out(0x00);              //start 0x0000
  data_out(0x00);
  data_out(0x9F);              //end 0x009F

  comm_out(0x36);              //memory access control
  data_out(0x80);

  comm_out(0x3A);              //pixel format = 18 bit per pixels
  data_out(0x06);

  comm_out(0x29);              //display ON
  delay(10);  
}

void loop(){
  //Showing displays
  disp();

  SD.begin(CS_SD); 
  //Printing BMP functions
  pic = SD.open("1_8-1.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-2.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-3.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-4.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-5.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-6.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("1_8-7.bmp", FILE_READ);
  PrintBMP();
  delay(1000);

}

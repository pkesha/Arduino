/* Created by Parham K.
1.8 EF TFT model with ILI9163V Controller and arduino SPI library
Purpose is to test the display by cycling through images*/

#include <SPI.h>
#include <SD.h>

/*NOTES for pinout below
CS_Controller - pin 0 - //B00000001 - Active LOW, so B0000 0000 To enable TFT CS
DC = pin 1 - //B000 0010 for data, B0000 0000 for command
Perform LOGIC OR for these values to have multiple pins set in one line of code*/
#define CS_SD 10            //This is on PORTB

#define Command B00000000   //Enable TFT MCU, setting it for command.  
#define Data B00000010      //Enable TFT MCU, setting it for data.
#define TFT_Disable B00000001 //Disable TFT

#define SD_Disable B00000100//Disable SD Card - Assign pin values to PORTD
#define SD_Enable B11110111 //Enable SD card - Assign pin values to PORTD

#define init_pin_D B00000011  //Use this value to initialize PORTD
#define init_pin_B B00000100  //Initialize pin 10 for CS_SD

File pic;
//--------------------------------------------------------------------------------display
void disp()
{
  unsigned int i; //number of pixels is too large, so unsigned number
  PORTD = Command;
  SPI.transfer(0x2C);              //command to begin writing to frame memory
  PORTD = Data;
  for (i = 0; i < 20480; i++)  //fill screen with blue pixels
  {
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0xFF);
  }
}
//------------------------------------------------------------Displaytest
void dispTest()
{
  unsigned int i; //number of pixels is too large, so unsigned number
  PORTD = Command; 
  SPI.transfer(0x2C);              //command to begin writing to frame memory
  PORTD = Data; 
  for (i = 0; i < 20480; i++)  //fill screen with blue pixels
  {
    SPI.transfer(0x00);
    SPI.transfer(0xFF);
    SPI.transfer(0x00);
  }
}

//----------------------------------------------------------------------------------PrintBMP()

void PrintBMP() {
  //Enabling SD card
  PORTB = SD_Enable; 
  PORTD = TFT_Disable;
  //Dummy reading BMP header files
  uint32_t dummy;
  //54 Bytes of garbage data from SD card header
  for (unsigned int i = 0; i < 54; i++)
  {
    dummy = pic.read();
  }

  //Command to write color data to frame memory 
  //- SD card CS needs to be disabled when using controller CS
  PORTB = SD_Disable;
  PORTD = Command; 
  SPI.transfer(0x2C); 

  //Read until there is no more data to read from BMP file
  while (pic.available())
  {
    PORTB = SD_Enable;
    PORTD = TFT_Disable; 
    uint32_t readBMP = pic.read();
    //- SD card CS needs to be disabled when using controller CS
    PORTD = Data; 
    SPI.transfer(readBMP);
  }
  pic.close();  //Closes files from SD card
  PORTB = SD_Disable;
}

//-----------------------------------------------------------------------------Setup/Initialization code
void setup()
{
  //pinMode(CS_Controller, OUTPUT);
  //pinMode(DC, OUTPUT);
  delay(100);
  pinMode(CS_SD, OUTPUT);
  DDRD = init_pin_D; //Initializing necessary pins for initialization
  DDRB = init_pin_B;
  PORTB = SD_Disable;

  SPI.begin();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  
  PORTD = Command; 
  SPI.transfer(0x11);              //exit SLEEP mode

  PORTD = Command; 
  SPI.transfer(0x28);              //display off
  
  PORTD = Command; 
  SPI.transfer(0x26);              //select gamma curve
  PORTD = Data; 
  SPI.transfer(0x04);

  PORTD = Command; 
  SPI.transfer(0xB1);              //frame rate control
  PORTD = Data; 
  SPI.transfer(0x0A);
  SPI.transfer(0x14);

  PORTD = Command; 
  SPI.transfer(0xC0);              //power control 1
  PORTD = Data; 
  SPI.transfer(0x0A);
  SPI.transfer(0x00);

  PORTD = Command; 
  SPI.transfer(0xC1);              //power control 2
  PORTD = Data; 
  SPI.transfer(0x02);

  PORTD = Command; 
  SPI.transfer(0xC5);              //VCOM control 1
  PORTD = Data; 
  SPI.transfer(0x2F);
  SPI.transfer(0x3E);

  PORTD = Command; 
  SPI.transfer(0xC7);              //VCOM control 2
  PORTD = Data; 
  SPI.transfer(0x40);

  PORTD = Command; 
  SPI.transfer(0x2A);              //column address set
  PORTD = Data; 
  SPI.transfer(0x00);
  SPI.transfer(0x00);              
  SPI.transfer(0x00);
  SPI.transfer(0x7F);              

  PORTD = Command; 
  SPI.transfer(0x2B);              //page address set
  PORTD = Data; 
  SPI.transfer(0x00);
  SPI.transfer(0x00);              //start 0x0000
  SPI.transfer(0x00);
  SPI.transfer(0x9F);              //end 0x009F

  PORTD = Command; 
  SPI.transfer(0x36);              //memory access control
  PORTD = Data; 
  SPI.transfer(0x40);

  PORTD = Command; 
  SPI.transfer(0x3A);              //pixel format = 18 bit per pixels
  PORTD = Data; 
  SPI.transfer(0x06);

  PORTD = Command; 
  SPI.transfer(0x29);              //display ON
  delay(10);
}

void loop(){
  //Showing displays
  disp();
  delay(1000);
  dispTest();
  delay(1000);

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

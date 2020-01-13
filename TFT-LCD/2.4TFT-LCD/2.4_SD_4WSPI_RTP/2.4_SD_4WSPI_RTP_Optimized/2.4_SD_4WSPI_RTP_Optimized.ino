/* General Notes: Parham K. August 2019
3.3 V logic used.  4-wire SPI. ST7789Vi controller and Arduino SPI library
Purpose: RTP testing and cycle through different SD card images
Pins notes
#define DC 0  //Data command pin
#define CS_Controller 10  //Active LOW CS_TFT. Enable: 00 0000. Disable: 00 0100 on PORT B
#define CS_SD 6 //Active LOW CS_SD. Enable: 0000 0000.  Disable: 0100 0000 on PORTD
#define RST 7  //Active LOW reset. Default 1000 0000 on PORTD*/

#include <SD.h>
#include <TFT.h>
#include <SPI.h>

//Pins LOGIC OR with each other to
#define TFT_Enable B000000 //PORTB
#define TFT_Disable B000100 //PORTB 
#define command1 B11000000  //Disables SD CS & Reset - PORTD
#define data1 B11000001 //Disables SD CS & reset - PORTD
#define SD_Enable B10000000 //Keeps Reset disable & enables SD CS - PORTD

//Resistive touch pins
#define YD A2
#define XL A3
#define YU A4
#define XR A5

#define TP 1 //Select for Touchpanel test
#define Pictures 1 //Select for cycling through images from SD card

File pic;

//////////////////////////////////////////////////////////////////////////////////////PRINT BACKGROUND
void Background() {
  //Command set tft receive data to frame memory
  PORTB = TFT_Enable;
  PORTD = command1; SPI.transfer(0x2C);

  PORTD = data1;
  //Sending 18-bit color data to frame memory
  for (unsigned long i = 0; i < 76800; i++) {
    SPI.transfer(0xFF);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
  }
}

//////////////////////////////////////////////////////////////////////////////////////Touchsceen function
void TestTouch() {
  PORTB = TFT_Enable;
  PORTD = command1; SPI.transfer(0x36);
  PORTD = data1; SPI.transfer(0xC8);   //MADCTL: memory data access control (Touch was printing pixels in wrong directions.  Had to change orientation)

  unsigned int X, Y;
  while (1) {
    //To find Y coordinate, YD tied to LOW and YU tied to HIGH
    pinMode(YU, OUTPUT);
    pinMode(YD, OUTPUT);

    //Setting a voltage range from one end to another
    digitalWrite(YD, LOW);
    digitalWrite(YU, HIGH);

    //Creating a voltage divider
    pinMode(XL, INPUT);
    pinMode(XR, INPUT);

    //Placement between XL and XR will divide by 3.3 or 5v, giving the Y value of touch point
    Y = analogRead(XL);
    Y = Y / (1024 / 320); //Input values from analog pin are from 0 to 1023

    //To find Y coordinate, XR tied to LOW and XL tied to HIGH
    pinMode(XL, OUTPUT);
    pinMode(XR, OUTPUT);

    //Setting a voltage range from one end to another
    digitalWrite(XR, LOW);
    digitalWrite(XL, HIGH);

    //Creating a voltage divider
    pinMode(YD, INPUT);
    pinMode(YU, INPUT);

    //Placement between XL and XR will divide by 3.3 or 5v, giving the Y value of touch point
    X = analogRead(YU);
    //Input values from analog pin are from 0 to 1023
    X = X / (1024 / 240); //Reads Y axis touch position

    //Setting X and Y coordinates New-----------------------------------------------
    PORTD = command1; SPI.transfer(0x2A);
    PORTD = data1;
    SPI.transfer(X >> 8);
    SPI.transfer(X);
    SPI.transfer(X >> 8);
    SPI.transfer(X);

    PORTD = command1; SPI.transfer(0x2B);
    PORTD = data1;
    SPI.transfer(Y >> 8);
    SPI.transfer(Y);
    SPI.transfer(Y >> 8);
    SPI.transfer(Y);

    //Print Black dots---------------------------------------------------------------
    PORTD = command1; SPI.transfer(0x2C);
    PORTD = data1;
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
  }
}

//////////////////////////////////////////Print BMP images
void PrintBMP() {
  PORTB = TFT_Disable;
  PORTD = SD_Enable;

  //Dummy reading BMP header files
  uint32_t dummy;
  //54 Bytes of garbage data from SD card header
  for (unsigned int i = 0; i < 54; i++)
  {
    dummy = pic.read();
  }

  //Command to write color data to frame memory
  //- SD card CS needs to be disabled when using controller CS
  PORTB = TFT_Enable;
  PORTD = command1;
  SPI.transfer(0x2C);
  PORTB = TFT_Disable;
  PORTD = SD_Enable;   //Enabling SD card

  //Read until there is no more data to read from BMP file
  while (pic.available())
  {
    uint32_t readBMP = pic.read();
    //SD card CS needs to be disabled when using controller CS
    PORTB = TFT_Enable;
    PORTD = data1;
    SPI.transfer(readBMP);
    PORTD = SD_Enable;
    PORTB = TFT_Disable;
  }
  pic.close();  //Closes files from SD card
}

void setup() {
  DDRD = 0xFF;
  DDRB = 0x3F;
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  PORTB = TFT_Enable;

  //Factory initialization
  PORTD = command1; SPI.transfer(0x28);//Turn off display
  SPI.transfer(0x11);//Exit sleep mode
  
  SPI.transfer(0x36);
  PORTD = data1; SPI.transfer(0x88);   //MADCTL: memory data access control Old: 0x88
  
  PORTD = command1; SPI.transfer(0x3A);
  PORTD = data1; SPI.transfer(0x66);   //Backup 66 //COLMOD: Interface Pixel format (18-bits per pixel @ 262K colors)
  
  PORTD = command1; SPI.transfer(0xB2);
  PORTD = data1; 
  SPI.transfer(0x0C);
  SPI.transfer(0x0C);
  SPI.transfer(0x00);
  SPI.transfer(0x33);
  SPI.transfer(0x33);   //PORCTRK: Porch setting
  
  PORTD = command1; SPI.transfer(0xB7);
  PORTD = data1; SPI.transfer(0x35);   //GCTRL: Gate Control
 
  PORTD = command1; SPI.transfer(0xBB);
  PORTD = data1; SPI.transfer(0x2B);   //VCOMS: VCOM setting
  
  PORTD = command1; SPI.transfer(0xC0);
  PORTD = data1; SPI.transfer(0x2C);   //LCMCTRL: LCM Control
  
  PORTD = command1; SPI.transfer(0xC2);
  PORTD = data1; 
  SPI.transfer(0x01);
  SPI.transfer(0xFF);   //VDVVRHEN: VDV and VRH Command Enable
  
  PORTD = command1; SPI.transfer(0xC3);
  PORTD = data1; SPI.transfer(0x11);   //VRHS: VRH set
  
  PORTD = command1; SPI.transfer(0xC4);
  PORTD = data1; SPI.transfer(0x20); //VDVS: VDV Set
  
  PORTD = command1; SPI.transfer(0xC6);
  PORTD = data1; SPI.transfer(0x0F); //FRCTRL2: Frame Rate control in normal mode
  
  PORTD = command1; SPI.transfer(0xD0);
  PORTD = data1; 
  SPI.transfer(0xA4);
  SPI.transfer(0xA1);   //PWCTRL1: Power Control 1
  
  PORTD = command1; SPI.transfer(0xE0);
  PORTD = data1; 
  SPI.transfer(0xD0);
  SPI.transfer(0x00);
  SPI.transfer(0x05);
  SPI.transfer(0x0E);
  SPI.transfer(0x15);
  SPI.transfer(0x0D);
  SPI.transfer(0x37);
  SPI.transfer(0x43);
  SPI.transfer(0x47);
  SPI.transfer(0x09);
  SPI.transfer(0x15);
  SPI.transfer(0x12);
  SPI.transfer(0x16);
  SPI.transfer(0x19); //PVGAMCTRL: Positive Voltage Gamma control
  
  PORTD = command1; SPI.transfer(0xE1);
  PORTD = data1; 
  SPI.transfer(0xD0);
  SPI.transfer(0x00);
  SPI.transfer(0x05);
  SPI.transfer(0x0D);
  SPI.transfer(0x0C);
  SPI.transfer(0x06);
  SPI.transfer(0x2D);
  SPI.transfer(0x44);
  SPI.transfer(0x40);
  SPI.transfer(0x0E);
  SPI.transfer(0x1C);
  SPI.transfer(0x18);
  SPI.transfer(0x16);
  SPI.transfer(0x19); //NVGAMCTRL: Negative Voltage Gamma control
  
  PORTD = command1; SPI.transfer(0x2A);
  PORTD = data1; 
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0xEF); //X address set
  
  PORTD = command1; SPI.transfer(0x2B);
  PORTD = data1; 
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x01);
  SPI.transfer(0x3F); //Y address set
  PORTD = command1; SPI.transfer(0x29);
}

void loop() {
  Background();

  SD.begin(6);
  if (Pictures) {
    //Printing BMP functions
    pic = SD.open("2_4-1.bmp", FILE_READ);
    PrintBMP();
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

  //Using touch screen
  Background();
  if (TP) {
    TestTouch();
  }
}

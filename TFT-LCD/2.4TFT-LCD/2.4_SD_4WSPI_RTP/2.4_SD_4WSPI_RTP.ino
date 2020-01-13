/* General Notes: Parham K. August 2019
3.3 V logic used.  4-wire SPI. ST7789Vi controller and Arduino SPI library
Purpose: RTP testing and cycle through different SD card images*/
#include <SD.h>
#include <TFT.h>
#include <SPI.h>

//Controller Pins
#define DC 0  //Data command pin
#define CS_Controller 10  //Chip-select pin for controller
#define CS_SD 6 //Chip-select for SD card

#define RST 7

//Resistive touch pins
#define YD A2
#define XL A3
#define YU A4
#define XR A5

File pic;
/////////////////////////////////////////////////////command sent function
void command(unsigned char a) {
  digitalWrite(DC, LOW);  //Setting Data/command pin to command
  digitalWrite(CS_Controller, LOW); //Enabling controller
  digitalWrite(CS_SD, HIGH);  //Disabling SD card
  SPI.transfer(a);  //Using HW/Arduino SPI to transfer data
  digitalWrite(CS_Controller, HIGH);  //'Disabling' controller
}

/////////////////////////////////////////////////////data sent function
void data(unsigned char b) {
  digitalWrite(DC, HIGH); //Setting data/command pin to data
  digitalWrite(CS_Controller, LOW); //Enabling controller
  digitalWrite(CS_SD, HIGH);  //Disabling SD card
  SPI.transfer(b);  //Using HW/Arduino SPI to transfer data
  digitalWrite(CS_Controller, HIGH); //'Disabling' controller
}

//////////////////////////////////////////////////////////////////////////////////////PRINT BACKGROUND
void Background() {
  //Command set tft receive data to frame memory
  command(0x2C);

  //Sending 18-bit color data to frame memory
  for (unsigned long i = 0; i < 76800; i++) {
    data(0xFF);
    data(0x00);
    data(0x00);
  }
}

//////////////////////////////////////////////////////////////////////////////////////Touchsceen function
void TestTouch() {
  command(0x36);
  data(0xC8);   //MADCTL: memory data access control (Touch was printing pixels in wrong directions.  Had to change orientation)
  
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
    command(0x2A);
    data(X >> 8);
    data(X);
    data(X >> 8);
    data(X);

    command(0x2B);
    data(Y >> 8);
    data(Y);
    data(Y >> 8);
    data(Y);

    //Print Black dots---------------------------------------------------------------
    command(0x2C);
    data(0x00);
    data(0x00);
    data(0x00);
  }
}


//////////////////////////////////////////Print BMP images
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
  command(0x2C);
  digitalWrite(CS_SD, LOW);   //Enabling SD card

  //Read until there is no more data to read from BMP file
  while (pic.available())
  {
    uint32_t readBMP = pic.read();
    //- SD card CS needs to be disabled when using controller CS
    digitalWrite(CS_SD, HIGH);  //disabling SD card
    data(readBMP);
    digitalWrite(CS_SD, LOW); //renables SD card
  }
  pic.close();  //Closes files from SD card
  digitalWrite(CS_SD, HIGH);  //disables SD card
}

////////////////////////////////////////Pin function initialization
void initPin() {
  //pinMode(RES, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(CS_Controller, OUTPUT);
  pinMode(CS_SD, OUTPUT);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
}

void setup() {
  initPin();
  SPI.begin();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  
  //Factory initialization
  command(0x28);//Turn off display
  command(0x11);//Exit sleep mode
  command(0x36);
  data(0x88);   //MADCTL: memory data access control Old: 0x88
  command(0x3A);
  data(0x66);   //Backup 66 //COLMOD: Interface Pixel format (18-bits per pixel @ 262K colors)
  command(0xB2);
  data(0x0C);
  data(0x0C);
  data(0x00);
  data(0x33);
  data(0x33);   //PORCTRK: Porch setting
  command(0xB7);
  data(0x35);   //GCTRL: Gate Control
  command(0xBB);
  data(0x2B);   //VCOMS: VCOM setting
  command(0xC0);
  data(0x2C);   //LCMCTRL: LCM Control
  command(0xC2);
  data(0x01);
  data(0xFF);   //VDVVRHEN: VDV and VRH Command Enable
  command(0xC3);
  data(0x11);   //VRHS: VRH set
  command(0xC4);
  data(0x20); //VDVS: VDV Set
  command(0xC6);
  data(0x0F); //FRCTRL2: Frame Rate control in normal mode
  command(0xD0);
  data(0xA4);
  data(0xA1);   //PWCTRL1: Power Control 1
  command(0xE0);
  data(0xD0);
  data(0x00);
  data(0x05);
  data(0x0E);
  data(0x15);
  data(0x0D);
  data(0x37);
  data(0x43);
  data(0x47);
  data(0x09);
  data(0x15);
  data(0x12);
  data(0x16);
  data(0x19); //PVGAMCTRL: Positive Voltage Gamma control
  command(0xE1);
  data(0xD0);
  data(0x00);
  data(0x05);
  data(0x0D);
  data(0x0C);
  data(0x06);
  data(0x2D);
  data(0x44);
  data(0x40);
  data(0x0E);
  data(0x1C);
  data(0x18);
  data(0x16);
  data(0x19); //NVGAMCTRL: Negative Voltage Gamma control
  command(0x2A);
  data(0x00);
  data(0x00);
  data(0x00);
  data(0xEF); //X address set
  command(0x2B);
  data(0x00);
  data(0x00);
  data(0x01);
  data(0x3F); //Y address set
  command(0x29);
  delay(10);
  
  while(1){}
}

void loop() {
  SD.begin(CS_SD);
  
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

  //Using touch screen
  //Background();
  //TestTouch();
}
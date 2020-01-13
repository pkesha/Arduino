/*Parham K. 
ST7789S Controller. 8-bit i80 interface.  3.3V logic.
Pupose of this is to cyle through SD card images & test RTP*/

/* PIN NOTES:
#define CS 8  //Active LOW chip select. Default:B000000
#define DC 9  //Data: 0000 1000.  Command: 000000
#define WR 10 //active LOW write.  To clock in: 000100
#define RD 11 //active LOW read. Default: 001000
#define RES 12 //active LOW reset. Default: 010000
#define CS_SD 13 //SD card chip select.  Disable: 100000*/

#include <SPI.h>
#include <SD.h>

//Controller pins
#define CS 8  //Active LOW chip select (can tie to ground)
#define DC 9  //Data is 1
#define WR 10 //active LOW write
#define RD A0 //active LOW read
#define RES A1 //active LOW reset (can tie to VCC)

//Resistive touch pins
#define YD A2
#define XL A3
#define YU A4
#define XR A5

File pic;

/////////////////////////////////////////////////////command sent function
void command(unsigned char a){
  //digitalWrite(CS, LOW);
  digitalWrite(DC, LOW); //Specifying command 
  PORTD = a;
  digitalWrite(WR, LOW); //Writing to TFT
  digitalWrite(WR, HIGH);  //Clocking it in
  //digitalWrite(CS, HIGH);
}

/////////////////////////////////////////////////////data sent function
void data(unsigned char b){           
  digitalWrite(DC, HIGH);//Specifying data
  PORTD = b;
  digitalWrite(WR, LOW); //Writing to TFT
  digitalWrite(WR, HIGH);//Clocking it in
}

/////////////////////////////////////////////////////testing if code is correct
void Background(){
    
  //Command set tft receive data to frame memory
  command(0x2C);
  //for loop to write to data to frame memory
  //This will be used to make touch background
  for(unsigned long i = 0; i < 76800; i++){
  data(0xFF);
  data(0x00);
  data(0x00);
  }
}

//////////////////////////////////////////////////////////////////////////////////////Touchsceen function
void TestTouch(){
  command(0x36);
  data(0xC8);   //MADCTL: memory data access control (Touch was printing pixels in wrong directions.  Had to change orientation)
  
  unsigned int X,Y;
  //Serial.begin(9600);
  while(1){
   //To find X coordinate, YD tied to LOW and YU tied to HIGH
   pinMode(YU,OUTPUT);
   pinMode(YD,OUTPUT);  
   digitalWrite(YD, LOW);
   digitalWrite(YU, HIGH);
   pinMode(XL,INPUT);
   //digitalWrite(XL,LOW);
   pinMode(XR, INPUT);
   Y = analogRead(XL);
   Y = Y/(1024/320);
   
//   Serial.print("Y = ");
//   Serial.print(Y, DEC);

   //Finding X
   pinMode(XL,OUTPUT);
   pinMode(XR,OUTPUT);
   digitalWrite(XR,LOW);
   digitalWrite(XL, HIGH);
   pinMode(YD,INPUT);
   pinMode(YU, INPUT);
   X = analogRead(YU);
   //Input values from analog pin are from 0 to 1023
   X = X/(1024/240); //Reads Y axis touch position
   
//   Serial.print(" X = ");
//   Serial.print(X, DEC);
//   Serial.println("");

  //Setting X and Y coordinates New-----------------------------------------------
  command(0x2A);
  data(X>>8);
  data(X);
  data(X>>8);
  data(X);
  
  command(0x2B);
  data(Y>>8);
  data(Y);
  data(Y>>8);
  data(Y);
  
  //Print Black dots---------------------------------------------------------------
  command(0x2C);
  data(0x00);
  data(0x00);
  data(0x00);
  }
}

////////////////////////////////////////Pin function initialization
void initPin(){
  //Setting pins as outputs
  DDRD = 0xFF;
  PORTD = 0x00;	//Pins for chip
  pinMode(RES, OUTPUT); 
  pinMode(CS, OUTPUT);  
  pinMode(DC, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(RD, OUTPUT);
  
  //Initialize pins
  digitalWrite(CS, LOW);
  digitalWrite(RD, HIGH);
  digitalWrite(WR, HIGH);
  digitalWrite(RES, LOW);
  delay(250);
  digitalWrite(RES, HIGH);
  delay(250);
}

//////////////////////////////////////////Print BMP images
void PrintBMP() {
  //Dummy reading BMP header files
  uint32_t dummy;
  
  //54 Bytes of garbage data from SD card header
  for(unsigned int i=0;i<54;i++)
  {
    dummy = pic.read();
  }

  //Read until there is no more data to read from BMP file
  while(pic.available())
  {
    data(pic.read());
  }
  pic.close();
}

void setup() {
  //put your setup code here, to run once:
  //Setting pins as input or output
  initPin();

  //Factory initialization
  command(0x28);//Turn off display
  command(0x11);//Exit sleep mode
  command(0x36);
  data(0x88);   //MADCTL: memory data access control
  command(0x3A);
  data(0x66);   //COLMOD: Interface Pixel format (18-bits per pixel @ 262K colors)
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
}

void loop() {
  //Writing bitmaps to display
  SD.begin(10); //SD card library is being initialized.  Pin 10 is SD CS
  command(0x2C);  //Print to frame memory
  pic = SD.open("240320_1.bmp", FILE_READ); //Open file from SD card 
  PrintBMP(); //Print SD card images
  pic = SD.open("240320_2.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("240320_3.bmp", FILE_READ);
  PrintBMP();
  pic = SD.open("240320_4.bmp", FILE_READ);
  PrintBMP();

  //Using touch screen
  Background();
  TestTouch();
}

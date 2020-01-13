#include <SPI.h>
#include <SD.h>

/*Created by Parham K.
8 bit i80 mode. 3.3 volt logic. ST7789Vi
Purpose: cycle through SD card images*/
/*PIN NOTES:
#define CS  8   //Active LOW chip TFT Chip select
#define DC  9   //Data command pin. Data: 10. 
#define WR  A1  //Active LOW write. 
#define RD  A2  //Active LOW read.  Default 0100
#define RES A3  //Active LOW reset. Default: 1000
#define CS_SD A0 //SD card chip select. Disable: 0001*/

File pic;
#define command1 B00  //Command for TFT controller, enables TFT - PORTB
#define data1 B10 //data for TFT controller, enables TFT - PORTB
#define TFT_Disable B11 //Disables TFT CS, Sets DC as data - PORTB

#define SD_Enable B1110//SD chip select enable. Default Values - PORTC
#define Write0 B1101 //Write is set low, CS_SD disable. Default values - PORTC
#define Clock_In_W B1111  //Clock in WR for TFT. Default values - PORTC


//-----------------------------------------------------------------------------------Clocking it in
void DC_out(unsigned char d)
{
  PORTC = Write0;
  PORTD = d;
  PORTC = Clock_In_W;  //Clocking it in
}

//---------------------------------------------------------------------------------Display function
void disp(){
  unsigned long int i; 
  PORTB = command1; DC_out(0x2C);              //command to begin writing to frame memory
  PORTB = data1;
  for(i=0;i<76800;i++)         //fill screen with green pixels
  {
    DC_out(0x00);
    DC_out(0xFF);
    DC_out(0x00);
  }
}

//----------------------------------------------------------------------------------PrintBMP
void PrintBMP() {
  //Enabling SD card
  PORTC = SD_Enable;
  PORTB = TFT_Disable;

  //Dummy reading BMP header files
  uint32_t dummy;
  //54 Bytes of garbage data from SD card header
  for (unsigned int i = 0; i < 54; i++)
  {
    dummy = pic.read();
  }

  //Command to write color data to frame memory 
  //- SD card CS needs to be disabled when using controller CS
  PORTB = command1; DC_out(0x2C);
  
  //PORTB = TFT_Disable;
  //PORTC = SD_Enable;   //Enabling SD car
  //Read until there is no more data to read from BMP file
  while (pic.available())
  {
    //- SD card CS needs to be disabled when using controller CS
    PORTB = data1;
    DC_out(pic.read());
    PORTB = TFT_Disable;  //Disables TFT but keeps data high
    PORTC = SD_Enable; //renables SD card
  }
  pic.close();  //Closes files from SD card
}

//----------------------------------------------------------------------------------------Setup
void setup()
{ 
  DDRD |= 0xFF;
  PORTD = 0x00;
  DDRB |= 0x3F;
  PORTB = 0x00;
  DDRC |= 0x3F;
  //while(1){PORTC = Clock_In_W;}

  //PORTB = TFT_Disable;
  PORTB = command1; DC_out(0x28);  //display off
  DC_out(0x11);  //exit SLEEP mode

  DC_out(0x36);  //MADCTL: memory data access control
  PORTB = data1;
  
  PORTB = command1; DC_out(0x3A); //COLMOD: Interface Pixel format 
  //*** 262K-colors in 18bit/pixel format when using 8-bit interface to allow 3-bytes per pixel
  DC_out(0x55);
  
  PORTB = command1; DC_out(0xB2);  //PORCTRK: Porch setting
  PORTB = data1;
  DC_out(0x0C);
  DC_out(0x0C);
  DC_out(0x00);
  DC_out(0x33);
  DC_out(0x33);
  
  PORTB = command1; DC_out(0xB7);  //GCTRL: Gate Control
  PORTB = data1;
  DC_out(0x35);
  
  PORTB = command1; DC_out(0xBB);  //VCOMS: VCOM setting
  PORTB = data1;
  DC_out(0x2B);
 
  PORTB = command1; DC_out(0xC0);  //LCMCTRL: LCM Control
  PORTB = data1;
  DC_out(0x2C);
  
  PORTB = command1; DC_out(0xC2);  //VDVVRHEN: VDV and VRH Command Enable
  PORTB = data1;
  DC_out(0x01);
  DC_out(0xFF);
  
  PORTB = command1; DC_out(0xC3);  //VRHS: VRH Set
  PORTB = data1;
  DC_out(0x11);
  
  PORTB = command1; DC_out(0xC4);  //VDVS: VDV Set
  PORTB = data1;
  DC_out(0x20);
  
  PORTB = command1; DC_out(0xC6);  //FRCTRL2: Frame Rate control in normal mode
  PORTB = data1;
  DC_out(0x0F);
  
  PORTB = command1; DC_out(0xD0);  //PWCTRL1: Power Control 1
  PORTB = data1;
  DC_out(0xA4);
  DC_out(0xA1);
  
  PORTB = command1; DC_out(0xE0);  //PVGAMCTRL: Positive Voltage Gamma control
  PORTB = data1;  
  DC_out(0xD0);
  DC_out(0x00);
  DC_out(0x05);
  DC_out(0x0E);
  DC_out(0x15);
  DC_out(0x0D);
  DC_out(0x37);
  DC_out(0x43);
  DC_out(0x47);
  DC_out(0x09);
  DC_out(0x15);
  DC_out(0x12);
  DC_out(0x16);
  DC_out(0x19);
  
  PORTB = command1; DC_out(0xE1);  //NVGAMCTRL: Negative Voltage Gamma control
  PORTB = data1;  
  DC_out(0xD0);
  DC_out(0x00);
  DC_out(0x05);
  DC_out(0x0D);
  DC_out(0x0C);
  DC_out(0x06);
  DC_out(0x2D);
  DC_out(0x44);
  DC_out(0x40);
  DC_out(0x0E);
  DC_out(0x1C);
  DC_out(0x18);
  DC_out(0x16);
  DC_out(0x19);
  
  PORTB = command1; DC_out(0x2A);  //X address set
  PORTB = data1;
  DC_out(0x00);
  DC_out(0x00);
  DC_out(0x00);
  DC_out(0xEF);

  PORTB = command1; DC_out(0x2B);  //Y address set
  PORTB = data1;
  DC_out(0x00);
  DC_out(0x00);
  DC_out(0x01);
  DC_out(0x3F);

  PORTB = command1; DC_out(0x36); //MADCTL memory data access control
  PORTB = data1; DC_out(0x88);

  PORTB = command1; DC_out(0x21); //invert display
  PORTB = command1; DC_out(0x29);  //display ON
}

void loop()
{
  disp();
  SD.begin(A0); 
  
  //Printing BMP functions
  pic = SD.open("2_4-1.bmp", FILE_READ);  //Open SD card file
  PrintBMP(); //Printing function
  delay(1000);
  pic = SD.open("2_4-2.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("2_4-3.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("2_4-4.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("2_4-5.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("2_4-6.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
  pic = SD.open("2_4-7.bmp", FILE_READ);
  PrintBMP();
  delay(1000);
}

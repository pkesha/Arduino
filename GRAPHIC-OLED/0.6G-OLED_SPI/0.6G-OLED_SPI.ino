#include <SPI.h>
/*Created by Parham K.
3.3V logic. 4 wire SPI. SSD1357
Purpose for this was to cycle through solid colors for V&V
#define CS 2	//Chip select 
#define RS 3	//Data/command pin*/

#define command B00000000
#define data B00001000

//Code for testing the display
void testdisp()
{   
  //if code is being run correctly, this function will produce a black screen
  unsigned int i; 
  
  PORTD = command;
  SPI.transfer(0x5C);              //command to begin writing to frame memory
  PORTD = data;
  for(i=0;i<4096;i++)          //fill screen with white
  {
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
  }
   delay(1000);

  for(i=0;i<4096;i++)          //fill screen with black
  {
    SPI.transfer(0x00);
    SPI.transfer(0x00);
  }
   delay(1000);
   
   for(i=0;i<4096;i++)          //fill screen with red
  {
    SPI.transfer(0x00);
    SPI.transfer(0x1F);
  }
  delay(1000);
  
  for(i=0;i<4096;i++)          //fill screen with blue
  {
    SPI.transfer(0xF8);
    SPI.transfer(0x00);
  }
  delay(1000);

  for(i=0;i<4096;i++)          //fill screen with Green
  {
    SPI.transfer(0x07);
    SPI.transfer(0xE0);
  }
  delay(1000);
  
}

void setup()
{ 
  DDRD = 0xFF;
  PORTD = 0x00;
  SPI.begin();
  
  PORTD = command;
  //Initialization Code
  SPI.transfer(0xFD); //Command Unlock
  PORTD = data;
  SPI.transfer(0x12);
 
  PORTD = command;
  SPI.transfer(0xAE); //Set Display OFF
  
  SPI.transfer(0xB3); //Set Display Clock Divide Ratio/Oscillator Frequency
  PORTD = data;
  SPI.transfer(0xB0);
  
  PORTD = command;
  SPI.transfer(0xCA); //Set MUX Ratio
  PORTD = data;
  SPI.transfer(0x3F);
  
  PORTD = command;
  SPI.transfer(0xA2); //Set Display Offset
  PORTD = data;
  SPI.transfer(0x40); 
  
  PORTD = command;
  SPI.transfer(0xA1); //Set Display Start Line
  PORTD = data;
  SPI.transfer(0x00);
  
  PORTD = command;
  SPI.transfer(0xA0); //Set Re-map & Color Depth (65K Colors)
  PORTD = data;
  SPI.transfer(0x70);
  SPI.transfer(0x00);
  
  PORTD = command;
  SPI.transfer(0xC1); //Set Contrast Current
  PORTD = data;
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  
  PORTD = command;
  SPI.transfer(0xC7); //Master Contrast Current Control
  PORTD = data;
  SPI.transfer(0x0F);
  
  PORTD = command;
  SPI.transfer(0xB1); //Set Phase Length
  PORTD = data;
  SPI.transfer(0x32);
  
  PORTD = command;
  SPI.transfer(0xB6); //Set Second Pre-charge Period
  PORTD = data;
  SPI.transfer(0x01);
  
  PORTD = command;
  SPI.transfer(0xB8); //Gamma Look-up Table
  PORTD = data;
  SPI.transfer(0x02);
  SPI.transfer(0x03);
  SPI.transfer(0x04);
  SPI.transfer(0x05);
  SPI.transfer(0x06);
  SPI.transfer(0x07);
  SPI.transfer(0x08);
  SPI.transfer(0x09);
  SPI.transfer(0x0A);
  SPI.transfer(0x0B);
  SPI.transfer(0x0C);
  SPI.transfer(0x0D);
  SPI.transfer(0x0E);
  SPI.transfer(0x0F);
  SPI.transfer(0x10);
  SPI.transfer(0x11);
  SPI.transfer(0x12);
  SPI.transfer(0x13);
  SPI.transfer(0x15);
  SPI.transfer(0x17);
  SPI.transfer(0x19);
  SPI.transfer(0x1B);
  SPI.transfer(0x1D);
  SPI.transfer(0x1F);
  SPI.transfer(0x21);
  SPI.transfer(0x23);
  SPI.transfer(0x25);
  SPI.transfer(0x27);
  SPI.transfer(0x2A);
  SPI.transfer(0x2D);
  SPI.transfer(0x30);
  SPI.transfer(0x33);
  SPI.transfer(0x36);
  SPI.transfer(0x39);
  SPI.transfer(0x3C);
  SPI.transfer(0x3F);
  SPI.transfer(0x42);
  SPI.transfer(0x45);
  SPI.transfer(0x48);
  SPI.transfer(0x4C);
  SPI.transfer(0x50);
  SPI.transfer(0x54);
  SPI.transfer(0x58);
  SPI.transfer(0x5C);
  SPI.transfer(0x60);
  SPI.transfer(0x64);
  SPI.transfer(0x68);
  SPI.transfer(0x6C);
  SPI.transfer(0x70);
  SPI.transfer(0x74);
  SPI.transfer(0x78);
  SPI.transfer(0x7D);
  SPI.transfer(0x82);
  SPI.transfer(0x87);
  SPI.transfer(0x8C);
  SPI.transfer(0x91);
  SPI.transfer(0x96);
  SPI.transfer(0x9B);
  SPI.transfer(0xA0);
  SPI.transfer(0xA5);
  SPI.transfer(0xAA);
  SPI.transfer(0xAF);
  SPI.transfer(0xB4);
  
  PORTD = command;
  SPI.transfer(0xBB); //Set Pre-charge Voltage
  PORTD = data;
  SPI.transfer(0xFF);
  
  PORTD = command;
  SPI.transfer(0xBE); //Set VCOMH
  PORTD = data;
  SPI.transfer(0x05);
  
  PORTD = command;
  SPI.transfer(0x15); //Set Column Address
  PORTD = data;
  SPI.transfer(0x20);
  SPI.transfer(0x5F);
  
  PORTD = command;
  SPI.transfer(0x75); //Set Row Address
  PORTD = data;
  SPI.transfer(0x00);
  SPI.transfer(0x3F);
  
  PORTD = command;
  SPI.transfer(0xA6); //Set Display Mode
	//oled_Clear_Screen(); //Clear Display (write all 0x00’s to display RAM)
  SPI.transfer(0xAF); //Set Display ON
	//comm_out(0x5C); //Enable Write Data into RAM
}



void loop()
{
  //Call function to print to display
  testdisp();
}

//---------------------------------------------------------
/*
NHD-3.5-240320T-ATXN-TF_uno.ino
Program for writing to Newhaven Display 3.5" Transflective TFT with ILI9341 controller
P/N: NHD-3.5-240320T-ATXN-TF

(c)2019 Michael LaVine - Newhaven Display International, Inc.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.
*/
//---------------------------------------------------------

// Seeeduino set for 3.3V mode

#define SCK_pin  7
#define MOSI_pin  6
#define MISO_pin  5
#define CS_pin  4
#define RES_pin  3

//SPI_Standard-------------------------------------------------------------------
void SPI_WriteComm(unsigned char c)
{
  unsigned char i;
  unsigned char mask = 0x80;
  
  digitalWrite(CS_pin, LOW);
  digitalWrite(SCK_pin, LOW);
  digitalWrite(MOSI_pin, LOW);
  digitalWrite(SCK_pin, HIGH);
  for(i=0;i<8;i++)
  {
     digitalWrite(SCK_pin, LOW);
     if((c & mask) >> 7 == 1)
     {
        digitalWrite(MOSI_pin, HIGH);
     }
     else
     {
        digitalWrite(MOSI_pin, LOW);
     }
     digitalWrite(SCK_pin, HIGH);
     c = c << 1;
  }
  //digitalWrite(CS_pin, HIGH);
}

void SPI_WriteData(unsigned char d)
{
  unsigned char i;
  unsigned char mask = 0x80;
  
  digitalWrite(CS_pin, LOW);
  digitalWrite(SCK_pin, LOW);
  digitalWrite(MOSI_pin, HIGH);
  digitalWrite(SCK_pin, HIGH);
  for(i=0;i<8;i++)
  {
     digitalWrite(SCK_pin, LOW);
     if((d & mask) >> 7 == 1)
     {
        digitalWrite(MOSI_pin, HIGH);
     }
     else
     {
        digitalWrite(MOSI_pin, LOW);
     }
     digitalWrite(SCK_pin, HIGH);
     d = d << 1;
  }
  //digitalWrite(CS_pin, HIGH);
}

//SPI_Fast---------------------------------------------------------
void SPI_WriteDataFast(unsigned char d)
{
  unsigned char i;
  unsigned char mask = 0x80;
  
  PORTD &= ~0x10;
  PORTD &= ~0x80;
  PORTD |= 0x40;
  PORTD |= 0x80;
  for(i=0;i<8;i++)
  {
     PORTD &= ~0x80;
     if((d & mask) >> 7 == 1)
     {
        PORTD |= 0x40;
     }
     else
     {
        PORTD &= ~0x40;
     }
     PORTD |= 0x80;
     d = d << 1;
  }
  PORTD |= 0x10;
}

//Standard_Display-----------------------------------------------------------
void disp()
{
  unsigned int i; 
  SPI_WriteComm(0x2C);         //command to begin writing to frame memory
  for(i=0;i<38400;i++)         //fill screen with red pixels
  {
      SPI_WriteData(0xF8);
      SPI_WriteData(0x00);
      SPI_WriteData(0xF8);
      SPI_WriteData(0x00);
  }
  for(i=0;i<38400;i++)         //fill screen with green pixels
  {
      SPI_WriteData(0x07);
      SPI_WriteData(0xE0);
      SPI_WriteData(0x07);
      SPI_WriteData(0xE0);
  }
  for(i=0;i<38400;i++)         //fill screen with blue pixels
  {
      SPI_WriteData(0x00);
      SPI_WriteData(0x1F);
      SPI_WriteData(0x00);
      SPI_WriteData(0x1F);
  }
}

//Display_Fast-----------------------------------------------------
void dispFast()
{
  unsigned int i; 
  SPI_WriteComm(0x2C);         //command to begin writing to frame memory
  for(i=0;i<38400;i++)         //fill screen with red pixels
  {
      SPI_WriteDataFast(0xF8);
      SPI_WriteDataFast(0x00);
      SPI_WriteDataFast(0xF8);
      SPI_WriteDataFast(0x00);
  }
  for(i=0;i<38400;i++)         //fill screen with green pixels
  {
      SPI_WriteDataFast(0x07);
      SPI_WriteDataFast(0xE0);
      SPI_WriteDataFast(0x07);
      SPI_WriteDataFast(0xE0);
  }
  for(i=0;i<38400;i++)         //fill screen with blue pixels
  {
      SPI_WriteDataFast(0x00);
      SPI_WriteDataFast(0x1F);
      SPI_WriteDataFast(0x00);
      SPI_WriteDataFast(0x1F);
  }
}


//Factory initialization----------------------------------------------------------------
void initFactory()
{
  digitalWrite(RES_pin, LOW);
  delay(10);
  digitalWrite(RES_pin, HIGH);
  delay(150);
  
   SPI_WriteComm(0xCF);     
   SPI_WriteData(0x00);    
   SPI_WriteData(0x83);
   SPI_WriteData(0x30);
  
   SPI_WriteComm(0xED);     
   SPI_WriteData(0x64);     
   SPI_WriteData(0x03);     
   SPI_WriteData(0x12);
   SPI_WriteData(0x81);

   SPI_WriteComm(0xE8);      
   SPI_WriteData(0x85);    
   SPI_WriteData(0x01);
   SPI_WriteData(0x79);

   SPI_WriteComm(0xCB);     
   SPI_WriteData(0x39);    
   SPI_WriteData(0x2C);     
   SPI_WriteData(0x00);     
   SPI_WriteData(0x34);   
   SPI_WriteData(0x06);

   SPI_WriteComm(0xF7);      
   SPI_WriteData(0x20);     
   
   SPI_WriteComm(0xEA);      
   SPI_WriteData(0x00);    
   SPI_WriteData(0x00);    
  
    SPI_WriteComm(0xC0);      
    SPI_WriteData(0x21);      

    SPI_WriteComm(0xC1);      
    SPI_WriteData(0x10); 
  
   SPI_WriteComm(0xC5);      
   SPI_WriteData(0x1F);     
   SPI_WriteData(0x27);  //0X3C   

    SPI_WriteComm(0xC7);      //Command  "3 Gamma Function"
    SPI_WriteData(0xBF); 
    SPI_WriteComm(0x36);      
    SPI_WriteData(0x80);

   SPI_WriteComm(0xB1);      
   SPI_WriteData(0x00);     
   SPI_WriteData(0x1b);

   SPI_WriteComm(0xB6);      
   SPI_WriteData(0x0A);     
   SPI_WriteData(0xA2);  

    SPI_WriteComm(0xF2);      
    SPI_WriteData(0x00); 

    SPI_WriteComm(0xE0);      
    SPI_WriteData(0x0F);     
    SPI_WriteData(0x26);  
    SPI_WriteData(0x22); 
    SPI_WriteData(0x0A);     
    SPI_WriteData(0x0B); 
    SPI_WriteData(0x05); 
    SPI_WriteData(0x4F);     
    SPI_WriteData(0xC6);  
    SPI_WriteData(0x3D); 
    SPI_WriteData(0x05);     
    SPI_WriteData(0x0D); 
    SPI_WriteData(0x00);  
    SPI_WriteData(0x13); 
    SPI_WriteData(0x0D);     
    SPI_WriteData(0x00); 
  

    SPI_WriteComm(0xE1);      
    SPI_WriteData(0x00); 
    SPI_WriteData(0x1B);     
    SPI_WriteData(0x1F);  
    SPI_WriteData(0x02); 
    SPI_WriteData(0x0E);     
    SPI_WriteData(0x02); 
    SPI_WriteData(0x38); 
    SPI_WriteData(0x33);     
    SPI_WriteData(0x51);  
    SPI_WriteData(0x06); 
    SPI_WriteData(0x14);     
    SPI_WriteData(0x0B); 
    SPI_WriteData(0x36); 
    SPI_WriteData(0x38);     
    SPI_WriteData(0x3F); 

   SPI_WriteComm(0xB0);      //Command  "Set Gamma Negative"
   SPI_WriteData(0xE0);     //Parameter '0x00"

   SPI_WriteComm(0x3A);      //Command  "Pixel Format Set"
   SPI_WriteData(0x66);     //Parameter '0x55" 16Bit per pixel//66,18bit

   SPI_WriteComm(0x11);   //sleep OUT
   delay(120);
   
  SPI_WriteComm(0x2A); 
  SPI_WriteData(0x00);                  //column address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x00);
  SPI_WriteData(0xEF);                  //end 0x00EF
  
  SPI_WriteComm(0x2B); 
  SPI_WriteData(0x00);                  //page address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x01);
  SPI_WriteData(0x3F);                  //end 0x003F
  
  SPI_WriteComm(0x29);      //Command  "Display ON" 
}


//Mikes_initialization------------------------------
void initML()
{
  digitalWrite(RES_pin, LOW);
  delay(10);
  digitalWrite(RES_pin, HIGH);
  delay(150);
  
  SPI_WriteComm(0x28);  //display off
  SPI_WriteComm(0x11);  //exit SLEEP mode

  delay(120);
  
  SPI_WriteComm(0xCF);  //power control B
  SPI_WriteData(0x00);
  SPI_WriteData(0x83);
  SPI_WriteData(0x30);

  SPI_WriteComm(0xED);     
  SPI_WriteData(0x64);     
  SPI_WriteData(0x03);     
  SPI_WriteData(0x12);
  SPI_WriteData(0x81);

  SPI_WriteComm(0xE8);      
  SPI_WriteData(0x85);    
  SPI_WriteData(0x01);
  SPI_WriteData(0x79);
  
  SPI_WriteComm(0xCB);  //power control A
  SPI_WriteData(0x39);
  SPI_WriteData(0x2C);
  SPI_WriteData(0x00);
  SPI_WriteData(0x34);
  SPI_WriteData(0x06);

  SPI_WriteComm(0xF7);      
  SPI_WriteData(0x20);

  SPI_WriteComm(0xEA);      
  SPI_WriteData(0x00);    ///// check this --- default = (0x66)
  SPI_WriteData(0x00); 

  SPI_WriteComm(0xC0); 
  SPI_WriteData(0x21);  //power control 1
  
  SPI_WriteComm(0xC1); 
  SPI_WriteData(0x10);  //power control 2
  
  SPI_WriteComm(0xC5); 
  SPI_WriteData(0x1F); 
  SPI_WriteData(0x27);  //VCOM control 1

  SPI_WriteComm(0xC7); 
  SPI_WriteData(0xBF);                  //VCOM control 2
  
  SPI_WriteComm(0x36); 
  SPI_WriteData(0x80);                  //memory access control = BGR
  
  SPI_WriteComm(0xB1); 
  SPI_WriteData(0x00); 
  SPI_WriteData(0x1B);  //frame rate control
  
  SPI_WriteComm(0xB6); 
  SPI_WriteData(0x0A); 
  SPI_WriteData(0xA2);  //display function control

  SPI_WriteComm(0xF2);                  //3g damma control
  SPI_WriteData(0x00);                  //off
  
  SPI_WriteComm(0x3A); 
  SPI_WriteData(0x55);                  //pixel format = 16 bit per pixel
  
  SPI_WriteComm(0xF2);                  //3g damma control
  SPI_WriteData(0x02);                  //off
  
  SPI_WriteComm(0x26);                  //gamma curve 3
  SPI_WriteData(0x01);
  
  SPI_WriteComm(0x2A); 
  SPI_WriteData(0x00);                  //column address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x00);
  SPI_WriteData(0xEF);                  //end 0x00EF
  
  SPI_WriteComm(0x2B); 
  SPI_WriteData(0x00);                  //page address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x01);
  SPI_WriteData(0x3F);                  //end 0x003F
  
  SPI_WriteComm(0x29);                  //display ON

  delay(10);
}


//Bare_Initialization--------------------------------------------------
void initReduced()
{
  digitalWrite(RES_pin, LOW);
  delay(10);
  digitalWrite(RES_pin, HIGH);
  delay(150);
  
  SPI_WriteComm(0x28);  //display off
  SPI_WriteComm(0x11);  //exit SLEEP mode

  delay(120);

  SPI_WriteComm(0x36); 
  SPI_WriteData(0x80);                  //memory access control = BGR

  SPI_WriteComm(0x3A); 
  SPI_WriteData(0x55);                  //pixel format = 16 bit per pixel

  SPI_WriteComm(0x2A); 
  SPI_WriteData(0x00);                  //column address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x00);
  SPI_WriteData(0xEF);                  //end 0x00EF
  
  SPI_WriteComm(0x2B); 
  SPI_WriteData(0x00);                  //page address set
  SPI_WriteData(0x00);                  //start 0x0000
  SPI_WriteData(0x01);
  SPI_WriteData(0x3F);                  //end 0x003F
  
  SPI_WriteComm(0x29);                  //display ON

  delay(10);
}

void setup()
{
  pinMode(CS_pin, OUTPUT);
  pinMode(MOSI_pin, OUTPUT);
  pinMode(SCK_pin, OUTPUT);
  pinMode(RES_pin, OUTPUT);
  pinMode(MISO_pin, OUTPUT);
  digitalWrite(CS_pin, HIGH);
  digitalWrite(SCK_pin, HIGH);
  digitalWrite(MOSI_pin, HIGH);
  digitalWrite(RES_pin, HIGH);
  delay(10);

  //Something wrong with factory initialization
  initFactory();
  //initML();
  //initReduced();
}

void loop()
{ 
  //disp();
  dispFast();
  delay(250);
}

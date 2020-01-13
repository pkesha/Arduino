/* PIN NOTES: SSD1322 Controller SPI mode.
  #define CS    2      //Active LOW chip select. Default: 0B000*0*0000
  #define RES   3      //Active LOW reset. Default: 0B00001000
  #define DC    4      //Command: 0B00000*0*00. Data: 0B00000100

  #define SDIN  5
  #define SCLK  6*/

//The addressing on the Keysight OLED is unusual.
void comm_out(unsigned char c)
{
  unsigned char i;
  for (i = 0; i < 8; i++)
  {
    //digitalWrite(SCLK, LOW);
    PORTD = B00001000;
    if ((c & 0x80) >> 7 == 1)
    {
      //digitalWrite(SDIN, HIGH);
      PORTD = B00101000;
    }
    else
    {
      //digitalWrite(SDIN, LOW);
      PORTD = B00001000;
    }
    c = c << 1;
    //digitalWrite(SCLK, HIGH);
    PORTD |= B01000000;
  }
  PORTD = B00001100;
}

void data_out(unsigned char d)
{
  unsigned char i;
  for (i = 0; i < 8; i++)
  {
    //digitalWrite(SCLK, LOW);
    PORTD = B00011000;
    if ((d & 0x80) >> 7 == 1)
    {
      //digitalWrite(SDIN, HIGH);
      PORTD = B00111000;
    }
    else
    {
      //digitalWrite(SDIN, LOW);
      PORTD = B00011000;
    }
    d = d << 1;
    PORTD |= B01000000;
  }
  PORTD = B00001100;
}

void Set_Column_Address_25664(unsigned char a, unsigned char b)
{
  comm_out(0x15);     // Set Column Address command
  data_out(a);        //   Default => 
  data_out(b);        //
}

void Set_Row_Address_25664(unsigned char a, unsigned char b)
{
  comm_out(0x75);     // Set Row Address command
  data_out(a);        //   Default => 
  data_out(b);        //   Default => 
}

void fillScreen(unsigned char pixelValue) {
  //Setting rows and columns to initial position
  Set_Column_Address_25664(0x1C, 0x5B); Set_Row_Address_25664(0x00, 0x3F); comm_out(0x5C);
  //Filling screen
  for (unsigned int i = 0; i < 63; i++) {
    for (unsigned int j = 0; j < 255; j++) {
      data_out(pixelValue);
    }
  }
}

void borders(unsigned char beginningColumnAddress, unsigned char endingColumnAddress,
             unsigned char beginningRowAddress, unsigned char endingRowAddress,
             unsigned int start, unsigned int endl, unsigned char fill1, unsigned char fill2) 
{
  //Setting row and column addresses
  Set_Column_Address_25664(beginningColumnAddress, endingColumnAddress);
  Set_Row_Address_25664(beginningRowAddress, endingRowAddress);

  //Printing to frame memory
  comm_out(0x5C);
  for (start; start < endl; start++) {
    data_out(fill1);
    data_out(fill2);
  }
  
}

void setup()
{
  DDRD |= 0xFF;

  comm_out(0xFD);
  data_out(0x12);

  comm_out(0xB3);
  data_out(0xD1);  //D0 not 91

  comm_out(0xCA);
  data_out(0x3F);

  comm_out(0xA2);
  data_out(0x00);

  comm_out(0xA1);
  data_out(0x00);

  comm_out(0xA0);
  data_out(0x14);  //14    //00  //try dif values
  data_out(0x11);  //11    //00  //same

  comm_out(0xB5);
  data_out(0x00);

  comm_out(0xAB);
  data_out(0x00);      //01 //00 //try values

  comm_out(0xB4);
  data_out(0xA0);
  data_out(0xB5);    //FD   //B5 //values

  comm_out(0xC1);
  data_out(0xFF);    //9F   //7F //poss

  comm_out(0xC7);
  data_out(0x0F);        //poss

  comm_out(0xB9);

  comm_out(0xB1);
  data_out(0x74);   //E2

  comm_out(0xD1);
  data_out(0xA2);
  data_out(0x20);        //???

  comm_out(0xBB);
  data_out(0x17);   //1F  //17

  comm_out(0xB6);
  data_out(0x08);    //?

  comm_out(0xBE);
  data_out(0x07);   //07 //?  //04

  comm_out(0xA6);

  comm_out(0xA9);

  comm_out(0xAF);
}


void loop()
{
  /*
   * Mysteries:
   * Ending values should be 256 for both top and bottom border, but it's 128
   * 
   * Printing to frame memory print 3 pixels from left to right at a time
   * ^it should be one pixel at a time
   * 
   * To print an entire column, it's 64*2, and not 64
   * 
   * Mystery solved: It writes 4 pixels at a time -> 0xFF is 4 pixels on from left to right
   */
   
  //fill screen
  fillScreen(0xFF);
  delay(8000);
  //clear screen
  fillScreen(0x00);

  //Single pixel border
  //Left border
  borders(0x1C, 0x1C, 0x00, 0x40, 0, 64, 0xF0, 0x00);
  //Right border
  borders(0x5B, 0x5B, 0x00, 0x40, 0, 64, 0x00, 0x0F);
  //bottom border
  borders(0x1C, 0x5B, 0x3F, 0x3F, 0, 64, 0xFF, 0xFF);
  //Top border
  borders(0x1C, 0x5B, 0x00, 0x00, 0, 64, 0xFF, 0xFF);
  delay(4000);

  //4-pixel border
  //Left border
  borders(0x1C, 0x1C, 0x00, 0x40, 0, 63, 0xFF, 0xFF);
  //Right border
  borders(0x5B, 0x5B, 0x00, 0x40, 0, 63, 0xFF, 0xFF);
  //bottom border
  borders(0x1C, 0x5B, 0x3C, 0x3F, 0, 64*4, 0xFF, 0xFF);
  //Top border
  borders(0x1C, 0x5B, 0x00, 0x03, 0, 64*4, 0xFF, 0xFF);
  delay(4000);
  
}

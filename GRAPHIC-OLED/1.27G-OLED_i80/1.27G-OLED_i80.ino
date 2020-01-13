/*
  3.3 V logic. SSD1351 controller: 8-bit i80
  Purpose: Cycle through solid colors for V&V of the driver breakout board
  Pin Notes:
  #define   RES 8    //Active LOW reset. Default: B0 0001
  #define   DC  9    //Data: B0 0010
  #define   CS 10    //Active LOW Chip select: Default: B0 0|0|00
  #define   WR 11    //Active LOW write 
  #define   RD 12    //Active LOW read. Default B1 0000*/

#define command0 B10001
#define data0 B10011
#define clock_in_W B11011 //Make sure data pin is set high when displaying colors

#define    RED  0x0000FF
#define  GREEN  0x00FF00
#define   BLUE  0xFF0000
#define  WHITE  0xFFFFFF
#define  BLACK  0x000000

void DC_out(unsigned char a){
  PORTD = a;
  PORTB = clock_in_W;
}

void OLED_FillScreen_128128RGB(unsigned long color)    // fill screen with a given color
{
  PORTB = command0; DC_out(0x15);
  PORTB = data0; DC_out(0x00);
  PORTB = data0; DC_out(0x7F);
  
  PORTB = command0; DC_out(0x75);
  PORTB = data0; DC_out(0x00);
  PORTB = data0; DC_out(0x7F);

  PORTB = command0; DC_out(0x5C);
  unsigned int i, j;
  for (i = 0; i < 96; i++)
  {
    for (j = 0; j < 128; j++)
    {
      PORTB = data0; DC_out(color>>16);
      PORTB = data0; DC_out(color>>8);
      PORTB = data0; DC_out(color);
    }
  }
}

void setup()                                       // for Arduino, runs first at power on
{
  DDRD = 0xFF;
  DDRB = 0x1F;
  
  PORTB = command0; DC_out(0xFD); // Command lock setting
  PORTB = data0; DC_out(0x12);    // unlock
  
  PORTB = command0; DC_out(0xFD); // Command lock setting
  PORTB = data0; DC_out(0xB1);    // unlock
  
  PORTB = command0; DC_out(0xAE);
  
  PORTB = command0; DC_out(0xB3); // clock & frequency
  PORTB = data0; DC_out(0xF1);    // clock=Diviser+1 frequency=fh
  
  PORTB = command0; DC_out(0xCA); // Duty
  PORTB = data0; DC_out(0x7F);    // OLED _END+1
  
  PORTB = command0; DC_out(0xA2); // Display offset
  PORTB = data0; DC_out(0x00);
  
  PORTB = command0; DC_out(0xA1); // Set display start line
  PORTB = data0; DC_out(0x00);    // 0x00 start line
  
  PORTB = command0; DC_out(0xA0); // Set Re-map, color depth
  PORTB = data0; DC_out(0xA0);    // 8-bit 262K
  
  PORTB = command0; DC_out(0xB5); // set GPIO
  PORTB = data0; DC_out(0x00);    // disabled
  
  PORTB = command0; DC_out(0xAB); // Function Set
  PORTB = data0; DC_out(0x01);    // 8-bit interface, internal VDD regulator
  
  PORTB = command0; DC_out(0xB4); // set VSL
  PORTB = data0; 
  DC_out(0xA0);    // external VSL
  DC_out(0xB5);
  DC_out(0x55);
  
  PORTB = command0; DC_out(0xC1); // Set contrast current for A,B,C
  PORTB = data0;
  DC_out(0xFF);    // Color A
  DC_out(0xFF);    // Color B
  DC_out(0xFF);    // Color C
  
  PORTB = command0; DC_out(0xC7); // Set master contrast
  PORTB = data0; DC_out(0x0F);    //
  
  PORTB = command0; DC_out(0xB9); // use linear grayscale LUT
  
  PORTB = command0; DC_out(0xB1); // Set pre & dis-charge
  PORTB = data0; DC_out(0x32);    // pre=1h, dis=1h
  
  PORTB = command0; DC_out(0xBB); // Set precharge voltage of color A,B,C
  PORTB = data0; DC_out(0xFF);    //
  
  PORTB = command0; DC_out(0xB2);       // display enhancement
  PORTB = data0;
  DC_out(0xa4);
  DC_out(0x00);
  DC_out(0x00);
  
  PORTB = command0; DC_out(0xB6); // precharge period
  PORTB = data0; DC_out(0x01);
  
  PORTB = command0; DC_out(0xBE); // Set VcomH
  PORTB = data0; DC_out(0x07);
  
  PORTB = command0; DC_out(0xA6); // Normal display

  PORTB = command0; DC_out(0xAF); // Display on
}

void loop()                                         // main loop, runs after "setup()"
{
  OLED_FillScreen_128128RGB(WHITE);                // fill screen with black
  delay(5000);
  OLED_FillScreen_128128RGB(BLACK);                // fill screen with black
  delay(5000);
  OLED_FillScreen_128128RGB(RED);                // fill screen with black
  delay(5000);
  OLED_FillScreen_128128RGB(BLUE);                // fill screen with black
  delay(5000);
  OLED_FillScreen_128128RGB(GREEN);                // fill screen with black
  delay(5000);
}

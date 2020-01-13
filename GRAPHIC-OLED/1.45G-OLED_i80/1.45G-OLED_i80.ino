/*Created by Parham K.
 * 3.3V logic.  SEPS525 microcontroller i80 mode
 * Purpose: V&V by cycling through solid colors.
 * Pin notes:
 * #define   RES_PIN    8    //Active LOW reset. Default: 00001
 * #define    RD_PIN    9    //Active LOW read. Default: 00010
 * #define    WR_PIN   10    //Active LOW write
 * #define    CS_PIN   11    //Active LOW chip select. Deafult: 0|0|000
 * #define    RS_PIN   12    //Data/command pin. Data: 10000
  */

#define command0 B00011
#define data0 B10011
#define clock_in_W B10111

//command function
void DC_out(unsigned char c)
{
  PORTD = c;  //Port D will output  c
  PORTB = clock_in_W;
}

void display1(){
  PORTB = command0; DC_out(0x22);
  //Sending Data to memory to print
  for (unsigned int i = 0; i < 20480; i++)
  {
    PORTB = data0; DC_out(0x1F);
    PORTB = data0; DC_out(0x1F);
    PORTB = data0; DC_out(0x1F);
  }
  delay(1000);
}

void display2() {
  //Write to memory
  PORTB = command0; DC_out(0x22);
  //Sending Data to memory to print
  for (unsigned int i = 0; i < 20480; i++)
  {
    PORTB = data0; DC_out(0xFF);
    PORTB = data0; DC_out(0x00);
    PORTB = data0; DC_out(0x00);
  }
  delay(1000);
}

void setup() {
  //Setting Pin modes
  DDRB = 0x1F;
  DDRD = 0xFF;

  // display off, analog reset
  PORTB = command0; DC_out(0x04);
  PORTB = data0; DC_out(0x01);

  // normal mode
  PORTB = command0; DC_out(0x04);
  PORTB = data0; DC_out(0x00);
 
  // display off
  PORTB = command0; DC_out(0x06);
  PORTB = data0; DC_out(0x00);

  // turn on internal oscillator using external resistor
  PORTB = command0; DC_out(0x02);
  PORTB = data0; DC_out(0x01);

  // 90 hz frame rate, divider 0
  PORTB = command0; DC_out(0x03);
  PORTB = data0; DC_out(0x30);

  // duty cycle 127
  PORTB = command0; DC_out(0x28);
  PORTB = data0; DC_out(0x7F);

  // start on line 0
  PORTB = command0; DC_out(0x29);
  PORTB = data0; DC_out(0x00);

  // rgb_if
  PORTB = command0; DC_out(0x14);
  PORTB = data0; DC_out(0x31);

  // Set Memory Write Mode
  PORTB = command0; DC_out(0x16);
  PORTB = data0; DC_out(0x76);

  // driving current r g b (uA)
  PORTB = command0; DC_out(0x10);
  PORTB = data0; DC_out(0xFF);
  PORTB = command0; DC_out(0x11);
  PORTB = data0; DC_out(0xFF);
  PORTB = command0; DC_out(0x12);
  PORTB = data0; DC_out(0xFF);

  // precharge time r g b
  PORTB = command0; DC_out(0x08);
  PORTB = data0; DC_out(0x04);
  PORTB = command0; DC_out(0x09);
  PORTB = data0; DC_out(0x05);
  PORTB = command0; DC_out(0x0A);
  PORTB = data0; DC_out(0x05);

  // precharge current r g b (uA)
  PORTB = command0; DC_out(0x0B);
  PORTB = data0; DC_out(0xFF);
  PORTB = command0; DC_out(0x0C);
  PORTB = data0; DC_out(0xFF);
  PORTB = command0; DC_out(0x0D);
  PORTB = data0; DC_out(0xFF);

  // Set Reference Voltage Controlled by External Resister
  PORTB = command0; DC_out(0x80);
  PORTB = data0; DC_out(0x00);

  // mode set
  PORTB = command0; DC_out(0x13);
  PORTB = data0; DC_out(0xA0);

  //Setting cursor position
  PORTB = command0; DC_out(0x20);
  PORTB = data0; DC_out(0x00);
  PORTB = command0; DC_out(0x21);
  PORTB = data0; DC_out(0x00);

  //Set column address
  PORTB = command0; DC_out(0x17);
  PORTB = data0; DC_out(0x00);
  PORTB = command0; DC_out(0x18);
  PORTB = data0; DC_out(159);

  //Set row address
  PORTB = command0; DC_out(0x19);
  PORTB = data0; DC_out(0);
  PORTB = command0; DC_out(0x1A);
  PORTB = data0; DC_out(127);

  // Display On
  PORTB = command0; DC_out(0x06);
  PORTB = data0; DC_out(0x01);

}

void loop()
{
  //display the displays
  display1();
  display2();
}

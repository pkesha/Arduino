/* Created by Parham K.
1.8 EF TFT model with ILI9163V Controller - 8 bit parallel interface
Purpose is to test the display by cycling through images
The 8 bit data bus is connected to PORTD of the Seeduino (Arduino Uno). 
This is in 3.3V mode*/
 
#define RST 8   //Active LOW Reset - Default: 00 0001
#define DC  9  // D/C signal connected to Arduino digital pin 10 - 00 0000 is command. 00 0010 is data.
#define WR  10  //Active LOW Read - 00 0100 is to clock in.  Always set it to 00 0000 prior
#define RD  11  // Active LOW Write - Default: 00 1000.  Only writing data to TFT controller.
#define CS	12	// Active LOW Chip-select - Default: 01 0000.  Only communication is with TFT controller

//LOGIC OR pin values for setting command and clocking it in
#define command1 B011101
#define data1 B011111
#define write_C B011001
#define write_D B011011

//----------------------------------------------------------------------------------Commands
void comm_out(unsigned char c)
{
  PORTB = command1;	//Data/command pin is for command
  PORTD = c;	//PORTD is used assigned the data
  PORTB = write_C;
}

//-------------------------------------------------------------------------------------Data
void data_out(unsigned char d)
{
  PORTB = data1;	//Data/command pin is for command
  PORTD = d;	//PortD is assigned value
  PORTB = write_D;
}

//------------------------------------------------------------------------------display
void disp_test()
{
  unsigned int i; //number of pixels is too large, so unsigned number
  comm_out(0x2C);              //command to begin writing to frame memory
  for (i = 0; i < 20480; i++)  //fill screen with red pixels
  {
    data_out(0xFF);
    data_out(0x00);
    data_out(0x00);
  }
  delay(500);
  for (i = 0; i < 20480; i++)  //fill screen with green pixels
  {
    data_out(0x00);
    data_out(0xFF);
    data_out(0x00);
  }
  delay(500);
  for (i = 0; i < 20480; i++)  //fill screen with blue pixels
  {
    data_out(0x00);
    data_out(0x00);
    data_out(0xFF);
  }
  delay(500);
  for (i = 0; i < 20480; i++)  //fill screen with black pixels
  {
    data_out(0x00);
    data_out(0x00);
    data_out(0x00);
  }
  delay(500);
  for (i = 0; i < 20480; i++)  //fill screen with white pixels
  {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
  }
  delay(500);
}

//----------------------------------------------------------------------------display2
void disp2()
{
  unsigned int i; //Number of total pixels is large, so it has to be unsigned number
  windowSet(20, 39, 20, 79);
  comm_out(0x2C);              //command to begin writing to frame memory

  digitalWrite(DC, HIGH); //Writing Data
  for (i = 0; i < 1200; i++)  //fill screen partially
  {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0x00);
  }
}


//----------------------------------------------------------------------Windows set(?)
void windowSet(unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye)
{
  comm_out(0x2A);              //column address set
  data_out(xs >> 8);
  data_out(xs);
  data_out(xe >> 8);
  data_out(xe);

  comm_out(0x2B);              //page address set
  data_out(ys >> 8);
  data_out(ys);
  data_out(ye >> 8);
  data_out(ye);
}

//-----------------------------------------------------------------------------Setup/Initialization code
void setup()
{
  DDRD = 0xFF;  //Initializeation of data pins
  PORTD = 0x00;
  DDRB = 0x3F;
  PORTB =0x00;

  comm_out(0x11);              //exit SLEEP mode
  delay(100);

  comm_out(0x28);              //display off

  comm_out(0x26);              //select gamma curve
  data_out(0x04);

  comm_out(0xB1);              //frame rate control
  data_out(0x0A);
  data_out(0x14);

  comm_out(0xC0);              //power control 1
  data_out(0x0A);
  data_out(0x00);

  comm_out(0xC1);              //power control 2
  data_out(0x02);

  comm_out(0xC5);              //VCOM control 1
  data_out(0x2F);
  data_out(0x3E);

  comm_out(0xC7);              //VCOM control 2
  data_out(0x40);

  comm_out(0x2A);              //column address set
  data_out(0x00);
  data_out(0x00);              //start 0x0000
  data_out(0x00);
  data_out(0x7F);              //end 0x007F

  comm_out(0x2B);              //page address set
  data_out(0x00);
  data_out(0x00);              //start 0x0000
  data_out(0x00);
  data_out(0x9F);              //end 0x009F

  comm_out(0x36);              //memory access control
  data_out(0xC8);

  comm_out(0x3A);              //pixel format = 18 bit per pixels
  data_out(0x06);

  comm_out(0x29);              //display ON
}

void loop()
{
  //Showing displays
  disp_test();
  disp2();
}

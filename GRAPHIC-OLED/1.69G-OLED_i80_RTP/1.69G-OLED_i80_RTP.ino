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

//TFT
#define command0 B00111      //port &= ~0x10
#define data0 B10111     //port |= 0x10
#define pull_low_w    B10011
#define pull_high_w    B10111

#define   RES_PIN    8    //Active LOW reset. Default: 00001
#define    RD_PIN    9    //Active LOW read. Default: 00010
#define    WR_PIN   10    //Active LOW write
#define    CS_PIN   11    //Active LOW chip select. Deafult: 0|0|000
#define    RS_PIN   12    //Data/command pin. Data: 10000

#define RES_BIT   0x01  //(1<<0)
#define RD_BIT    0x02  //(1<<1)
#define WR_BIT    0x04  //(1<<2)
#define CS_BIT    0x08  //(1<<3)
#define RS_BIT    0x10  //(1<<4)

//RTP Pins
#define YD A2
#define XL A3
#define YU A4
#define XR A5

//command function
void comm_out(unsigned char c)
{
  //PORTB |= 00100;
  //Need to set low and then high or vice versa
  PORTB &= ~CS_BIT;
  PORTB &= ~RS_BIT;
  PORTB &= ~WR_BIT;
  PORTD = c;  //Port D will output  c
  PORTB |= WR_BIT;
  PORTB |= CS_BIT;
  PORTB |= RS_BIT;
}

//data function
void data_out(unsigned char d)
{
  //PORTB |= 00100;
  //Need to set low and then high or vice ve
  PORTB &= ~WR_BIT;
  PORTB &= ~CS_BIT;
  PORTD = d;  //Port D will output  c
  PORTB |= WR_BIT;
  PORTB |= CS_BIT;
}

void display1(unsigned char colorHex1, unsigned char colorHex2, unsigned colorHex3){
  comm_out(0x22);
  //Sending Data to memory to print
  for (unsigned int i = 0; i < 20480; i++)
  {
    data_out(colorHex1);
    data_out(colorHex2);
    data_out(colorHex3);
  }
  delay(1000);
}

//Change command value, potentially orientation, and pixels used
void TestTouch() {
  //Check if the pixels are being placed in wrong location
  //PORTD = command0; DC_out(0x36);
  //PORTD = data0; DC_out(0xC8);   //MADCTL: memory data access control (Touch was printing pixels in wrong directions.  Had to change orientation)

  unsigned long X, Y;
  while (true) {
    //Switch YU, YD
    //Swtich XL, XR
    
    //To find Y coordinate, YD tied to LOW and YU tied to HIGH
    pinMode(YU, OUTPUT);
    pinMode(YD, OUTPUT);

    //Setting a voltage range from one end to another
    digitalWrite(YU, HIGH);
    digitalWrite(YD, LOW);

    //Creating a voltage divider
    pinMode(XL, INPUT);
    pinMode(XR, INPUT);

    //Placement between XL and XR will divide by 3.3 or 5v, giving the Y value of touch point
    Y = analogRead(XL);
    Y = Y / (1024 / 128); //Input values from analog pin are from 0 to 1023
//    Serial.print("Y is: ");
//    Serial.print(Y, HEX);
//    Serial.println(" ");
//    delay(500);

    //To find Y coordinate, XR tied to LOW and XL tied to HIGH
    pinMode(XL, OUTPUT);
    pinMode(XR, OUTPUT);

    //Setting a voltage range from one end to another
    digitalWrite(XL, LOW);
    digitalWrite(XR, HIGH);

    //Creating a voltage divider
    pinMode(YD, INPUT);
    pinMode(YU, INPUT);

    //Placement between XL and XR will divide by 3.3 or 5v, giving the Y value of touch point
    X = analogRead(YU);
    //Input values from analog pin are from 0 to 1023
    X = X / (1024 / 160); //Reads Y axis touch position
//    Serial.print("X is: ");
//    Serial.print(X, HEX);
//    Serial.println(" ");
    

    //Setting X and Y coordinates New-----------------------------------------------
    //Need to change the the command to match with OLED controller
    comm_out(0x20); 
    data_out(X);
    //DC_out(X >> 8);
    //DC_out(X);
    //DC_out(X >> 8);
    //DC_out(X);
    
    comm_out(0x21);
    //DC_out(Y >> 8);
    //DC_out(Y);
    data_out(Y);
    //DC_out(Y >> 8);
    //DC_out(Y);

    //Print Black dots---------------------------------------------------------------
    comm_out(0x22);
    data_out(0x00);
    data_out(0x00);
    data_out(0xFF);
  }
}

void setup() {
  //Setting Pin modes
  DDRB |= 0x1F;
  DDRD |= 0xFF;
  //Serial.begin(9600);

  digitalWrite(RES_PIN, HIGH);
  digitalWrite(RD_PIN, HIGH);
  digitalWrite(WR_PIN, HIGH);
  digitalWrite(CS_PIN, HIGH);
  digitalWrite(RS_PIN, LOW);
  
  //display off, analog reset
  comm_out(0x04);
  data_out(0x01);

  //normal mode
  comm_out(0x04);
  data_out(0x00);
 
  //display off
  comm_out(0x06);
  data_out(0x00);

  //turn on internal oscillator using external resistor
  comm_out(0x02);
  data_out(0x01);

  //90 hz frame rate, divider 0
  comm_out(0x03);
  data_out(0x30);

  //duty cycle 127
  comm_out(0x28);
  data_out(0x7F);

  //start on line 0
  comm_out(0x29);
  data_out(0x00);

  //rgb_if
  comm_out(0x14);
  data_out(0x31);

  //Set Memory Write Mode
  comm_out(0x16);
  data_out(0x76);

  //driving current r g b (uA)
  comm_out(0x10);
  data_out(0xFF);
  comm_out(0x11);
  data_out(0xFF);
  comm_out(0x12);
  data_out(0xFF);

  //precharge time r g b
  comm_out(0x08);
  data_out(0x04);
  comm_out(0x09);
  data_out(0x05);
  comm_out(0x0A);
  data_out(0x05);

  //precharge current r g b (uA)
  comm_out(0x0B);
  data_out(0xFF);
  comm_out(0x0C);
  data_out(0xFF);
  comm_out(0x0D);
  data_out(0xFF);

  //Set Reference Voltage Controlled by External Resister
  comm_out(0x80);
  data_out(0x00);

  //mode set
  comm_out(0x13);
  data_out(0xA0);

  //Setting cursor position
  comm_out(0x20);
  data_out(0x00);
  comm_out(0x21);
  data_out(0x00);

  //Set column address
  comm_out(0x17);
  data_out(0x00);
  comm_out(0x18);
  data_out(159);

  //Set row address
  comm_out(0x19);
  data_out(0);
  comm_out(0x1A);
  data_out(127);

  //Display On
  comm_out(0x06);
  data_out(0x01);
}

void loop()
{
  //display the displays
  display1(0xFF, 0x00, 0x00);
  delay(1000);
  display1(0x00, 0xFF, 0x00);
  delay(1000);
  display1(0x00, 0x00, 0xFF);
  delay(1000);
  display1(0x00, 0x00, 0x00);
  delay(1000);
  display1(0xFF, 0xFF, 0xFF);
  display1(0x00, 0x00, 0x00);
  delay(1000);
  TestTouch();
}

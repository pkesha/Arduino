#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
//#include <SD.h>

//Print JPG function
void printJPG(){
  GD.Clear(); //clear screen
  GD.Begin(BITMAPS);  //Begin loading JPGs from SD card
  GD.Vertex2ii(0, 0, 0); //Position set
  GD.swap();  //Show the imgae
  delay(5000);
}

void setup()
{
  //Initialization code for the 4.3 TFT
  delay(10);
  GD.begin();
  delay(10);
  GD.wr16(REG_HCYCLE, 928);  //orig 928
  GD.wr16(REG_HOFFSET, 88); //Orig: 88. offsets horizontal view. IC Pin diplay starts at 88 for horizontal dimension
  GD.wr16(REG_HSIZE, 800); //Pixel dimension
  GD.wr16(REG_HSYNC0, 0); //Orig: 0 Shifts the display over to the left
  GD.wr16(REG_HSYNC1, 48); //orig: 48
  GD.wr16(REG_VCYCLE, 525); //Orig 525.  When changing this value, screen flickers.
  GD.wr16(REG_VOFFSET, 32); //Orig: 32. //Orig: 32. offsets verital view to origin. IC Pin diplay starts at 88 for horizontal dimension
  GD.wr16(REG_VSIZE, 480); //Pixel dimension
  GD.wr16(REG_VSYNC0, 0); //Orginal 0 //Shifts the display up
  GD.wr16(REG_VSYNC1, 3); //Orginal 3
  GD.wr16(REG_CSPREAD, 0); //Original 0. 
  GD.wr16(REG_DITHER, 1);  //Dithering is enabled
  GD.wr16(REG_PCLK_POL, 1);  //Orig: 0. Best value: 1.  Blue line shown when zero. 'Almost nonexistant
  GD.wr16(REG_PCLK, 2); //Orig: 4.  Fixes screen flickering issue
}

void loop(){
  //Make sure to calibrate a CTP and store the info on Arduino EEPROM
  GD.cmd_loadimage(0, 0); //Set-up for loading images from SD card
  GD.load("wga1.png"); //Designate specific jpg to load
  printJPG(); 

  GD.cmd_loadimage(0, 0);
  GD.load("wga2.png");
  printJPG();

  GD.cmd_loadimage(0, 0);
  GD.load("wga3.png");
  printJPG();

  GD.cmd_loadimage(0, 0);
  GD.load("wga4.png");
  printJPG();

  GD.cmd_loadimage(0, 0);
  GD.load("wga5.png");
  printJPG();

  GD.cmd_loadimage(0, 0);
  GD.load("wga6.png");
  printJPG();

  GD.cmd_loadimage(0, 0);
  GD.load("wga7.png");
  printJPG();
}

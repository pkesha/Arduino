//Defining colors
# define DARK_GREEN 0x007000
# define LIGHT_GREEN 0x33ff33
# define DARK_RED 0x700000
# define LIGHT_RED 0xff3333
# define DARK_YELLOW 0x707000
# define LIGHT_YELLOW 0xffff33
# define DARK_BLUE 0x007070
# define LIGHT_BLUE 0x33ffff

void drawscreen (int pressed)
{
  
  GD.get_inputs();  //Get's touch point tags
  GD.Clear();

  GD.PointSize(16 * 60);  //60-pixel points
  GD.Begin(POINTS);
  
  GD.Tag(1);
  //Depending on the input, it will either be light or dark
  if (pressed == 1)
  {GD.ColorRGB(LIGHT_GREEN);}
  else{GD.ColorRGB(DARK_GREEN);}
  GD.Vertix2ii(240 - 70, 136 - 70); //Position of the circle

  GD.Tag(2);
  if (pressed == 2)
  {GD.ColorRGB(LIGHT_RED);}
  else{GD.ColorRGB(DARK_RED);}
  GD.Vertix2ii(240 + 70, 136 - 70);

  GD.Tag(3);
  if (pressed == 3)
  {GD.ColorRGB(LIGHT_YELLOW);}
  else{GD.ColorRGB(DARK_YELLOW);}
  GD.Vertix2ii(240 - 70, 136 + 70); 

  GD.Tag(4);
  if (pressed == 4)
  {GD.ColorRGB(LIGHT_BLUE);}
  else{GD.ColorRGB(DARK_BLUE);}
  GD.Vertix2ii(240 + 70, 136 + 70);

  GD.swap();
}

void play(int pressed)
{
  byte note[] = {0, 52, 69, 61, 64};
  GD.play(BELL, note[pressed]);
  for (int i = 0; i < 30; i++)
  {
    drawscreen(pressed);
  }

  for (int i = 0; i < 15; i++)
  {
    drawscreen(0);
  }

  if ((
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

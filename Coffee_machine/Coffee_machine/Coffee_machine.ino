/*
 *  Reference: The Gameduino 2 Tutorial, Reference and Cookbook James Bowman 
 *  Newhaven Display NHD-4.3 -480272FT-CSXV-CTP
 *  NHD-FT81x-SHIELD REV 1.0
 */
#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define ESPRESSO 1     /*TAG names to their value number assigned*/
#define CAPPUCCHINO 2
#define HOT_CHOCOLATE 3
#define LATTE 4
#define INSTANT_TEA 5
#define HOT_WATER 6
#define SIZE_BUTTON 7
#define SETTINGS_BUTTON 8
#define START_BUTTON 9
#define FOUR_OZ     10
#define EIGHT_OZ    11
#define TWELVE_OZ   12
#define BACK_BUTTON   13
#define BRIGHTNESS   14
#define VOLUME   15
#define COFFEE_TIME   16
#define CHANGE_TIME   17
#define CONNECT_TO_PHONE   18
#define BACK_BUTTON 19
#define BACK_BUTTON2 20
#define w 148   /* width of buttons from page one*/
#define h 50    /* height of buttons from page one*/
#define Zero     100  /* Buttons Zero - Minutes. Located on Settings->Change Time. "page_five()"*/
#define One      101
#define Two      102
#define Three    103
#define Four     104
#define Five     105
#define Six      106
#define Seven    107 
#define Eight    108 
#define Nine     109
#define AM       110
#define PM       111
#define Hours     112
#define Minutes   113
#define CUP60 60    /* TAG name and TAG number of image cup60.png, smallest cup size in the "page_two()" about 60 pixels wide*/
#define CUP80 80    /* TAG name and TAG number of image cup80.png, medium cup size in the "page_two()" about 80 pixels wide*/
#define CUP100 90   /* TAG name and TAG number of image cup100.png, largest cup size in the "page_two()" about 100 pixels wide*/
char label[20];     /* label has an input of 20 characters, used to send strings within functions*/
int x0 = 6, x1 = 166, x2 = 326;          /* x-cordinate for "page_one()" buttons there is a total of 3 columns*/
int y0 = 50, y1 = 120, y2 = 190,y3 =260; /* y0,y1 y-cordinates are used for "page_one()", the y2 and y3 are used for the keypad located on Settings-> Change Time for the last two rows*/
static int pressed = 0;        /* "page_one()" button pressed tracker for the rectangle buttons(6 in total)*/
static int size_pressed = 0;   /* "page_two()" button pressed tracker for the rectangle buttons(3 in total)*/
static int start_state = 0;    /* "page_one()" button pressed tracker for the circle button(1 in total, buttom right)*/
static int pressed_AM_PM=0;    /* "page_change_time()" button pressed tracker for the rectangle button(1 in total)*/
static int pressed_HM=0;       /* "page_change_time()" button pressed tracker for the rectangle button(1 in total)*/


/* 
 *  "button(int x, int y, char label[], int TAG)" is used for "page_one()" This function is used to make new rectangle buttons. This functions accepts the x & y cordinates to place the new button,the
 * title of the button to display as output, and takes in the TAG value that it will be assigned to that button
 * Creating a button:  To create a button we must use the library GD. This library includes the function GD.Begin(RECTS). This function MUST be called before any cordinates are placed.
 * The color of the rectagle to be drawn is to be set using the fucntion GD.ColorRGB(0x######).This "button" function takes x & y coordinates, this function is called twice and must be given 
 * opposite corners of the rectangle to be drawn, the order of the two corners do not matter.
 * To make a button appearance 5 rectangles were created with different color shades.
 * This function also includes a function from the library called GD.Tag() and GD.cmd_track(x,y,w,h,name_of_tag). These functions must be called in this order in order to create a tag.
 * After calling the functions the first rectagle created will be the area size of the TAG. The TAG must be a number. The tags in this code were initialized at the beginning. 
 * For example the tag of the top left rectangle on page_one() is 1 initialized as  ESPRESSO: #define ESPRESSO 1
 * Each button also has a text to show when displayed which is given as label to the the function GD.cmd_text(x,y,font size, OPT_CENTER, "TEXT TO DISPLAY").
 * After this function is called one button is created.
 */
static void button(int x, int y, char label[], int TAG){     /*rectangle button creator*/
      GD.Begin(RECTS);  
      GD.Tag(TAG); GD.cmd_track(x,y,w,h,TAG);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);     GD.Vertex2ii(x+w, y+h);
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+w-1, y+h-1);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+w-3, y+h-3);                
      GD.ColorRGB(0x804518); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+w-5, y+h-5);  
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+w-7, y+h-7);
      GD.ColorRGB(0xffffff); 
      GD.cmd_text(x+w/2, y+h/2, 27, OPT_CENTER, label);     
}


/* 
 *  "button_pressed(int x, int y, char label[], int TAG)" , this funtions does the same thing as function "button(int x, int y, char label[], int TAG)" except that it changes the color of 
 *  the button to green when pressed, and calleds title,buttons,time,date,temp,and load_circles(). This function is called in "void loop() and page_one()" When a button from "page_one" is pressed the 
 *  background is rewritten whe GD.Vertex2ii(0,0) is called. The background got saved to that value, the placement of the background can change based on where the user decides to place it. 
 *  Then an IF statement is made to decide if it is now acceptable to start making the drink.The "pressed" variable is static and keeps track of the beverage button being chosen. The "size_pressed" static 
 *  variable is used to keep track of the size of the drink. When the  "start_state" static variale is set to one,  this lets the START button to turn green in font after the function "load_circles()" is called.
 *  In order to make a drink a chosen beverage must be made and a size must be choosen. After these two requierements are met, the "Start" button on the bottom right of "page_one()" is then turned 
 *  green, allowing the user to continue to the next step. ALso, if a size option has already been selected, this function will output to the "size" button (bottom left) that size that was chosen. This 
 *  is done by setting the color to green using GD.ColorRGB(0x679713) and writing to the output 4 oz, 8 oz, or 12 oz. The function was called twice so that two font could be used, a bigger font for the number 
 *  and a smaller font for the "oz". This function ends calling the function "GD.swap()", this function must always be called when drawing is completed. 
 *  If this function is not called after everrything was drawn no output will be displayed.
 */
static void button_pressed(int x, int y, char label[], int TAG){     /*detecting a rectangle button pressed,green pressed button creator */
      GD.Begin(BITMAPS);
      GD.Vertex2ii(0,0);
      if((pressed < 7) && (pressed > 0) && (size_pressed > 0) && (size_pressed < 4)){
        start_state=1;load_circles();
      }
      load_title("CHOOSE YOUR BEVERAGE");
      load_buttons();
      load_time();
      load_date();
      load_temp();load_circles();
      GD.Begin(RECTS);  
      GD.Tag(TAG); GD.cmd_track(x,y,w,h,TAG);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);     GD.Vertex2ii(x+w, y+h);
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+w-1, y+h-1);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+w-3, y+h-3);                
      GD.ColorRGB(0x567919); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+w-5, y+h-5);  
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+w-7, y+h-7);
      GD.ColorA(255); GD.ColorRGB(0xffffff);  
      GD.cmd_text(x+w/2, y+h/2, 27, OPT_CENTER, label);
      if(size_pressed==1){
        GD.ColorRGB(0x679713);   
        GD.cmd_text(215,245, 29, OPT_CENTER, "4"); 
        GD.cmd_text(232,245, 20, OPT_CENTER, "oz"); 
        }
      if(size_pressed==2){
        GD.ColorRGB(0x679713);  
        GD.cmd_text(215,245, 30, OPT_CENTER, "8"); 
        GD.cmd_text(232,245, 20, OPT_CENTER, "oz"); 
        }
      if(size_pressed==3){
        GD.ColorRGB(0x679713);  
        GD.cmd_text(210,245, 29, OPT_CENTER, "12");
        GD.cmd_text(235,245, 20, OPT_CENTER, "oz");  
        }
      GD.swap();
}


/* 
 *  "circle_button(int x,int y,char label[],int TAG,uint32_t color)" is used for "page_one()" This function is used to make new circle buttons. This functions accepts the x & y cordinates to place 
 *  the new button,the title of the button to display as output, the TAG value that it will be assigned to that button, and a color value to be used forthe font.
 * Creating a circle button:  To create a circle button we must use the library GD. This library includes the function GD.Begin(POINTS). This function MUST be called before any parameters 
 * are placed.Then A tag is created using GD.Tag(#) function.Explainedin "button()" function. The GD.cmd_track(x,y,w,h,TAG_value) is then called to set a parameter to detect if the button was pressed or not.
 * The color of the circle to be drawn is to be set using the function GD.ColorRGB(0x######).The "circle_button" function takes x & y coordinates.  Which will be sent to the function GD.Vertex2ii(), the center of the circle.
 * To make a button appearance 5 circles were created with different color shades.
 * This function also includes a function from the library called GD.Tag() and GD.cmd_track(x,y,w,h,name_of_tag). These functions must be called in this order in order to create a tag.
 * After calling the functions the first circle created will be the area size of the TAG. The TAG must be a number. The tags in this code were initialized at the beginning. 
 * For example the tag of the top left rectangle on page_one() is 2 initialized as  CAPPUCCHINO : #define CAPPUCCHINO 2
 * Each button also has a text to show when displayed which is given as label to the the function GD.cmd_text(x,y,font size, OPT_CENTER, "TEXT TO DISPLAY"). Before the GD.cmd_text() function is called, 
 * the circle_button() function input a color number, which wil be used to first used to set the color of the text.
 * The reason this function does not have a GD.swap() at the end of drawing is because "circle_button(int x,int y,char label[],int TAG,uint32_t color)" is called by another function which at the end calls GD.swap()
 * After this function is called one circle button is created.
 */
static void circle_button(int x,int y,char label[],int TAG,uint32_t color){  /*circle button creator*/  
      GD.Begin(POINTS);  
      GD.Tag(TAG); GD.cmd_track(x,y,41,41,TAG);
      GD.PointSize(16 * 42); GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);
      GD.PointSize(16 * 41); GD.ColorRGB(0x6A3914); GD.Vertex2ii(x, y);
      GD.PointSize(16 * 39); GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);
      GD.PointSize(16 * 37); GD.ColorRGB(0x804518); GD.Vertex2ii(x, y);
      GD.PointSize(16 * 35); GD.ColorRGB(0x6A3914); GD.Vertex2ii(x, y);
      GD.ColorRGB(color);   
      GD.cmd_text(x, y, 27, OPT_CENTER, label);
}


/*
 * The function "setting_size(int x,int y, char label[], int TAG, int new_W, int new_h)" does the same thing as function "button(int x, int y, char label[], int TAG)", except that it has two 
 * more inputs, new_W and new_h. These two inputs are used to change the size of the rectagle.  The user is able to changle the size of the rectangle to their liking. new_w is subtracted 
 * by 80 to ajust to the liking of this project. This function creates on button at a time. After this function is called throughout the code, it creates 5 rows in the settings page called "page_three()"
 * In this page I used the function "GD.ColorA(#)" to set the transperancy a little higher. This makes the background a little more visible.
 * The GD.ColorRGB function is called once again to set the color to white for the text to display. The reason this function does not have a GD.swap() at the end of drawing is 
 * because "settings_size(int x, int y, char label[], int TAG,int new_w,int new_h)" is called by another function which at the end calls GD.swap()
 */
static void settings_size(int x, int y, char label[], int TAG,int new_w,int new_h){    /*settings button creator*/ 
      GD.Begin(RECTS);  
      new_w=new_w-80;
      GD.Tag(TAG); 
      GD.cmd_track(x,y,new_w,new_h,TAG);
      GD.ColorRGB(0x000000); GD.ColorA(80); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+new_w, y + new_h+10);
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+new_w-1, y+new_h-1+10);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+new_w-3, y+new_h-3+10);                
      GD.ColorRGB(0x804518); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+new_w-5, y+new_h-5+10);  
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+new_w-7, y+new_h-7+10);
      GD.ColorRGB(0xffffff); 
      GD.ColorA(255);
      GD.cmd_text(x+new_w/2, y+new_h/2+5, 27, OPT_CENTER, label);
}


/*
 * The function "setting_size(int x,int y, char label[], int TAG, int new_W, int new_h)" does the same thing as function " drink_size_button_pressed(int x, int y, char label[], int TAG,int new_h)", except 
 * that it only has one more input, new_h and changes the color of the button to green. This input is used to change the height of the rectagle.  The user is able to changle the size of the rectangle to 
 * there liking. This function creates a button at a time. After this function is called throughout the code, it creates 3 columns in the drink_size page called "page_two()". In this page 
 * The function "GD.ColorA(#)" was used to set the transperancy a little higher. This makes the background a more visible. The GD.ColorA function is called once again to set the color to white for 
 * the text to display.  This function also cals sets the Tag and loads the tile,back_button,drink_size_button, and call_cup_images. The call_cup_images() functions loads the images onto page_two to 
 * display the cups in the button for visuala aid. Itis important to call GD.swap every time a drawing has been completed.  The reason this function does not have a GD.swap() at the end of drawing is 
 * because "drink_size_button_pressed(int x, int y, char label[], int TAG,int new_h)" is called by another function which at the end calls GD.swap()
 */
static void drink_size_button_pressed(int x, int y, char label[], int TAG,int new_h){     /*rectangle button*/
      GD.Begin(BITMAPS);GD.Vertex2ii(0,0);
      load_title("CHOOSE THE SIZE!");
      load_back_button();
      load_drink_size_button();
      GD.Begin(RECTS);  
      GD.Tag(TAG); GD.cmd_track(x,y,w,new_h,TAG); 
      GD.ColorRGB(0x000000); GD.ColorA(80); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+w, y + new_h+10);
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+w-1, y+new_h-1+10);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+w-3, y+new_h-3+10);                
      GD.ColorRGB(0x567919); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+w-5, y+new_h-5+10);  
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+w-7, y+new_h-7+10);
      GD.ColorA(255); 
      GD.ColorRGB(0xffffff);  
      GD.cmd_text(x+w/2, y+new_h-10, 27, OPT_CENTER, label);
      call_cup_images();  
      GD.swap();
}


/*
 * The function "setting_size(int x,int y, char label[], int TAG, int new_W, int new_h)" does the same thing as function " drink_size_button(int x, int y, char label[], int TAG,int new_h)", except 
 * that it only has one more input, new_h. This input is used to change the height of the rectagle.  The user is able to changle the size of the rectangle to 
 * their liking. This function creates a button at a time. After this function is called throughout the code, it creates 3 columns in the drink_size page called "page_two()". In this page 
 * The function "GD.ColorA(#)" was used to set the transperancy a little higher. This makes the background a more visible. The GD.ColorA function is called once again to set the color to white for 
 * the text to display.  This function also cals sets the Tag. After this function is called it creates a button long rectagle look.  The reason this function does not have a GD.swap() at the end of drawing is 
 * because "drink_size_button(int x, int y, char label[], int TAG,int new_h)" is called by another function which at the end calls GD.swap().
 */
static void drink_size_button(int x, int y, char label[], int TAG,int new_h){    /*3 big rectangles with different drink size*/ 
      GD.Begin(RECTS);                
      GD.Tag(TAG); GD.cmd_track(x,y,w,new_h,TAG); 
      GD.ColorRGB(0x000000); GD.ColorA(80); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+w, y + new_h+10);
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+w-1, y+new_h-1+10);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+w-3, y+new_h-3+10);                
      GD.ColorRGB(0x804518); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+w-5, y+new_h-5+10);  
      GD.ColorRGB(0x6A3914); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+w-7, y+new_h-7+10);
      GD.ColorA(255); GD.ColorRGB(0xffffff);   GD.cmd_text(x+w/2, y+new_h-10, 27, OPT_CENTER, label);     
}


/*
 * The function "load_drink_size_button()" calls the function "drink_size_button(int x, int y, char label[], int TAG,int new_h)" three times, resulting in three long columns. 
 * It sends the x & y coordinates, the title for the button, the Tag value and a new changeble height. This function also calls the funtion call_cup_images() whih will load the images to the buttons location.
 * The reason this function does not have a GD.swap() at the end of drawing is because "load_drink_size_button()" is called by another function which at the end calls GD.swap().
 */
static void load_drink_size_button(){
      drink_size_button(x0, y0+40,"4 oz",FOUR_OZ, h*3); 
      drink_size_button(x1, y0+40,"8 oz",EIGHT_OZ, h*3);
      drink_size_button(x2, y0+40,"12 oz",TWELVE_OZ,h*3);  
      call_cup_images(); 
}


/*
 * The function "load_cup_button()" loads the images from the SD card and saves the each individually  using the function  GD.BitmapHandle(),generated from the library. When loading an 
 * image GD.Begin(RECTS) must be called. GD.ColorRGB() is called to set the color. GD,BitmapHandle(#) is then called followed by GD.cmd_loadimage(-#,0); It is important to not 
 * forget the (-) sign for the first parameter and make sure that both numbers match  as showen in the function below. This sets the image to a certain BitmapHandle number. 
 * GD.load("image.xyz") is then loaded into memory and saved. Now to display the image GD.Begin(BITMAPS) must be called first, followed by the GD.Vertex(x,y,BitmapHandle_number),
 * If GD.Vertex2ii(x,y) is called, the background will be called since that was not saved specifically saved with a BitmapHandle number. The "GD.Vertex(x,y,BitmapHandle_number)" can just be called when 
 * it is needed. In the case below, it is not needed at the moment but is implemented to show the formart to display an image using BitmapHandle .
 */
static void load_cup_button (){/*loads the background*/
    GD.Begin(RECTS); 
    GD.ColorRGB(0xffffff); GD.BitmapHandle(2);GD.cmd_loadimage(-2, 0);GD.load("cup60.png");GD.Begin(BITMAPS);GD.Vertex2ii(55,135, 2);
    GD.ColorRGB(0xffffff); GD.BitmapHandle(3);GD.cmd_loadimage(-3, 0);GD.load("cup80.png");GD.Begin(BITMAPS);GD.Vertex2ii(210,115, 3);            
    GD.ColorRGB(0xffffff); GD.BitmapHandle(4);GD.cmd_loadimage(-4, 0);GD.load("cup100.png");GD.Begin(BITMAPS);GD.Vertex2ii(360,90, 4);
}



/*
 * The function "call_cup_images()" displays cup60,cup80,cup100 images imported. The GD.Tag(tag_name) and GD.cmd_track(x,y,w,h,tag_name) functions are used to keep track 
 * of were the image is incase a user input presses the cup instead of the button. Using a OR statement allows two parameters to be set to detect a certain area. If no
 * Tag were to be placed then the cup image will overwrite anything that was there before and it would not be able to detect if a certain area was pressed. The if statement
 * using this logic is described in the while loop section. The reason this function does not have a GD.swap() at the end of drawing is 
 * because "call_cup_images()" is called by another function which at the end calls GD.swap().
 */
static void call_cup_images (){/*loads the background*/
  GD.Tag(CUP60); GD.cmd_track(60,120,30,30,CUP60);//MAKE THE TAG FIRST THEN CALL THE IMAGE
  GD.Vertex2ii(55,135, 2); 
  GD.Tag(CUP80); GD.cmd_track(200,100,40,40,CUP80);
  GD.Vertex2ii(210,115, 3); 
  GD.Tag(CUP100); GD.cmd_track(350,80,50,50,CUP100);
  GD.Vertex2ii(360,90, 4);   
}



/*
 * The function "load_settings()" calls the function "settings_size(int x, int y, char label[], int TAG,int new_w,int new_h)" and sends the parameteres needed for placement and display.
 * By calling this function, it creates 5 buttons for "page_three()" Where it displays  the buttons BRIGHTNESS,VOLUME,COFFEE TIME, CHAGE TIME, and CONNECT TO PHONE, these pages don't work 
 * but are placed there to show an example. The CHANGE TIME  button when pressed will lead the user to adiffrent page where a keypad will show with Hours,Minutes, AM, and PM  options are chosen.
 * Pressing these buttons will NOT  change the time in the main page ("page_one()"), it is just displayed as an example.The reason this function does not have a GD.swap() at the end of drawing is 
 * because "load_settings()" is called by another function which at the end calls GD.swap().
 */
static void load_settings(){
      settings_size(x0+120, y0+10, "BRIGHTNESS",BRIGHTNESS,w*2,25); 
      settings_size(x0+120, y0+50, "VOLUME",VOLUME,w*2,25);
      settings_size(x0+120, y0+90, "COFFEE TIME",COFFEE_TIME,w*2,25); 
      settings_size(x0+120, y0+130, "CHANGE TIME",CHANGE_TIME,w*2,25); 
      settings_size(x0+120, y0+170, "CONNECT TO PHONE",CONNECT_TO_PHONE,w*2,25); 
}




/*
 * The function "load_buttons()"  creates six working buttons to be displayed in "page_one()". Recall that the function "button(int x, int y, char label[], int TAG)" is the function that creates 
 * one buttons after making 5 rectagnels with different shades and initializes a TAG value to that section, this as the same thing as "load_settings()" is used to simplify the code. Throughout 
 * the code this function will be called  many times.The reason this function does not have a GD.swap() at the end of drawing is  because "load_buttons()" is called by another function 
 * which at the end calls GD.swap().
 */
static void load_buttons(){/*loads all the rectangle buttons into the function buttons where pararameters are transfer to make each with its own ,placemet,title,and tag number*/
      button(x0, y0, "ESPRESSO",ESPRESSO); 
      button(x1, y0, "CAPPUCCHINO",CAPPUCCHINO);
      button(x2, y0, "HOT CHOCOLATE",HOT_CHOCOLATE); 
      button(x0, y1, "LATTE",LATTE); 
      button(x1, y1, "INSTANT TEA",INSTANT_TEA);
      button(x2, y1, "HOT WATER",HOT_WATER);
}



/*
 * The function "load_circles()"  creates threee working buttons to be displayed in "page_one()". Recall that the function "circle_button(int x, int y, char label[], int TAG,int color)" is the function that creates 
 * one buttons after making 5 circles with different shades and initializes a TAG value to that section, this as the same thing as "load_settings()" is used to simplify the code. Throughout 
 * the code this function will be called  many times. The if statment is to check if the start_state has changed to one. Recall that this varible checks if a drink option and a size have been choosen. If true 
 * then it will proceed to change the color of the text displayed as the name of the button to green.Until othrwise the text will remain white.The reason this function does not have a GD.swap() at the end of drawing 
 * is  because "load_circles()" is called by another function  which at the end calls GD.swap().
 */
static void load_circles (){
      circle_button(420,225,"START",START_BUTTON,16777215);
      circle_button(320,225,"",SETTINGS_BUTTON,16777215);call_settings_button();
      circle_button(220,225,"SIZE",SIZE_BUTTON,16777215); 
      if (start_state==1){
         circle_button(420,225,"START",START_BUTTON,1288462);
      }
}




/*
 * The function "load_back_button()"  calls the function that creates a circle button. This button shows when size or settings button have been pressed in "page_one()",after this button is pressed it 
 * will take the user back to "page_one()".The reason this function does not have a GD.swap() at the end of drawing is  because "load_back_button()" is called by another function 
 * which at the end calls GD.swap().
 */
static void load_back_button (){
      circle_button(70,45,"BACK",BACK_BUTTON,16777215);
}



/*
 * The function "load_back_button2()"  function is called when the user has gone to a page that is beyond the "load_back_button ()" reach. This button is pressed after the user is already on a third page away from "page_one()"
 * For example, the button settings when pressed gies the user a second page displayed where options are listed to click on, If the only button that works "Change Time" is clicked it will take the user to a third page. This third page 
 * will use this button to give the user the opportunity to go back to the the previous options page.Hence that options page will have the "load_back_button2()" to take the user back to "page_one()". The reason this function does 
 * not have a GD.swap() at the end of drawing is  because "load_back_button2()" is called by another function  which at the end calls GD.swap().
 */
static void load_back_button2 (){
      circle_button(70,45,"BACK",BACK_BUTTON2,16777215);
}



/*
 * The function "load_background()" loads the image "b1.png" short for background1 to display. In this example there is less instructions needed compare to BitmapHandle.  This is done to show different ways to load an image.
 * Recall that the function GD.Vertex2ii(x,y) will be the starting point of the the image that it will display.
 */
static void load_background (){/*loads the background*/
    GD.ColorRGB(0xffffff); GD.cmd_loadimage(0, 0); GD.load("b1.png");  GD.Clear(); GD.Begin(BITMAPS);GD.Vertex2ii(0,0);//BACKGROUND IMAGE
}



/*
 * The function "load_title ( char label[])" accepts a parameter to change the page title up to 20 characters the user wants. When this function is called, the GD.ColorRGB(#) is set to white, and 
 * the GD.cmd_text(x,y,font,OPT_CENTER, label) is used to display the text. The reason this function does not have a GD.swap() at the end of drawing is  because "load_title()" is called by another function 
 * which at the end calls GD.swap().
 */
static void load_title ( char label[]){/*loads title for any page, sending the page title as a parameter*/
    GD.ColorRGB(0xffffff); GD.cmd_text(240, 15, 27, OPT_CENTER, label);
}



/*
 * The function "load_time()" function was used to display the time at the button left coner of "page_one()". All these can be simplyfied and just use on GD.cmd_text function with 
 * all data, but it is done this way just to show different ways of using the function, The color is set to white once again. The reason this function does not have a GD.swap() at 
 * the end of drawing is  because "load_time()" is called by another function which at the end calls GD.swap().
 */
static void load_time (){
    GD.ColorRGB(0xffffff); GD.cmd_text(20, 225,31, OPT_CENTER, "7");
    GD.ColorRGB(0xffffff); GD.cmd_text(45, 225,31, OPT_CENTER, ":");
    GD.ColorRGB(0xffffff); GD.cmd_text(80, 225,31, OPT_CENTER, "21");
    GD.ColorRGB(0xffffff); GD.cmd_text(140, 225,31, OPT_CENTER, "pm");
}



/*
 * The function "load_date()" function loads a date as text to the top right of "page_one()". Remember that the function "GD.cmd_text()"  and "GD.ColorRGB()" is a built in function  from the library GD initialzed 
 * at the beginning of the code. The reason this function does not have a GD.swap() at the end of drawing is  because "load_date()" is called by another function which at the end calls GD.swap().
 */
static void load_date (){
    GD.ColorRGB(0xffffff); GD.cmd_text(390, 34,27, OPT_CENTER, "Thurs, 5/23/19");/*places the date at the top right corner*/
}



/*
 * The function "load_temp()"  function loads a the temperature  as text to the top left of "page_one()". Remember that the function "GD.cmd_text()"  and "GD.ColorRGB()" is a built in function from the library GD initialzed 
 * at the beginning of the code. The reason this function does not have a GD.swap() at the end of drawing is  because "load_temp()" is called  by another function which at the end calls GD.swap().
 */
static void load_temp (){
    GD.ColorRGB(0xffffff); GD.cmd_text(30, 34,27, OPT_CENTER, "73 C");/*paces the temperature at the top left corner*/
}



/*
 *  The function "load_settings_button()" imports and saves the image "S87.png" size of 87 pixels wide to be saved as BitmapHandle "1", The use of these functions is explained in the function "load_cup_button()". This image is only displayed in "page_one()". 
 */
static void load_settings_button (){
    GD.ColorRGB(0xffffff); GD.BitmapHandle(1);GD.cmd_loadimage(-1, 0);GD.load("S87.png");GD.Begin(BITMAPS);GD.Vertex2ii(277,184, 1);
}



/*
 * The function "call_settings_button()" is used to display the image of the settings icon to be displayed in the middle of the bottom row buttons in "page_one()". The reason this function does not 
 * have a GD.swap() at the end of drawing is  because "call_settings_button()" is called by another function  which at the end calls GD.swap().
 */
static void call_settings_button (){
   GD.Vertex2ii(277,184, 1);
}



/*
 *  The function rotate_30_37 applies a rotation around pixel (30, 37), the 3 functions listed are part of the GD library.It sets (x,y)-> (30,37) as the new center, since 
 *  it is initialized as (0,0). It also allows rotation  within tht new center point. The reason this function does not have a GD.swap() at the end of drawing is  because "rotate_30_37()" is called by another function 
 * which at the end calls GD.swap().
 */
static void rotate_30_37(uint16_t a){
GD.cmd_translate(F16(30),F16(37));
GD.cmd_rotate(a);
GD.cmd_translate(F16(-30), F16(-37));
}



/*
 * The function "r_value()" sets to the display the backgroud by first initializing Bitmaps and then calling the background image by using the function GD.Vertexx2ii(0,0). After the backgroundis loaded, it then proceeds 
 * to call "page_five_details()" which contains the  title of that page.After this function is called. GD.Begin (BITMAPS) is called again to prepare for the new image that will be displayed. Now the function rotate_30_37() is called 
 * where it is in charge of rotatin the image.After the rotation is complete, cmd_setmatrix() is called allowing the translation to be made and then it is finally displayed by calling Vertex2ii and closingit with GD.swap().
 */
static void r_value(uint8_t a){
GD.Begin(BITMAPS);/*set up for the GD.Vertex2ii*/
GD.Vertex2ii(0,0);/*sets the background image */
page_five_details();
GD.Begin(BITMAPS);
rotate_30_37(DEGREES(-a));
GD.cmd_setmatrix();
GD.Vertex2ii(240, 100,2);
GD.swap();
}



/*
 * The function "slide()" sets displays the backgroud by first initializing Bitmaps and then calling the background image by using the function GD.Vertexx2ii(0,0). After the backgroundis loaded, it then proceeds 
 * to call "page_five_details()" which contains the  title of that page.After this function is called. GD.Begin (BITMAPS) is called again to prepare for the new image that will be displayed. Now the function 
 * rotate_30_37() is called where it is in charge of rotatin the image.After the rotation is complete, cmd_setmatrix() is called allowing the translation to be made and then it is finally displayed 
 * by calling Vertex2ii and closingit with GD.swap(). The purpose of this function is to slide the image at a -45 degree angle across the right half part of the screen. 
 */
static void slide(uint8_t a){
GD.Begin(BITMAPS);/*set up for the GD.Vertex2ii*/
GD.Vertex2ii(0,0);/*sets the background image */
page_five_details();
GD.Begin(BITMAPS);
rotate_30_37(DEGREES(-45));
GD.cmd_setmatrix();
GD.Vertex2ii(240+a, 100,2);
GD.swap();
}



/*
 * The function "slide2()" sets displays the backgroud by first initializing Bitmaps and then calling the background image by using the function GD.Vertexx2ii(0,0). After the backgroundis loaded, it then proceeds 
 * to call "page_five_details()" which contains the  title of that page.After this function is called. GD.Begin (BITMAPS) is called again to prepare for the new image that will be displayed. Now the function 
 * rotate_30_37() is called where it is in charge of rotatin the image.After the rotation is complete, cmd_setmatrix() is called allowing the translation to be made and then it is finally displayed 
 * by calling Vertex2ii and closingit with GD.swap(). The purpose of this function is to slide the image at a -45 degree angle across the first half part of the screen. 
 */
static void slide2(uint8_t a){
GD.Begin(BITMAPS);/*set up for the GD.Vertex2ii*/
GD.Vertex2ii(0,0);/*sets the background image */
page_five_details();
GD.Begin(BITMAPS);
rotate_30_37(DEGREES(-45));
GD.cmd_setmatrix();
GD.Vertex2ii(a,100,2);
GD.swap();
}


/*
 * The funciton "serving_cup()" calls the functions r_value,slide, and slide2 to display a moving cup across the screen.  It is in a for loop allowing 
 * the x cordinates to change for the sliding cup, and allowing the degress to change for the rotation of the cup as well. The reason this function does not have a GD.swap() at the 
 * end of drawing is  because "serving_cup()" is called by another function which at the end calls GD.swap().
 */
static void serving_cup(){    
  int i,j,k;
  for( i =0;i<45;i++){ 
     r_value(i); 
  }
  for(j=1;j<166;j++){ 
     slide(j);                                                  
  }
  for(k =1;k<240;k++){
     slide2(k);
  }
}



/*
 * The function "page_five_details()" load text as title to page_five()  which is displayed along with the moving/sliding cup. The reason this function does not have a GD.swap() at the end of 
 * drawing is  because "page_five_details()" is called by another function which at the end calls GD.swap(). 
 */
static void page_five_details(){
 GD.ColorRGB(0xffffff); GD.cmd_text(240, 25, 31, OPT_CENTER,"SERVING");
 GD.ColorRGB(0xffffff); GD.cmd_text(240, 60, 31, OPT_CENTER,"PLEASE WAIT!");  
}




/*
 * The function  "Hour_Min_AM_PM_pressed(int x, int y,int ww, int hh, char label[], int TAG)" is used to change the color of the button  for Hours,Minutes,AM, and PM button located 
 * on the setting section, under change time button. This fucnton does the same thing as the "button_pressed()" function. The reason this function does not have a GD.swap() at the 
 * end of drawing is  because "Hour_Min_AM_PM_pressed()" is called by another function which at the end calls GD.swap().
 */
static void Hour_Min_AM_PM_pressed(int x, int y,int ww, int hh, char label[], int TAG){    
      GD.Begin(RECTS);  
      GD.Tag(TAG);
      GD.cmd_track(x,y,ww,hh,TAG);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+ww, y+hh);
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+ww-1, y+hh-1);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+ww-3, y+hh-3);                
      GD.ColorRGB(0x193B61); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+ww-5, y+hh-5);  
      GD.ColorRGB(0x394C61); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+ww-7, y+hh-7);
      GD.ColorA(255); GD.ColorRGB(0xffffff);   GD.cmd_text(x+ww/2, y+hh/2, 27, OPT_CENTER, label);   
}



/* 
 * The function "Time_pressed(int x, int y,int ww, int hh, char label[], int TAG)" is used to the detect when a number button is pressed, and keeps track if the AM,PM,Hours,or Ninutes button were pressed.
 * This functions loads the background image, and loads the keypad number, and Hours,Minutes,AM,PM, and the back_button2 to the page. In the while loop, there is GD.get_inputs()function which is cosntantly 
 * chekcing which button is prrssed, and a button is prrssed the state of that button changes. For example, in this function pressed_AM_PM, is checked to see if it was pressed, if pressed then  
 * it will proceed to highlight that button and keep the state it is currently in without changing other data around. This allows the use to see the option they choose on Hours vs Minutes and AM vs PM. 
 * After checking if a button was pressed, it proceeds to change the color or the button that was currently pressed to green. This function ends by calling GD.swap() function so it displaysthe results to the screen.
 */
static void Time_pressed(int x, int y,int ww, int hh, char label[], int TAG){     
      GD.Begin(BITMAPS);/*set up for the GD.Vertex2ii*/
      GD.Vertex2ii(0,0);/*sets the background image */
      load_numbers();
      load_back_button2(); 
          if(pressed_AM_PM==1){ Hour_Min_AM_PM_pressed(130, y3-45,100,50, "AM",AM); }
          if(pressed_AM_PM==2){ Hour_Min_AM_PM_pressed(350, y3-45,100,50, "PM",PM); }
          if(pressed_HM==1){ Hour_Min_AM_PM_pressed(10, y2-45,90,50, "Hour",Hours); }
          if(pressed_HM==2){Hour_Min_AM_PM_pressed(10, y3-45,90,50, "Minutes",Minutes); }
      GD.Begin(RECTS);  
      GD.Tag(TAG); GD.cmd_track(x,y,ww,hh,TAG);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+ww, y+hh);
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+ww-1, y+hh-1);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+ww-3, y+hh-3);                
      GD.ColorRGB(0x567919); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+ww-5, y+hh-5);  
      GD.ColorRGB(0x679713); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+ww-7, y+hh-7);
      GD.ColorA(255); GD.ColorRGB(0xffffff);   GD.cmd_text(x+ww/2, y+hh/2, 27, OPT_CENTER, label);   
      GD.swap();
}


/*
 * The function "change_time(int x, int y,int ww, int hh, char label[], int TAG)" is the same function as the function "button()" except that it makes the size of the rectangles a little smaller
 * Refer to the function "button()" for more information. The reason this function does not have a GD.swap() at the end of drawing is  because "change_time(int x, int y,int ww, int hh, char label[], int TAG)" is called by another function which at the end calls GD.swap().
 */
static void change_time(int x, int y,int ww, int hh, char label[], int TAG){
      GD.Begin(RECTS);  
      GD.Tag(TAG); 
      GD.cmd_track(x,y,ww,hh,TAG);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x, y);  GD.Vertex2ii(x+ww, y+hh);
      GD.ColorRGB(0xD8D5D5); GD.Vertex2ii(x+1, y+1); GD.Vertex2ii(x+ww-1, y+hh-1);
      GD.ColorRGB(0x000000); GD.Vertex2ii(x+3, y+3); GD.Vertex2ii(x+ww-3, y+hh-3);                
      GD.ColorRGB(0xA89F9F); GD.Vertex2ii(x+5, y+5); GD.Vertex2ii(x+ww-5, y+hh-5);  
      GD.ColorRGB(0x7F7F7F); GD.Vertex2ii(x+7, y+7); GD.Vertex2ii(x+ww-7, y+hh-7);
      GD.ColorA(255); GD.ColorRGB(0xffffff);   GD.cmd_text(x+ww/2, y+hh/2, 27, OPT_CENTER, label);     
}


/*
 * The function "load_numbers()" is the same function as the function "load_buttons()". Refer to funciton "load_buttons()" for more information. 
 *  The reason this function does not have a GD.swap() at the end of drawing is  because "load_numbers()" is called by another function which at the end calls GD.swap().
 */
static void load_numbers(){
          change_time(130, y0-45,100,50, "1",One);
          change_time(240, y0-45,100,50, "2",Two);
          change_time(350, y0-45,100,50, "3",Three);
          change_time(130, y1-45,100,50, "4",Four);
          change_time(240, y1-45,100,50, "5",Five);
          change_time(350, y1-45,100,50, "6",Six);
          change_time(130, y2-45,100,50, "7",Seven);
          change_time(240, y2-45,100,50, "8",Eight);
          change_time(350, y2-45,100,50, "9",Nine);
          change_time(130, y3-45,100,50, "AM",AM);
          change_time(240, y3-45,100,50, "0",Zero);
          change_time(350, y3-45,100,50, "PM",PM);
          change_time(10, y2-45,90,50, "Hour",Hours);
          change_time(10, y3-45,90,50, "Minutes",Minutes);
}



/*
 * The function "page_one()" displays a background, 6 rectangluar buttons, 3 circles buttons, time, date, title, and the temperature. This function also chekcs if a certain button was pressed 
 * and calls the function to change the color of the button. The funciton also uses two differnet cmd_text()  fonts  to  show a font difference in the output.
 */
static void page_one (){
          Serial.println("In page One   ");
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          load_title("CHOOSE YOUR BEVERAGE");                              /*load the title for page_one()*/
          load_buttons();                                                  /*load the top two rows of buttons*/
          load_circles();                                                  /*load the three bottom circle buttons*/
          load_time();                                                     /*load the time to the lower left corner of the display*/
          load_date();                                                     /*load the date at the top right corner of the display*/
          load_temp();                                                     /*load the temperature "73 C" at the top left corner of the display */
         
            
          if(size_pressed==1){                                             /*check if a beverage size has been picked and display in the button of page_one() */
              GD.ColorRGB(0x679713);   
              GD.cmd_text(215,245, 29, OPT_CENTER, "4"); 
              GD.cmd_text(232,245, 20, OPT_CENTER, "oz"); 
          }
          if(size_pressed==2){                                             /*check if a beverage size has been picked and display in the button of page_one() */
              GD.ColorRGB(0x679713);  
              GD.cmd_text(215,245, 30, OPT_CENTER, "8"); 
              GD.cmd_text(232,245, 20, OPT_CENTER, "oz"); 
          }
          if(size_pressed==3){                                             /*check if a beverage size has been picked and display in the button of page_one() */
              GD.ColorRGB(0x679713);  
              GD.cmd_text(210,245, 29, OPT_CENTER, "12");
              GD.cmd_text(235,245, 20, OPT_CENTER, "oz");  
          }

          GD.ColorRGB(0xffffff);                                           /*reset the color to white, or else the ColorRBG(green"0x679713") previously set or it will make the screen appear green*/
          if(pressed==1){
            button_pressed(x0, y0, "ESPRESSO",ESPRESSO);                   /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function */
          }
          if(pressed==2){
            button_pressed(x1, y0, "CAPPUCCHINO",CAPPUCCHINO);             /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function*/
          }
          if(pressed==3){
            button_pressed(x2, y0, "HOT CHOCOLATE",HOT_CHOCOLATE);         /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function*/
          }
          if( pressed==4){
            button_pressed(x0, y1, "LATTE",LATTE);                         /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function*/
          } 
          if(pressed==5){
            button_pressed(x1, y1, "INSTANT TEA",INSTANT_TEA);             /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function*/
          }
          if(pressed==6){
            button_pressed(x2, y1, "HOT WATER",HOT_WATER);                 /*check if a beverage has been picked and if so, change the color of the button of page_one() to green. The GD.swap() function is called in the button_pressed() function*/
          }  
          if(pressed==0){                                                  /*check if a beverage has been picked and if not call the GD.swap() function. */
            GD.swap();
          }  
   
        
         
}



/*
 * The function "page_two()" displays a background, 3 long rectangluar buttons, 1 back button,and a title. This function also checks if a certain button was pressed 
 * and calls the function to change the color of the button. 
 */
static void page_two (){
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          load_back_button();                                              /*load the back_button to return to page_one()*/
          load_title("CHOOSE YOUR SIZE");                                  /*load the title of page two*/
          if(size_pressed== 1){                                            /*check if a beverage size has been picked and call the function "drink_size_button_pressed()" to change the color of the button  to green in page_two() */
            drink_size_button_pressed(x0, y0+40,"4 oz",FOUR_OZ, h*3);      /*The GD.swap() function is called in the drink_size_button_pressed() function*/
          }
          if(size_pressed==2 ){                                            /*check if a beverage size has been picked and call the function "drink_size_button_pressed()" to change the color of the button  to green in page_two() */
            drink_size_button_pressed(x1, y0+40,"8 oz",EIGHT_OZ, h*3);     /*The GD.swap() function is called in the drink_size_button_pressed() function*/
          }
          if(size_pressed== 3){                                            /*check if a beverage size has been picked and call the function "drink_size_button_pressed()" to change the color of the button  to green in page_two() */
            drink_size_button_pressed(x2, y0+40,"12 oz",TWELVE_OZ,h*3);    /*The GD.swap() function is called in the drink_size_button_pressed() function*/
          }
          if(size_pressed ==0){                                             /*check if a beverage size has been picked and if not, then just display all buttons in the original color, with no selection made in page_two() and call the GD.swap() function */
            load_drink_size_button();                               
            GD.swap();
          }
}



/*
 * The function "page_three()" displays a background, 5 wide rectangluar buttons, 1 back button,and a title. 
 */
static void page_three (){
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          load_title("SETTINGS");                                          /*loads the title to this page*/
          load_back_button();                                              /*load the back_button to return to page_one()*/
          load_settings();                                                 /*loads the 5 wide buttons*/
          GD.swap();                                                       /*calls the GD.swap() function to display what was drawn*/
}

/*
 * The function page_four() displays a blank page with a background a title saying that the page is still in progress
 */
static void page_four (){
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          GD.ColorRGB(0xffffff);                                           /*set the color to white for the text to be displayed*/
          GD.cmd_text(240, 136, 31, OPT_CENTER,"PAGE IN PROGRESS!");       /*write PAGE in progress as a title to the page without using the title function*/
          load_back_button2();                                             /*load the back_button to return to page_three()*/
          GD.swap();                                                       /*calls the GD.swap() function to display what was drawn*/
          
}



/*
 * The function page_five() loads the background image, the moving cup and a thankyou text after it is served, waits 2 seconds and proceeds to page_one()
 */
static void page_five (){
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          serving_cup();                                                   /*calls the moving cup to display as it serves the drink*/ 
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*calls "background" again*/
          GD.ColorRGB(0xffffff);                                           /*sets the color to write orthe text to be displayed*/
          GD.cmd_text(240, 150, 31, OPT_CENTER,"THANK YOU");               /*writes THANK YOU  to the screen */
          GD.swap();                                                       /*calls the GD.swap() function to display what was drawn*/
          pressed=0;                                                       /*sets pressed to zero to restart*/
          size_pressed=0;                                                  /* sets size_pressed to zero to restart*/
          start_state=0;                                                   /*sets start_state to zero to restart*/
          delay(2000);                                                     /*makes a delay to allow the user to read the Thank you text*/
          page_one();                                                      /*goes back to page_one() to start all over again*/
}



/*
 * The function page_change_time() loads the background image,back butotn to go back to page_three() ,loads_numbers() which have a keypad format  and checks if AM,PM,Hours,or Minutes button was pressed and calls a function tochange the color.
 */
static void page_change_time (){
          GD.Begin(BITMAPS);                                               /*set up for the GD.Vertex2ii*/
          GD.Vertex2ii(0,0);                                               /*sets the background image */
          load_numbers();
          load_back_button2();                                             /*load the back_button to return to page_three()*/
          if(pressed_AM_PM==1){
            Hour_Min_AM_PM_pressed(130, y3-45,100,50, "AM",AM);            /*check if a AM button has been picked and call the function "Hour_Min_AM_PM_pressed()" to change the color of the button in page_change_time() */
            }
          if(pressed_AM_PM==2){ 
            Hour_Min_AM_PM_pressed(350, y3-45,100,50, "PM",PM);            /*check if a PM button has been picked and call the function "Hour_Min_AM_PM_pressed()" to change the color of the button in page_change_time() */
            } 
          if(pressed_HM==1){ 
            Hour_Min_AM_PM_pressed(10, y2-45,90,50, "Hour",Hours);         /*check if a Hours button has been picked and call the function "Hour_Min_AM_PM_pressed()" to change the color of the button in page_change_time() */ 
          }
          if(pressed_HM==2){ 
            Hour_Min_AM_PM_pressed(10, y3-45,90,50, "Minutes",Minutes);    /*check if a Minute button has been picked and call the function "Hour_Min_AM_PM_pressed()" to change the color of the button in page_change_time() */
          }      
          GD.swap();                                                       /*calls the GD.swap() function to display what was drawn, in the Hourd_Min_AM_PM_pressed() the function GD.swap() is not called, unlike page_two()*/
}


/*
 * The void setup() imports the background image along with the cup images and settings icon so they dont have to be importedlater on in the program, it then proceeds to call page_one(). Serial.begin is called for 
 * debugging purposes. Before using the GD library function GD.begin() MUST be call.
 */
void setup()
{     
          Serial.begin(9600);
          GD.begin();    
          load_background();
          load_settings_button();
          load_cup_button();
          page_one();    
}



/*
 * The void loop()  gets the inputs of the display using GD.get_inputs and prints to console the output, along with any other button pressed. In this loop, it tracks and updates any buttons being pressed
 */
void loop()
{ 
          GD.get_inputs();                                               /*Get the inputs from the display.*/
          Serial.print(GD.inputs.x);Serial.print("\t");                  /*print the x cordinate  */
          Serial.print(GD.inputs.y);Serial.print("\t");                  /*print the y cordinate  */
          Serial.print(GD.inputs.tag);                                   /*print the tag value    */
          
          if(GD.inputs.tag==SIZE_BUTTON){                                /*check if SIZE_BUTTON was pressed, print to serial port that it was pressed, wait for 300 miliseconds,and proceed to page_two() */
            Serial.print("   SIZE BUTTON PRESSED   ");    delay(300);
            page_two ();
            }
          if(GD.inputs.tag==SETTINGS_BUTTON){                            /*check if SETTINGS_BUTTON was pressed, print to serial port that it was pressed, wait for 300 miliseconds,and proceed to page_three() */
            Serial.print("   SETTINGS BUTTON PRESSED   ");delay(300);
            page_three(); 
            }
          if(GD.inputs.tag==BACK_BUTTON){                                /*check if BACK_BUTTON was pressed, print to serial port that it was pressed, wait for 300 miliseconds,and proceed to page_one() */
            Serial.print("   BACK BUTTON PRESSED   ");    delay(300);
            page_one();
            }
          if(GD.inputs.tag==BACK_BUTTON2){                               /*check if BACK_BUTTON2 was pressed, print to serial port that it was pressed, wait for 300 miliseconds,and proceed to page_three() */
            Serial.print("   BACK BUTTON 2 PRESSED   ");  delay(300);
            page_three();                                      
            pressed_AM_PM=0;                                             /*reset pressed_AM_PM to zero*/
            pressed_HM=0;                                                /*reset pressed_HM to zero*/
            }
        
        
        
          if(GD.inputs.tag==Hours){                                      /*check if Hours was pressed, print to serial port that it was pressed */
             Serial.print("   HOURS BUTTON PRESSED   ");
            pressed_HM=1;                                                /*set pressed_HM=1 to track that it Hours button was pressed */
            Time_pressed(10, y2-45,90,50, "Hour",Hours);                 /*call function Time_pressed() to change the color of the Hours button */
            }
          if(GD.inputs.tag==Minutes){
             Serial.print("   MINUTES BUTTON PRESSED   ");               /*check if Minutes was pressed, print to serial port that it was pressed*/
            pressed_HM=2;                                                /*set pressed_HM=2 to track that it Minutes button was pressed */
            Time_pressed(10, y3-45,90,50, "Minutes",Minutes);            /*call function Time_pressed() to change the color of the Minutes button */
            }
        
        
        

          /*
           * The following 12 lines each are an if statemenet checking the tag of the buttons for the settings page-> change time. The if statement states
           *  that if that button is pressed then it will print to the serial port that a certain button value was presssed. It will then proceed to call the 
           *  function Time_pressed and updated the color of that button to green. In the case of lines 11 and 12 (the last two of this section) each have 
           *  the variable pressed_AM_PM set to deferent values to keep track if the AM or the PM button is pressed.
           */
          if(GD.inputs.tag==One)  {   Serial.print("   One   "  );  Time_pressed(130, y0-45,100,50, "1",One  );   }
          if(GD.inputs.tag==Two)  {   Serial.print("   Two   "  );  Time_pressed(240, y0-45,100,50, "2",Two  );   }
          if(GD.inputs.tag==Three){   Serial.print("   Three   ");  Time_pressed(350, y0-45,100,50, "3",Three);   }
          if(GD.inputs.tag==Four) {   Serial.print("   Four   " );  Time_pressed(130, y1-45,100,50, "4",Four );   }
          if(GD.inputs.tag==Five) {   Serial.print("   Five   " );  Time_pressed(240, y1-45,100,50, "5",Five );   }
          if(GD.inputs.tag==Six)  {   Serial.print("   Six   "  );  Time_pressed(350, y1-45,100,50, "6",Six  );   }
          if(GD.inputs.tag==Seven){   Serial.print("   Seven   ");  Time_pressed(130, y2-45,100,50, "7",Seven);   }      
          if(GD.inputs.tag==Eight){   Serial.print("   Eight   ");  Time_pressed(240, y2-45,100,50, "8",Eight);   }
          if(GD.inputs.tag==Nine) {   Serial.print("   Nine   " );  Time_pressed(350, y2-45,100,50, "9",Nine );   }
          if(GD.inputs.tag==Zero) {   Serial.print("   Zero   " );  Time_pressed(240, y3-45,100,50, "0",Zero );   }
          if(GD.inputs.tag==AM)   {   Serial.print("   AM   "   );  Time_pressed(130, y3-45,100,50, "AM",AM  );    pressed_AM_PM =1 ; }
          if(GD.inputs.tag==PM)   {   Serial.print("   PM   "   );  Time_pressed(350, y3-45,100,50, "PM",PM  );    pressed_AM_PM =2 ; }

          
          /* 
           *  The following next 6 lines of each have the same logic as previously described above.Each if statement have a pressed variable set to a certain
           *  value to keep track of which button is being pressed.ESPRESSO,CAPPUCCHINO,and HOT CHOCOLATE belong to the first row of the page_one(). and the 
           *  LATTE,INSTANT TEAM and HOT WATER belong to the second row in page_one().
           */
          if(GD.inputs.tag==ESPRESSO)     {  Serial.print("   ESPRESSO   "     ); pressed=1;  button_pressed(x0, y0, "ESPRESSO",ESPRESSO);           }
          if(GD.inputs.tag==CAPPUCCHINO)  {  Serial.print("   CAPPUCCHINO   "  ); pressed=2;  button_pressed(x1, y0, "CAPPUCCHINO",CAPPUCCHINO);     }
          if(GD.inputs.tag==HOT_CHOCOLATE){  Serial.print("   HOT CHOCOLATE   "); pressed=3;  button_pressed(x2, y0, "HOT CHOCOLATE",HOT_CHOCOLATE); }
          if(GD.inputs.tag==LATTE)        {  Serial.print("   LATTE   "        ); pressed=4;  button_pressed(x0, y1, "LATTE",LATTE);                 } 
          if(GD.inputs.tag==INSTANT_TEA)  {  Serial.print("   INSTANT TEA   "  ); pressed=5;  button_pressed(x1, y1, "INSTANT TEA",INSTANT_TEA);     }
          if(GD.inputs.tag==HOT_WATER)    {  Serial.print("   HOT WATER   "    ); pressed=6;  button_pressed(x2, y1, "HOT WATER",HOT_WATER);         }  


          /*
           * The following 3 lines each have the same logic as previously described above. These if statement have 2 statements though. This is because
           * of the image that was inserted, that image obtained its own tag in order to track when it was pressed since when it was disokayed it overwerite whatever was 
           * written before. Checking both tag allows all the area of that button to be checked. The size_pressed variable will also determine with button was pressed
           * which will be used for logic if statements when the function  drink_size_button_pressed() function is called and changes the color.
           */
          if(GD.inputs.tag==FOUR_OZ | GD.inputs.tag==CUP60    ) {  Serial.print("   FOUR_OZ   "  ); size_pressed=1; drink_size_button_pressed(x0, y0+40,"4 oz",FOUR_OZ, h*3);   }
          if(GD.inputs.tag==EIGHT_OZ | GD.inputs.tag==CUP80   ) {  Serial.print("   EIGHT_OZ   " ); size_pressed=2; drink_size_button_pressed(x1, y0+40,"8 oz",EIGHT_OZ, h*3);  }
          if(GD.inputs.tag==TWELVE_OZ | GD.inputs.tag==CUP100 ) {  Serial.print("   TWELVE_OZ   "); size_pressed=3; drink_size_button_pressed(x2, y0+40,"12 oz",TWELVE_OZ,h*3); }  

          /*
           * The following if staement checks if a beverage and a beverage size has been choosen. If that is true then it will set the variable start_state to one; This 
           * will allow the user to press the start button and obtain their drink of choice
           */
          if((pressed < 7) && (pressed > 0) && (size_pressed > 0) && (size_pressed < 4)){
              start_state=1;
            }
          /*
           * The following if statement checks if the start_state is met along if the user is pressing the START_BUTTON. If this is true it will then delay  half a second
           * and then proceed to call page_five() which contains the cup animation, rotating and sliding across the screen
           */
          if(GD.inputs.tag==START_BUTTON  && start_state==1){ 
            delay(500);
            page_five();
            }

          /*
           * The last following if statement just checks which button has been pressed under the settings page. They all goto page_four() which is a page in progress exceptfor CHANGE_TIME. CHANGE_TIME 
           * goes to the function page_change_time()  which displays the keypad options to change the time.
           */
          if(GD.inputs.tag==BRIGHTNESS      ) {page_four();       }
          if(GD.inputs.tag==VOLUME          ) {page_four();       }
          if(GD.inputs.tag==COFFEE_TIME     ) {page_four();       }
          if(GD.inputs.tag==CHANGE_TIME     ) {page_change_time();} 
          if(GD.inputs.tag==CONNECT_TO_PHONE) {page_four();       }
          
          Serial.print("\n");  /*This ends the line and starts a new one in Serial COM output.*/
}


   



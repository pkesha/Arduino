/*** Copyright (C) 2013-2018 by James Bowman 
* Reference: The Gameduino 2 Tutorial, Reference and Cookbook James Bowman
* @file                           ReadMe.txt
* @brief                          Contains information regarding how to setup your device and run the Coffee Machine example			
* @version                        Tested on UNO R3
* @date                           06/04/2019
*/

/********************************************************************************
*
* THANK YOU for purchasing Newhaven Display International  
* NHD-4.3 -480272FT-CSXV-CTP TFT.
* We appreciate your support!
*
/********************************************************************************

Materials Needed:

- UNO R3 
- Newhaven Display NHD-4.3 480272FT-CSXV-CTP
- NHD-FT81x-SHIELD REV 1.0
- Newhaven Display 20 POS FFC
- USB Cable-Standard A-B programming cable
- MicroSD card




Getting Started:
1. Unzip the compressed folder Coffee_Machine 
2. Copy the images : "b1.png" "cup60.png"  "cup80.png"  "cup100.png"  "S87.png" to the MicroSD card
3. Insert the MicroSD card in the NHD-FT81x-SHIELD REV 1.0
4. Open Arduino IDE
5. Go to Sketch -> Include Library -> Add.ZIP Library
6. Navigate to Coffee_Machine folder and find the zip library compressed.
7. Select the ZIP library  and import it 
8. Go to your Arduino library and look for Gameduino2 folder and open it
9. Open the file "GD2.cpp" 

			Edit the following:
								#if defined(ESP8266)
								#define SD_PIN        D9    
								#else
								//#define SD_PIN        9             // comment this line 
								#define SD_PIN        5               // add this line 
								#endif

								
								
				//comment the following:
								/*#if (BOARD == BOARD_FTDI_80x) || defined(RASPBERRY_PI) || defined(DUMPDEV) || defined(SPIDRIVER)
								#undef STORAGE
								#define STORAGE 0
								#endif */
					
10. Now go to transports folder and open the file "wiring.h" 

			Edit the following:
								#ifndef CS
								#if defined(ESP8266)
								#define CS D8
								#else
								//#define CS 8                        //comment this line 
								#define CS 10                         //add this line
								#endif
								#endif

11.  Save the file "GD2.cpp" and the file "wiring.h"
12. Mount your UNO R3 with the NHD-FT81x-SHIELD REV 1.0
13. Connect the Newhaven Display 20 POS FFC from the SHIELD to the TFT
14. Connect the USB Cable-Standard A-B programming cable from your computer to the UNO R3
15. Go to the unzip folder created in step 1.
16. Open the Coffee_machine.ino file with ARDUINO IDE.
17. Go to Tools ->  Board: -> and select Arduino/Genuino Uno
18. Go to Tools -> Port:-> and select your port number
19. Upload the code to the board by clicking Sketch -> Upload using programmer
20. Read Functions_Summary.txt
21. Build your own design















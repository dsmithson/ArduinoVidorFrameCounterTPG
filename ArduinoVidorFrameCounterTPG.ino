#include "VidorGraphics.h"
#include "Vidor_GFX.h"

Vidor_GFX  vdgfx;

const int DRAW_PIN = 0; //Pin used to signal while we're drawing
const int DRAW_LOOP_SLEEP_US = 11667;  //Amount of time to sleep (in us) between frame render loops.

void setup() {
  Serial.begin(9600);

  // wait for the serial monitor to open,
  // if you are powering the board from a USB charger remove the next line
  //while (!Serial);

  // initialize digital pin DRAW_PIN as an output.
  pinMode(DRAW_PIN, OUTPUT);

  // Initialize the FPGA
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
  delay(4000);
}

bool on = false;
long frameCount = 0;

void loop()
{
  /**
  *  Draw an Arduino logo
  */
  
  // Fill the screen with a white background
  vdgfx.fillRect(0,0,640,480,vdgfx.White(),1);

  /**
  *  The library allows drawing some basic elements to the view, like circles, rectangles, lines
  */  

  vdgfx.fillCircle(225,225,100 ,vdgfx.lightBlue(),1);
  vdgfx.fillCircle(415,225,100 ,vdgfx.lightBlue(),1);
  vdgfx.fillCircle(225,225,90 ,vdgfx.White(),1);
  vdgfx.fillCircle(415,225,90 ,vdgfx.White(),1);
  vdgfx.fillRect(175,220,100,10 ,vdgfx.lightBlue(),1);
  vdgfx.fillRect(365,220,100,10 ,vdgfx.lightBlue(),1);
  vdgfx.fillRect(410,175,10,100 ,vdgfx.lightBlue(),1);

  /**
  *  To draw a text we can use the classic functions like write() and print()
  *  Text size, color and position can be changed using the .text subclass
  */
  vdgfx.text.setCursor(150,375);
  vdgfx.text.setAlpha(1);
  vdgfx.text.setSize(3);
  vdgfx.text.setColor(vdgfx.lightBlue());
  vdgfx.println("ARDUINO");
  vdgfx.text.setCursor(480,145);
  vdgfx.text.setSize(1);
  vdgfx.println("TM");

  //Do our frame updates on an interrupt
  //attachInterrupt(IRQ_PIN, drawFrame, FALLING);

  //One-Time drawing setup
  vdgfx.text.setAlpha(1);
  vdgfx.text.setSize(1);
  vdgfx.text.setColor(vdgfx.White());
  
  while (1) {    
    if(++frameCount > 100000) {
      frameCount = 0;
    }
    
    drawFrame();
    delayMicroseconds(DRAW_LOOP_SLEEP_US);
  }
}

void drawFrame() {
  
 digitalWrite(DRAW_PIN, HIGH); 
   
  // Fill the screen with a white background
  vdgfx.fillRect(47,432,92,22,vdgfx.Black(),1);

  vdgfx.text.setCursor(50,450);
  vdgfx.print(frameCount, DEC);

  digitalWrite(DRAW_PIN, LOW);  
}


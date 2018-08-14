#include "VidorGraphics.h"
#include "Vidor_GFX.h"

Vidor_GFX  vdgfx;

const int DRAW_PIN = 0; //Pin used to signal while we're drawing
const int BUTTON_PIN = 1; //Pin used as in input
const int DRAW_LOOP_SLEEP_US = 8667;  //Amount of time to sleep (in us) between frame render loops.

const int WIDTH = 640;
const int HEIGHT = 480;

void setup() {
  Serial.begin(9600);

  // wait for the serial monitor to open,
  // if you are powering the board from a USB charger remove the next line
  //while (!Serial);

  // initialize digital pin DRAW_PIN as an output.
  pinMode(DRAW_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Initialize the FPGA
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
  delay(4000);

  //Do our frame updates on an interrupt
  int buttonIRQ = digitalPinToInterrupt(BUTTON_PIN);
  attachInterrupt(buttonIRQ, onButtonPressed, RISING);
}

bool cyclePatternOnNextFrame = 1;
byte patternIndex = 0;
long frameCount = 0;

void loop()
{  
  //One-Time drawing setup
  vdgfx.text.setAlpha(1);
  vdgfx.text.setSize(1);
  vdgfx.text.setColor(vdgfx.White());
  
  while (1) {    
    if(++frameCount >= 100000) {
      frameCount = 0;
    }

    if(cyclePatternOnNextFrame) {
      cyclePatternOnNextFrame= false;
      drawNextPattern();
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

void onButtonPressed() {
  cyclePatternOnNextFrame = 1;
}

void drawNextPattern() {
  
  switch(++patternIndex) {
        case 0:
          drawArduinoLogo();
          break;
        case 1:
          drawColorBars(true);
          break;
        case 2:
          drawColorBars(false);
          break;
        case 3:
          drawHorizontalGrayscale(true);
          break;
        case 4:
          drawHorizontalGrayscale(false);
          break;
        default:
          patternIndex = 0;
          drawArduinoLogo();
      }

  //Each pattern generated should reset text color to white afterwards to ensure our frame counter remains white
  vdgfx.text.setColor(vdgfx.White());
}

void drawArduinoLogo() {
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
}

void drawColorBars(bool grayScaleLower) {
  const byte colorCount = 7;
  uint16_t colors[] = {
    Color(255, 255, 255), //White
    Color(255, 255, 0),   //Yellow
    Color(0, 255, 255),   //Cyan
    Color(0, 255, 0),     //Green
    Color(255, 0, 255),   //Purple
    Color(255, 0, 0),     //Red
    Color(0, 0, 255)
  };

  //Draw our main color bars
  int i;
  int columnWidth = WIDTH / colorCount;
  for(i=0; i<colorCount; i++) {
    vdgfx.fillRect(columnWidth * i, 0, columnWidth, HEIGHT,colors[i],1);
  }

  //Draw our gray gradients
  if(grayScaleLower) {
    const int grayHeight = 40;
    drawHorizontalGrayBars(0, HEIGHT - grayHeight, WIDTH, grayHeight, 32, false);
    drawHorizontalGrayBars(0, HEIGHT - (grayHeight * 2), WIDTH, grayHeight, 32, true);
  }
}

void drawHorizontalGrayscale(bool invertDirection) {
  drawHorizontalGrayBars(0, 0, WIDTH, HEIGHT, 32, invertDirection);
}

void drawHorizontalGrayBars(int x, int y, int width, int height, int steps, bool invertDirection) {
  byte i;
  int columnWidth = width / steps;
  if(width % steps != 0) {
    columnWidth++;
  }
  
  for(i=0; i<steps; i++) {
    byte val = 255 / steps * i;
    if(invertDirection) {
      val = 255 - val;
    }
    vdgfx.fillRect(x + (columnWidth * i), y, columnWidth, height, Color(val, val, val), 1);
  }
}

uint16_t Color(byte r, byte g, byte b) {
  return vdgfx.Color(r, g, b);
}


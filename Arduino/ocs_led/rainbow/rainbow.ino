// Pins
const int red = 9;
const int green = 10;
const int blue = 11;
const int potPin = 0;

// Colors
int ocsAvailableColor[] = {0, 255, 0}; // Green
int ocsAvailable = 97; // a
int ocsBusyColor[] = {255, 0, 0}; // Red
int ocsBusy = 98; // b
int ocsAwayColor[] = {255, 70, 0}; // Yellow but Blue for now
int ocsAway = 119; // w
int ocsUnknownColor[] = {0, 0, 0}; // Nothing
int ocsDebug = 48; // 0
int ocsDebugColor[] = {255, 255, 255}; // ALL
int ocsRainbow = 114; // r
int ocsDisturb = 100; // d
int noneColor[] = {0, 0, 0}; // Blank

boolean rainbowMode = true;
int incomingByte = 0;
int* currentColor = ocsDebugColor;

/**
 * Calculate the actual value to spit out to the pins based on brightness
 */
void calculateColorValue(int brightness, int *color, int *outputColor) {
  float multiplyer = brightness / 1024.0;
  outputColor[0] = (int)(multiplyer * color[0]);
  outputColor[1] = (int)(multiplyer * color[1]);
  outputColor[2] = (int)(multiplyer * color[2]);
}

/**
 * Display a color to the pins
 */
void displayColor(int* color, int brightness) {
  int outputColor[] = {0, 0, 0};
  calculateColorValue(brightness, color, outputColor);
  
  analogWrite(red, outputColor[0]);
  analogWrite(green, outputColor[1]);
  analogWrite(blue, outputColor[2]);
}

/**
 * Setup the board
 */
void setup() {                
  // initialize the digital pin as an output.
  pinMode(red, OUTPUT);    
  pinMode(green, OUTPUT);    
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
}

/**
 * Logic loop
 */
void loop() {
  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    // Determine rollover between blue and red (last and first)
    int incColour = 0;
    if (decColour != 2) {
      incColour = decColour + 1;
    }

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      // Read the brightness multiplier
      int brightness = analogRead(potPin);
      
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        rainbowMode = false;
        if (incomingByte == ocsAvailable) {
          currentColor = ocsAvailableColor;
        }
        else if (incomingByte == ocsBusy) {
          currentColor = ocsBusyColor;
        }
        else if (incomingByte == ocsAway) {
          currentColor = ocsAwayColor;
        }
        else if (incomingByte == ocsDebug) {
          currentColor = ocsDebugColor;
        }
        else if (incomingByte == ocsDisturb) {
          currentColor = ocsBusyColor;
          displayColor(currentColor, brightness);
          delay(1000);
          currentColor = noneColor;
          displayColor(currentColor, brightness);
          delay(1000);
        }
        else {
          rainbowMode = true;
          int rainbowColor[] = {rgbColour[0], rgbColour[1], rgbColour[2]};
          currentColor = rainbowColor;
        }
      }
      else if (rainbowMode == true) {
          int rainbowColor[] = {rgbColour[0], rgbColour[1], rgbColour[2]};
          currentColor = rainbowColor;
      }
      displayColor(currentColor, brightness);
      delay(5);
    }
  }
}

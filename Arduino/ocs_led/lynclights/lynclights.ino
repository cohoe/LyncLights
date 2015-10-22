// Pins
const int red = 9;
const int green = 10;
const int blue = 11;
const int potPin = 0;

// Colors
const int ocsAvailableColor[] = {0, 255, 0}; // Green
const int ocsAvailable = 97; // a
const int ocsBusyColor[] = {255, 0, 0}; // Red
const int ocsBusy = 98; // b
const int ocsAwayColor[] = {255, 70, 0}; // Yellow but Blue for now
const int ocsAway = 119; // w
const int ocsUnknownColor[] = {0, 0, 0}; // Nothing
const int ocsDebug = 48; // 0
const int ocsDebugColor[] = {255, 255, 255}; // ALL
const int ocsRainbow = 114; // r
const int ocsDisturb = 100; // d
const int noneColor[] = {0, 0, 0}; // Blank

// Data input
int incomingByte = 0;

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
 * Get the brightness value
 */
int getBrightness() {
  return analogRead(potPin);
}

/**
 * Rainbow loop
 */
void loopRainbow() {
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
      
      // Set the color
      int rainbowColor[] = {rgbColour[0], rgbColour[1], rgbColour[2]};
      // Display it
      displayColor(rainbowColor, getBrightness());
      delay(4);
    }
  }
}

/**
 * Red alert
 */
void loopAlert() {
  // Need to loop to pick up new brightness settings
  for (int i=0; i<1000; i++) {
    displayColor(ocsBusyColor, getBrightness());
    delay(1);
  }
  // No loop required for no color because.... no color.
  displayColor(noneColor, getBrightness());
  delay(1000);
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
  // If there is a new character, get it
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }
  
  // Display based on what we have
  if (incomingByte == ocsAvailable) {
    displayColor(ocsAvailableColor, getBrightness());
  }
  else if (incomingByte == ocsBusy) {
    displayColor(ocsBusyColor, getBrightness());
  }
  else if (incomingByte == ocsAway) {
    displayColor(ocsAwayColor, getBrightness());
  }
  else if (incomingByte == ocsDebug) {
    displayColor(ocsDebugColor, getBrightness());
  }
  else if (incomingByte == ocsDisturb) {
    loopAlert();
  }
  else {
    loopRainbow();
  }
}

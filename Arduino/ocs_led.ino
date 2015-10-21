//int led = 11;
int incomingByte = 0;
int red = 9;
int green = 10;
int blue = 11;
int potPin = 0;

int ocsAvailableColor[] = {0, 255, 0}; // Green
int ocsAvailable = 97; // a
int ocsBusyColor[] = {255, 0, 0}; // Red
int ocsBusy = 98; // b
int ocsAwayColor[] = {255, 70, 0}; // Yellow but Blue for now
int ocsAway = 119; // w
int ocsUnknownColor[] = {0, 0, 0}; // Nothing
int ocsDebug = 48; // 0
int ocsDebugColor[] = {255, 255, 255}; // ALL

//int (*currentColor)[3];
//int*[3] currentColor;
//int currentColor[3];
//currentColor = new int*[3];
int* currentColor = ocsDebugColor;

void calculateColorValue(int brightness, int *color, int *outputColor) {
  float multiplyer = brightness / 1024.0;
  outputColor[0] = (int)(multiplyer * color[0]);
  outputColor[1] = (int)(multiplyer * color[1]);
  outputColor[2] = (int)(multiplyer * color[2]);
}

void displayColor(int* color, int brightness) {
  // Reset  
  // Send new color
  //Serial.print("Brightness is ");
  //Serial.println(brightness, DEC);
  
  int outputColor[] = {0, 0, 0};
  calculateColorValue(brightness, color, outputColor);
  
  analogWrite(red, outputColor[0]);
  //Serial.print("Setting red to ");
  //Serial.println((int)((float)(brightness / 1024.0) * color[0]), DEC);
  analogWrite(green, outputColor[1]);
  analogWrite(blue, outputColor[2]);
}

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(red, OUTPUT);    
  pinMode(green, OUTPUT);    
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
int inLoop = 1;
void loop() {

  int brightness = analogRead(potPin);
  //Serial.print("Brightness is ");
  //Serial.println(brightness, DEC);
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingByte, DEC);
    //                switch(incomingByte) {
    //                  case 97:
    //                    displayColor(ocsAvailableColor);
    //                    break;
    //                  case 98:
    //                    displayColor(ocsBusyColor);
    //                    break;
    //                  case 119:
    //                    displayColor(ocsAwayColor);
    //                    break;
    //                  default:
    //                    displayColor(ocsUnknown);
    //                    break;
    //                }
    
      
      
      if (incomingByte == ocsAvailable) {
        //displayColor(ocsAvailableColor, brightness); 
        currentColor = ocsAvailableColor;
      }
      else if (incomingByte == ocsBusy) {
        //displayColor(ocsBusyColor, brightness); 
        currentColor = ocsBusyColor;
      }
      else if (incomingByte == ocsAway) {
        //displayColor(ocsAwayColor, brightness); 
        currentColor = ocsAwayColor;
      }
      else if (incomingByte == ocsDebug) {
        currentColor = ocsDebugColor;
      }
      else {
        //displayColor(ocsUnknownColor, brightness); 
        currentColor = ocsUnknownColor;
      }
    
  }
  
  displayColor(currentColor, brightness);
}

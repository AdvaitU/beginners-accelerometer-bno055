/*  Accelerometer Class - To be used for the Adafruit BNO055 Sensor interfacing with an Arduino.
    Author: Advait Ukidve (ukidveadvait@gmail.com)
    License: This code is intended to be completely public domain and to be used under the Creative Commons CC0 License. No attribution is required, but any is welcome :)

    Adafruit BNO055 Sensor:
      Buy Here:
      Specifications Here:


  Interfacing with an Arduino Leonardo over I2C:
    BNO055 -----  Arduino
    Vin    ---->  5V
    GND    ---->  GND
    SDA    ---->  SDA
    SCL    ---->  SCL
*/

#include <Wire.h>                  // For communication with I2C device - the BNO055 sensor
#include <Adafruit_Sensor.h>       // Adafruit Unified Sensor Library - https://github.com/adafruit/Adafruit_Sensor 
#include <Adafruit_BNO055.h>       // Adafruit BNO055 Library - https://github.com/adafruit/Adafruit_BNO055 
#include <utility/imumaths.h>      // For vector calculations in BNO055 Library - https://github.com/adafruit/Adafruit_BNO055/blob/master/utility/imumaths.h 
#include <Vector.h>                // For saving x, y, z as vectors and subsequent vector calculations - Documentation: https://github.com/janelia-arduino/Vector

// Target Engine - Unity has Y, and Unreal has Z as the vertical Cartesian axes. Slightly differing settings
enum Target_Platform {      // Use Unity for settings with Vertical Y-Axis and Unreal for Settings with Vertical Z-Axis
  Unity,
  Unreal
};

// ACCELEROMETER CLASS DECLARATION ---------------------------------------------------------------------------------------------

class BeginnersBNO055 {             

// PRIVATE MEMBERS ----------------------------------------------------------------  
  private:

    // DEFAULTS -----------------------------------------------------------
    constexpr static int DEF_BAUD_RATE = 9600;      // Default Baud Rate for Serial Communication
    constexpr static int DEF_LATENCY = 45;          // Default latency i.e. Time (in ms) between subsequent Serial Comm transmissions
    const String DEF_DELIMITER = ",";
    const int MARGINS[3] = {30, 30, 30};

    int x = 0, y = 0, z = 0;                  // To store x, y, and z values
    int rotation[3] = {x, y, z};              // To store tilt along the x-axis (Roll), To store tilt along the y-axis (Pitch), To store tilt along the z-axis (Yaw)
    

    int latency = DEF_LATENCY;        // Latency of sending data
    Target_Platform targetPlatform;   // Target Platform
    String delimiter = ",";
    bool newline = true;

    String output;      // String that save the output string in comma separated format i.e. "x,y,z" - Print this to console/serial monitor

    Adafruit_BNO055 bno = Adafruit_BNO055(55);   // BNO055 Object from Adafruit BNO055 Library
    sensors_event_t event;                       // Adafruit Unified Sensor Library Event struct to save event data

// PRIVATE METHODS ------------------------------------------------------------

    void GetEulerAngles() {

      bno.getEvent(&event);        // Get event

      rotation[0] = event.orientation.x;

      if (targetPlatform == Unity) {
        rotation[1] = event.orientation.y;
        rotation[2] = event.orientation.z;
      }
      else {
        rotation[1] = event.orientation.z;
        rotation[2] = event.orientation.y;
      }
    }

    // MODULAR MAP ------------------------------------------------------------------------------------------------------------------
    // Same as mapXYZ but 6 arguments set the min and max range for mapping x, y, and z respectively
    void MapXYZ(float xMin = 0, float xMax = 360, float yMin = -90, float yMax = 90, float zMin = -180, float zMax = 180) {

      bno.getEvent(&event);        // Get event

      x = map(event.orientation.x, 0, 360, xMin, xMax);        // Rotating the ball left to right
      y = map(event.orientation.y, -90, 90, yMin, yMax);       // Tilting the ball sideways
      z = map(event.orientation.z, -180, 180, zMin, zMax);     // Moving the ball front and back

    }


    // RETURN STRING METHOD --------------------------------------------------------------------------------------------------------
    void CreateString() {       // (Ideally) put in loop() of Arduino Programme

      output.remove(0,output.length());  // Flush everything from output
      output = String(rotation[0]) + delimiter + String(rotation[1]) + delimiter + String(rotation[2]);  // Create final string to send out over Serial
      if (newline) output += "\n";

    }
    

    // MODULAR CREATE STRING - CHOOSE WHETHER TO SEND X, Y, and Z VALUES INDIVIDUALLY-------------------------------------------------
    // Three arguments in booleans to choose whether to send x,y, and z values (in that order)
    void ModCreateString(bool sendX, bool sendY, bool sendZ) {

      output.remove(0,output.length());  // Flush everything from output

      if (sendX) {
        output += String(x) + delimiter;
      }
      if(sendY) {
        output += String(y) + delimiter;
      }
      if(sendZ) {
        output += String(z);
      }

      if (newline) output += "\n";

    }

    // TEST STRING PRINT ---------------------------------------------------------------------------------------------------------------
    // For testing and calibration purposes - prints x,y,z with labels.
    // NOT RECOMMENDED to be sent to Serial Montor in final run as labels will require removal in the programme accessing the serial monitor.

    void CreateTestString() {

      output = "X: " + String(x) + ", Y: " + String(y) + ", Z: " + String(z);  // Create final string to send out over Serial

    }

// PUBLIC METHODS ----------------------------------------------------------------  
  
  public:      

    // GETTERS -----------------------------------------------------------
    int getX() {         // Get x
      return x;
    }

    int getY() {         // Get y
      return y;
    }

    int getZ() {         // Get z
      return z;
    }

    int getLatency() {   // Get latency
      return latency;
    }

    // SETUP --------------------------------------------------------------------------------------------------------------

    // Put in setup() of Arduino Programme. This sets up the Accelerometer object from the BNO055 Library
    /* ARGS (All Optional)
        Target Platform - Unity (Y vertical) or Unreal (Z vertical) Engine-like Cartesian Environment (Default: Unity)
        Baud Rate - Rate of Transmission of data over wired Serial Comm connection (Default: 9600)
        Latency - Time (in ms) between subsequent transmissions of data (Default: 45ms)
        Delimiter - Delimiter of created string (Default: ",")
        Newline - Whether to print every Serial Comm string in a new line for easier processing (Default: true i.e. print in new line)
    */

    void SetUpComms(Target_Platform tarPlatform = Unity, int baud_rate = DEF_BAUD_RATE, int lat = DEF_LATENCY, String _delimiter = ",", bool _newline = true) {   
      
      if(!bno.begin())            // Initialise the sensor
      {
        Serial.print("No BNO055 was detected. Please check your connections");  // Error if BNO055 not initialised
        while(1);
      }

      bno.setExtCrystalUse(true);      // Use external crystal for timing
      latency = lat;                   // This adjusts the rate at which the programme sends data to the Serial Monitor
      targetPlatform = tarPlatform;    // Setting custom member values if included as arguments
      newline = _newline;
      delimiter = _delimiter;

      Serial.begin(baud_rate);
      /* 
        - Setting latency is a trade-off between the latency of the movement of the 
          player and camera in-game versus the impact of the natural shaking of one's hand.
        - For example, the minimum recommended latency of 30 will make the camera and player movements 
          smoother by sending data to the serial port every 30 milliseconds but will cause the game to 
          pick up the player's hand motions frequently resulting in camera jitter.
        - A latency close to the maximum recommended latency of 150 will send signals every 150 milliseconds
          making the camera less susceptible to hand jitter but will cause less smooth movement of the 
          player and camera.
        - The recommended range is between 30 and 150. The recommended value is 45.
      */
    }

    
    // LOOP FUNCTIONS -----------------------------------------------------------------------------------------------------


    /* CONTINUOUS ROTATION ------------------------------------------------------------------------------------------------
    Used to continuously stream x, y, and z axis Euler angle orientation values with chosen delimiter (new line for every message).
    This can be linked directly to the Euler angle in an Object's transform to rotate the object accordingly.
    Convert to Quaternion in Unity script firsty since Unity Transform's Rotation Euler angles are Read-only
    */
    void ContinuousRotation() {
      GetEulerAngles();
      CreateString();
      Serial.print(output);
      delay(DEF_LATENCY);

    }
    
    /* DELTA ROTATION ------------------------------------------------------------------------------------------------------
    Used to stream string of format (1,0,1) for example to see if a delta above default margins has been detected
    This can be used to trigger things if the accelerometer has been rotated beyond a margin
    */
    void DeltaRotation() {
      GetEulerAngles();
      bool deltaFound[3] = {false, false, false};
      for (int i = 0; i < 3; i++) {
          if (rotation[i] > MARGINS[i]){
            deltaFound[i] = true;
          }
          else deltaFound[i] = false;
      }
      Serial.print(deltaFound[0] + delimiter + deltaFound[0] + delimiter + deltaFound[0]);

    }

    // END ------------------------------------------------------------------------------------------------------------------------------
    





















};
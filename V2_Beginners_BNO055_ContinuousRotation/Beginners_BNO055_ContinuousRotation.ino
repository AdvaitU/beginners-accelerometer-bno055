#include "./BeginnersBNO055.h"

BeginnersBNO055 acc;          // Create object of the Accelerometer class

// SETUP --------------------------------------------------------------------------------------------------------------------

void setup(void) {

  acc.SetUpComms(Unreal, 9600, 45, ",", true);

}

// LOOP -------------------------------------------------------------------------------------------------------------------

void loop(void) {

  acc.ContinuousRotation();
  
}


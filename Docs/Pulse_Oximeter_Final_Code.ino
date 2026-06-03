//include libraries
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

//initialize sensor
MAX30105 sensor;

//initialize variables
bool sensor_found = false;
long current_time; 
long previous_time; 
long elapsed_time; 
bool first_beat = true;

//create and define any custom functions
float calculateBPM(long ir_values){

  if (checkForBeat(ir_values)){
    if (first_beat){
      current_time = millis();
      first_beat = false; 
      return -1;
    }
      
    previous_time = current_time; 
    current_time = millis();
    elapsed_time = current_time - previous_time; 
    float bpm = 60000.0 / elapsed_time;
    return bpm; 
  }

  else{
    return -1; 
  }
} 


void setup() {
  Serial.begin(9600); //begin serial clock 
 
  if (sensor.begin(Wire)){ //establish connection with the sensor
    sensor_found = true;
  }
  
  if (sensor_found){ //set up the sensor
    sensor.setup();
  }
}

void loop() {
  if (!sensor_found){ //exit function if sensor connection is not successful
    return;
  }

  long ir_values = sensor.getIR(); //retrieve IR values

  float BPM = calculateBPM(ir_values); //call function to calculate BPM 

  if (BPM >= 0){ //print BPM values only if valid values are returned from the function 
    Serial.print("BPM: ");
    Serial.println(BPM); 
  }

}

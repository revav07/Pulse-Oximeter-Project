//include libraries
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

//initialize sensor
MAX30105 sensor;

//initialize variables for bpm 
bool sensor_found = false;
long current_time; 
long previous_time; 
long elapsed_time; 
bool first_beat = true;

//initialize variables for spo2
const int max_size = 50; 
long IR_array[max_size];
long Red_array[max_size]; 
int current_index = 0; 
bool array_is_full = false; 


//create and define any custom functions
float calculateBPM(long ir_values){

  if (checkForBeat(ir_values)){
    Serial.println("BEAT"); 

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

float calculateSPO2(long ir_value, long red_value){
  
  //add readings  to arrays 
  if (current_index >= max_size){
    current_index = current_index % max_size;
    array_is_full = true; 
  }
  IR_array[current_index] = ir_value;
  Red_array[current_index] = red_value;
  current_index = current_index + 1; 

  //when the array is full start calculations
  if (array_is_full){
  
    float ir_sum = 0; 
    float red_sum = 0;
    float ir_min = IR_array[0]; 
    float red_min = Red_array[0]; 
    float ir_max = IR_array[0]; 
    float red_max = Red_array[0]; 
    
    //iterate through each data point in the array 
    for(int i=0; i < max_size; i++){
      //find the sums of the arrays
      ir_sum = IR_array[i] + ir_sum;
      red_sum = Red_array[i] + red_sum;

      //find min/max of arrays
      if(IR_array[i] < ir_min){
        ir_min = IR_array[i];
      }

      if(IR_array[i] > ir_max){
        ir_max = IR_array[i];
      }

      if(Red_array[i] < red_min){
        red_min = Red_array[i];
      }

      if(Red_array[i] > red_max){
        red_max = Red_array[i];
      }
    } 
    //calculate ac and dc values 
    float ir_dc = ir_sum / max_size; 
    float red_dc = red_sum / max_size; 
    float ir_ac = ir_max - ir_min; 
    float red_ac = red_max - red_min; 

    // calculate spo2 
    if (ir_dc <= 0 || red_dc <= 0 || ir_ac <= 0 || red_ac <= 0){ //protect against dividing by 0
      return -1;
    }
    float R = (red_ac / red_dc) / (ir_ac / ir_dc); 
    float spo2 = 104 - 17*R; //found this estimation online 

    return spo2; 
  }

  return -1; 

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
  long red_values = sensor.getRed(); //retrieve Red values 

  float BPM = calculateBPM(ir_values); //call function to calculate BPM 
  float SPO2 = calculateSPO2(ir_values, red_values); //call function to calculate spo2

  //print spo2 and bpm values only if valid; 
  if (BPM >= 0){
    Serial.print("BPM: ");
    Serial.println(BPM);
  }

  
  if (SPO2 >= 0){
    Serial.print("SPO2: ");
    Serial.println(SPO2);
  }

}



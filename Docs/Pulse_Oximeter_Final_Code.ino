//include libraries
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

//initialize sensor
MAX30105 sensor;
bool sensor_found = false;

//initialize variables for bpm system 
bool first_beat = true; 

long current_time; 
long previous_time; 
long elapsed_time; 

const int bpm_max_size = 10; 
long elapsed_array[bpm_max_size]; 
int bpm_index = 0; 
bool bpm_array_full = false; 

//initialize variables for spo2
const int spo2_max_size = 50; 

long IR_array[spo2_max_size];
long Red_array[spo2_max_size]; 

int spo2_index = 0; 
bool spo2_array_full = false; 


//create and define any custom functions
float calculateBPM(long ir_values){

  if (checkForBeat(ir_values)){ 
    //Serial.println("Beat Detected");

    //calculate elapsed time 
    if (first_beat){
      current_time = millis();
      first_beat = false; 
      return -1;
    }
      
    previous_time = current_time; 
    current_time = millis();
    elapsed_time = current_time - previous_time; 

    //ignore outliers
    if (elapsed_time < 300 || elapsed_time > 1500) {
      return -1;
    }

    //store elapsed time in array
    elapsed_array[bpm_index] = elapsed_time; 
    bpm_index++; 

    if(bpm_index >= bpm_max_size){
      bpm_index = 0; 
      bpm_array_full = true; 
    }

    //calculate the average of the array
    if(bpm_array_full){
      
      float elapsed_sum = 0; 

      for(int i=0; i < bpm_max_size; i++){
        elapsed_sum = elapsed_sum + elapsed_array[i]; 
      }

      float avg_time = elapsed_sum / bpm_max_size; 
      float bpm = 60000.0 / avg_time;
      return bpm; 
    }

    return -1; 

  }

  return -1; 

} 

float calculateSPO2(long ir_value, long red_value){
  
  //add readings  to arrays 
  IR_array[spo2_index] = ir_value;
  Red_array[spo2_index] = red_value;
  spo2_index++; 

  if (spo2_index >= spo2_max_size){
    spo2_index = 0;
    spo2_array_full = true; 
  }

  //when the array is full start calculations
  if (spo2_array_full){
  
    float ir_sum = 0; 
    float red_sum = 0;
    float ir_min = IR_array[0]; 
    float red_min = Red_array[0]; 
    float ir_max = IR_array[0]; 
    float red_max = Red_array[0]; 
    
    //iterate through each data point in the array 
    for(int i=0; i < spo2_max_size; i++){
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
    float ir_dc = ir_sum / spo2_max_size; 
    float red_dc = red_sum / spo2_max_size; 
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
  if (BPM >= 0 && SPO2 >= 0){
    Serial.print("BPM: "); 
    Serial.println(BPM); 
    Serial.print("SPO2: ");
    Serial.println(SPO2); 
  }

}

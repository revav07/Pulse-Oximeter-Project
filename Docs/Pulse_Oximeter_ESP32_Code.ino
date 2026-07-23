//include libraries
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//create an BLE characteristic
BLECharacteristic *pCharacteristic;

//initialize sensor
MAX30105 sensor;
bool sensor_found = false;

//initialize variables for bpm system 
unsigned long current_time; 
unsigned long previous_time = 0; 
unsigned long elapsed_time; 
unsigned long avg_time = 0; 

//initialize variables for spo2
const int spo2_max_size = 50; 

long IR_array[spo2_max_size];
long Red_array[spo2_max_size]; 

int spo2_index = 0; 
bool spo2_array_full = false; 

// initialize timer variables 
unsigned long past_time = 0; 
float latest_BPM = -1;
float latest_SPO2 = -1;

//initialize button variables + pin
byte lastButtonState;
bool measuring = false; 
#define BUTTON_PIN 4 

//create and define any custom functions
float calculateBPM(long ir_values){

  if (checkForBeat(ir_values)){ 
    //Serial.println("Beat Detected");

    //calculate elapsed time 
    current_time = millis();
    elapsed_time = current_time - previous_time;   
    previous_time = current_time; 
 

    //ignore outliers
    if (elapsed_time > 300 && elapsed_time < 1500) {
      
      //calculate average time of intervals
      if(avg_time ==0){
        avg_time = elapsed_time; 
      }
      avg_time = 0.8*avg_time + 0.2*elapsed_time; 

      //calculate and return bpm 
      float bpm = 60000.0 / avg_time;
      return bpm; 
    }

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

float updateLatestBPM(float BPM){
  
  if(BPM >=1){
    latest_BPM = BPM; 
  }

  return latest_BPM; 

}

float updateLatestSPO2(float SPO2){

  if(SPO2 >= 1){
    latest_SPO2 = SPO2; 
  }

  return latest_SPO2; 
}

void printTimer(float latest_BPM, float latest_SPO2, float ir_values){
  unsigned long time = millis(); 

  // print if one second has passed
  if(time - past_time >= 1000){
    past_time = time;

    String bpmString;
    String spo2String;

    // format BPM
    if(latest_BPM >= 0){
      bpmString = String(latest_BPM);
    }
    else{
      bpmString = "---";
    }

    // format SpO2
    if(latest_SPO2 >= 0){
      spo2String = String(latest_SPO2);
    }
    else{
      spo2String = "---";
    }

    // Print to Serial Monitor
    Serial.print("IR: ");
    Serial.println(ir_values);

    Serial.print("BPM: ");
    Serial.println(bpmString);

    Serial.print("SpO2: ");
    Serial.println(spo2String);


    // Send over BLE
    String data = "IR: " + String(ir_values) +
                  "\nBPM: " + bpmString +
                  "\nSpO2: " + spo2String;

    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();
  }
}

void setup() {
  Wire.begin(21,22); 
  Serial.begin(115200); //begin serial clock 

  //set up bluetooth (BLE)
  BLEDevice::init("ESP32 Pulse Oximeter");

  BLEServer *server = BLEDevice::createServer();

  BLEService *service = server->createService(
    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
  );

  pCharacteristic = service->createCharacteristic(
    "beb5483e-36e1-4688-b7f5-ea07361b26a8",
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->addDescriptor(new BLE2902());

  service->start();

  server->getAdvertising()->start();

  Serial.println("BLE Started");


  //make sure the sensor is working and found 
  if (sensor.begin(Wire)){
    sensor_found = true;
    Serial.println("MAX30102 found!");
  }
  else {
    Serial.println("MAX30102 not found!");
  }

  if (sensor_found){
    sensor.setup();
    sensor.setPulseAmplitudeRed(0x0A);
    sensor.setPulseAmplitudeGreen(0);
  }

  //set up button 
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  lastButtonState = digitalRead(BUTTON_PIN); 
}


void loop() {
  if (!sensor_found){ //exit function if sensor connection is not successful
    return;
  }

  byte buttonState = digitalRead(BUTTON_PIN); 

  //if button is pressed
  if(buttonState != lastButtonState){
    lastButtonState = buttonState; 
    
    if(buttonState == LOW){
      measuring = !measuring; //toggle monitoring on and off 

      if(measuring){
        Serial.println("Monitoring ON");

        pCharacteristic->setValue("Monitoring ON");
        pCharacteristic->notify();
      }      
      else{
        Serial.println("Monitoring OFF");

        pCharacteristic->setValue("Monitoring OFF");
        pCharacteristic->notify();
      }
      
    }
  }

  //if monitoring is on
  if(measuring){
    sensor.check();

    long ir_values = sensor.getIR(); //retrieve IR values
    long red_values = sensor.getRed(); //retrieve Red values 

    float BPM = calculateBPM(ir_values); //call function to calculate BPM 
    float SPO2 = calculateSPO2(ir_values, red_values); //call function to calculate spo2

    //update latest values 
    latest_BPM = updateLatestBPM(BPM);
    latest_SPO2 = updateLatestSPO2(SPO2);

    //print spo2 and bpm values only if valid
    printTimer(latest_BPM, latest_SPO2, ir_values); 
  }

}

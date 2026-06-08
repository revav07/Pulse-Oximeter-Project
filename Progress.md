# Pulse Oximeter Project Log 

## Day 0 - Preparation and Soldering 
### What I Did 
- Learned soldering basics 
- Successfully soldered header pins into MAX30102 module 

### Challenges
- Keeping header pins aligned while soldering


## Day 1 - Research and Wiring 
### What I Did 
- Researched I2C communication (SDA and SCL wiring)
- Conducted background research on how MAX30102 module works (see the Guide I created in Docs)
- Created schematics diagram 

### Challenges
- Understanding different communication types used by PCBs 
- Utilizing a new platform (easyEDA) to create schematics 


## Day 2 - Calculating BPM 
### What I Did
- wired the MAX30102 to the arduino 
- conducted an I2C scan to ensure the MAX30102 was detected 
- retrieved IR data 
- calculated BPM based on IR data 

### Challenges
- understanding how to download libraries
- creating functions in Arduino IDE


## Day 3 - Calculating SPO2 
### What I Did
- researched how SPO2 is calculated in pulse oximeters
- created a custom function to implement these calculations

### Challenges
- understanding the spo2 calculations 
- translating these calculations into code that is updating in real time 

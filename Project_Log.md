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
- Conducted background research on how MAX30102 module works 
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
- researched how SpO₂ is calculated in pulse oximeters.
- developed a custom function to implement SpO₂ estimation using IR and red light sensor data.

### Challenges
- understanding the underlying SpO₂ calculation method and signal processing involved.
- translating the mathematical process into code that updates in real time using continuously incoming sensor data.


## Day 4 - Smoothing out BPM Calculations
### What I Did
- BPM was previously calculated using a single heartbeat interval, which caused fluctuations.
- implemented a moving average of multiple heartbeat intervals to make readings more reliable.
- reused the rolling average approach previously used in SpO₂ processing.

### Challenges
- adapting the averaging system from SpO₂ (signal data) to BPM (time interval data).
- ensuring the system remains responsive while still reducing noise in the readings.


## Day 5 - Printout Timer and LCD Display 
### What I Did
- implemented a timed update system to display the most recent BPM and SpO₂ readings at 1-second intervals
- assembled and wired an LCD display for real-time data output

### Challenges
- balanced real-time data processing with display updates to prevent flickering and maintain readable output
- resolved incorrect LCD row formatting issues due to wiring configuration and cursor positioning errors


## Day 6 - Updated BPM Calculator
### What I Did
- removed the moving average previously used for bpm calculations
- implemented a running average that was continuously updating and did not require a 5-beat warm up
- this new code allows the bpm to be detected slightly faster

### Challenges
- Evaluating whether the simplified averaging method maintained accuracy while improving responsiveness


## Day 7 - Planning Wearable Prototype
### What I Did
- Researched and compared the Arduino Nano and ESP32 microcontrollers based on size, cost, processing speed, pin availability, connectivity features, and ease of use.
- Selected the ESP32 as the microcontroller for the final wearable prototype due to its built-in Bluetooth and WiFi capabilities. These features will allow the device to communicate with a phone, reducing the need for a physical LCD display and helping minimize the overall device size.
- Decided to use a USB-powered prototype instead of implementing battery power in the first iteration. Adding a battery would require additional components, including a charging circuit and power management system, which would introduce more potential points of failure and increase development complexity.
- Investigated ESP32 boards with integrated battery charging capabilities, but found that many low-cost options had inconsistent documentation and reported battery polarity issues, making them less reliable for an initial prototype.

### Challenges
- Compared different microcontrollers and had to balance: portability vs. ease of prototyping, number of pins vs. board size, cost vs. built-in features

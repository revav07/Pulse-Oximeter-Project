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


## Day 6 - Updating BPM Calculator
### What I Did
- Replaced the previous wraparound array-based moving average used for BPM calculations with an exponential moving average.
- The original method stored multiple BPM readings before calculating an average, requiring a warm-up period of approximately 5 beats and causing significant delays in displaying a stable BPM value.
- Implemented a continuously updating weighted average using:
    - average = 0.8(current_avg) + 0.2(new_value)
- This allowed BPM values to update more quickly while still filtering out sudden fluctuations and maintaining a stable reading.

### Challenges
- Balancing responsiveness and stability when choosing the averaging method and weighting factors.
- Evaluating whether the simplified exponential averaging approach could reduce latency without significantly reducing BPM accuracy.



## Day 7 - Planning Wearable Prototype
### What I Did
- Researched and compared the Arduino Nano and ESP32 microcontrollers based on size, cost, processing speed, pin availability, connectivity features, and ease of use.
- Selected the ESP32 as the microcontroller for the final wearable prototype due to its built-in Bluetooth and WiFi capabilities. These features will allow the device to communicate with a phone, reducing the need for a physical LCD display and helping minimize the overall device size.
- Decided to use a USB-powered prototype instead of implementing battery power in the first iteration. Adding a battery would require additional components, including a charging circuit and power management system, which would introduce more potential points of failure and increase development complexity.
- Investigated ESP32 boards with integrated battery charging capabilities, but found that many low-cost options had inconsistent documentation and reported battery polarity issues, making them less reliable for an initial prototype.

### Challenges
- Compared different microcontrollers and had to balance: 
    - Portability vs. ease of prototyping
    - Number of pins vs. board size
    - Cost vs. built-in features


## Day 8 - Understanding ESP32 and Creating Schematics
### What I Did
- Researched the ESP32 pin layout and functionality, including different pin types such as ADC, DAC, digital input/output, and communication pins.
- Learned how to interface the MAX30102 sensor with the ESP32, including the use of I2C communication through the SDA and SCL pins (GPIO21 and GPIO22).
- Created a detailed wiring schematic in EasyEDA to document the ESP32 pulse oximeter hardware configuration.

### Challenges
- Transitioning from the Arduino Uno to the ESP32 required learning a new pin architecture and understanding the differences in available peripherals and GPIO functionality.
- Familiarizing myself with the ESP32's more flexible but complex hardware configuration compared to the simpler Arduino platform.


## Day 9 - Integrating ESP32 and Streamlining BPM Code
### What I Did
- Rewired the pulse oximeter prototype by replacing the Arduino Uno and LCD display with an ESP32 microcontroller.
- Installed the ESP32 board package in the Arduino IDE and adapted the original Arduino code for ESP32 compatibility.
- Refactored the BPM detection code by comparing it with the SparkFun MAX30105 example and improving the filtering logic.
- Reduced initial heartbeat detection time from 20+ seconds to approximately 10–15 seconds by improving sensor initialization and BPM calculation logic.

### Challenges
- The ESP32 was initially not detected by the Arduino IDE. I resolved this by using a USB cable with data-transfer capability and installing the appropriate CP210x USB-to-UART driver for serial communication.
- The first ESP32 compilation required downloading additional packages, which caused Arduino IDE to become extremely slow and temporarily froze my computer. After restarting, subsequent uploads worked normally.
- The original BPM algorithm had a long delay before detecting valid heartbeats, and BPM values were not being calculated reliably afterward. I analyzed the SparkFun MAX30105 example code and improved my implementation by:
    - Adjusting sensor initialization parameters.
    - Changing the BPM filtering approach from rejecting invalid results after calculation to accepting only valid heartbeat intervals.
    - Simplifying the BPM calculation flow to improve responsiveness.


## Day 10 - Adding Button Toggling
### What I Did
- Added a physical button to toggle the pulse oximeter monitoring system on and off.
- Modified the code to incorporate the button functionality, allowing the user to manually start and stop measurements.
- This prevents the ESP32 from immediately collecting sensor data when it is powered on.

### Challenges
- Initially, the button was connected to what I believed was a second GND pin on the ESP32, but pressing the button did not change the system state.
- Created a separate test sketch to isolate the button functionality and found that the button input was always being read as HIGH.
- Checked the wiring and tested the button using an external pull-up resistor instead of the ESP32's internal pull-up resistor, but the issue remained.
- After further debugging, I discovered that the pin I was using was not actually connected to GND.
- Created a shared GND rail on the breadboard and connected all components to the same ground reference, which resolved the issue.
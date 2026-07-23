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


## Day 11 - Establishing Bluetooth Connection
### What I Did
- Modified the code to support iOS Bluetooth connectivity
- Implemented BLE communication using the ESP32 and created a BLE characteristic to transmit sensor data.
- Tested the wireless connection using the nRF Connect app on iPhone and successfully received data from the ESP32.

### Challenges
- In my original implementation used BluetoothSerial, which relies on Bluetooth Classic and is not supported for custom device communication on iOS.
- Since I am developing with an iPhone, I had to replace the Bluetooth communication system with BLE and learn a new communication protocol.
- BLE was more complex because it uses a server, services, and characteristics rather than a simple serial communication stream.


## Day 12 - Soldering ESP32 + Button
### What I Did
- Soldered the ESP32 header pins onto a 5 × 7 cm perfboard to prepare it for permanent assembly.
- Soldered a push button onto the perfboard and connected it to the ESP32 GPIO input.
- Tested the button functionality using a simple test sketch to verify proper communication between the button and ESP32.

### Challenges
- Improved my soldering technique by practicing with stranded wires, wire stripping, and creating clean, reliable solder joints.
- The button placement resulted in a short connection distance between pads, requiring a solder bridge to establish the connection.
- Accidentally connected the button to the wrong GPIO pin and used solder wick to remove the connection before rewiring it correctly.


## Day 13 - 3D Printing ESP32 Compartment
### What I Did
- Carefully measured the dimensions of the perfboard and component layout to create an accurate CAD model.
- Used **Onshape** to design a custom enclosure for the ESP32 module that could be mounted on the wrist.
- Added raised alignment pegs that fit into the perfboard mounting holes to secure the board and prevent shifting during use.
- Designed a lower wire-routing compartment to allow connections to exit cleanly while protecting the electronics.
- Incorporated clearances and tolerances into the design to ensure the enclosure could be assembled without damaging components.

### Challenges
- Taking precise measurements of small hardware components and translating them into an accurate CAD model.
- Designing around existing electronics while ensuring that pressure points or tight fits would not damage the components.
- Getting re-acclimated to CAD workflows and constraints after several years without using Onshape.


## Day 14 - 3D Printing MAX30102 Holder
### What I Did
- Used **Onshape** to design a custom enclosure for the MAX30102 module that can be secured to the pointer finger.
- Designed a hole along the back wall of the enclosure to allow wire connections to exit while protecting the sensor.
- Added side slots for an elastic strap to thread through and secure the module comfortably.
- Created a lid with a cutout to allow light from the sensor LEDs to reach the finger.
- After printing the enclosure, soldered the wires connecting the ESP32 to the MAX30102 sensor pads.
- Ran the code once and ensured all the hardware was connected properly 

### Challenges
- Soldering was challenging because the stranded wires were flexible and difficult to hold in place while attaching them to the small MAX30102 pads.
- The first lid design had excessive tolerance, causing it to fit too loosely and slide off the enclosure.
- Redesigned the lid with reduced clearance and a smaller cutout, improving the fit but still not achieving a tight friction fit.
- Plan to use adhesive to permanently secure the lid while keeping the sensor accessible.
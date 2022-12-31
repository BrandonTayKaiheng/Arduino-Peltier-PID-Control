# Arduino-PID-Temperature-for-Peltier

The classic Proportional-Integral_Differential (PID) Control Algorithm is implemented digitally using an Arduino Uno microcontroller to regulate the temperature of a Peltier thermoelectric device. Here, the Peltier device maintains its temperature of 12 degree Celsius regardless of changing ambient temperature. The temperature response can be monitored using the Serial Monitor, or Data Streamer on Microsoft Excel. Controller gains should be experimentally tuned upon implementation for best results. 

## Components Required
1. Logic Level MOSFET x1 (IRLZ44N or any equivalent)
2. TEC Peltier Module x1
3. 12V fan x1
4. Heat Sink x1
5. Arduino Uno x1 (or equivalent microcontroller)
6. Analog Temperature Sensor x2 
7. Power source
(See schematic for connection)

## Control System Architecture
* To implment PID control digitally, the standard PID equation in Time Domain is discretized (See Equation.png)
* The error term is e(t) is the calculated difference between the Peltier and the Ambient temperature sampled
* The sensor provides negative feedback to complete the control loop (See BlockDiagram.png)

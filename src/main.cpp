#include <Arduino.h>  // Delete if running on Arduino IDE

// Declare Pins
#define sensorPin1 A0
#define sensorPin2 A1
const int PELTIER = 6;

// Temperature Sensor variables
float sensor_value_1;  // Sensor placed on Peltier
float sensor_value_2;  // Sensor detecting ambient temperature
float sensor_value;
float volt1;
float volt2;
float T1;  // Peltier Temperature
float T2;  // Ambient Temperature

double output;
double input;
double setpoint;

// PID Controller variables (Gains K tuned experimentally)
double kp = 3.8;  // Proportional gain
double ki = 0.2;  // Integral gain
double kd = 0;  // Derivative gain
double elapsed_time; // Sample time in ms
unsigned long prev_time, current_time;
double total_error, prev_error, rate_error, error;
double control_volts;

// Excel Data Streamer variables
unsigned long duration = 0;
unsigned long PreviousTime = 0;
const int RefreshInterval = 1000;  


void setup() {

  setpoint = 12; // Temperature of the Peltier we desire (12 degrees)
  Serial.begin( 9600);
  //Serial.println('Initializing...');
  pinMode(PELTIER, OUTPUT);
  analogReference(5);    // Setting a 5V analog reference

}

// Compute and return the temperature difference to be compared against the setpoint value
double read_temp() {

  sensor_value_1 = analogRead(sensorPin1);    
  sensor_value_2 = analogRead(sensorPin2);
  
  // Convert readings from 10 bit ADC to voltage
  volt1 = (float(sensor_value_1) / 1024) * 5; 
  volt2 = (float(sensor_value_2) / 1024) * 5;
  
  // Convert voltage into temperature (based on sensor calibration)
  T1 = (volt1 - 0.40) / 0.0204;   // (Vin - V0)/Tc 
  T2 = (volt2 - 0.40) / 0.0204;

  return T1;

}

// Implementation of PID feedback controller to calculate the duty cycle of the Pulse Width Modulation
// signal used to control the Peltier output. 
double PID(double temp_sensed) {

  current_time = millis(); // Return time passed since arduino started running
  elapsed_time = current_time - prev_time;

  error = temp_sensed - setpoint; // Proportional Control
  total_error += error;  // Integral Control
  rate_error = error - prev_error; // Derivative Control

  double out = kp * error + ki * (elapsed_time * total_error) + kd*(rate_error/elapsed_time);

  prev_error = error;
  prev_time = current_time;

  if (out > 5) {
    out = 5;  
  }

  if (out < 0) {
    out = 0;
  }

  return (out / 5) * 255; 

}


// Serial output 
void SendData() {
  
  Serial.print("Ambient_Temperature:");
  Serial.print(T2);  // Ambient Temperature
  Serial.print(",");
  Serial.print("Sensor_Temperature:");
  Serial.print(T1); // Sensor Temperature
  Serial.print(",");
  Serial.print("Error:");
  Serial.print(error);
  Serial.print(",");
  Serial.print("Controller_Output:");
  Serial.print(control_volts, 4);  // Average voltage of PWM from controller
  Serial.print(",");
  Serial.print(output, 4); // 8 bit PWM 0 to 255
  Serial.print(",");
  Serial.println();

}

// Serial data to Excel data streamer
void OutgoingSerial() {

  duration = millis() - PreviousTime;
  if (duration > RefreshInterval) {
    PreviousTime = millis();
    SendData();
  }

}

// Main loop
void loop() {

  input = read_temp(); // Read current temperature
  output = PID(input);    // Calculate PID
  analogWrite(PELTIER, output); // PWM signal to control Peltier
  output = output / 100;
  control_volts = (output/2.55)*5;
  OutgoingSerial();

}

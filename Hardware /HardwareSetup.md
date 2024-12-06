Hardware Guide for Canine Alert System
This section provides detailed instructions on the hardware components, connections, and assembly for the Canine Alert System.

Hardware Components
1. ESP32-CAM Module
Function: Real-time dog detection using a camera.
Specifications:
Camera: OV2640 (2MP)
Connectivity: Wi-Fi and Bluetooth
Power Supply: 5V via micro-USB or external battery
2. OLED Display
Function: Displays alerts for the bike rider.
Recommended Type: 0.96-inch OLED (I2C interface)
Connection Pins:
VCC: Connect to 3.3V (ESP32-CAM)
GND: Connect to GND (ESP32-CAM)
SCL: Connect to GPIO22 (I2C Clock)
SDA: Connect to GPIO21 (I2C Data)
3. Buzzer
Function: Emits a sound to alert the rider and deter the dog.
Specifications:
Type: Active buzzer
Voltage: 3.3V–5V
Connection:
+ (Positive): Connect to GPIO13 (PWM pin)
- (Negative): Connect to GND
4. Servo Motor
Function: Deploys a protective cover for the rider’s legs.
Recommended Type: SG90 Micro Servo
Connection:
VCC: Connect to an external 5V power supply.
GND: Connect to GND.
Signal: Connect to GPIO12.
5. Power Supply
Function: Provides power to all components.
Options:
5V power bank with USB cable for ESP32-CAM
Dedicated LiPo battery for servo motor (to avoid voltage drops)
6. Protective Covers
Function: Shields the rider’s legs from chasing dogs.
Specifications:
Material: Lightweight, durable plastic or carbon fiber
Mount: Attached to servo arms for deployment
Hardware Assembly
Prepare the ESP32-CAM:

Connect the OLED display, buzzer, and servo motor to the corresponding pins on the ESP32-CAM as mentioned above.
Mount Components on the Bike:

Secure the ESP32-CAM module on the bike’s handlebar for an optimal camera view.
Attach the OLED display next to the module for easy visibility.
Mount the buzzer in an open space to maximize sound output.
Attach the servo motors to the bike’s frame near the legs, ensuring smooth deployment of the protective covers.
Power Configuration:

Use a 5V power bank to power the ESP32-CAM and OLED.
Use a separate battery for the servo motor to handle its power demands.
Cable Management:

Securely route and fasten wires to avoid interference with bike operation.
Circuit Diagram
Components and Connections:
markdown
Copy code
ESP32-CAM:
  - GPIO21 -> SDA (OLED)
  - GPIO22 -> SCL (OLED)
  - GPIO13 -> Positive (Buzzer)
  - GPIO12 -> Signal (Servo Motor)
  - 3.3V  -> VCC (OLED and Buzzer)
  - GND   -> GND (OLED, Buzzer, and Servo Motor)
Hardware Tips
Power Stability: Ensure a stable 5V power source for consistent performance.
Servo Stress: Avoid overloading the servo; use lightweight protective covers.
Camera Focus: Adjust the ESP32-CAM's focus for clear detection.
Weatherproofing: Use protective casings for electronics to prevent damage from rain or dust.

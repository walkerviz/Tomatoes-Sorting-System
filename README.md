# Automated Tomato Sorting System
This project is an automated tomato sorting system designed to categorize tomatoes by their weight and color. It utilizes a combination of sensors and a microcontroller to streamline the sorting process, improving both efficiency and accuracy.

## Features
* Automated Sorting: The system automatically sorts tomatoes based on predefined weight and color criteria.
* Dual-Sensor Data Collection: Utilizes a load cell for precise weight measurements and a color sensor for accurate color detection.
* Real-time Monitoring: Data for each tomato is processed and displayed instantly on a 20x4 LCD screen.
* Enhanced Efficiency: The automated process streamlines sorting, reducing manual labor and increasing accuracy.

## Images
![Picture2](https://github.com/user-attachments/assets/1f7b5f77-3637-494b-aeeb-0332f3a5303b)

## Hardware Requirements
* Microcontroller: Arduino Mega (or similar)
* Weight Measurement:
  * Load Cell
  * HX711 Module
* Color Detection:
  * TCS34725 Color Sensor
* Display:
  * 20x4 LCD Screen
 
## Software & Protocols
* Firmware: Developed for the microcontroller to read sensor data, process the sorting logic and control the LCD display.
* Sensor Libraries:
  * I2C protocol interfaces for LCD display, LyquidCrystal library is needed.
  * Requires libraries for the HX711 and TCS34725 sensors to interface with the microcontroller.

## How It Works
1. A tomato is placed on the platform.
2. The load cell and HX711 module measure the tomato's weight.
3. The TCS34725 color sensor reads the tomato's color.
4. The microcontroller processes both the weight and color data.
5. Based on predefined criteria, the system determines the correct category for the tomato (e.g., ripe, unripe, overweight, etc.).
6. The results are displayed on the 20x4 LCD screen.

This system aims to simplify the sorting process, making it a valuable tool for agricultural applications, small-scale farming or educational projects.

## Contributing
This project is open to contributions. If you'd like to improve the code, add new features or fix bugs, please feel free to submit a pull request.





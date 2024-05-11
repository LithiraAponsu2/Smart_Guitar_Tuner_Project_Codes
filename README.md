# Smart Guitar Tuner with Raspberry Pi Pico

This repository contains the code files for a Smart Guitar Tuner project (Tune Bot). The tuner is designed with a Raspberry Pi Pico, Analog Mic 4466, 0.91 OLED display, L9110S DC H bridge, 12V 170RPM DC motor, TP4056 Adjustable step-up charging module, and a 1000mAh Li-ion battery.

## Components

- **Raspberry Pi Pico**: The core of the project, responsible for controlling the other components.
- **Analog Mic 4466**: Listens to the sound of the guitar string being plucked.
- **0.91 OLED Display**: Displays the tuning information.
- **L9110S DC H Bridge**: Controls the DC motor.
- **12V 170RPM DC Motor**: Turns the tuning pegs of the guitar.
- **TP4056 Adjustable Step-up Charging Module**: Manages the power supply.
- **1000mAh Li-ion Battery**: Powers the device.

## Code

The code for this project is mainly written in C, with some test files written in Python. The C code controls the hardware components, while the Python code is used for testing purposes.

## Design

This project is designed as a full product, with a focus on usability and efficiency. The tuner listens to the sound of a string being plucked and decides which note it needs to be tuned to. It then automatically turns the tuning keys on the guitar's headstock just the right amount until it achieves the correct note[^1^][1][^2^][2].

## Images

Images of the product are attached in the repository.
![WhatsApp Image 2024-05-11 at 15 30 54_c95ca86e](https://github.com/LithiraAponsu2/Smart_Guitar_Tuner_Project_Codes/assets/95391677/725f2309-9e4b-4882-9a15-3b438fd6f692)
![WhatsApp Image 2024-05-11 at 15 30 54_bcb8b091](https://github.com/LithiraAponsu2/Smart_Guitar_Tuner_Project_Codes/assets/95391677/ff1bd8c2-a735-4252-8b27-f35d5fc2bc75)
![WhatsApp Image 2024-05-11 at 15 30 54_c2b3141b](https://github.com/LithiraAponsu2/Smart_Guitar_Tuner_Project_Codes/assets/95391677/0804d302-a939-4d7d-ba7a-f330d685151b)



# Simple_DS18B20_One_Wire

The programs allow to operate DS18B20 1-Wire temperature sensor via Nucleo L476RGT6 board, using bit-banging or UART.  
All the code was made using STM32 Cube IDE, so it can be easily migrated to another STM32-type board and used in simple projects.  

The code is a part of my STM32 training with an online course from polish website Forbot.pl, but I find it nice so I decided to share it. It's mainly focused on being easy to understand and as simple as possible.  

There are two versions that communicate via 1-Wire differently - one uses bit-banging, the other uses UART (more efficient way, but needs additional peripheral that is free).

The project uses only the UART pins to communicate with PC and just one pin to talk with temperature sensor (excluding power and GND).
You can also omit VCC and power with just a signal line, but this project was tested only for "VCC, GND, SIG" version.

![sensor](https://github.com/user-attachments/assets/d90b0126-5c40-4fc2-9b9d-61d09cf96840)

The library is in files ds18b20.h/.c and wire.h/.c. (note that the two different versions have the same filenames but are in separate example projects).

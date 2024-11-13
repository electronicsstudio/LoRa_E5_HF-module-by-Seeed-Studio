# LoRa_E5_HF-module-by-Seeed-Studio.
In this repository, you will learn the complete details about the LoRa_E5_HF module by Seeed Studio.

/* So, guys, in this repository, I’ve provided all the reference material related to the LoRa_E5_HF module by Seeed Studio. You’ll find two 
   code files: “Receiver_Code_Trial_1” and “Transmitter_Code_Trial_1,” which you can use for reference. */
   
Transmitter_Code_Trial_1

In this code, I created a setup for the LoRa-E5 module using the LoRa module and the RTC DS3231 module with an Arduino UNO controller. First, I obtain the current time from the RTC, then transmit this time using the LoRa module over a command (CMD). The RTC module is connected via I2C, which is available on the UNO board, and the LoRa module uses pins 6 (TX) and 7 (RX).

To use this LoRa module, we must first send a CMD after powering up, then we can transmit data to the desired location.


Receiver_Code_Trial_1

In this code, I created a setup for the LoRa-E5 module using the LoRa module and an ESP32. First, I send a CMD to set it up as a receiver, after which it displays the data coming from the transmitter side.

To use this LoRa module, we must first send a CMD after powering up, and then we can receive data as needed.

The circuit Diagra;
ESP32 Devkit V1 (GPIO17, GPIO16, GND, 5v) to ESP_CAM(UOR, UOT, GND, 5V)


1. ESP32 send data at 921600 baud rate. This high baud rate works fine without any problem. May be even a higher baudrate can be used. Should be tested before using.
2. The sended data from ESP32 Devkit v1 is of this form   "s44,33,22,11,11.22, 22,33,n" .
3. The ESP CAM receives the data and removes the s at the start and ,n at the end and store the data into a csv file named log.


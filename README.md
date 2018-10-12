# ADS1232
ADS1232/ADS1234 Arduino Library

Here is an Arduino Library for ADS1232/ADS1234.

Features:

- usage of the 1st differential input.
- usage of the onboard oscillator.

Missing features:

- this library doens't support reading from 2nd (3rd & 4th for ADS1234) differential input.
- offset calibration
- sleep mode

Below you can see some pictures from test circuit and a Fritzing diagram.

For test circuit I've used **CJMCU-1232** development board and Arduino UNO.

![test setup 1](https://github.com/ciorceri/ADS1232/raw/master/img/test_setup_1.jpg)
![test setup 2](https://github.com/ciorceri/ADS1232/raw/master/img/test_setup_2.jpg)

Below is a Fritzing diagram with connection details.

![fritzing diagram](https://github.com/ciorceri/ADS1232/raw/master/img/fritzing_diagram.png)

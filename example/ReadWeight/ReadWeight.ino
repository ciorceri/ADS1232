#define _dout 6
#define _sclk 7
#define _pdwn 8

#include "ADS1232.h"

ADS1232 weight = ADS1232(_pdwn, _sclk, _dout);

void do_calibration() {
  long t_new_offset = 0;
  long t_raw_read = 0;
  float t_set_scale_value = 0;
  float t_weight = 0;

  // reset to default values
  weight.OFFSET = 0;
  weight.SCALE = 1.0;

  // tare
  t_new_offset = weight.raw_read(3);
  weight.OFFSET = t_new_offset;
  Serial.print("Calibration offset = ");Serial.println(weight.OFFSET);
  Serial.println("You have 10 seconds to put a 2L CocaCola bottle on scale");
  delay(10000);

  // do calibration based on a known weight
  t_raw_read = weight.raw_read(3);
  Serial.print("Units read = ");Serial.println(t_raw_read);
  t_set_scale_value = t_raw_read / 2.0;  // divide it to the weight of a CocaCola bottle
  weight.SCALE = t_set_scale_value;
  Serial.print("Calibration scale value = ");Serial.println(weight.SCALE);

  // read weight
  t_weight = weight.units_read(3);
  Serial.print("Weight = ");Serial.println(t_weight);
}

void setup() {
  Serial.begin(9600);
  weight.power_up();
  do_calibration();
}

void loop() {
  Serial.println(weight.units_read(3));
  delay(1000);
}

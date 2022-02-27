/*
* Copyright (C) 2022 Congduc Pham, University of Pau, France
*
* Congduc.Pham@univ-pau.fr
*/

#ifndef WATERMARK_H
#define WATERMARK_H
#include "Sensor.h"

#define WATERMARKANALOG_SCALE _BOARD_MVOLT_SCALE

class watermark : public Sensor {
  public:    
    watermark(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, int pin_read, int pin_power, int pin_trigger);
    void update_data();
    double get_value();
};

#endif

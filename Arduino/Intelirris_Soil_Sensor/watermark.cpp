/*
* Copyright (C) 2022 Congduc Pham, University of Pau, France
*
* based on code from IRD (N. Silvera, J-F. Printanier)
* 
* Congduc.Pham@univ-pau.fr
*/

#include "watermark.h"

watermark::watermark(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, int pin_read, int pin_power, int pin_trigger):Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power, pin_trigger){
  if (get_is_connected()){

    if (get_pin_read()!=-1)
    	pinMode(get_pin_read(), INPUT);
	
    set_warmup_time(0);
  }
}

void watermark::update_data()
{	
  if (get_is_connected()) {
  	
		// All pin high Z, probably not needed with only 1 watermark 
		pinMode(get_pin_power(), INPUT);
    pinMode(get_pin_trigger(), INPUT);

    // wait
    delay(get_warmup_time());
    
    uint32_t k=0;
    uint32_t l=0;
    
		pinMode(get_pin_power(), OUTPUT);
    pinMode(get_pin_trigger(), OUTPUT);

    for (int s=0; s<get_n_sample(); s++)
    {     
      digitalWrite(get_pin_power(),LOW);
      digitalWrite(get_pin_trigger(),HIGH);
 //     delay(0.1); // change a litle, not important
      k += analogRead(get_pin_read());
    
      digitalWrite(get_pin_power(),HIGH);
      digitalWrite(get_pin_trigger(),LOW);
 //     delay(0.1); 
      l += analogRead(get_pin_read());
    }
    
    digitalWrite(get_pin_power(),LOW); // some time low is good
    
    k = 10000*k/l;
    Serial.println(k);

		set_data(k/1.0);

    //probably not needed with only 1 watermark 
    pinMode(get_pin_power(), INPUT); // hZ
    pinMode(get_pin_trigger(), INPUT); // hZ

  	// All pin low Z to avoid interferences
		pinMode(get_pin_power(), OUTPUT);
    pinMode(get_pin_trigger(), OUTPUT);       
  }
  else {
  		// if not connected, set a random value (for testing)  	
  		if (has_fake_data())
    		set_data((double)random(0,1024));
  }
}

double watermark::get_value()
{
  update_data();
  return get_data();
}

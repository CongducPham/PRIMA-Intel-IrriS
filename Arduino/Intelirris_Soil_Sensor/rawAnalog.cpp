/*
* Copyright (C) 2016-2021 Congduc Pham, University of Pau, France
*
* Congduc.Pham@univ-pau.fr
*/


#include "rawAnalog.h"

rawAnalog::rawAnalog(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, int pin_read, int pin_power):Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power){
  if (get_is_connected()){

    if (get_pin_read()!=-1)
    	pinMode(get_pin_read(), INPUT);
    	
    if (get_pin_power()!=-1) {	
    	pinMode(get_pin_power(),OUTPUT); 
    
			if (get_is_low_power())
       	digitalWrite(get_pin_power(),LOW);
    	else
				digitalWrite(get_pin_power(),HIGH);
		}
	
    set_warmup_time(100);
  }
}

void rawAnalog::update_data()
{	
  if (get_is_connected()) {
  	
    int aux_value = 0;
    int value = 0;
    
    // if we use a digital pin to power the sensor...
    if (get_is_low_power())
    	digitalWrite(get_pin_power(),HIGH);
    	
    // wait
    delay(get_warmup_time());
   
    for(int i=0; i<get_n_sample(); i++) {
    	
    	if (get_is_analog()) {
        	aux_value = analogRead(get_pin_read());
          //Serial.println(aux_value);
      }
         
      value += aux_value;
      delay(10);
	}
	
    if (get_is_low_power())        
        digitalWrite(get_pin_power(),LOW);
        
    // getting the average
    //Serial.println(value);
    set_data(value/(double)get_n_sample());        
  }
  else {
  		// if not connected, set a random value (for testing)  	
  		if (has_fake_data())
    		set_data((double)random(0,1024));
  }
}

double rawAnalog::get_value()
{
  update_data();
  return get_data();
}

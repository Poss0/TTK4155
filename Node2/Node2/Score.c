#include <avr/delay.h>
#include "ADC.h"

//Set a boolean value to say if the core has been increased or not already

int get_score(int score){
	int goal = ADC_convert();
	if(goal < 50){
		score ++;
		//while(ADC_convert() < 100);
	}
	return score;
}

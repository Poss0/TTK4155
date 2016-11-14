#include <avr/delay.h>
#include "ADC.h"

int scoreUpdated = 0;

int get_score(int score){
	int goal = ADC_convert();
	if(goal < 50 && scoreUpdated == 0){
		score ++;
		scoreUpdated = 1;
	}
	else if(goal > 100){
		scoreUpdated = 0;
	}
	return score;
}

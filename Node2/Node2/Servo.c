
void Servo_init(){
	//Set OC1A as an output
	DDRB |= (1 << PB5);
	//Select fast PWM mode
	COM0A0 = 1;
	COM0A1 = 1;
	//Set top value
	TCNT0 = 
	//Set compare value
	OCR0A = 
}

#include "FastSin.h"
#include <TimerThree.h>


#define debugPin	 (8)

//use timer3 for PWM
//T3A pin 5  ; T3B pin 2  ; T3C pin 3
#define  phaseA  (5)
#define  phaseB  (2)
#define  phaseC  (3)

unsigned int degree = 0;  // to count degree: 0, 6, 12, 18, ... 354, 0, 6, 12,... 
unsigned int SV_angle = 0; //angle of space vector: from [0 to 360)
float        SV_magnitude = 0.8f; //magnitude of space vector: from [0 to 1]


//timer3 interupt
void timer3Int(void)  //278us
{
	//debug pin high when begin interupt
	digitalWrite(debugPin, HIGH);

	if (degree == 360){
		degree = 0;
	}
	else{
		degree += 6;
	}

	SV_angle = degree;
	SVPWM_run(SV_angle, SV_magnitude);

	//debug pin low when exit interupt
	digitalWrite(debugPin, LOW);
}

void setup()
{
	pinMode(debugPin, OUTPUT);
	Timer3.initialize(278*5); //timer period us
	// we divide 360 degree (1 sin wave cycle (~16,667us) into 60 part)
	// 16,667 / 60 ~ 278
	// real cycle = 278*60 = 16,680 us  (~ 60hz)
	// pwm freq : 1/ 278us ~ 3.6kHz
  

	//call these func to setup ports to ouput PWM // These are prettly slow compare to  Timer3.setPwmDuty
	Timer3.pwm(phaseA, 0);
	Timer3.pwm(phaseB, 0);
	Timer3.pwm(phaseC, 0);

	Timer3.attachInterrupt(timer3Int); // Timer 3 overflow will call timer3Int()

	Serial.begin(9600);
}

void loop()
{
  
  
}

//this func will calculate and output PWM for 3 switches from angle and magnitude of space vector
void SVPWM_run(unsigned int a, float m)
{
	// use "static" to make it a little bit faster. But consume more memory
	static float Ualpha, Ubeta;
	static float X, Y, Z;
	static float PWMa, PWMb, PWMc;
	uint8_t sector;

	//change from polar to rectangular coordinate
	Ualpha = m*icos(a);
	Ubeta = m*isin(a);

	//X Y Z is temporary variables for convenient
        X = Ubeta;		//Ubeta
        Y = Ubeta*0.5f + Ualpha*0.8660254f;   // Ubeta/2 + Ualpha*sqrt(3)/2
        Z = Y - X;			// -Ubeta/2 + Ualpha*sqrt(3)/2


	if (a < 60){
		sector = 1;
	}
	else if (a < 120){
		sector = 2;
	}
	else if (a < 180){
		sector = 3;
	}
	else if (a < 240){
		sector = 4;
	}
	else if (a < 300){
		sector = 5;
	}
	else{
		sector = 6;
	}


	// the calculation below is taken from TI code. I it would take a long time to prove it mathematically.
	// So just use the result.. lol.. :)
	switch (sector){
	case 1:   // sector 1 or 4
        case 4:
		PWMa = Y;  
		PWMb = X - Z;		
		PWMc = -Y;		
		break;
	case 2:  // sector 2 or 5
        case 5:
		PWMa = Z + Y;  
		PWMb = X;		
		PWMc = -X;	
		break;
	default: // sector 3 or 6
                PWMa = Z;  
		PWMb = -Z;		
		PWMc = -(X+Y);
		break;
	}

	//after the calculation, PWM is a number from -1 to 1
	// we scale it to 0 to 1
        PWMa = (PWMa+1)*0.5;
        PWMb = (PWMb+1)*0.5;
        PWMc = (PWMc+1)*0.5;

	//maximum PWM for each pin is 1023. We just need the percentage of 1023
	Timer3.setPwmDuty(phaseA, 1023 * PWMa);
	Timer3.setPwmDuty(phaseB, 1023 * PWMb);
	Timer3.setPwmDuty(phaseC, 1023 * PWMc);

	//this is for debugging purposes
	//Serial.print("\n");
	//Serial.print(a); Serial.print("\t"); Serial.print(sector);
	//Serial.print("\n");
        //Serial.print(Ualpha, 5); Serial.print("\t");Serial.print(Ubeta, 5);Serial.print("\t");Serial.print(sqrt(Ubeta*Ubeta + Ualpha*Ualpha), 5);
	//Serial.print(sector);Serial.print("\t");Serial.print(PWMa, 5); Serial.print("\t"); Serial.print(PWMb, 5); Serial.print("\t"); Serial.print(PWMc, 5);
}

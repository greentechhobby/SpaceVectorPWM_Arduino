#include "FastSin.h"
#include <TimerThree.h>

const int led = LED_BUILTIN;  // the pin with a LED
const int debugPin = 8;

//use timer3 for PWM
//T3A pin 5  ; T3B pin 2  ; T3C pin 3
const int phaseA = 5;
const int phaseB = 2;
const int phaseC = 3;

unsigned int degree = 0;  // to count degree: 0, 6, 12, 18, ... 354, 0, 6, 12,... 
unsigned int SV_angle = 0; //angle of space vector: from [0 to 360)
float        SV_magnitude = 0.8f; //magnitude of space vector: from [0 to 1]

void timer3Int(void)
{
	digitalWrite(debugPin, HIGH);
  
	if(degree == 360){
		degree = 0; 
	}else{
		degree += 6;
	}
        
        SV_angle = degree;
	SVPWM_run(SV_angle, SV_magnitude);

	digitalWrite(debugPin, LOW);
}






void SVPWM_run(unsigned int a, float m)
{
	float Ualpha, Ubeta;
	float X, Y, Z;
	float t1, t2;
	float PWMa, PWMb, PWMc;
	uint8_t sector;
  
	Ualpha = m*icos(a);
	Ubeta  = m*isin(a);  
  
	X = Ubeta;
	Y = Ubeta*0.5f + Ualpha*0.8660254f; //(Ubeta + Ualpha*sqrt(3))/2
	Z = X - Y;				//(Ubeta - Ualpha*sqrt(3))/2
  
	sector = 3;
	sector = (Y > 0) ? (sector - 1) : sector;
	sector = (Z < 0) ? (sector - 1) : sector;
	sector = (X < 0) ? (7 - sector) : sector;

	switch (sector){
	case 0:
		PWMa = 0;
		PWMb = 0;
		PWMb = 0;
		break;
	case 1:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	case 2:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	case 3:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	case 4:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	case 5:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	case 6:
		t1 = Z;
		t2 = Y;
		PWMb = (1 - t1 - t2)*0.5f;  // tbon = (1-t1-t2)/2
		PWMa = PWMb + t1;		// taon = tbon+t1
		PWMc = PWMa + t2;		// tcon = taon+t2
		break;
	default:
		break;
			
	}

	Timer3.setPwmDuty(phaseA, 1024 * PWMa);
	Timer3.setPwmDuty(phaseB, 1024 * PWMb);
	Timer3.setPwmDuty(phaseC, 1024 * PWMc);
}


void setup()
{
	pinMode(led, OUTPUT);
	pinMode(debugPin, OUTPUT);
	Timer3.initialize(278); //timer period us
	// we divide 360 degree (1 sin wave cycle (~16,667us) into 60 part)
	// 16,667 / 60 ~ 278
	// real cycle = 278*60 = 16,680 us  (~ 60hz)
	// pwm freq : 1/ 278us ~ 3.6kHz
  

	//call this func to setup port ouput PWM
	Timer3.pwm(phaseA, 256);
	Timer3.pwm(phaseB, 1000);
	Timer3.pwm(phaseC, 512);

	Timer3.attachInterrupt(timer3Int); // blinkLED to run every 0.15 seconds

	Serial.begin(9600);
}

void loop()
{
	int i;


  
	Serial.print("\n");
	Serial.print(isin(30),5); Serial.print("\t");Serial.print(isin(60), 5);
  
  
}

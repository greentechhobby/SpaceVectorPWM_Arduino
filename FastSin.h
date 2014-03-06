#ifndef _FAST_SIN_H_INCLUDED_
#define _FAST_SIN_H_INCLUDED_

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
//#include "Wire.h"

// 91 x 2 bytes ==> 182 bytes
const unsigned int isinTable16[] = {
	0, 1144, 2287, 3430, 4571, 5712, 6850, 7987, 9121, 10252, 11380,
	12505, 13625, 14742, 15854, 16962, 18064, 19161, 20251, 21336, 22414,
	23486, 24550, 25607, 26655, 27696, 28729, 29752, 30767, 31772, 32768,

	33753, 34728, 35693, 36647, 37589, 38521, 39440, 40347, 41243, 42125,
	42995, 43851, 44695, 45524, 46340, 47142, 47929, 48702, 49460, 50203,
	50930, 51642, 52339, 53019, 53683, 54331, 54962, 55577, 56174, 56755,

	57318, 57864, 58392, 58902, 59395, 59869, 60325, 60763, 61182, 61583,
	61965, 62327, 62671, 62996, 63302, 63588, 63855, 64103, 64331, 64539,
	64728, 64897, 65047, 65176, 65286, 65375, 65445, 65495, 65525, 65535,
};

float isin(int x)
{
  boolean pos = true;  // positive - keeps an eye on the sign.
  if (x < 0) 
  {
    x = -x;
    pos = !pos;  
  }  
  if (x >= 360) x %= 360;   
  if (x > 180) 
  {
    x -= 180;
    pos = !pos;
  }
  if (x > 90) x = 180 - x;
//  if (pos) return isinTable8[x] * 0.003921568627; // = /255.0
//  return isinTable8[x] * -0.003921568627 ;
  if (pos) return isinTable16[x] * 0.0000152590219; // = /65535.0
  return isinTable16[x] * -0.0000152590219 ;
}


float icos(int x)
{
	return isin(x + 90);
}

float itan(int x)
{
	return isin(x) / icos(x);
}



#endif

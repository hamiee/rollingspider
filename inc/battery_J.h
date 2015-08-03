#ifndef __BATTERY_J__
#define __BATTERY_J__


#include "ultra_snd.h"
#define DEV_ULTRA "/dev/ultra_snd"
int battery_init();



int battery_read(int *val);



#endif

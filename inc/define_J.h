
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/time.h>


typedef float float_j_t ;
#define RAD_TO_DEG  57.295779f
#define DEG_TO_RAD	0.0174532925f

#define INIT_DT 0.002
//#define INIT_DT 0.01

extern uint32_t cnt;
#define PRINT_BUG(x,...) {if(cnt%(x)==0)printf(__VA_ARGS__);}


#define GRAVITY 9.8


/*
   sample rate = Gyro output rate 8Khz  / ( 1+ sample_rate )
   */

#define MPU_SAMPLE_RATE  0

/*
 * DLPF ACC_Hz  Delay(ms)   GYRO_Hz     Delay   Fs
 *  0   260     0           256         0.98    8
 *  1   184     2.0         288         1.9     1
 *  2   94      3           98          2.8     1
 *  3   44      4.9         42          4.8     1
 *  4   21      8.5         20          8.3     1
 *  5   10      13.8        10          13.4    1
 *  6   5       19.0        5           18.6    1
 *  7   reserved            reserved            8
 */

#define DLPF_CFG 3

/*
AFS_SEL Full Scale Range
0   ± 2g
1   ± 4g
2   ± 8g
3   ± 16g
*/
#define ACC_FS_SEL 3
/*
0 ±2g 16384 LSB/g
1 ±4g 8192 LSB/g
2 ±8g 4096 LSB/g
3 ±16g 2048 LSB/g
*/
#define ACC_LSB 2048


/*
FS_SEL  Full Scale Range
0   ± 250 °/s
1   ± 500 °/s
2   ± 1000 °/s
3   ± 2000 °/s
*/
#define GYRO_FS_SEL 3
/*
EL  Full    Scale       Range   LSB Sensitivity
0   ±   250     °/s     131     LSB/°/s
1   ±   500     °/s     65.5    LSB/°/s
2   ±   1000    °/s     32.8    LSB/°/s
3   ±   2000    °/s     16.4    LSB/°/s
*/
#define GYRO_LSB    16.4F



//void PRINT_BUG(int aa , char *format, ...);


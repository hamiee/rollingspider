CC=arm-none-linux-gnueabi-g++
BINDIR=bin
SRCDIR=src
# creating var to pass options to compiler:
CFLAGS=-c -Wall -lm -I./ -I./inc -I./src  -lpthread  -lrt
LFLAGS=-lm  -lpthread  -lrt
all: $(BINDIR)/hello

$(BINDIR)/hello: $(BINDIR)/main.o $(BINDIR)/HAL_motor_J.o $(BINDIR)/battery_J.o $(BINDIR)/gpio_J.o $(BINDIR)/HAL_pid_J.o $(BINDIR)/HAL_sensor_J.o $(BINDIR)/i2c_J.o $(BINDIR)/KalmanJ.o  $(BINDIR)/mpu6050_J.o $(BINDIR)/pwm_J.o $(BINDIR)/ms5607.o $(BINDIR)/lpf_J.o $(BINDIR)/I2Cdev.o $(BINDIR)/MPU6050.cpp.o  $(BINDIR)/Extended_Kalman.o $(LFLAGS)
	$(CC) $(BINDIR)/main.o $(BINDIR)/HAL_motor_J.o $(BINDIR)/battery_J.o $(BINDIR)/gpio_J.o $(BINDIR)/HAL_pid_J.o $(BINDIR)/HAL_sensor_J.o $(BINDIR)/i2c_J.o $(BINDIR)/KalmanJ.o  $(BINDIR)/mpu6050_J.o $(BINDIR)/pwm_J.o $(BINDIR)/ms5607.o $(BINDIR)/lpf_J.o $(BINDIR)/I2Cdev.o $(BINDIR)/MPU6050.cpp.o $(BINDIR)/Extended_Kalman.o $(LFLAGS)

$(BINDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(BINDIR)/main.o

$(BINDIR)/HAL_motor_J.o: $(SRCDIR)/HAL_motor_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/HAL_motor_J.cpp -o $(BINDIR)/HAL_motor_J.o

#$(BINDIR)/define_J.o: $(SRCDIR)/define_J.cpp
#	$(CC) $(CFLAGS) $(SRCDIR)/define_J.cpp -o $(BINDIR)/define_J.o

$(BINDIR)/battery_J.o: $(SRCDIR)/battery_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/battery_J.cpp -o $(BINDIR)/battery_J.o

$(BINDIR)/gpio_J.o: $(SRCDIR)/gpio_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/gpio_J.cpp -o $(BINDIR)/gpio_J.o

$(BINDIR)/HAL_pid_J.o: $(SRCDIR)/HAL_pid_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/HAL_pid_J.cpp -o $(BINDIR)/HAL_pid_J.o

$(BINDIR)/HAL_sensor_J.o: $(SRCDIR)/HAL_sensor_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/HAL_sensor_J.cpp -o $(BINDIR)/HAL_sensor_J.o

$(BINDIR)/i2c_J.o: $(SRCDIR)/i2c_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/i2c_J.cpp -o $(BINDIR)/i2c_J.o

$(BINDIR)/KalmanJ.o: $(SRCDIR)/KalmanJ.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/KalmanJ.cpp -o $(BINDIR)/KalmanJ.o

#$(BINDIR)/Kalman_J.o: $(SRCDIR)/Kalman_J.cpp
#	$(CC) $(CFLAGS) $(SRCDIR)/Kalman_J.cpp -o $(BINDIR)/Kalman_J.o

$(BINDIR)/Extended_Kalman.o: $(SRCDIR)/Extended_Kalman.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/Extended_Kalman.cpp -o $(BINDIR)/Extended_Kalman.o

$(BINDIR)/mpu6050_J.o: $(SRCDIR)/mpu6050_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/mpu6050_J.cpp -o $(BINDIR)/mpu6050_J.o

$(BINDIR)/pwm_J.o: $(SRCDIR)/pwm_J.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/pwm_J.cpp -o $(BINDIR)/pwm_J.o

$(BINDIR)/ms5607.o: $(SRCDIR)/ms5607.c
	$(CC) $(CFLAGS) $(SRCDIR)/ms5607.c -o $(BINDIR)/ms5607.o

$(BINDIR)/lpf_J.o: $(SRCDIR)/lpf_J.c
	$(CC) $(CFLAGS) $(SRCDIR)/lpf_J.c -o $(BINDIR)/lpf_J.o

$(BINDIR)/I2Cdev.o: $(SRCDIR)/I2Cdev.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/I2Cdev.cpp  -o $(BINDIR)/I2Cdev.o

$(BINDIR)/MPU6050.cpp.o: $(SRCDIR)/MPU6050.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/MPU6050.cpp -o $(BINDIR)/MPU6050.cpp.o




$
$
clean:
	rm -rf $(BINDIR)/*o $(BINDIR)/hello



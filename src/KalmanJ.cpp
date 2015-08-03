/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#include "KalmanJ.h"

void KalmanInit(Kalman_t *kal) {
    /* We will set the variables like so, these can also be tuned by the user */
    kal->Q_angle = 0.001f;
    kal->Q_bias = 0.003f;
    kal->R_measure = 0.03f;

    kal->angle = 0.0f; // Reset the angle
    kal->bias = 0.0f; // Reset bias

    kal->P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    kal->P[0][1] = 0.0f;
    kal->P[1][0] = 0.0f;
    kal->P[1][1] = 0.0f;
};

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float KalmangetAngle(Kalman_t *kal, float newAngle, float newRate, float dt) {
    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    kal->rate = newRate - kal->bias;
    kal->angle += dt * kal->rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    kal->P[0][0] += dt * (dt*kal->P[1][1] - kal->P[0][1] - kal->P[1][0] + kal->Q_angle);
    kal->P[0][1] -= dt * kal->P[1][1];
    kal->P[1][0] -= dt * kal->P[1][1];
    kal->P[1][1] += kal->Q_bias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = kal->P[0][0] + kal->R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = kal->P[0][0] / S;
    K[1] = kal->P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - kal->angle; // Angle difference
    /* Step 6 */
    kal->angle += K[0] * y;
    kal->bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = kal->P[0][0];
    float P01_temp = kal->P[0][1];

    kal->P[0][0] -= K[0] * P00_temp;
    kal->P[0][1] -= K[0] * P01_temp;
    kal->P[1][0] -= K[1] * P00_temp;
    kal->P[1][1] -= K[1] * P01_temp;

    return kal->angle;
};

void KalmansetAngle(Kalman_t *kal, float angle) { kal->angle = angle; }; // Used to set angle, this should be set as the starting angle
float KalmangetRate(Kalman_t *kal) { return kal->rate; }; // Return the unbiased rate

/* These are used to tune the Kalman filter */
void KalmansetQangle(Kalman_t *kal,float Q_angle) { kal->Q_angle = Q_angle; };
void KalmansetQbias(Kalman_t *kal,float Q_bias) { kal->Q_bias = Q_bias; };
void KalmansetRmeasure(Kalman_t *kal,float R_measure) { kal->R_measure = R_measure; };

float KalmangetQangle(Kalman_t *kal) { return kal->Q_angle; };
float KalmangetQbias(Kalman_t *kal) { return kal->Q_bias; };
float KalmangetRmeasure(Kalman_t *kal) { return kal->R_measure; };

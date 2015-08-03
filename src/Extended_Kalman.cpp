#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "define_J.h"
#include "HAL_sensor_J.h"


//typedef float float_j_t;
int GravityToEuler(float_j_t a_x, float_j_t a_y, float_j_t a_z, float_j_t z_k[][3])
{
//theta=atan2(-A1,C1)*180/3.14-90
//pi=atan2(-B1,-C1)*180/3.14+90

	//float_j_t psi = atan2(a[1][0],a[0][0]);
//	float_j_t theta = atan2(a_x,a_z);//*180/3.14;
//	float_j_t pi = atan2(a_y,a_z);//*180/3.14;
	


    if(a_z>=0){
	    z_k[0][0] = atan2f(a_y,a_z);
        z_k[1][0] = atan2f(-a_x, sqrtf( powf(a_y,2)+powf(a_z,2) ));
    }else if(a_z<0){
        z_k[1][0] = -atan2f(a_x,a_z);
        z_k[0][0] = atan2f(-a_y,sqrtf( powf(a_y,2)+powf(a_z,2) ));
    }
	/*if(z_k[0][0]<0){
		z_k[0][0] = atan2f(-a_y,-a_z)+3.14;	
	}else{
		z_k[0][0] = atan2f(-a_y,-a_z)-3.14;	
	}*/
	//z_k[1][0] = atan2f(-a_x,a_z);
	
#if 0
#define Gravity -9.8
		float_j_t estimate_pi;
		float_j_t estimate_theta;
		
		/*roll*/
		double val1;
		if(a_z==0.0) a_z = 0.00001;
		estimate_pi = (float_j_t)atan2((double)-a_y,(double)-a_z);
		//estimate_pi = (float_j_t)atan2((double)-a_y,(double)-a_z)+90.0; //error correct
		//estimate_pi = (float_j_t)atan2((double)-a_y,(double)-a_z)*180.0/3.14; //rad to degree

		/*pitch*/
		double val2 = (double)-a_x/Gravity;
		estimate_theta = (float_j_t)asinf(val2);
		//estimate_theta = (float_j_t)asinf((double)-a_x/Gravity)*180.0/3.14; //rad to degree 

		z_k[0][0] = estimate_pi;
		z_k[1][0] = estimate_theta;
#endif

		return (0);
}

int printMatrix(float_j_t work1[][3], int row, int col) {
	int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			printf("[%d][%d] = %f \n",i,j,work1[i][j]);


	return (0);
}


int zeroMatrix(float_j_t work1[][3], int row, int col) {
work1[0][0]=0;
work1[0][1]=0;
work1[0][2]=0;
work1[1][0]=0;
work1[1][1]=0;
work1[1][2]=0;
work1[2][0]=0;
work1[2][1]=0;
work1[2][2]=0;
return 0;
#if 0
    int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			work1[i][j] = 0.0;


	return (0);
#endif
}



// transformation 매트릭스를 산출해 내는 함수
int transMatrix( float_j_t work1[][3], float_j_t work2[][3], int row, int col) {

    work2[0][0] = work1[0][0];
    work2[0][1] = work1[1][0];
    work2[0][2] = work1[2][0];
    work2[1][0] = work1[0][1];
    work2[1][1] = work1[1][1];
    work2[1][2] = work1[2][1];
    work2[2][0] = work1[0][2];
    work2[2][1] = work1[1][2];
    work2[2][2] = work1[2][2];

#if 0
    int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			work2[j][i] = work1[i][j];

	return (0);
#endif
}



// 두 매트릭스의 곱을 구하는 함수
int multiMatrix(float_j_t work1[][3],float_j_t work2[][3], float_j_t result[][3], int row, int col) {
	int i, j, k;
	float_j_t resTemp=0.0;

	if (row < 1 || col < 1)
		return (-1);
//zeroMatrix(result,3,3);
	for (i=0; i<row; i++) {
		for (j=0; j<row; j++) {
			result[i][j] = (work1[i][0] * work2[0][j]) + (work1[i][1] * work2[1][j]) + (work1[i][2] * work2[2][j]);
            
           /* 
            for (k=0; k<col; k++) {
				resTemp += work1[i][k] * work2[k][j];
			}
			result[i][j] += resTemp;
			resTemp = 0.0;
            */

			//printf("result[%d][%d] = %f  ", i, j, result[i][j]);
		}
	//	printf("\n");
	}

	return (0);
} 

int insertMatrix(float_j_t work1[][3], float_j_t result[][3], int row, int col) {

result[0][0]=work1[0][0];
result[0][1]=work1[0][1];
result[0][2]=work1[0][2];
result[1][0]=work1[1][0];
result[1][1]=work1[1][1];
result[1][2]=work1[1][2];
result[2][0]=work1[2][0];
result[2][1]=work1[2][1];
result[2][2]=work1[2][2];

#if 0
    int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			result[i][j] = work1[i][j];
#endif

	return (0);
} 

int subMatrix(float_j_t work1[][3], float_j_t work2[][3], float_j_t result[][3], int row, int col) {
	
			result[0][0] = work1[0][0] - work2[0][0];
			result[0][1] = work1[0][1] - work2[0][1];
			result[0][2] = work1[0][2] - work2[0][2];
			result[1][0] = work1[1][0] - work2[1][0];
			result[1][1] = work1[1][1] - work2[1][1];
			result[1][2] = work1[1][2] - work2[1][2];
			result[2][0] = work1[2][0] - work2[2][0];
			result[2][1] = work1[2][1] - work2[2][1];
			result[2][2] = work1[2][2] - work2[2][2];
    
#if 0
    int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			result[i][j] = work1[i][j] - work2[i][j];
#endif 

	return (0);
} 


int sumMatrix(float_j_t work1[][3], float_j_t work2[][3], float_j_t result[][3], int row, int col) {
			result[0][0] = work1[0][0] + work2[0][0];
			result[0][1] = work1[0][1] + work2[0][1];
			result[0][2] = work1[0][2] + work2[0][2];
			result[1][0] = work1[1][0] + work2[1][0];
			result[1][1] = work1[1][1] + work2[1][1];
			result[1][2] = work1[1][2] + work2[1][2];
			result[2][0] = work1[2][0] + work2[2][0];
			result[2][1] = work1[2][1] + work2[2][1];
			result[2][2] = work1[2][2] + work2[2][2];
    

#if 0
    int i, j;

	if (row < 1 || col < 1)
		return (-1);

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			result[i][j] = work1[i][j] + work2[i][j];

#endif 
	return (0);
} 

int similarMatrix(float_j_t work1[][3], float_j_t work2[][3], float_j_t result[][3], int row, int col) {

	float_j_t res[3][3]={0,};
	float_j_t res_t[3][3]={0,};

	transMatrix(work1,res_t,3,3);
//printMatrix(work1,3,3);
//printMatrix(res_t,3,3);

	multiMatrix(work1,work2,res,3,3);
//printMatrix(work2,3,3);
	multiMatrix(res,res_t,result,3,3);
//printMatrix(result,3,3);
return 0;
}

int inverseMatrix(float_j_t a[][3], float_j_t out[][3], int row, int col) {

//	printMatrix(a,3,3);
	double d;//determinant

	d=(a[0][0]*a[1][1]*a[2][2]) + (a[0][1]*a[1][2]*a[2][0]) + (a[0][2]*a[1][0]*a[2][1]) - (a[0][1]*a[1][0]*a[2][2]) - (a[0][2]*a[1][1]*a[2][0]) - (a[0][0]*a[1][2]*a[2][1]);
	if(d==0){
		printf("determinant error\n");
		return -1;
	}
	out[0][0] = (a[1][1]*a[2][2] - a[1][2]*a[2][1]) /d;
	out[0][1] = (-a[0][1]*a[2][2] + a[0][2]*a[2][1])/d; 
	out[0][2] = (a[0][1]*a[1][2] - a[0][2]*a[1][1]) /d;

	out[1][0] = (-a[1][0]*a[2][2] + a[1][2]*a[2][0])/d;
	out[1][1] = (a[0][0]*a[2][2] - a[0][2]*a[2][0]) /d;
	out[1][2] = (-a[0][0]*a[1][2] + a[0][2]*a[1][0])/d; 

	out[2][0] = (a[1][0]*a[2][1] - a[1][1]*a[2][0]) /d;
	out[2][1] = (-a[0][0]*a[2][1] + a[0][1]*a[2][0])/d;
	out[2][2] = (a[0][0]*a[1][1] - a[0][1]*a[1][0]) /d;
	return 0;



}


int _2by2_inverseMatrix(float_j_t in[][3], float_j_t out[][3], int row, int col){

	float_j_t a = in[0][0];
	float_j_t b = in[0][1];
	float_j_t c = in[1][0];
	float_j_t d = in[1][1];

	float_j_t determinant = a*d-b*c;
	if(determinant == 0){
		printf("determinant error\n");
		return -1;
	}

	out[0][0] = d/determinant;
	out[0][1] = -b/determinant;
	out[1][0] = -c/determinant;
	out[1][1] = a/determinant;
	
	out[0][2] = 0;
	out[1][2] = 0;
	out[2][0] = 0;
	out[2][1] = 0;
	out[2][2] = 0;

	return 0;
}

/*
typedef struct sensor_struct_t{
		float accX;
		float accY;
		float accZ;
		float gyroX;
		float gyroY;
		float gyroZ;
}sensor_struct_t;
*/
#if 1
int kalman_function(sensor_struct_t* sen,float_j_t dt){

	/*
	 *
	 *	system model 
	 *	omega = [ w_x w_y w_z ]^T
	 *	x = [pi theta psi]^T
	 *	used  x_dot, C_inv, omega, x_k_1, x_k, dt, 
	 *
	 * */
#define Def_x	0
#define Def_y	1
#define Def_z	2
#define Def_pi		0
#define Def_theta	1
#define Def_psi		2
	//float_j_t dt;
	//float_j_t B[3][3]={0,};

	float_j_t res[3][3]={0,};
	float_j_t res2[3][3]={0,};

	float_j_t omega[3][3]={0,};
	omega[0][0] = sen->gyroX*DEG_TO_RAD;
	omega[1][0] = sen->gyroY*DEG_TO_RAD;
	omega[2][0] = sen->gyroZ*DEG_TO_RAD;

	static float_j_t x_k_1[3][3]={0,}; 
	float_j_t x_k_p[3][3]={0,};

	float_j_t pi,theta,psi;
	pi = x_k_1[Def_pi][0];
	theta = x_k_1[Def_theta][0];
	psi = x_k_1[Def_psi][0];

if(cosf(theta)==0.0){ 
		printf("cosf error\n ");
		theta += 0.00001;
}

	float_j_t C_inv[3][3]={0,};
	C_inv[0][0] = 1*dt;
	C_inv[0][1] = sinf(pi)*tanf(theta)*dt; 
	C_inv[0][2] = cosf(pi)*tanf(theta)*dt;
	C_inv[1][0] = 0;
	C_inv[1][1] = cosf(pi)*dt;
	C_inv[1][2] = -sinf(pi)*dt;
	C_inv[2][0] = 0;
	C_inv[2][1] = (sinf(pi)/cosf(theta))*dt;
	C_inv[2][2] = (cosf(pi)/cosf(theta))*dt;

	//x_k = x_k_1 + C_inv * omega * dt
	multiMatrix(C_inv,omega,res,3,3);
	sumMatrix(res,x_k_1,x_k_p,3,3);




	/* A_k*/
	float_j_t A[3][3]={0,};
	A[0][0] = 1 + (sinf(theta)*(omega[1][0]*cosf(pi)-omega[2][0]*sinf(pi)) / cosf(theta))*dt;
	A[0][1] = ((omega[1][0]*sinf(pi)+omega[2][0]*cosf(pi)) / (cosf(theta)*cosf(theta)))*dt;
	A[0][2] = 0;

	A[1][0] = (-omega[1][0]*sinf(pi)-omega[2][0]*cosf(pi))*dt;
	A[1][1] = 1;
	A[1][2] = 0;

	A[2][0] = ((omega[1][0]*cosf(pi)-omega[2][0]*sinf(pi)) / cosf(theta))*dt;
	A[2][1] = (sinf(theta)*(omega[1][0]*sinf(pi)+omega[2][0]*cosf(pi)) / (cosf(theta)*cosf(theta)))*dt;
	A[2][2] = 1;

	float_j_t P_k_p[3][3]={0,};
	static float_j_t P_k_1[3][3]={1,0,0,0,1,0,0,0,1};

	float_j_t Q[3][3]={1,0,0,0,1,0,0,0,1};
	//P_k = A* P_k_1 * A^T + B * Q * B^T
	similarMatrix(A,P_k_1,res,3,3);
	similarMatrix(C_inv,Q,res2,3,3);
	sumMatrix(res,res2,P_k_p,3,3);

	//Correct term 

	float_j_t H[3][3]={1,0,0,0,1,0,0,0,0};

	float_j_t norm_a=9.8;
	norm_a=sqrtf(powf(sen->accX,2)+powf(sen->accY,2)+powf(sen->accZ,2));
//		printf("norm_a = %f\n",norm_a);
	float_j_t R[3][3]={0,};
	if((norm_a<10.8 )&&(norm_a>8.8)){
		R[0][0]=1;
		R[1][1]=1;
	}else{
		R[0][0]=1000;
		R[1][1]=1000;
	}


	float_j_t res3[3][3] = {0,};

	similarMatrix(H,P_k_p,res,3,3);
	

	//printf("similarMatrix res\n");
	//printMatrix(res,3,3);

	sumMatrix(res,R,res2,3,3);
	
	_2by2_inverseMatrix(res2,res,3,3);

	//inverseMatrix(res2,res,3,3);
	//result inverse matrix is res
	//so  kalman gain  K = Pk* H^T* res
	float_j_t K[3][3]={0,};
	
	transMatrix(H,res3,3,3);
	multiMatrix(P_k_p,res3,res2,3,3);
	res2[0][2]=0;
	res2[1][2]=0;
	res2[2][0]=0;
	res2[2][1]=0;
	res2[2][2]=0;

		
//	printf("res2 gain\n");
//	printMatrix(res2,3,3);
//	printf("res gain\n");
//	printMatrix(res,3,3);
	multiMatrix(res2,res,K,3,3);
//	printf("kalman gain\n");
//	printMatrix(K,3,3);


	/* state variable,  covaliance mat  update*/
	float_j_t x_k[3][3]={0,};

	//calculate z_k  roll, ptich,  with gravitational euler angle calc.
	float_j_t z_k[3][3] ={0,};
	
#if 1
	z_k[0][0] = atan2f(sen->accY,sen->accZ);
    z_k[1][0] = atan2f(-sen->accX, sqrtf( powf(sen->accY,2)+powf(sen->accZ,2) ));
#else
    if(sen->accZ>=0){
	    z_k[0][0] = atan2f(sen->accY,sen->accZ);
        z_k[1][0] = atan2f(-sen->accX, sqrtf( powf(sen->accY,2)+powf(sen->accZ,2) ));
    }else if(sen->accZ<0){
        omega[1][0] = -omega[1][0];
	    z_k[1][0] = -atan2f(sen->accX,sen->accZ);
        z_k[0][0] = atan2f(-sen->accY, sqrtf( powf(sen->accX,2)+powf(sen->accZ,2) ));
    }
#endif
    //GravityToEuler(sen->accX, sen->accY, sen->accZ, z_k);
    //printf("r %f p %f ",z_k[0][0]*180/3.14,z_k[1][0]*180/3.14);
	multiMatrix(H,x_k_p,res,3,3);

	subMatrix(z_k,res,res2,3,3);
	multiMatrix(K,res2,res,3,3);
	sumMatrix(x_k_p,res,x_k,3,3);

	float_j_t P_k[3][3]={0,};
	multiMatrix(K,H,res,3,3);
	multiMatrix(res,P_k_p,res2,3,3);
	subMatrix(P_k_p,res2,P_k,3,3);

	/*insert prev value*/
	insertMatrix(x_k, x_k_1,3,3);
	insertMatrix(P_k, P_k_1,3,3);
	/*Debug print*/
	//printf("roll = %f , pitch = %f \n",x_k[0][0]*180.0/3.14,x_k[1][0]*180.0/3.14);
    sen->roll=x_k[0][0]*RAD_TO_DEG;
    sen->pitch=x_k[1][0]*RAD_TO_DEG;
    return 0;
}
#endif 

/*
int main() {
		int i,j;
		sensor_struct_t sensor;
		float dt;
		int testcase;
	scanf("%d",&testcase);
	printf("testcase %d\n",testcase);	
		for(i=0; i<testcase; i++)
		{
			float a,b,c;
//			printf("example!\ninput accX,Y,Z, gyroX,Y,Z, dt 7Parameter\n");
			scanf("%f %f %f %f %f %f %f", &sensor.accX, &sensor.accY, &sensor.accZ, &a,&b,&c, &dt);
			sensor.gyroX=a;
			sensor.gyroY=b;
			sensor.gyroZ=c;

			kalman_function(&sensor,dt);
		}
		return (0);
}

*/

/*   
 * Differential-steering vehicle
 * -----------------------------
 *
 * Connecting robot with sensors and motors:  
 *
 *  - MotorA is connected to the left wheel;
 *  - MotorC is connected to the right wheel;
 *  - Light sensor on the front side (s1) is connected to port3;
 *  - Light sensor on the back side (s2) is connected to port2.
 *
 * Position of the robot on the track:
 *
 *  - Left wheel is in the dark side of the track 
 *  - Right wheel is in the light side of the track
 * (in this way theta angle will be positive when vehicle turns anti-orario)
 *
 * Setup:
 *  
 *  use the sensor on the front side (s1,port3) to setup these parameters:
 *  - ydark: put s1 on the darkest point of the track and press Run Button;
 *  - yref: put s1 on the point corresponding on the drive-line and press Run;
 *  - ylight: put s1 on the lightest point of the track and press Run;
 *  Now put the vehicle on the track and press Run Button to exit setup mode
 *  and the vehicle will start....
 *  
 *  Note that you can enter into setup mode everytime just pressing Run button.
 */
 
/* MATLAB SIMULATION:
 *
 *	NORMALE_MODE:
 *	    	Carrello_Stato_new.mdl
 *		WS_Carrello_Stato_new.mat  
 *			    
 *	EMERGENCY_MODE:
 *		Carrello_Out1.mdl	  
 *      	WS_Carrello_Out1.mat
 */

#include "osektask.h" 
#include "code.h"
//#define __NO_CONTROL__

#ifdef __NO_CONTROL__

void CTRL_init(void) {}
void CTRL_set_normalpowref(ERIKA_INT16 value) {}
void CTRL_readsensors(ERIKA_INT16 a, ERIKA_INT16 b) {}
void CTRL_convertsensors(void) {}

/* the two control laws */
void CTRL_feedback_law(void){}
void CTRL_observer_law(void){}

/* these three functions are used during the setup phase to get the
   reference values */
void CTRL_set_black(void){}
void CTRL_set_yref(void){}
void CTRL_set_ywhite(void){}

ERIKA_INT16 CTRL_get_lmotor(void){return 0;}
ERIKA_INT16 CTRL_get_rmotor(void){return 0;}

#else

/****************************************************************************
  Control algorithm variables
****************************************************************************/

#define CTRL_1SENSOR  0
#define CTRL_2SENSORS 1


static ERIKA_DF_TYPE w, y, yscale, theta, y1, y2, 
  q, b, cr, cl, temp1, temp2, 
  K1l[2], K2l[2], K1r[2], K2r[2], K0r, K0l;

static ERIKA_INT16   s1, s2, yref=0, yblack=0, ywhite=0;

static ERIKA_INT16 powref[2];
static ERIKA_INT16 l_motor_pow, r_motor_pow;

/* used by observer */
static ERIKA_DF_TYPE Kt, Ku, l, theta_prev, y_prev, u_prev;


/****************************************************************************
  initialization
****************************************************************************/

void CTRL_init(void)
{
  /* 
   * init application data 
   */    
     
  /* Center to center wheels distance (m) */
  ERIKA_int2df(&temp1, 73);
  ERIKA_int2df(&temp2, 1000);    
  ERIKA_df_div(&b, &temp1, &temp2); 		// b=0.105  (0.073)
    
  /* Sensor to sensor distance (m) */
  ERIKA_int2df(&temp1, 665);
  ERIKA_int2df(&temp2, 10000);        
  ERIKA_df_div(&q, &temp1, &temp2);		// q=0.0665 (new car)
    
  /* Width of track (from dark side to white side) */
  ERIKA_int2df(&temp1, 1);
  ERIKA_int2df(&temp2, 10);
  ERIKA_df_div(&w, &temp1, &temp2);		// w=0.1 m 
    
    
  /*                    MAPPING LINEAR SPEED AND MOTOR POWER 
   * c(t) is supposed expressed in m/s (linear speed): 
   *
   *       pow= f(linear speed) = f(c(t));  where c(t)=alfa*y(t) + beta*theta(t);
   *
   * For example we can have a linear map (it depends on your vehicle): 
   *       pow(t) = G1*c(t) + G0;
   * So you obtain a new c(t) which is expressed in terms of power:
   *       
   * cp(t)=  pow(t) = G1*(alfa*y(t) + beta*theta(t)) + G0 = 
   *            K1*y(t) + K2*theta(t) + K0
   *
   * so:
   *       K1=G1*alfa;    K2=G1*beta;     K0=G0;
   */

  /* K0r = G0r */
  ERIKA_int2df(&temp1, -2581);
  ERIKA_int2df(&temp2, 100);    
  ERIKA_df_div(&K0r, &temp1, &temp2);	

  /* K0l = G0l */
  ERIKA_int2df(&temp1, -2667);
  ERIKA_int2df(&temp2, 100);    
  ERIKA_df_div(&K0l, &temp1, &temp2);	


  /************************************
   * One Sensor mode (EMERGENCY_MODE) *
   ************************************/

  /* Here we have two different cp(t): (one for each wheel)
   *
   * alfa = -30; beta=-5; 
   *   (in matlab simulation we have k=[alfa, beta]=[-10,-3] )
   *
   * right wheel:
   *    G1r=201.33;   G0r=-25.81;    => K1r=-6004;  K2r=-1006;  K0r=-25.81;
   *
   *  	==> cpr(t)=pow_right(t)= K1r * y(t) + G0r;
   *
   * left wheel:
   *    G1l=208.05;   G0l=-26.67     => K1l=-6242;    K2l=-1040;    K0l=-26.67
   *
   *  	==> cpl(t)=pow_left(t)= K1l * y(t) + K0l;
   *
   */

  /* Here we use some K1 and K2 obtained with matlab simulations */

  /* K1r = alfa * G1r */
  ERIKA_int2df(&K1r[CTRL_1SENSOR], -6004);

  /* K2r = beta * G1r */
  ERIKA_int2df(&K2r[CTRL_1SENSOR], -1006);
    
  /* K1l = alfa * G1l*/
  ERIKA_int2df(&K1l[CTRL_1SENSOR], -6242);

  /* K2l = beta * G1l */
  ERIKA_int2df(&K2l[CTRL_1SENSOR], -1040);
    
  /* set motors power */
  powref[CTRL_1SENSOR] = EMRGSPEED_POWREF;

  /* constants used by the observer */
  /* Kt */
  ERIKA_int2df(&temp1, 9992);
  ERIKA_int2df(&temp2, 10000);
  ERIKA_df_div(&Kt, &temp1, &temp2);
  /* Ku */
  ERIKA_int2df(&temp1, 381);
  ERIKA_int2df(&temp2, 10000);
  ERIKA_df_div(&Ku, &temp1, &temp2);	
  /* l=11 */
  ERIKA_int2df(&temp1, 1);
  ERIKA_int2df(&temp2, 1);
  ERIKA_df_div(&l, &temp1, &temp2);
    
  /* y_prev(0)=0;    theta_prev(0)=0;   u_prev(0)=0; */
  ERIKA_int2df(&y_prev, 0);
  ERIKA_int2df(&theta_prev, 0);
  ERIKA_int2df(&u_prev, 0);
    
  /*******************
   * Two Sensor mode *
   *******************/
  /* Here we have two different cp(t): (one for each wheel)
   *
   * alfa = -27; beta=-3; (in matlab simulation we have k=[alfa, beta]=[-10,-3] )
   *
   * right wheel:
   *    G1r=201.33;   G0r=-25.81;    => K1r=-5436;  K2r=-604;  K0r=-25.81;
   *
   *  	==> cpr(t)=pow_right(t)= K1r * y(t) + G0r;
   *
   * left wheel:
   *    G1l=208.05;   G0l=-26.67     => K1l=-5617;    K2l=-624;    K0l=-26.67
   *
   *  	==> cpl(t)=pow_left(t)= K1l * y(t) + K0l;
   *
   */

  /* Here we use some K1 and K2 obtained with matlab simulations */

  /* K1r = alfa * G1r */
  ERIKA_int2df(&K1r[CTRL_2SENSORS], -5436);
  //    ERIKA_int2df(&temp2, 10);    
  //    ERIKA_df_div(&K1r, &temp1, &temp2);	

  /* K2r = beta * G1r */
  ERIKA_int2df(&K2r[CTRL_2SENSORS], -604);
  //    ERIKA_int2df(&temp2, 10);    
  //    ERIKA_df_div(&K2r, &temp1, &temp2);	
    
    
  /* K1l = alfa * G1l*/
  ERIKA_int2df(&K1l[CTRL_2SENSORS], -5617);
  //    ERIKA_int2df(&temp2, 10);    
  //    ERIKA_df_div(&K1l, &temp1, &temp2);	

  /* K2l = beta * G1l */
  ERIKA_int2df(&K2l[CTRL_2SENSORS], -624);
  //    ERIKA_int2df(&temp2, 10);    
  //    ERIKA_df_div(&K2l, &temp1, &temp2);	
    


  /* set motors power */
  //powref[CTRL_2SENSORS]=110; // 0.4 m/s Michael set it to 80
  powref[CTRL_2SENSORS]=NORMSPEED_POWREF;
}





/****************************************************************************
  The algorithms
****************************************************************************/

void CTRL_feedback_law(void)
{
  // y = (y1 - (yref-ywhite)) * yscale 
  ERIKA_int2df(&temp1, yref-ywhite);
  ERIKA_df_sub(&temp2, &y1, &temp1);
  ERIKA_df_mul(&y, &temp2, &yscale);
	
  /* theta= ( (y1-y2) / q ) * yscale */
  ERIKA_df_sub(&temp1, &y1, &y2);
  ERIKA_df_div(&theta, &temp1, &q);
  ERIKA_df_mul(&theta, &theta, &yscale);

  /* cpr(t)=K1r*y(t) + K2r*theta(t) + K0r*/
  ERIKA_df_mul(&temp1, &K1r[CTRL_2SENSORS], &y);
  ERIKA_df_mul(&temp2, &K2r[CTRL_2SENSORS], &theta);    
  ERIKA_df_add(&cr, &temp1, &temp2);
  ERIKA_df_add(&cr, &cr, &K0r);

  /* cpl(t)=K1l*y(t) + K2l*theta(t) + K0l*/
  ERIKA_df_mul(&temp1, &K1l[CTRL_2SENSORS], &y);
  ERIKA_df_mul(&temp2, &K2l[CTRL_2SENSORS], &theta);    
  ERIKA_df_add(&cl, &temp1, &temp2);
  ERIKA_df_add(&cl, &cl, &K0l);

  /* Calculate new power */
  l_motor_pow=powref[CTRL_2SENSORS] - (ERIKA_df2int(&cl)>>1); /* motorA controls left wheel */
  r_motor_pow=powref[CTRL_2SENSORS] + (ERIKA_df2int(&cr)>>1); /* motorC controls right wheel */
	    
  /* now you can apply l_motor_pow and r_motor_pow to the motors */
}



void CTRL_observer_law(void)
{
  /*
   * this control law uses only y1 (suppose y2 is not usable)
   */
	 	
  // y(k) = (y1 - (yref-ywhite)) * yscale 
  ERIKA_int2df(&temp1, yref-ywhite);
  ERIKA_df_sub(&temp2, &y1, &temp1);
  ERIKA_df_mul(&y, &temp2, &yscale);

  /* theta(k)= theta(k-1) + 2*tau*u(k-1) + L*(y(k)-ys(k));
   * ys(k)=y(k-1) + tau*d*u(k-1) + vref*T*theta(k-1);
   * u(k)=K1*y(k) + K2*theta(k) + K0;
   * ==>  theta(k)=(1-L*Vr*T)*theta(k-1) + (2-L*d)*tau*u(k-1) + L*(y(k)-y(k-1));
   *
   *     ************************************************************
   * ==> *** theta(k)=Kt*theta(k-1) + Ku*u(k-1) + l*(y(k)-y(k-1)) ***
   *     ************************************************************
   * where:
   * 	Kt=(1-L*Vr*T);         =0.9992
   *      Ku=(2-L*d)*tau;        =0.0381
   *	l		       = 1
   *
   *      d=Vr*T		       =0.0008
   *      tau=T/(2*b)	       =0.019	
   */
	     	 
	
	
  ERIKA_df_mul(&temp1, &Kt, &theta_prev);
	
  ERIKA_df_mul(&temp2, &Ku, &u_prev);
	
  ERIKA_df_add(&temp1, &temp1, &temp2);
	
  ERIKA_df_sub(&temp2, &y, &y_prev);
  ERIKA_df_mul(&temp2, &temp2, &l); 

  ERIKA_df_add(&theta, &temp1, &temp2);


  /* cpr(t)=K1r*y(t) + K2r*theta(t) + K0r*/
  ERIKA_df_mul(&temp1, &K1r[CTRL_1SENSOR], &y);
  ERIKA_df_mul(&temp2, &K2r[CTRL_1SENSOR], &theta);    
  ERIKA_df_add(&cr, &temp1, &temp2);
  ERIKA_df_add(&cr, &cr, &K0r);

  /* cpl(t)=K1l*y(t) + K2l*theta(t) + K0l*/
  ERIKA_df_mul(&temp1, &K1l[CTRL_1SENSOR], &y);
  ERIKA_df_mul(&temp2, &K2l[CTRL_1SENSOR], &theta);    
  ERIKA_df_add(&cl, &temp1, &temp2);
  ERIKA_df_add(&cl, &cl, &K0l);

  /* Calculate new power */
  l_motor_pow=powref[CTRL_1SENSOR] - (ERIKA_df2int(&cl)>>1); /* motorA controls left wheel */
  r_motor_pow=powref[CTRL_1SENSOR] + (ERIKA_df2int(&cr)>>1); /* motorC controls right wheel */
	
  /* now you can apply l_motor_pow and r_motor_pow to the motors */
}

/****************************************************************************
  Read and convert sensors
****************************************************************************/

/* convertsensors and readsensors must be executed in mutual exclusion */

void CTRL_readsensors(ERIKA_INT16 a, ERIKA_INT16 b)
{
  s1=a;   
  s2=b;
}

void CTRL_convertsensors(void)
{
  ERIKA_int2df(&y1, s1-ywhite);
  ERIKA_int2df(&y2, s2-ywhite);	
}


/****************************************************************************
  References read at setup time
****************************************************************************/

void CTRL_set_black(void)
{
  yblack=s1;	
  ERIKA_lcd_write_num(yblack);
}

void CTRL_set_yref(void)
{
  yref=s1;
  
  ERIKA_lcd_write_num(yref);
}

void CTRL_set_ywhite(void)
{
  ywhite=s1;

  if (yblack > ywhite)
    ERIKA_int2df(&temp1, yblack-ywhite); 
  else
    ERIKA_int2df(&temp1, ywhite-yblack);
  
  ERIKA_df_div(&yscale, &w, &temp1);
  
  ERIKA_lcd_write_num(ywhite);	
}


/****************************************************************************
  Changing the power reference
****************************************************************************/


void CTRL_set_normalpowref(ERIKA_INT16 value)
{
  powref[CTRL_2SENSORS]=value;
}

/****************************************************************************
  returns of the control law considering saturation
****************************************************************************/
ERIKA_INT16 CTRL_get_rmotor(void)
{
  if (r_motor_pow<0)
    return 0;
  else
    if (r_motor_pow>255)
      return 255;
    else
      return r_motor_pow;
}

ERIKA_INT16 CTRL_get_lmotor(void)
{
  if (l_motor_pow<0)
    return 0;
  else
    if (l_motor_pow>255)
      return 255;
    else
      return l_motor_pow; 
}
#endif

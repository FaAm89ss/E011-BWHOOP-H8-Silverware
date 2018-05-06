#include "gestures.h"
#include "sixaxis.h"
#include "drv_time.h"
#include "defines.h"
#include "config.h"
#include "pid.h"

extern int ledcommand;
extern int ledblink;
extern int onground;
extern char aux[AUXNUMBER];
extern int play_led;

extern float acro_expo_roll;
extern float acro_expo_pitch;
extern float acro_expo_yaw;
extern float throttle_expo;

extern void flash_save( void);
extern void flash_load( void);

int pid_gestures_used = 0;

void gestures( void)
{
	#ifndef DISABLE_GESTURES2
		int command = gestures2();

		if (command!=GESTURE_NONE)
        {
            if (command == GESTURE_DDD)
		    {
			                  
                //skip accel calibration if pid gestures used
                if ( !pid_gestures_used )
                {
                    gyro_cal();	// for flashing lights
                    acc_cal();                   
                }
                else
                {
                    ledcommand = 1;
                    pid_gestures_used = 0;
                }
                #ifdef FLASH_SAVE2
                extern float accelcal[3];
                flash2_fmc_write( accelcal[0] + 127 , accelcal[1] + 127);
                #endif
                
                #ifdef FLASH_SAVE1
			    
                
                flash_save( );
                flash_load( );
                #endif
			    // reset loop time 
			    extern unsigned long lastlooptime;
			    lastlooptime = gettime();
		    }		
            if (command == GESTURE_UUU)
              {
                 #ifdef RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND                  
                 extern int rx_bind_enable;
                 rx_bind_enable=!rx_bind_enable;
                 ledblink = 2 - rx_bind_enable;
                 pid_gestures_used = 1;  
                 #endif
              }
              
            if (command == GESTURE_RRD)
              {
                  aux[CH_GES_1] = 1;
                  ledcommand = 1;
              }
            if (command == GESTURE_LLD)
              {
                  ledcommand = 1;
                  aux[CH_GES_1] = 0;
              }
							
							
							
							
              
            if (command == GESTURE_LDD)
              {
                  aux[CH_GES_2] = 0;
                  ledcommand = 1;
              }
            if (command == GESTURE_RDD)
              {
                  ledcommand = 1;
                  aux[CH_GES_2] = 1;
              }
              
            if (command == GESTURE_LUU)
              {
                  aux[CH_GES_3] = 0;
                  ledcommand = 1;
              }
            if (command == GESTURE_RUU)
              {
                  ledcommand = 1;
                  aux[CH_GES_3] = 1;
              }
              
            if (command == GESTURE_RLL)
              {
                  if (aux[CH_GES_LOOP_1] == 1) {
										aux[CH_GES_LOOP_1] = 0;
									} else {
										aux[CH_GES_LOOP_1] = 1;
									}
                  ledcommand = 1;
              }
            if (command == GESTURE_LRR)
              {
                  if (aux[CH_GES_LOOP_2] == 1) {
										aux[CH_GES_LOOP_2] = 0;
									} else {
										aux[CH_GES_LOOP_2] = 1;
									}
                  ledcommand = 1;
              }
            if (command == GESTURE_RRL)
              {
                  if (play_led == 1) {
										play_led = 0;
									} else {
										play_led = 1;
									}
                  ledcommand = 1;
              }
							
            if (command == GESTURE_RRR)
              {
								if (acro_expo_roll + ACRO_EXPO_ADJUST_MULTIPLIER <= 1) {
                  acro_expo_roll += ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
								if (acro_expo_pitch + ACRO_EXPO_ADJUST_MULTIPLIER <= 1) {
                  acro_expo_pitch += ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
							
            if (command == GESTURE_LLL)
              {
								if (acro_expo_roll - ACRO_EXPO_ADJUST_MULTIPLIER >= 0) {
                  acro_expo_roll -= ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
								if (acro_expo_pitch - ACRO_EXPO_ADJUST_MULTIPLIER >= 0) {
                  acro_expo_pitch -= ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
							
            if (command == GESTURE_RRU)
              {
								if (acro_expo_yaw + ACRO_EXPO_ADJUST_MULTIPLIER <= 1) {
                  acro_expo_yaw += ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
							
            if (command == GESTURE_LLU)
              {
								if (acro_expo_yaw - ACRO_EXPO_ADJUST_MULTIPLIER >= 0) {
                  acro_expo_yaw -= ACRO_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
							
            if (command == GESTURE_UUD)
              {
								if (throttle_expo + THROTTLE_EXPO_ADJUST_MULTIPLIER <= 1) {
                  throttle_expo += THROTTLE_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
							
            if (command == GESTURE_DDU)
              {
								if (throttle_expo - THROTTLE_EXPO_ADJUST_MULTIPLIER >= 0) {
                  throttle_expo -= THROTTLE_EXPO_ADJUST_MULTIPLIER;
                  ledcommand = 1;
								}
              }
									
						if (ledcommand == 1 && (command == GESTURE_RRR || command == GESTURE_LLL ||command == GESTURE_RRU || command == GESTURE_LLU || command == GESTURE_UUD || command == GESTURE_DDU)) {
							flash_save( );
							flash_load( );
						}

							
							
							
							
							
							
            #ifdef PID_GESTURE_TUNING              
            if ( command >= GESTURE_UDR ) pid_gestures_used = 1;   
              
            if (command == GESTURE_UDU)
              {
                        // Cycle to next pid term (P I D)
                        ledblink = next_pid_term();
              }
            if (command == GESTURE_UDD)
              {
                        // Cycle to next axis (Roll Pitch Yaw)
                        ledblink = next_pid_axis();
              }
            if (command == GESTURE_UDR)
              {
                  // Increase by 10%
                        ledblink = increase_pid();
              }
            if (command == GESTURE_UDL)
              {
                        // Descrease by 10%
                  ledblink = decrease_pid();
              }
            // flash long on zero  
            if ( pid_gestures_used && ledblink == 0) ledcommand = 1; 
              
                // U D U - Next PID term
                // U D D - Next PID Axis
                // U D R - Increase value
                // U D L - Descrease value
               // ledblink = blink; //Will cause led logic to blink the number of times ledblink has stored in it.
                #endif

	  }
		#endif		
 }


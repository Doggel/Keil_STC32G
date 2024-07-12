/*
 * PID Controller Implementation in C
 * 
 * Created by Joshua Saxby (aka @saxbophone) on 1 Jan, 2016
 * 
 * My own attempt at implementing the PID algorithm in some (hopefully) clean, understandable C.
 *
 * See LICENSE for licensing details.
 */

#include "pid.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      λ��ʽPID
//  @param      calibration			�Ա��ض���PID�����Ĳ���
//  @param      state				���ض���ṹ��
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
PID_State* pid_location(PID_Calibration * calibration, PID_State * state) 
{
	long error,derivative;
    // calculate difference between desired and actual values (the error)
    error = state->target - state->actual;
    // calculate and update integral
	
	state->integral += error;
//	if (((int)state->integral) >= 2000)
//		state->integral = 2000;
//	if (((int)state->integral) <= -2000)
//		state->integral = -2000;
    // calculate derivative
    derivative = (error - state->previous_error);
    // calculate output value according to algorithm
    state->output = (
        (calibration->kp * error / 100) 
		+ (calibration->ki * state->integral * 10 / 10000) 
		+ (calibration->kd * derivative / 100)
		);
    // update state.previous_error to the error value calculated on this iteration
    state->previous_error = error;
    // return the state struct reflecting the calculations
    return state;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      λ��ʽPID
//  @param      calibration			�Ա��ض���PID�����Ĳ���
//  @param      state				���ض���ṹ��
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
PID_State_float* pid_location_float(PID_Calibration_float * calibration, PID_State_float * state) 
{
	float error,derivative;
	
    // calculate difference between desired and actual values (the error)
    error = state->target - state->actual;
    // calculate and update integral
	state->integral += error;
//	if ((state->integral) >= 2000)
//		state->integral = 2000;
//	if ((state->integral) <= -2000)
//		state->integral = -2000;
	
    // calculate derivative
    derivative = (error - state->previous_error);
    // calculate output value according to algorithm
    state->output = (
        (calibration->kp * error / 100) 
		+ (calibration->ki * state->integral * 10 / 10000) 
		+ (calibration->kd * derivative / 100)
		);
    // update state.previous_error to the error value calculated on this iteration
    state->previous_error = error;
    // return the state struct reflecting the calculations
    return state;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ʽPID
//  @param      calibration			�Ա��ض���PID�����Ĳ���
//  @param      state				���ض���ṹ��
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
PID_State* pid_increase(PID_Calibration * calibration, PID_State * state) 
{
	long int error,derivative;
	
    error = state->target - state->actual;		//���㵱ǰ���
    derivative = (error - 2*state->previous_error + state->pre_previous_error);
    state->output = (
		state->output
        + (calibration->kp * (error - state -> previous_error) / 100) 
		+ (calibration->ki * error * 10 / 100000) 		// 30%
		+ (calibration->kd * derivative / 10)
		);
	
	//�����ϴ��������ϴ����
    state->pre_previous_error = state -> previous_error;	
    state->previous_error = error;
    return state;
}
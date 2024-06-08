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
	unsigned int error,derivative;
    // calculate difference between desired and actual values (the error)
    error = state->target - state->actual;
    // calculate and update integral
    state->integral += error;
    // calculate derivative
    derivative = (error - state->previous_error);
    // calculate output value according to algorithm
    state->output = (
        (calibration->kp * error) 
		+ (calibration->ki * state->integral) 
		+ (calibration->kd * derivative)
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
	unsigned int error,derivative;
	
    error = state->target - state->actual;		//���㵱ǰ���
    derivative = (error - 2*state->previous_error + state->pre_previous_error);
    state->output = (
		state->output
        + (calibration->kp * (error - state -> previous_error)) 
		+ (calibration->ki * error) 
		+ (calibration->kd * derivative)
		);
	
	//�����ϴ��������ϴ����
    state->pre_previous_error = state -> previous_error;	
    state->previous_error = error;
    return state;
}
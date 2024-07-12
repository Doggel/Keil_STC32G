/*
 * PID Controller Implementation in C
 * 
 * Created by Joshua Saxby (aka @saxbophone) on 1 Jan, 2016
 * 
 * My own attempt at implementing the PID algorithm in some (hopefully) clean, understandable C.
 *
 * See LICENSE for licensing details.
 */

// protection against multiple includes
#ifndef SAXBOPHONE_PID_H
#define SAXBOPHONE_PID_H

#ifdef __cplusplus
extern "C"{
#endif

// PID����
    typedef struct pid_calibration {
        /*
         * struct PID_Calibration
         * 
         * Struct storing calibrated PID constants for a PID Controller
         * These are used for tuning the algorithm and the values they take are
         * dependent upon the application - (in other words, YMMV...)
         */
        long kp; // Proportional gain
        long ki; // Integral gain
        long kd; // Derivative gain
    } PID_Calibration;
	
	// PID����-float�汾
    typedef struct pid_calibration_float {
        /*
         * struct PID_Calibration
         * 
         * Struct storing calibrated PID constants for a PID Controller
         * These are used for tuning the algorithm and the values they take are
         * dependent upon the application - (in other words, YMMV...)
         */
        float kp; // Proportional gain
        float ki; // Integral gain
        float kd; // Derivative gain
    } PID_Calibration_float;

// PID��ǰ״̬������ֵ
    typedef struct pid_state {
        /*
         * struct PID_State
         * 
         * Struct storing the current state of a PID Controller.
         * This is used as the input value to the PID algorithm function, which also
         * returns a PID_State struct reflecting the adjustments suggested by the algorithm.
         * 
         * NOTE: The output field in this struct is set by the PID algorithm function, and
         * is ignored in the actual calculations.
         */
        long actual;                // ��������ʵֵ
        long target;                // Ŀ��ֵ
        long previous_error;        // ��һ�����
		long pre_previous_error;	// ���ϴ���ֻ������ʽpid��ʹ��
        long integral;              // �����֣�ֻ��λ��ʽpid��ʹ��
        long output;                // ����ͨ��pid����õ����������
    } PID_State;

	// PID��ǰ״̬������ֵ-float�汾
    typedef struct pid_state_float {
        /*
         * struct PID_State
         * 
         * Struct storing the current state of a PID Controller.
         * This is used as the input value to the PID algorithm function, which also
         * returns a PID_State struct reflecting the adjustments suggested by the algorithm.
         * 
         * NOTE: The output field in this struct is set by the PID algorithm function, and
         * is ignored in the actual calculations.
         */
        float actual;                // ��������ʵֵ
        float target;                // Ŀ��ֵ
        float previous_error;        // ��һ�����
		float pre_previous_error;	// ���ϴ���ֻ������ʽpid��ʹ��
        float integral;              // �����֣�ֻ��λ��ʽpid��ʹ��
        float output;                // ����ͨ��pid����õ����������
    } PID_State_float;

    /*
     * PID Controller Algorithm implementation
     * 
     * Given a PID calibration for the P, I and D values and a PID_State for the current
     * state of the PID controller, calculate the new state for the PID Controller and set
     * the output state to compensate for any error as defined by the algorithm
     */
    PID_State* pid_location(PID_Calibration * calibration, PID_State * state);
	PID_State* pid_increase(PID_Calibration * calibration, PID_State * state);
	PID_State_float* pid_location_float(PID_Calibration_float * calibration, PID_State_float * state);

#ifdef __cplusplus
} // extern "C"
#endif

// end of header
#endif

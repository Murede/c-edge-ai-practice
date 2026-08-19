#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_MAX_SPEED 5000

typedef enum
{
    MOTOR_IDLE,
    MOTOR_STARTING,
    MOTOR_RUNNING,
    MOTOR_STOPPING,
    MOTOR_FAULT
} MotorState;

typedef enum
{
    EVENT_START,
    EVENT_STOP,
    EVENT_START_COMPLETE,
    EVENT_STOP_COMPLETE,
    EVENT_FAULT,
    EVENT_RESET
} MotorEvent;

typedef enum
{
    MOTOR_OK = 0,
    MOTOR_ERROR_NULL,
    MOTOR_ERROR_INVALID_TRANSITION,
    MOTOR_ERROR_INVALID_SPEED,
    MOTOR_ERROR_FAULT_STATE,
    MOTOR_ERROR_INVALID_STATE
} MotorError;

typedef struct
{
    int current_speed;
    int target_speed;
    double temperature;
    MotorState state;
    MotorError error;
} MotorController;

MotorError motor_init(MotorController *controller, int target_speed);

MotorError motor_handle_event(
    MotorController *controller,
    MotorEvent event
);

MotorError motor_set_target_speed(
    MotorController *controller,
    int target_speed
);

const char *motor_state_to_string(MotorState state);
const char *motor_error_to_string(MotorError error);

#endif
#include "motor.h"
#include <stddef.h>

static MotorError determine_next_state(
    MotorState current_state,
    MotorEvent event,
    MotorState *next_state)
{
    if (next_state == NULL)
    {
        return MOTOR_ERROR_NULL;
    }

    switch (current_state)
    {
        case MOTOR_IDLE:
            switch (event)
            {
                case EVENT_START:
                    *next_state = MOTOR_STARTING;
                    return MOTOR_OK;

                case EVENT_FAULT:
                    *next_state = MOTOR_FAULT;
                    return MOTOR_OK;

                default:
                    return MOTOR_ERROR_INVALID_TRANSITION;
            }

        case MOTOR_STARTING:
            switch (event)
            {
                case EVENT_START_COMPLETE:
                    *next_state = MOTOR_RUNNING;
                    return MOTOR_OK;

                case EVENT_STOP:
                    *next_state = MOTOR_STOPPING;
                    return MOTOR_OK;

                case EVENT_FAULT:
                    *next_state = MOTOR_FAULT;
                    return MOTOR_OK;

                default:
                    return MOTOR_ERROR_INVALID_TRANSITION;
            }

        case MOTOR_RUNNING:
            switch (event)
            {
                case EVENT_STOP:
                    *next_state = MOTOR_STOPPING;
                    return MOTOR_OK;

                case EVENT_FAULT:
                    *next_state = MOTOR_FAULT;
                    return MOTOR_OK;

                default:
                    return MOTOR_ERROR_INVALID_TRANSITION;
            }

        case MOTOR_STOPPING:
            switch (event)
            {
                case EVENT_STOP_COMPLETE:
                    *next_state = MOTOR_IDLE;
                    return MOTOR_OK;

                case EVENT_FAULT:
                    *next_state = MOTOR_FAULT;
                    return MOTOR_OK;

                default:
                    return MOTOR_ERROR_INVALID_TRANSITION;
            }

        case MOTOR_FAULT:
            switch (event)
            {
                case EVENT_RESET:
                    *next_state = MOTOR_IDLE;
                    return MOTOR_OK;

                default:
                    return MOTOR_ERROR_INVALID_TRANSITION;
            }

        default:
            return MOTOR_ERROR_INVALID_STATE;
    }
}


static void apply_state_actions(
    MotorController *controller,
    MotorState next_state)
{
    switch (next_state)
    {
        case MOTOR_IDLE:
            controller->current_speed = 0;
            break;

        case MOTOR_STARTING:
            break;

        case MOTOR_RUNNING:
            controller->current_speed = controller->target_speed;
            break;

        case MOTOR_STOPPING:
            break;

        case MOTOR_FAULT:
            controller->current_speed = 0;
            break;

        default:
            break;
    }
}


MotorError motor_init(
    MotorController *controller,
    int target_speed)
{
    if (controller == NULL)
    {
        return MOTOR_ERROR_NULL;
    }

    if (target_speed < 0 || target_speed > MOTOR_MAX_SPEED)
    {
        return MOTOR_ERROR_INVALID_SPEED;
    }

    controller->current_speed = 0;
    controller->target_speed = target_speed;
    controller->temperature = 25.0;
    controller->state = MOTOR_IDLE;
    controller->error = MOTOR_OK;

    return MOTOR_OK;
}


MotorError motor_handle_event(
    MotorController *controller,
    MotorEvent event)
{
    if (controller == NULL)
    {
        return MOTOR_ERROR_NULL;
    }

    MotorState next_state;

    controller->error = determine_next_state(
        controller->state,
        event,
        &next_state
    );

    if (controller->error != MOTOR_OK)
    {
        return controller->error;
    }

    apply_state_actions(controller, next_state);

    controller->state = next_state;

    return MOTOR_OK;
}


MotorError motor_set_target_speed(
    MotorController *controller,
    int target_speed)
{
    if (controller == NULL)
    {
        return MOTOR_ERROR_NULL;
    }

    if (target_speed < 0 || target_speed > MOTOR_MAX_SPEED)
    {
        return MOTOR_ERROR_INVALID_SPEED;
    }

    if (controller->state == MOTOR_FAULT)
    {
        return MOTOR_ERROR_FAULT_STATE;
    }

    controller->target_speed = target_speed;

    return MOTOR_OK;
}


const char *motor_state_to_string(MotorState state)
{
    switch (state)
    {
        case MOTOR_IDLE:
            return "MOTOR_IDLE";

        case MOTOR_STARTING:
            return "MOTOR_STARTING";

        case MOTOR_RUNNING:
            return "MOTOR_RUNNING";

        case MOTOR_STOPPING:
            return "MOTOR_STOPPING";

        case MOTOR_FAULT:
            return "MOTOR_FAULT";

        default:
            return "UNKNOWN_STATE";
    }
}


const char *motor_error_to_string(MotorError error)
{
    switch (error)
    {
        case MOTOR_OK:
            return "MOTOR_OK";

        case MOTOR_ERROR_NULL:
            return "MOTOR_ERROR_NULL";

        case MOTOR_ERROR_INVALID_TRANSITION:
            return "MOTOR_ERROR_INVALID_TRANSITION";

        case MOTOR_ERROR_INVALID_SPEED:
            return "MOTOR_ERROR_INVALID_SPEED";

        case MOTOR_ERROR_FAULT_STATE:
            return "MOTOR_ERROR_FAULT_STATE";

        case MOTOR_ERROR_INVALID_STATE:
            return "MOTOR_ERROR_INVALID_STATE";

        default:
            return "UNKNOWN_ERROR";
    }
}
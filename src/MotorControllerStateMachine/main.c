#include <stdio.h>
#include "motor.h"

static void print_motor_status(
    const MotorController *controller)
{
    printf(
        "State: %s\n",
        motor_state_to_string(controller->state)
    );

    printf(
        "Current Speed: %d RPM\n",
        controller->current_speed
    );

    printf(
        "Target Speed: %d RPM\n",
        controller->target_speed
    );

    printf(
        "Temperature: %.1f C\n\n",
        controller->temperature
    );
}


static void print_result(
    const char *test_name,
    MotorError result)
{
    printf(
        "%s: %s\n",
        test_name,
        motor_error_to_string(result)
    );
}


int main(void)
{
    MotorController motor;
    MotorError result;


    // -------------------------
    // Test 1: Initialization
    // -------------------------

    printf("=== Test 1: Initialization ===\n");

    result = motor_init(&motor, 1500);

    print_result("Initialization", result);

    if (result != MOTOR_OK)
    {
        return 1;
    }

    print_motor_status(&motor);


    // -------------------------
    // Test 2: Normal Operation
    // -------------------------

    printf("=== Test 2: Normal Operation ===\n");

    result = motor_handle_event(&motor, EVENT_START);

    print_result("EVENT_START", result);
    print_motor_status(&motor);


    result = motor_handle_event(
        &motor,
        EVENT_START_COMPLETE
    );

    print_result("EVENT_START_COMPLETE", result);
    print_motor_status(&motor);


    result = motor_handle_event(&motor, EVENT_STOP);

    print_result("EVENT_STOP", result);
    print_motor_status(&motor);


    result = motor_handle_event(
        &motor,
        EVENT_STOP_COMPLETE
    );

    print_result("EVENT_STOP_COMPLETE", result);
    print_motor_status(&motor);


    // -------------------------
    // Test 3: Fault Validation
    // -------------------------

    printf("=== Test 3: Fault Validation ===\n");

    result = motor_handle_event(&motor, EVENT_START);

    result = motor_handle_event(
        &motor,
        EVENT_START_COMPLETE
    );

    result = motor_handle_event(&motor, EVENT_FAULT);

    print_result("EVENT_FAULT", result);
    print_motor_status(&motor);


    // Attempt invalid transition while faulted

    result = motor_handle_event(&motor, EVENT_START);

    print_result(
        "START while faulted",
        result
    );

    print_motor_status(&motor);


    // Attempt target speed change while faulted

    result = motor_set_target_speed(&motor, 2500);

    print_result(
        "Target speed change while faulted",
        result
    );

    print_motor_status(&motor);


    // Reset fault

    result = motor_handle_event(&motor, EVENT_RESET);

    print_result("EVENT_RESET", result);
    print_motor_status(&motor);


    // -------------------------
    // Test 4: Invalid Inputs
    // -------------------------

    printf("=== Test 4: Invalid Inputs ===\n");


    result = motor_init(NULL, 1500);

    print_result(
        "NULL initialization",
        result
    );


    result = motor_init(&motor, -500);

    print_result(
        "Negative speed initialization",
        result
    );


    result = motor_init(
        &motor,
        MOTOR_MAX_SPEED + 1
    );

    print_result(
        "Above-maximum speed initialization",
        result
    );


    result = motor_handle_event(
        NULL,
        EVENT_START
    );

    print_result(
        "NULL event handling",
        result
    );


    result = motor_set_target_speed(
        NULL,
        2000
    );

    print_result(
        "NULL target speed update",
        result
    );


    // Reinitialize for invalid transition test

    result = motor_init(&motor, 1000);

    if (result != MOTOR_OK)
    {
        print_result(
            "Reinitialization",
            result
        );

        return 1;
    }


    // EVENT_STOP is invalid while already IDLE

    result = motor_handle_event(
        &motor,
        EVENT_STOP
    );

    print_result(
        "Invalid IDLE + STOP",
        result
    );

    print_motor_status(&motor);


    return 0;
}
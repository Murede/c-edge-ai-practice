#include <stdio.h>

#include "sensor_driver.h"

int main(void)
{
    SensorDriverError result;

    // =====================================================
    // Phase 1: CONTROL Register Operations
    // =====================================================

    SensorRegister sensor = {
        .CONTROL = 0u,
        .STATUS = SENSOR_READY_MASK,
        .DATA = 2048u,
        .ERROR = 0u};


    // Initial
    printf(
        "CONTROL: 0x%08X\n",
        (unsigned int)sensor.CONTROL
    );

    // Enable sensor 
    result = sensor_enable(&sensor);

    printf(
        "Enable result: %s\n",
        sensor_driver_error_to_string(result)
    );

    printf(
        "CONTROL: 0x%08X\n",
        (unsigned int)sensor.CONTROL
    );

    // Start Sensor 
    result = sensor_start(&sensor);

    printf(
        "Start result: %s\n",
        sensor_driver_error_to_string(result)
    );

    printf(
        "CONTROL: 0x%08X\n",
        (unsigned int)sensor.CONTROL
    );

    // Reset Sensor
    result = sensor_reset(&sensor);

    printf(
        "Reset result: %s\n",
        sensor_driver_error_to_string(result)
    );

    printf(
        "CONTROL: 0x%08X\n",
        (unsigned int)sensor.CONTROL
    );

    // Disable Sensor 
    result = sensor_disable(&sensor);
    
    printf(
        "Disable result: %s\n",
        sensor_driver_error_to_string(result)
    );

    printf(
        "CONTROL: 0x%08X\n",
        (unsigned int)sensor.CONTROL
    );

    SensorRegister sensor1 = {
    .CONTROL = 0u,
    .STATUS = SENSOR_READY_MASK,
    .DATA = 2048u,
    .ERROR = 0u
    };

    uint8_t is_ready = 0u;
    uint8_t is_busy = 0u;
    uint8_t has_error = 0u;

    // =====================================================
    // Phase 2: STATUS Register Checks
    // =====================================================

    // Check READY flag
    result =
        sensor_is_ready(
            &sensor1,
            &is_ready
        );

    printf(
        "READY check: result=%s, value=%u\n",
        sensor_driver_error_to_string(result),
        (unsigned int)is_ready
    );


    // Check BUSY flag
    result =
        sensor_is_busy(
            &sensor1,
            &is_busy
        );

    printf(
        "BUSY check: result=%s, value=%u\n",
        sensor_driver_error_to_string(result),
        (unsigned int)is_busy
    );


    // Check ERROR flag
    result =
        sensor_has_error(
            &sensor1,
            &has_error
        );

    printf(
        "ERROR check: result=%s, value=%u\n",
        sensor_driver_error_to_string(result),
        (unsigned int)has_error
    );

    // =====================================================
    // Phase 3: ADC DATA Register Reading
    // =====================================================
    //
    // The simulated sensor was initialized with:
    //
    //     STATUS = SENSOR_READY_MASK
    //     DATA   = 2048
    //
    // Since READY is set, sensor_read() should successfully
    // extract the 12-bit ADC value from the DATA register.
    //

    uint16_t reading = 0u;

    result =
        sensor_read(
            &sensor,
            &reading
        );

    printf(
        "ADC read: result=%s, value=%u\n",
        sensor_driver_error_to_string(result),
        (unsigned int)reading
    );


    // =====================================================
    // Phase 4: Hardware Error State Testing
    // =====================================================
    //
    // Create a second simulated peripheral representing a sensor
    // that has reported a hardware fault.
    //
    // STATUS:
    //     ERROR bit = 1
    //
    // ERROR:
    //     error code = 0x05
    //
    // This lets us test the distinction between:
    //     1. Detecting that an error exists using STATUS.
    //     2. Reading the detailed error code using ERROR.
    //

    SensorRegister fault_sensor = {
        .CONTROL = 0u,
        .STATUS = SENSOR_ERROR_MASK,
        .DATA = 0u,
        .ERROR = 0x05u
    };

    uint8_t fault_detected = 0u;
    uint8_t error_code = 0u;


    // Check whether the STATUS register reports an error.
    result =
        sensor_has_error(
            &fault_sensor,
            &fault_detected
        );

    printf(
        "Hardware error flag: result=%s, value=%u\n",
        sensor_driver_error_to_string(result),
        (unsigned int)fault_detected
    );


    // Read the detailed 4-bit hardware error code.
    result =
        sensor_get_error(
            &fault_sensor,
            &error_code
        );

    printf(
        "Hardware error code: result=%s, code=0x%02X\n",
        sensor_driver_error_to_string(result),
        (unsigned int)error_code
    );


    // =====================================================
    // Phase 5: Driver Error and Invalid-Input Testing
    // =====================================================
    //
    // These tests intentionally provide invalid states or pointers
    // to verify that the driver rejects unsafe operations instead
    // of dereferencing invalid memory or returning invalid data.
    //


    // -----------------------------------------------------
    // Test 1: DATA register is not ready
    // -----------------------------------------------------
    //
    // DATA contains a value, but READY is not set.
    // sensor_read() should therefore refuse to return the data.
    //

    SensorRegister not_ready_sensor = {
        .CONTROL = 0u,
        .STATUS = 0u,
        .DATA = 1234u,
        .ERROR = 0u
    };

    reading = 0u;

    result =
        sensor_read(
            &not_ready_sensor,
            &reading
        );

    printf(
        "Not-ready ADC read: %s\n",
        sensor_driver_error_to_string(result)
    );


    // -----------------------------------------------------
    // Test 2: NULL sensor pointer
    // -----------------------------------------------------

    result =
        sensor_enable(
            NULL
        );

    printf(
        "NULL sensor test: %s\n",
        sensor_driver_error_to_string(result)
    );


    // -----------------------------------------------------
    // Test 3: NULL READY output pointer
    // -----------------------------------------------------

    result =
        sensor_is_ready(
            &sensor,
            NULL
        );

    printf(
        "NULL READY output test: %s\n",
        sensor_driver_error_to_string(result)
    );


    // -----------------------------------------------------
    // Test 4: NULL ADC output pointer
    // -----------------------------------------------------

    result =
        sensor_read(
            &sensor,
            NULL
        );

    printf(
        "NULL ADC output test: %s\n",
        sensor_driver_error_to_string(result)
    );


    // -----------------------------------------------------
    // Test 5: NULL error-code output pointer
    // -----------------------------------------------------

    result =
        sensor_get_error(
            &fault_sensor,
            NULL
        );

    printf(
        "NULL error-code output test: %s\n",
        sensor_driver_error_to_string(result)
    );


    // -----------------------------------------------------
    // Test 6: Disable an already-disabled sensor
    // -----------------------------------------------------
    //
    // The simulated sensor begins disabled. Attempting to disable
    // it again should exercise SENSOR_DRIVER_ERROR_NOT_READY.
    //

    SensorRegister disabled_sensor = {
        .CONTROL = 0u,
        .STATUS = 0u,
        .DATA = 0u,
        .ERROR = 0u
    };

    result =
        sensor_disable(
            &disabled_sensor
        );

    printf(
        "Already-disabled sensor test: %s\n",
        sensor_driver_error_to_string(result)
    );
     return 0;
}
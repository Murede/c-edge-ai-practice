/*
 * Simulated Sensor Peripheral Driver
 * ----------------------------------
 * This file implements the public API defined in sensor_driver.h.
 *
 * Driver responsibilities:
 *
 *   - Modify CONTROL register bits safely.
 *   - Read and normalize STATUS register flags.
 *   - Verify DATA is ready before returning an ADC measurement.
 *   - Extract the hardware error code from the ERROR register.
 *   - Return explicit SensorDriverError values for invalid operations.
 *
 * The driver hides register manipulation from higher-level application
 * code so that main.c can interact with the sensor through functions
 * rather than directly modifying hardware-style registers.
 */

#include "sensor_driver.h"


/* =========================================================
 * CONTROL Register Implementation
 * =========================================================
 */


/*
 * Enable the sensor peripheral.
 *
 * Sets only the ENABLE bit while preserving all other CONTROL bits.
 */
SensorDriverError sensor_enable(
    SensorRegister *sensor)
{
    if (sensor == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    sensor->CONTROL |= SENSOR_ENABLE_MASK;

    return SENSOR_DRIVER_OK;
}


/*
 * Disable the sensor peripheral.
 *
 * The function first checks whether the ENABLE bit is currently set.
 * If the peripheral is already disabled, the operation returns
 * SENSOR_DRIVER_ERROR_NOT_READY.
 */
SensorDriverError sensor_disable(
    SensorRegister *sensor)
{
    if (sensor == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    if ((sensor->CONTROL & SENSOR_ENABLE_MASK) == 0u)
    {
        return SENSOR_DRIVER_ERROR_NOT_READY;
    }

    /*
     * Clear only the ENABLE bit.
     *
     * ~SENSOR_ENABLE_MASK creates a mask containing a zero at
     * the ENABLE position and ones everywhere else.
     */
    sensor->CONTROL &= ~SENSOR_ENABLE_MASK;

    return SENSOR_DRIVER_OK;
}


/*
 * Start a sensor operation.
 *
 * Sets the START bit without modifying the other CONTROL bits.
 */
SensorDriverError sensor_start(
    SensorRegister *sensor)
{
    if (sensor == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    sensor->CONTROL |= SENSOR_START_MASK;

    return SENSOR_DRIVER_OK;
}


/*
 * Request a peripheral reset.
 *
 * Sets the RESET bit while preserving all other CONTROL bits.
 */
SensorDriverError sensor_reset(
    SensorRegister *sensor)
{
    if (sensor == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    sensor->CONTROL |= SENSOR_RESET_MASK;

    return SENSOR_DRIVER_OK;
}


/* =========================================================
 * STATUS Register Implementation
 * =========================================================
 */


/*
 * Read the READY flag.
 *
 * The bitwise AND isolates the READY bit.
 * The != 0u comparison normalizes the result to:
 *
 *      0 -> not ready
 *      1 -> ready
 */
SensorDriverError sensor_is_ready(
    const SensorRegister *sensor,
    uint8_t *is_ready)
{
    if (sensor == NULL || is_ready == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    *is_ready = (uint8_t)(
        (sensor->STATUS & SENSOR_READY_MASK) != 0u
    );

    return SENSOR_DRIVER_OK;
}


/*
 * Read the BUSY flag.
 *
 * The result is normalized to either 0 or 1.
 */
SensorDriverError sensor_is_busy(
    const SensorRegister *sensor,
    uint8_t *is_busy)
{
    if (sensor == NULL || is_busy == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    *is_busy = (uint8_t)(
        (sensor->STATUS & SENSOR_BUSY_MASK) != 0u
    );

    return SENSOR_DRIVER_OK;
}


/*
 * Read the STATUS register's ERROR flag.
 *
 * This flag only indicates that an error exists.
 * The detailed error code is stored separately in the ERROR register.
 */
SensorDriverError sensor_has_error(
    const SensorRegister *sensor,
    uint8_t *has_error)
{
    if (sensor == NULL || has_error == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    *has_error = (uint8_t)(
        (sensor->STATUS & SENSOR_ERROR_MASK) != 0u
    );

    return SENSOR_DRIVER_OK;
}


/* =========================================================
 * DATA Register Implementation
 * =========================================================
 */


/*
 * Read the sensor's ADC measurement.
 *
 * DATA is considered valid only while the READY flag is set.
 *
 * The ADC measurement occupies bits 11-0 of the DATA register.
 * SENSOR_ADC_MASK removes all reserved upper bits.
 */
SensorDriverError sensor_read(
    const SensorRegister *sensor,
    uint16_t *reading)
{
    if (sensor == NULL || reading == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    /*
     * Do not treat DATA as valid unless READY is set.
     *
     * Only the READY bit is checked. BUSY or ERROR flags must
     * not accidentally cause the operation to succeed.
     */
    if ((sensor->STATUS & SENSOR_READY_MASK) == 0u)
    {
        return SENSOR_DRIVER_ERROR_NOT_READY;
    }

    /*
     * Extract the ADC field.
     *
     * SENSOR_ADC_MASK  = 0x0FFF
     *
     * This preserves bits 11-0 while clearing all upper bits.
     * The field already starts at bit 0, so the shift is zero.
     */
    *reading = (uint16_t)(
        (sensor->DATA & SENSOR_ADC_MASK)
        >> SENSOR_ADC_SHIFT
    );

    return SENSOR_DRIVER_OK;
}


/* =========================================================
 * ERROR Register Implementation
 * =========================================================
 */


/*
 * Read the hardware error code.
 *
 * The detailed error value is stored in bits 3-0 of the
 * peripheral ERROR register.
 */
SensorDriverError sensor_get_error(
    const SensorRegister *sensor,
    uint8_t *error_code)
{
    if (sensor == NULL || error_code == NULL)
    {
        return SENSOR_DRIVER_ERROR_NULL;
    }

    /*
     * Extract the four-bit hardware error code.
     *
     * SENSOR_ERROR_CODE_MASK = 0x000F
     */
    *error_code = (uint8_t)(
        (sensor->ERROR & SENSOR_ERROR_CODE_MASK)
        >> SENSOR_ERROR_CODE_SHIFT
    );

    return SENSOR_DRIVER_OK;
}


/* =========================================================
 * Driver Error String Helper
 * =========================================================
 */


/*
 * Convert a SensorDriverError enum value into readable text.
 *
 * Intended for debugging, logging, and the project's test harness.
 */
const char *sensor_driver_error_to_string(
    SensorDriverError error)
{
    switch (error)
    {
        case SENSOR_DRIVER_OK:
            return "SENSOR_DRIVER_OK";

        case SENSOR_DRIVER_ERROR_NULL:
            return "SENSOR_DRIVER_ERROR_NULL";

        case SENSOR_DRIVER_ERROR_NOT_READY:
            return "SENSOR_DRIVER_ERROR_NOT_READY";

        default:
            return "UNKNOWN_SENSOR_DRIVER_ERROR";
    }
}
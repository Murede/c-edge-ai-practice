#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>

/*
 * Simulated Sensor Peripheral Driver
 * ----------------------------------
 * This header defines the public interface and register map for a simulated
 * memory-mapped sensor peripheral.
 *
 * Driver Architecture:
 *
 *      Application / main.c
 *              |
 *              v
 *      sensor_driver.h
 *      Public API + register definitions
 *              |
 *              v
 *      sensor_driver.c
 *      Driver implementation
 *              |
 *              v
 *      SensorRegister
 *      Simulated memory-mapped register block
 *
 * The application should interact with the sensor through the public
 * driver functions rather than manipulating register fields directly.
 *
 * Register Map:
 *
 * Offset   Register   Access   Purpose
 * ---------------------------------------------------------
 * 0x00     CONTROL    R/W      Enable, start, and reset
 * 0x04     STATUS     R        Ready, busy, and error flags
 * 0x08     DATA       R        12-bit ADC measurement
 * 0x0C     ERROR      R        4-bit hardware error code
 *
 * CONTROL Register:
 *
 * Bit:       2        1        0
 *          RESET    START    ENABLE
 *
 * STATUS Register:
 *
 * Bit:       2        1        0
 *          ERROR     BUSY     READY
 *
 * DATA Register:
 *
 * Bits 11-0 : 12-bit ADC measurement
 *
 * ERROR Register:
 *
 * Bits 3-0 : 4-bit hardware error code
 */


/* =========================================================
 * Register Map
 * =========================================================
 *
 * The fields are declared volatile because they model hardware
 * registers whose accesses must be preserved by the compiler.
 *
 * STATUS, DATA, and ERROR are also const because software should
 * only read these registers.
 */

typedef struct
{
    volatile uint32_t CONTROL;        // Offset 0x00 - Read/Write
    const volatile uint32_t STATUS;   // Offset 0x04 - Read only
    const volatile uint32_t DATA;     // Offset 0x08 - Read only
    const volatile uint32_t ERROR;    // Offset 0x0C - Read only

} SensorRegister;


/* =========================================================
 * CONTROL Register Bit Positions and Masks
 * =========================================================
 */

// Bit 0: Enable the sensor peripheral.
#define SENSOR_ENABLE_BIT   0u
#define SENSOR_ENABLE_MASK  (1u << SENSOR_ENABLE_BIT)

// Bit 1: Start a sensor operation.
#define SENSOR_START_BIT    1u
#define SENSOR_START_MASK   (1u << SENSOR_START_BIT)

// Bit 2: Request a peripheral reset.
#define SENSOR_RESET_BIT    2u
#define SENSOR_RESET_MASK   (1u << SENSOR_RESET_BIT)


/* =========================================================
 * STATUS Register Bit Positions and Masks
 * =========================================================
 */

// Bit 0: Sensor data is ready to be read.
#define SENSOR_READY_BIT    0u
#define SENSOR_READY_MASK   (1u << SENSOR_READY_BIT)

// Bit 1: Sensor is currently performing an operation.
#define SENSOR_BUSY_BIT     1u
#define SENSOR_BUSY_MASK    (1u << SENSOR_BUSY_BIT)

// Bit 2: Sensor hardware has reported an error.
#define SENSOR_ERROR_BIT    2u
#define SENSOR_ERROR_MASK   (1u << SENSOR_ERROR_BIT)


/* =========================================================
 * DATA Register Field
 * =========================================================
 *
 * ADC measurement occupies bits 11-0.
 */

#define SENSOR_ADC_SHIFT    0u
#define SENSOR_ADC_MASK     0x0FFFu


/* =========================================================
 * ERROR Register Field
 * =========================================================
 *
 * Hardware error code occupies bits 3-0.
 */

#define SENSOR_ERROR_CODE_SHIFT   0u
#define SENSOR_ERROR_CODE_MASK    0x000Fu


/* =========================================================
 * Driver Error Type
 * =========================================================
 *
 * These values describe failures in the software driver API.
 * They are separate from the hardware error code stored in the
 * peripheral ERROR register.
 */

typedef enum
{
    SENSOR_DRIVER_OK = 0,

    // A required input or output pointer was NULL.
    SENSOR_DRIVER_ERROR_NULL,

    // The requested operation requires the sensor to be ready/enabled.
    SENSOR_DRIVER_ERROR_NOT_READY
} SensorDriverError;


/* =========================================================
 * Public Driver API
 * =========================================================
 *
 * CONTROL functions modify the peripheral's CONTROL register.
 *
 * STATUS functions read hardware-generated flags.
 *
 * sensor_read() extracts the 12-bit ADC measurement from DATA.
 *
 * sensor_get_error() extracts the 4-bit hardware error code.
 */


/* CONTROL Register Functions */

SensorDriverError sensor_enable(
    SensorRegister *sensor
);

SensorDriverError sensor_disable(
    SensorRegister *sensor
);

SensorDriverError sensor_start(
    SensorRegister *sensor
);

SensorDriverError sensor_reset(
    SensorRegister *sensor
);


/* STATUS Register Functions */

SensorDriverError sensor_is_ready(
    const SensorRegister *sensor,
    uint8_t *is_ready
);

SensorDriverError sensor_is_busy(
    const SensorRegister *sensor,
    uint8_t *is_busy
);

SensorDriverError sensor_has_error(
    const SensorRegister *sensor,
    uint8_t *has_error
);


/* DATA Register Function */

SensorDriverError sensor_read(
    const SensorRegister *sensor,
    uint16_t *reading
);


/* ERROR Register Function */

SensorDriverError sensor_get_error(
    const SensorRegister *sensor,
    uint8_t *error_code
);


/* Debug / Logging Helper */

const char *sensor_driver_error_to_string(
    SensorDriverError error
);


#endif /* SENSOR_DRIVER_H */
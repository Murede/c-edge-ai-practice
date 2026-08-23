#ifndef STATUS_REGISTER_H
#define STATUS_REGISTER_H

#include <stdint.h>

/*
 * Status Register Layout
 *
 * Bit:      7  6 | 5  4 | 3 | 2 | 1 | 0
 *          -------+------+---+---+---+---
 *           RSV   | MODE |RDY|FLT|DIR|ENA
 *
 * Bits 7-6 : Reserved
 * Bits 5-4 : Operating mode
 * Bit  3   : Data-ready flag
 * Bit  2   : Fault flag
 * Bit  1   : Direction flag
 * Bit  0   : Enable flag
 */

/* Single-bit flag positions */
#define STATUS_ENABLE_BIT      0u
#define STATUS_DIRECTION_BIT   1u
#define STATUS_FAULT_BIT       2u
#define STATUS_READY_BIT       3u

/* Single-bit flag masks */
#define STATUS_ENABLE_MASK \
    (1u << STATUS_ENABLE_BIT)

#define STATUS_DIRECTION_MASK \
    (1u << STATUS_DIRECTION_BIT)

#define STATUS_FAULT_MASK \
    (1u << STATUS_FAULT_BIT)

#define STATUS_READY_MASK \
    (1u << STATUS_READY_BIT)

/* MODE field configuration */
#define STATUS_MODE_SHIFT       4u
#define STATUS_MODE_VALUE_MASK  0x03u
#define STATUS_MODE_FIELD_MASK  0x30u


typedef enum
{
    STATUS_OK = 0,
    STATUS_ERROR_NULL,
    STATUS_ERROR_INVALID_BIT,
    STATUS_ERROR_INVALID_MODE
} StatusError;


/* Public API */

StatusError status_register_set_flag(
    uint8_t *reg,
    int bit
);

StatusError status_register_clear_flag(
    uint8_t *reg,
    int bit
);

StatusError status_register_toggle_flag(
    uint8_t *reg,
    int bit
);

StatusError status_register_is_flag_set(
    const uint8_t *reg,
    int bit,
    uint8_t *is_set
);

StatusError status_register_set_mode(
    uint8_t *reg,
    uint8_t mode
);

StatusError status_register_get_mode(
    const uint8_t *reg,
    uint8_t *mode
);

const char *status_error_to_string(
    StatusError error
);

#endif /* STATUS_REGISTER_H */
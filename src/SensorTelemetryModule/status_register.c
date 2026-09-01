/*
 * Status Register Module
 * ----------------------
 * This module simulates an 8-bit hardware status/control register for a
 * sensor device. Individual bits represent device flags, while a multi-bit
 * field represents the device operating mode.
 *
 * Register Layout:
 *
 * Bit:      7  6 | 5  4 | 3 | 2 | 1 | 0
 *          -------+------+---+---+---+---
 *           RSV   | MODE |RDY|FLT|DIR|ENA
 *
 * Bits 7-6 : Reserved
 * Bits 5-4 : Operating mode
 * Bit  3   : Data-ready flag
 * Bit  2   : Fault flag
 * Bit  1   : Motor/sensor direction
 * Bit  0   : Device enable
 *
 * The module provides operations to:
 *   - Set individual status flags
 *   - Clear individual status flags
 *   - Toggle individual status flags
 *   - Check whether a flag is set
 *   - Set the operating-mode field
 *   - Extract the current operating mode
 *   - Validate register operations where necessary
 *
 * The purpose of this module is to practice hardware-style register
 * manipulation using fixed-width integers, bit masks, bitwise operators,
 * shifts, and clean C module interfaces.
 */

#include <stddef.h>

#include "status_register.h"


StatusError status_register_set_flag(
    uint8_t *reg,
    int bit)
{
    if (reg == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    if (bit < 0 || bit > 3)
    {
        return STATUS_ERROR_INVALID_BIT;
    }

    uint8_t set_mask =
        (uint8_t)(1u << bit);

    *reg =
        (uint8_t)(*reg | set_mask);

    return STATUS_OK;
}


StatusError status_register_clear_flag(
    uint8_t *reg,
    int bit)
{
    if (reg == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    if (bit < 0 || bit > 3)
    {
        return STATUS_ERROR_INVALID_BIT;
    }

    uint8_t clear_mask =
        (uint8_t)~(1u << bit);

    *reg =
        (uint8_t)(*reg & clear_mask);

    return STATUS_OK;
}


StatusError status_register_toggle_flag(
    uint8_t *reg,
    int bit)
{
    if (reg == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    if (bit < 0 || bit > 3)
    {
        return STATUS_ERROR_INVALID_BIT;
    }

    uint8_t toggle_mask =
        (uint8_t)(1u << bit);

    *reg =
        (uint8_t)(*reg ^ toggle_mask);

    return STATUS_OK;
}


StatusError status_register_is_flag_set(
    const uint8_t *reg,
    int bit,
    uint8_t *is_set)
{
    if (reg == NULL || is_set == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    if (bit < 0 || bit > 3)
    {
        return STATUS_ERROR_INVALID_BIT;
    }

    *is_set =
        (uint8_t)((*reg & (uint8_t)(1u << bit)) != 0u);

    return STATUS_OK;
}


StatusError status_register_set_mode(
    uint8_t *reg,
    uint8_t mode)
{
    if (reg == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    if (mode > STATUS_MODE_VALUE_MASK)
    {
        return STATUS_ERROR_INVALID_MODE;
    }

    /*
     * Clear the existing MODE field.
     *
     * STATUS_MODE_FIELD_MASK = 0x30 = 0011 0000
     * Inverting it produces:          1100 1111
     *
     * ANDing with the inverted mask clears bits 5-4
     * while preserving every other register bit.
     */
    *reg &=
        (uint8_t)~STATUS_MODE_FIELD_MASK;

    /*
     * Restrict the requested mode to its 2-bit width.
     *
     * STATUS_MODE_VALUE_MASK = 0x03 = 0000 0011
     */
    mode &=
        STATUS_MODE_VALUE_MASK;

    /*
     * Shift the mode from bits 1-0 into bits 5-4.
     */
    uint8_t shifted_mode =
        (uint8_t)(mode << STATUS_MODE_SHIFT);

    /*
     * Insert the new MODE field into the register.
     */
    *reg =
        (uint8_t)(*reg | shifted_mode);

    return STATUS_OK;
}


StatusError status_register_get_mode(
    const uint8_t *reg,
    uint8_t *mode)
{
    if (reg == NULL || mode == NULL)
    {
        return STATUS_ERROR_NULL;
    }

    uint8_t mode_field =
        (uint8_t)(*reg & STATUS_MODE_FIELD_MASK);

    *mode =
        (uint8_t)(mode_field >> STATUS_MODE_SHIFT);

    return STATUS_OK;
}


const char *status_error_to_string(
    StatusError error)
{
    switch (error)
    {
        case STATUS_OK:
            return "STATUS_OK";

        case STATUS_ERROR_NULL:
            return "STATUS_ERROR_NULL";

        case STATUS_ERROR_INVALID_BIT:
            return "STATUS_ERROR_INVALID_BIT";

        case STATUS_ERROR_INVALID_MODE:
            return "STATUS_ERROR_INVALID_MODE";

        default:
            return "UNKNOWN_STATUS_ERROR";
    }
}

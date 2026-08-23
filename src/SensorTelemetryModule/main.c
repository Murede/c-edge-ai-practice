/*
 * Sensor Telemetry and Status Register Test Program
 * -------------------------------------------------
 * This program validates the operation and integration of the
 * status-register and sensor-packet modules.
 *
 * Testing is divided into four phases:
 *
 * Phase 1 - Exercise normal status-register operations.
 * Phase 2 - Encode register status and ADC data into a sensor packet,
 *           then decode the packet to verify the original data.
 * Phase 3 - Simulate a device fault and verify that the fault state
 *           survives packet encoding and decoding.
 * Phase 4 - Test invalid inputs and boundary values to verify that
 *           both modules handle errors safely.
 */

#include <stdio.h>

#include "status_register.h"
#include "sensor_packet.h"

int main(void)
{
    // =========================================================
    // Phase 1: Normal Status Register Behaviour
    // =========================================================
    //
    // Build a normal device state by manipulating individual
    // flags and the multi-bit MODE field of the 8-bit register.

    // Start with all register bits cleared.
    uint8_t reg = 0u;

    // Used to store the result of flag-check operations.
    uint8_t is_set = 0u;

    // Stores MODE values written to and read from bits 5-4.
    uint8_t mode = 0u;

    // Stores error codes returned by status-register operations.
    StatusError status_result;

    // Set the ENABLE flag at bit 0.
    status_result =
        status_register_set_flag(
            &reg,
            STATUS_ENABLE_BIT);

    printf(
        "Set ENABLE: %s\n",
        status_error_to_string(status_result));

    // Set the READY flag at bit 3.
    status_result =
        status_register_set_flag(
            &reg,
            STATUS_READY_BIT);

    printf(
        "Set READY: %s\n",
        status_error_to_string(status_result));

    // Verify that the ENABLE flag is currently set.
    // is_set receives 1 if the bit is set and 0 otherwise.
    status_result =
        status_register_is_flag_set(
            &reg,
            STATUS_ENABLE_BIT,
            &is_set);
    printf(
        "ENABLE check: result=%s, value=%u\n",
        status_error_to_string(status_result),
        (unsigned int)is_set);

    // Verify that the READY flag is currently set.
    status_result =
        status_register_is_flag_set(
            &reg,
            STATUS_READY_BIT,
            &is_set);
    printf(
        "READY check: result=%s, value=%u\n",
        status_error_to_string(status_result),
        (unsigned int)is_set);

    // Set MODE to binary 10 (decimal 2).
    // status_register_set_mode() places this 2-bit value
    // into bits 5-4 of the register.
    mode = 2u;

    status_result =
        status_register_set_mode(
            &reg,
            mode);

    printf(
        "MODE readback: result=%s, value=%u\n",
        status_error_to_string(status_result),
        (unsigned int)mode);

    // Read the MODE field back from bits 5-4.
    // The function shifts the field back down and stores
    // the resulting value in mode.
    status_result =
        status_register_get_mode(
            &reg,
            &mode);

    // Toggle the DIRECTION flag at bit 1.
    // XOR changes the bit from 0 -> 1 or 1 -> 0.
    status_result =
        status_register_toggle_flag(
            &reg,
            STATUS_DIRECTION_BIT);

    printf(
        "Toggle DIRECTION: %s\n",
        status_error_to_string(status_result));

    // Clear the READY flag while preserving the other fields.
    status_result =
        status_register_clear_flag(
            &reg,
            STATUS_READY_BIT);
    printf(
        "Clear READY: %s\n",
        status_error_to_string(status_result));

    /*
     * Expected final Phase 1 register:
     *
     * Bit:      7  6 | 5  4 | 3 | 2 | 1 | 0
     *           RSV  | MODE |RDY|FLT|DIR|ENA
     *            0  0 | 1  0 | 0 | 0 | 1 | 1
     *
     * Binary: 0010 0011
     * Hex:    0x23
     */
    printf(
        "Final Phase 1 register: 0x%02X\n",
        (unsigned int)reg);

    // =========================================================
    // Phase 2: Sensor Packet Integration
    // =========================================================
    //
    // Take the lower four status-register flags and combine them
    // with a 12-bit ADC reading to create a 16-bit telemetry packet.
    //
    // Packet layout:
    //
    // Bits 15-12 : Status
    // Bits 11-0  : ADC value

    // Simulated ADC reading within the valid 12-bit range.
    uint16_t adc_value = 2016u;

    // Receives the completed 16-bit encoded packet.
    uint16_t packet = 0u;

    // Stores the four status bits that will be transmitted.
    uint8_t packet_status;

    // Holds the fields recovered when the packet is decoded.
    SensorData decoded;

    // Stores errors returned by sensor-packet operations.
    SensorError sensor_result;

    // Extract bits 3-0 from the status register.
    //
    // These bits represent:
    // READY | FAULT | DIRECTION | ENABLE
    //
    // MODE and reserved bits are intentionally not transmitted.
    packet_status =
        (uint8_t)(reg & 0x0Fu);

    // Encode the 4-bit status value and 12-bit ADC value
    // into one 16-bit packet.
    sensor_result =
        sensor_packet_encode(
            packet_status,
            adc_value,
            &packet);

    // Do not use the packet if encoding failed.
    if (sensor_result != SENSOR_OK)
    {
        printf(
            "Packet encoding failed: %s\n",
            sensor_error_to_string(sensor_result));

        return 1;
    }

    // Display the complete packet as four hexadecimal digits.
    // Four hex digits correspond directly to the packet's 16 bits.
    printf(
        "Encoded Packet: 0x%04X\n",
        (unsigned int)packet);

    // Split the encoded packet back into its original
    // ADC and status fields.
    decoded =
        sensor_packet_decode(packet);

    // Display the recovered values to verify the round trip.
    sensor_data_print(&decoded);

    // =========================================================
    // Phase 3: Fault / Status Behaviour
    // =========================================================
    //
    // Simulate a device fault and verify that the FAULT flag
    // survives the complete register -> packet -> decoder path.

    // Set the FAULT flag at bit 2 of the status register.
    status_result =
        status_register_set_flag(
            &reg,
            STATUS_FAULT_BIT);

    // Read the FAULT flag back to verify that it was set.
    status_result =
        status_register_is_flag_set(
            &reg,
            STATUS_FAULT_BIT,
            &is_set);

    printf(
        "Fault flag set: %u\n",
        (unsigned int)is_set);

    // Re-extract the lower status nibble now that the
    // FAULT bit has changed.
    packet_status =
        (uint8_t)(reg & 0x0Fu);

    // Encode another packet containing the faulted status.
    sensor_result =
        sensor_packet_encode(
            packet_status,
            adc_value,
            &packet);

    // Stop the test if the fault packet cannot be encoded.
    if (sensor_result != SENSOR_OK)
    {
        printf(
            "Fault packet encoding failed: %s\n",
            sensor_error_to_string(sensor_result));

        return 1;
    }

    // Decode the fault packet back into its individual fields.
    decoded =
        sensor_packet_decode(packet);

    sensor_data_print(&decoded);

    // Test the decoded status using the FAULT mask.
    // AND is used without assignment because the status value
    // should only be inspected, not modified.
    if (decoded.status & STATUS_FAULT_MASK)
    {
        printf(
            "Decoded packet contains FAULT flag\n");
    }

    // =========================================================
    // Phase 4: Invalid Input / Boundary Condition Testing
    // =========================================================
    //
    // Deliberately provide invalid inputs to verify that each
    // module returns the correct error instead of performing an
    // unsafe or invalid operation.

    // ---------------------------------------------------------
    // Status Register Error Tests
    // ---------------------------------------------------------

    // A function that modifies the register cannot operate
    // without a valid register address.
    status_result =
        status_register_set_flag(
            NULL,
            STATUS_ENABLE_BIT);

    printf(
        "NULL register: %s\n",
        status_error_to_string(status_result));

    // Only bits 0-3 are valid individual flags.
    // Bit 7 is reserved and should therefore be rejected.
    status_result =
        status_register_set_flag(
            &reg,
            7);

    printf(
        "Invalid bit test: %s\n",
        status_error_to_string(status_result));

    // MODE is a 2-bit field and therefore only accepts
    // values 0-3. A value of 4 must be rejected.
    status_result =
        status_register_set_mode(
            &reg,
            4u);

    printf(
        "Invalid Mode Test: %s\n",
        status_error_to_string(status_result));

    // get_mode() requires an output address where the
    // extracted MODE value can be stored.
    status_result =
        status_register_get_mode(
            &reg,
            NULL);

    printf(
        "NULL mode output test: %s\n",
        status_error_to_string(status_result));

    // Verify that the highest legal MODE value is accepted.
    status_result =
        status_register_set_mode(
            &reg,
            3u);

    printf(
        "Maximum valid mode test: %s\n",
        status_error_to_string(status_result));

    // ---------------------------------------------------------
    // Sensor Packet Error Tests
    // ---------------------------------------------------------

    // sensor_packet_encode() requires a valid output pointer
    // where the completed 16-bit packet can be stored.
    sensor_result =
        sensor_packet_encode(
            1u,
            1000u,
            NULL);

    printf(
        "NULL output pointer test: %s\n",
        sensor_error_to_string(sensor_result));

    // A 12-bit ADC field has a maximum value of 4095.
    // Test the first value outside the legal range.
    sensor_result =
        sensor_packet_encode(
            1u,
            SENSOR_ADC_MAX + 1u,
            &packet);

    printf(
        "Invalid ADC Test: %s\n",
        sensor_error_to_string(sensor_result));

    // A 4-bit status field has a maximum value of 15.
    // Test the first value outside the legal range.
    sensor_result =
        sensor_packet_encode(
            SENSOR_STATUS_MAX + 1u,
            1000u,
            &packet);

    printf(
        "Invalid Status Test: %s\n",
        sensor_error_to_string(sensor_result));

    // ---------------------------------------------------------
    // Maximum Boundary Test
    // ---------------------------------------------------------
    //
    // Maximum status = 0xF
    // Maximum ADC    = 0xFFF
    //
    // Combining them should produce:
    //
    // 1111 1111 1111 1111 = 0xFFFF

    sensor_result =
        sensor_packet_encode(
            SENSOR_STATUS_MAX,
            SENSOR_ADC_MAX,
            &packet);

    printf(
        "Maximum valid packet test: %s\n",
        sensor_error_to_string(sensor_result));

    printf(
        "Maximum valid packet: 0x%04X\n",
        (unsigned int)packet);

    // ---------------------------------------------------------
    // Minimum Boundary Test
    // ---------------------------------------------------------
    //
    // A status of 0 and ADC value of 0 are both valid.
    // The resulting packet should therefore be 0x0000.

    sensor_result =
        sensor_packet_encode(
            0u,
            0u,
            &packet);

    printf(
        "Minimum valid packet test: %s\n",
        sensor_error_to_string(sensor_result));

    printf(
        "Minimum valid packet: 0x%04X\n",
        (unsigned int)packet);

    return 0;
}
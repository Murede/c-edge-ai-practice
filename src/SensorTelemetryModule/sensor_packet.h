#ifndef SENSOR_PACKET_H
#define SENSOR_PACKET_H

#include <stdint.h>

/*
 * Packet layout: 
 *
 * Bits 15-12 : Status 
 * Bits 11-0 : ADC Value
 */

// Public Constans 
#define SENSOR_ADC_MAX 0x0FFFu
#define SENSOR_STATUS_MAX 0x0Fu

#define SENSOR_ADC_MASK 0x0FFFu
#define SENSOR_STATUS_MASK 0x0Fu

#define SENSOR_STATUS_SHIFT 12u

// Public error type 
 
typedef enum{
    SENSOR_OK =0,
    SENSOR_ERROR_NULL,
    SENSOR_ERROR_INVALID_ADC,
    SENSOR_ERROR_INVALID_STATUS
}  SensorError; 

// Public decoded-data type 

typedef struct 
{
    uint16_t adc_value; 
    uint8_t status;
} SensorData;

// Public API
const char *sensor_error_to_string(SensorError error);

SensorError sensor_packet_encode(
    uint8_t status,
    uint16_t adc_value,
    uint16_t *packet
);

SensorData sensor_packet_decode(
    uint16_t sensor_packet
);

#endif /* SENSOR_PACKET_H */

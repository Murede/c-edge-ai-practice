#include "sensor_packet.h"

const char *sensor_error_to_string(SensorError error)
{
    switch (error)
    {
        case SENSOR_OK:
            return "SENSOR_OK";

        case SENSOR_ERROR_NULL:
            return "SENSOR_ERROR_NULL";

        case SENSOR_ERROR_INVALID_ADC:
            return "SENSOR_ERROR_INVALID_ADC";

        case SENSOR_ERROR_INVALID_STATUS:
            return "SENSOR_ERROR_INVALID_STATUS";

        default:
            return "UNKNOWN_ERROR";
    }
}

SensorError sensor_packet_encode(
    uint8_t status,
    uint16_t adc_value,
    uint16_t *packet
)
{
      if (packet == NULL)
    {
        return SENSOR_ERROR_NULL; 
    }

    if ( adc_value > SENSOR_ADC_MAX){
        return SENSOR_ERROR_INVALID_ADC;
    }

    if (status > SENSOR_STATUS_MAX){
        return SENSOR_ERROR_INVALID_STATUS;
    } 

    
    // Step 1: mask + shift applied to adc_value to allow 
    uint16_t masked_adc = adc_value & SENSOR_ADC_MASK;

    //Step 2: Shift status bits up to 15-12 
    uint16_t shifted_status = ((uint16_t)(status & SENSOR_STATUS_MASK)) 
                      << SENSOR_STATUS_SHIFT; 

    *packet =  shifted_status | masked_adc;

    return SENSOR_OK; 
}

SensorData sensor_packet_decode(
    uint16_t sensor_packet
)
{
    SensorData sensor = {
        .adc_value = sensor_packet & SENSOR_ADC_MASK,
        .status = (uint8_t)((sensor_packet << SENSOR_STATUS_SHIFT>>) 
                   & SENSOR_STATUS_MASK)
    };

    return sensor;
}

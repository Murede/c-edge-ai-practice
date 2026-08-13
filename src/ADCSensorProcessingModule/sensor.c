#include "sensor.h"

int sensor_min(const int *samples, size_t length)
{
    if ((samples == NULL) || (length == 0U))
    {
        return 0;
    }

    // Initially set the minimum value to the first sample
    int min_value = samples[0];

    for (size_t i = 1U; i < length; i++)
    {
        if (samples[i] < min_value)
        {
            min_value = samples[i];
        }
    }
    return min_value;
}

int sensor_max(const int *samples, size_t length)
{
    if ((samples == NULL) || (length == 0U))
    {
        return 0;
    }

    // Initally set the maximum value to the first sample
    int max_value = samples[0];

    for (size_t i = 1U; i < length; i++)
    {
        if (samples[i] > max_value)
        {
            max_value = samples[i];
        }
    }
    return max_value;
}

double sensor_average(const int *samples, size_t length)
{
    if ((samples == NULL) || (length == 0U))
    {
        return 0.0;
    }

    // Initialize the sum to 0
    int sum = 0;

    for (size_t i = 0U; i < length; i++)
    {
        sum += samples[i];
    }

    return sum / (double)length;
}

double adc_to_voltage(double adc_value, double reference_voltage)
{

    // Convert ADC value to voltage using the formula: voltage = (adc_value / max_adc_value) * reference_voltage

    return (adc_value / 4095.0) * reference_voltage;
}

int voltage_in_range(double voltage,
                     double min_voltage,
                     double max_voltage)
{
    // Check if the voltage is within the specified range
    return (voltage >= min_voltage) && (voltage <= max_voltage);
}

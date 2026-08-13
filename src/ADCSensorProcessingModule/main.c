#include <stdio.h>
#include "sensor.h"

int main(void)
{
    const double REFERENCE_VOLTAGE = 3.3;
    const double MIN_SAFE_VOLTAGE = 1.5;
    const double MAX_SAFE_VOLTAGE = 1.8;

    const int samples[] = {2048, 1995, 2102, 2010, 2055, 2075};
    const size_t length = sizeof(samples) / sizeof(samples[0]);
    size_t out_of_range_count = 0U;

    // Calculate the minimal, maximum, average values of the ADC samples using the sensor functions
    int min_adc = sensor_min(samples, length);
    int max_adc = sensor_max(samples, length);
    double average_adc = sensor_average(samples, length);

    // Convert each of the ADC values to voltage using the adc_to_voltage function
    for (size_t i = 0U; i < length; i++)
    {
        const double voltage = adc_to_voltage(samples[i], REFERENCE_VOLTAGE);

        // Check if the voltage is within the safe range using the voltage_in_range function
        if (!voltage_in_range(voltage, MIN_SAFE_VOLTAGE, MAX_SAFE_VOLTAGE))
        {
            out_of_range_count++;
            printf("Sample %zu: ADC Value = %d. Voltage = %.2f V [OUT OF RANGE]\n",
                   i + 1U,
                   samples[i],
                   voltage);
        }
        else
        {
            printf("Sample %zu: ADC Value = %d. Voltage = %.2f V\n",
                   i + 1U,
                   samples[i],
                   voltage);
        }
    }

    printf("Minimum ADC Value: %d\n", min_adc);
    printf("Maximum ADC Value: %d\n", max_adc);
    printf("Average ADC Value: %.2f\n", average_adc);

    printf("Number of samples out of range: %zu\n", out_of_range_count);

    if (out_of_range_count == 0U)
    {
        printf("System Status: SAFE\n");
    }
    else
    {
        printf("System Status: FAULT\n");
    }

    return 0;
}

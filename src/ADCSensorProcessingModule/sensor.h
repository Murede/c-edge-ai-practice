#ifndef SENSOR_H
#define SENSOR_H

#include <stddef.h>

int sensor_min(const int *samples, size_t length);
int sensor_max(const int *samples, size_t length);
double sensor_average(const int *samples, size_t length);
double adc_to_voltage(double adc_value, double reference_voltage);
int voltage_in_range(double voltage,
                     double min_voltage,
                     double max_voltage);

#endif /* SENSOR_H */

# ADC Sensor Processing Module

## Overview

This modular C program processes raw 12-bit ADC samples and reports whether every converted voltage is within a configured safe range.

The architecture remains deliberately small:

```text
main.c   -> application flow and reporting
sensor.c -> sensor-processing implementations
sensor.h -> public sensor module interface
```

## Features

- Raw 12-bit ADC sample processing
- Minimum ADC calculation
- Maximum ADC calculation
- Average ADC calculation
- Per-sample ADC-to-voltage conversion
- Per-sample safe-range checking
- Out-of-range sample counting
- Final `SAFE` or `FAULT` system status
- `const` read-only sensor data
- `size_t` array lengths and loop indices
- Basic null-pointer and zero-length protection
- Modular `sensor.h`, `sensor.c`, and `main.c` structure

## Program flow

```text
Raw ADC Samples
       |
       v
Minimum / Maximum / Average
       |
       v
Convert Each ADC Sample to Voltage
       |
       v
Check Every Voltage Against Safe Limits
       |
       v
Count Out-of-Range Samples
       |
       v
SAFE / FAULT Report
```

## ADC model

The project models a 12-bit ADC with codes from `0` through `4095`. Code `4095` corresponds to the configured reference voltage:

```c
voltage = (adc_value / 4095.0) * reference_voltage;
```

## Public interface

```c
int sensor_min(const int *samples, size_t length);
int sensor_max(const int *samples, size_t length);
double sensor_average(const int *samples, size_t length);
double adc_to_voltage(double adc_value, double reference_voltage);
int voltage_in_range(double voltage,
                     double min_voltage,
                     double max_voltage);
```

The three array-processing functions return `0` or `0.0` when `samples` is null or `length` is zero. This is a simple temporary safeguard; a future error-code design will distinguish invalid input from a valid zero measurement.

## Build and run

Compile with strict warnings:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror main.c sensor.c -o adc_test
```

Run on Windows PowerShell:

```powershell
.\adc_test.exe
```

## Concepts practised

- Functions and explicit interfaces
- Arrays and array-to-pointer decay
- Explicit buffer lengths
- `size_t` for object counts
- `const` API contracts
- Header guards
- Separate compilation and linking
- Physical-unit correctness
- Per-sample fault detection
- Defensive boundary handling


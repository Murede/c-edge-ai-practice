# C and Edge AI Practice

[![C module tests](https://github.com/Murede/c-edge-ai-practice/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/Murede/c-edge-ai-practice/actions/workflows/build-and-test.yml)

A learn-as-I-go engineering repository documenting my progression from foundational C programming toward reliable embedded software and Edge AI integration.

Rather than presenting isolated syntax exercises, this project applies each new concept to small host-side modules inspired by embedded systems: sensor processing, motor-control state machines, telemetry packets, register access, and event callbacks. Each module is kept small enough to understand, compile with strict warnings, and improve as my skills develop.

## Why this repository exists

My goal is to build the C foundations required for firmware and intelligent edge devices before moving the same patterns onto an ESP32 robot platform. The repository intentionally preserves the learning progression, including design decisions, test cases, limitations, and follow-up work.

The current work focuses on:

- Safe C interfaces and explicit error handling
- Header/source separation and module encapsulation
- Array bounds, pointers, `const`, and fixed-width integer types
- Finite-state-machine design for motor control
- Bit masks and memory-mapped register abstractions
- Function pointers and callback-based event dispatch
- Host-side testing before hardware integration
- Strict compiler warnings and small, reviewable Git commits

## Implemented practice modules

| Module | Concepts demonstrated | Status |
|---|---|---|
| [ADC Sensor Processing](src/ADCSensorProcessingModule/) | Array processing, ADC conversion, range checking, defensive input handling | Implemented and host-tested |
| [Motor Controller State Machine](src/MotorControllerStateMachine/) | Structs, enums, explicit transitions, safe fault states, error codes | Implemented and host-tested |
| [Sensor Telemetry Module](src/SensorTelemetryModule/) | Packet structures, status fields, validation, modular interfaces | Implemented and host-tested |
| [Memory-Mapped Peripheral Driver](src/MemoryMappedPeripheralDriver/) | Fixed-width registers, masks, read/modify/write operations | Implemented as a host-side model |
| [Sensor Event Dispatcher](src/SensorEventDispatcher/) | Function pointers, callback registration, event dispatch | Implemented and host-tested |

The modules are simulations and learning exercises, not production firmware or measured hardware implementations. Hardware timing, real peripheral behavior, and ESP32 integration remain future validation work.

## Learning roadmap

The broader [eight-week roadmap](ROADMAP.md) progresses through:

1. Functions, arrays, modules, and compiler warnings
2. Structs, enums, state machines, and error codes
3. Pointers, memory lifetime, and buffer boundaries
4. Bits, registers, masks, and `volatile`
5. Circular buffers, packets, and checksums
6. Callbacks, timers, interrupts, and concurrency
7. Testing, debugging, sanitizers, and refactoring
8. ESP32 robot integration

`PROGRESS.md` and the dated files under `daily-logs/` record completed work separately from planned work.

## Repository layout

```text
c-edge-ai-practice/
|-- src/          Independent C practice modules
|-- tests/        Focused experiments and regression tests
|-- include/      Shared public headers as the project grows
|-- daily-logs/   Dated learning and verification records
|-- notes/        Concept, debugger, datasheet, and Edge AI notes
|-- ROADMAP.md    Eight-week learning progression
`-- PROGRESS.md   Verified status and next actions
```

## Build and run

The modules are currently built independently with GCC. GitHub Actions compiles and runs all five modules on every push and pull request. To build selected modules locally from the repository root:

```bash
mkdir -p build

gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror \
  src/ADCSensorProcessingModule/main.c \
  src/ADCSensorProcessingModule/sensor.c \
  -o build/adc_sensor_processing

gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror \
  src/MotorControllerStateMachine/main.c \
  src/MotorControllerStateMachine/motor.c \
  -o build/motor_controller
```

Run the resulting programs:

```bash
./build/adc_sensor_processing
./build/motor_controller
```

On PowerShell, use `build\adc_sensor_processing.exe` and `build\motor_controller.exe`.

The project deliberately uses strict warning flags. A build is not considered clean until warnings are understood and resolved rather than suppressed.

## Current status

This is an active learn-as-I-go project. Five host-side modules currently compile with strict warnings and run successfully. The next major phase is to expand the regression-test structure and introduce additional buffer, packet, and timing exercises before transferring selected modules to ESP32 hardware.

## Engineering principles

- Begin from the last passing commit.
- Keep hardware-independent logic testable on a computer.
- Record measured or observed results instead of assuming success.
- Treat compiler warnings as engineering feedback.
- Use explicit units, bounds, states, and error codes.
- Default motors and actuators to safe states.
- Separate verified capabilities from planned extensions.

## Author

Murede Adetiba

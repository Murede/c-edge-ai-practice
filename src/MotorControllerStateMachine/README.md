# Motor Controller State Machine

A host-side C project that models a simplified embedded motor controller using structs, enums, explicit state transitions, and error codes.

## Learning Goals

This project was built to practice:

- C structs for grouping controller state
- Enums for states, events, and error codes
- Finite-state-machine design
- Pointers and output parameters
- `static` helper functions and module encapsulation
- `const` for read-only access
- Defensive input validation
- Explicit error handling instead of terminating the program
- Separation between a public API and private implementation
- Strict compiler warnings

## Project Structure

```text
MotorControllerStateMachine/
├── motor.h
├── motor.c
├── main.c
└── README.md
```

## Architecture

`main.c` only uses the public API declared in `motor.h`.

```text
main.c
   |
   v
motor.h
   |
   v
motor.c
   |
   +-- determine_next_state()   [private]
   +-- apply_state_actions()    [private]
```

The private helper functions are declared `static`, keeping the internal state-machine implementation hidden from the rest of the program.

## Motor States

| State | Meaning |
| --- | --- |
| `MOTOR_IDLE` | Motor is stopped and ready |
| `MOTOR_STARTING` | Start command accepted and startup is in progress |
| `MOTOR_RUNNING` | Motor is running at its target speed |
| `MOTOR_STOPPING` | Stop command accepted and stopping is in progress |
| `MOTOR_FAULT` | Motor is faulted and operational commands are restricted |

## State Transitions

| Current State | Event | Next State | Result |
| --- | --- | --- | --- |
| `MOTOR_IDLE` | `EVENT_START` | `MOTOR_STARTING` | `MOTOR_OK` |
| `MOTOR_IDLE` | `EVENT_FAULT` | `MOTOR_FAULT` | `MOTOR_OK` |
| `MOTOR_STARTING` | `EVENT_START_COMPLETE` | `MOTOR_RUNNING` | `MOTOR_OK` |
| `MOTOR_STARTING` | `EVENT_STOP` | `MOTOR_STOPPING` | `MOTOR_OK` |
| `MOTOR_STARTING` | `EVENT_FAULT` | `MOTOR_FAULT` | `MOTOR_OK` |
| `MOTOR_RUNNING` | `EVENT_STOP` | `MOTOR_STOPPING` | `MOTOR_OK` |
| `MOTOR_RUNNING` | `EVENT_FAULT` | `MOTOR_FAULT` | `MOTOR_OK` |
| `MOTOR_STOPPING` | `EVENT_STOP_COMPLETE` | `MOTOR_IDLE` | `MOTOR_OK` |
| `MOTOR_STOPPING` | `EVENT_FAULT` | `MOTOR_FAULT` | `MOTOR_OK` |
| `MOTOR_FAULT` | `EVENT_RESET` | `MOTOR_IDLE` | `MOTOR_OK` |
| Unsupported combination | — | No change | `MOTOR_ERROR_INVALID_TRANSITION` |

`MOTOR_OK` describes whether an event was successfully processed. It does not mean that the physical motor is fault-free.

For example, successfully processing `EVENT_FAULT` moves the controller into `MOTOR_FAULT` while returning `MOTOR_OK`.

## State-Specific Actions

- `MOTOR_IDLE` sets current speed to `0`.
- `MOTOR_STARTING` leaves speed unchanged.
- `MOTOR_RUNNING` sets current speed to the configured target speed.
- `MOTOR_STOPPING` leaves speed unchanged until stopping completes.
- `MOTOR_FAULT` sets current speed to `0`.

Gradual acceleration and deceleration could later be implemented using a periodic update function.

## Public API

```c
MotorError motor_init(
    MotorController *controller,
    int target_speed
);

MotorError motor_handle_event(
    MotorController *controller,
    MotorEvent event
);

MotorError motor_set_target_speed(
    MotorController *controller,
    int target_speed
);

const char *motor_state_to_string(
    MotorState state
);

const char *motor_error_to_string(
    MotorError error
);
```

## Error Handling

The module validates inputs before modifying the controller.

Handled errors include:

- Null controller pointers
- Negative target speeds
- Target speeds above `MOTOR_MAX_SPEED`
- Unsupported state transitions
- Target-speed changes while faulted
- Invalid state values

Invalid transitions do not commit a new state or execute state-specific actions.

## Build

Compile using strict warnings:

```powershell
gcc main.c motor.c -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -o motor_test
```

Run:

```powershell
./motor_test
```

## Test Plan

`main.c` tests four major areas.

### Test 1 — Initialization

Verifies the controller initializes with:

```text
state         = MOTOR_IDLE
current_speed = 0
target_speed  = requested target
temperature   = 25.0 C
```

### Test 2 — Normal Operation

Tests the normal state sequence:

```text
IDLE
 ↓
STARTING
 ↓
RUNNING
 ↓
STOPPING
 ↓
IDLE
```

### Test 3 — Fault Validation

Tests:

```text
RUNNING
 ↓ EVENT_FAULT
FAULT
```

Then verifies that:

- Unsafe state transitions are rejected.
- Target-speed changes are rejected while faulted.
- The motor remains stopped.
- `EVENT_RESET` provides the valid recovery path.

### Test 4 — Invalid Inputs

Tests:

- Null controller pointers
- Negative speeds
- Speeds above `MOTOR_MAX_SPEED`
- Invalid state/event combinations

## Key Design Decisions

### Temporary Next State

`determine_next_state()` writes the proposed state through a `MotorState *` output parameter while returning a `MotorError`.

This lets `motor_handle_event()` validate the transition before committing the new state.

### Early Returns

Validation failures return immediately.

This prevents invalid inputs from partially modifying the motor controller.

### Public vs. Private Functions

Application code does not directly call the state-transition or state-action helpers.

Instead, it communicates through the public API while the implementation details remain inside `motor.c`.

## Possible Future Extensions

- Gradual acceleration and deceleration
- Temperature limits
- Over-temperature faults
- Encoder feedback
- Periodic `motor_update()` function
- Event logging and telemetry
- Automated unit tests
- ESP32 motor and encoder integration
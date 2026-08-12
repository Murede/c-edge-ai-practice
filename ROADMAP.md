# Eight-Week Roadmap

The cycle contains 56 incremental days. If it repeats, the themes remain but the tasks increase in depth and build on verified prior modules.

## Weekly rhythm

| Day | Primary activity | Supporting practice |
|---|---|---|
| Monday | Learn one concept and run a tiny experiment | Embedded fundamentals |
| Tuesday | Implement one module | Interfaces, build tools, documentation |
| Wednesday | Debug and test edge cases | Linux or Python/ML integration |
| Thursday | Integrate with the simulator or ESP32 project | Peripherals, registers, timing |
| Friday | Test, remove warnings, refactor, document | Clean interfaces and tooling |
| Saturday | 90-minute mini-project or milestone | Edge AI integration |
| Sunday | 30-minute review and planning | Flashcards and Git cleanup |

## Week 1 — Functions, arrays, modules, warnings

Build the foundation of a host-side motor-control simulator. Practice function contracts, fixed-size arrays, `.c`/`.h` separation, boundary handling, and strict compiler warnings.

Deliverable: a small, warning-free motor sample module with tests and documented assumptions.

## Week 2 — Structs, enums, state machines, error codes

Represent motor state explicitly and implement a safe command/state machine. Replace ambiguous return values with defined error codes.

Deliverable: a simulator that transitions predictably among stopped, running, fault, and recovery states.

## Week 3 — Pointers, const, memory, buffer boundaries

Pass data safely through module interfaces, distinguish stack and static lifetime, apply `const`, and validate every buffer length.

Deliverable: pointer-safe APIs for motor samples, commands, and telemetry.

## Week 4 — Bits, registers, masks, volatile

Model peripheral registers, set and clear fields with masks, and understand where `volatile` is necessary—and where it is insufficient.

Deliverable: a host-tested register abstraction ready to map onto ESP32 peripheral concepts.

## Week 5 — Circular buffers, packets, checksums

Buffer asynchronous data, parse UART-style frames, detect malformed input, and validate payload integrity.

Deliverable: bounded telemetry and command queues with a documented packet format.

## Week 6 — Callbacks, timers, interrupts, concurrency

Separate time-critical work from deferred processing, design minimal interrupt responsibilities, and reason about shared state.

Deliverable: a deterministic simulated control loop with timer events and safe data handoff.

## Week 7 — Tests, debugging, sanitizers, refactoring

Expand automated coverage, reproduce faults, use compiler sanitizers where supported, and improve interfaces without changing behavior.

Deliverable: a regression-tested simulator with known limitations and repeatable build/test instructions.

## Week 8 — ESP32 robot integration

Transfer validated logic to the ESP32 robot: motors, encoders, telemetry, fault states, timers, and command expiry.

Deliverable: a safely integrated robot-control milestone with host tests, hardware validation notes, and measured results.

## Skills practised throughout

- Git: small commits, meaningful messages, diffs, branches when useful.
- Toolchain: warnings, debugger, build automation, sanitizers.
- Engineering: requirements, interfaces, units, failure behavior, tests.
- Edge AI: understand how deterministic firmware receives, validates, and acts on model outputs.
- Communication: concise daily logs and defensible technical explanations.


# Week 1 — Functions, Arrays, Modules, and Warnings

Dates: 2026-08-10 to 2026-08-16  
Program start: Wednesday, 2026-08-12  
Weekly deliverable: warning-free, host-tested code for storing and analysing bounded motor-speed samples.

The first two calendar days are context only. The active program begins on Day 3.

## Monday — Concept experiment (context)

- Review function declarations, definitions, parameters, return values, and scope.
- Write tiny pure functions such as `clamp_pwm()` and `rpm_error()`.
- Note how pure logic can be tested on a computer before ESP32 integration.

## Tuesday — Module implementation (context)

- Separate a small motor-simulation API into a header and source file.
- Use include guards and expose only the intended interface.
- Compile with warnings enabled.

## Wednesday — Array boundary debugging (active Day 1)

Focus: safely process a fixed-size array of motor RPM samples.

Concrete task:

- Create a function that calculates an average from an RPM sample array.
- Pass both the array and its valid element count.
- Define behavior for a null pointer, zero samples, and a count beyond capacity.
- Add tests for one sample, several samples, zero samples, and maximum capacity.
- Intentionally create one off-by-one defect, observe the failing test or sanitizer result, then fix it.

Acceptance criteria:

- No out-of-bounds read or write.
- Invalid inputs return an explicit status instead of producing an unexplained value.
- Tests pass with strict warnings enabled.
- The public function contract is documented in the header.

Secondary skill: run the executable from PowerShell and learn how the process exit code signals test success or failure. Connect this to validating a rolling sensor window before its values are fed to an Edge AI feature pipeline.

## Thursday — Simulator integration

- Integrate the sample-analysis function into a minimal motor simulator step.
- Keep sample storage independent from motor-actuation logic.
- Use consistent units in identifiers (`rpm`, `pwm_percent`, `period_ms`).
- Read a relevant ESP32 timer or PWM reference and record one timing assumption in `notes/`.

Definition of done: a simulated step consumes validated samples and returns a deterministic result without hardware dependencies.

## Friday — Tests, warnings, and refactoring

- Add boundary and invalid-input regression tests.
- Remove warnings without suppressing them blindly.
- Review public names and move private helpers into the implementation file.
- Document build and test commands.

Definition of done: a clean rebuild produces zero warnings and all tests pass.

## Saturday — 90-minute mini-project

Build a small command-line motor telemetry analyser that accepts or generates RPM samples, reports basic statistics, and rejects invalid data.

- Keep input, calculation, and output responsibilities separate.
- Optionally use a short Python script to generate predictable sample data.
- Explain how the same C module could validate features before an embedded classifier consumes them.

Definition of done: the program demonstrates one normal case and at least two fault cases, with repeatable commands recorded.

## Sunday — 30-minute review

- Review function contracts, array bounds, header/source separation, and warning flags.
- Create five brief flashcards in `notes/week-01-flashcards.md`.
- Ensure the repository is clean or that unfinished work is clearly identified.
- Update `PROGRESS.md` with verified outcomes and choose the first Week 2 step.

Definition of done: the week has a truthful status, passing evidence, open issues, and a clear next action.


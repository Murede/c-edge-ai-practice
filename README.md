# C + Edge AI Practice

Status: In progress  
Started: 2026-08-12  
Current cycle: Week 1 of 8

This workspace is the source of truth for the daily C practice program. The work begins as a host-side motor-control simulator and gradually moves toward ESP32 robot integration. C remains the primary language; Git, debugging, testing, Linux, Python, and Edge AI are introduced in small supporting exercises.

## Start here

1. Read [`ROADMAP.md`](ROADMAP.md) for the eight-week progression.
2. Open [`WEEK_01.md`](WEEK_01.md) for the current week's daily breakdown.
3. Do the day's work in `src/`, `include/`, and `tests/`.
4. Copy [`daily-logs/TEMPLATE.md`](daily-logs/TEMPLATE.md) to a dated file and complete it after the session.
5. Send the completed summary to the agent. The verified result will be used to adjust the next task without assuming uncompleted work.

## Directory layout

```text
c-edge-ai-practice/
|-- README.md
|-- ROADMAP.md
|-- WEEK_01.md
|-- PROGRESS.md
|-- include/       Public C headers
|-- src/           C implementations and small programs
|-- tests/         Unit and regression tests
|-- daily-logs/    One dated completion summary per practice day
`-- notes/         Concept, debugger, datasheet, and Edge AI notes
```

## Working rules

- Begin from the last passing commit.
- Compile with strong warnings and treat warnings as work to resolve.
- Keep hardware-independent logic testable on the host.
- Record actual results; never mark planned work as completed.
- Make small commits that describe one working change.
- When hardware begins moving, motors must default to stopped and commands must expire safely.

Suggested host compile command (GCC or Clang):

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror \
  -Iinclude src/*.c tests/*.c -o build/test_runner
```

Adjust the source list as the project grows. A build system will be introduced deliberately during the program.


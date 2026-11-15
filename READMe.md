*This project has been created as part of the 42 curriculum by hmnatsak.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads.

N coders sit in a circle around a shared Quantum Compiler. There are N USB
dongles on the table, one between each pair of neighbours. To compile, a coder
must hold two dongles at once (left and right). After compiling, a coder
debugs, then refactors, then tries to compile again. A coder who does not
start a compile within `time_to_burnout` milliseconds burns out, and the
simulation stops.

The goal is to share the dongles without deadlock, without starving any coder,
and with precise burnout detection, using only mutexes and condition variables.

Overview of the design:

- one thread per coder
- one monitor thread that detects burnout and the end of the simulation
- every dongle has its own mutex, condition variable, cooldown timestamp and a
  small priority queue (a binary heap written from scratch) ordered by the
  chosen scheduler (`fifo` or `edf`)
- one log mutex that serializes the output and protects the shared state of
  the simulation

## Instructions

### Build

```
make          # builds ./codexion
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # full rebuild
```

The code is compiled with `cc -Wall -Wextra -Werror -pthread`.

### Run

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning | Valid values |
|---|---|---|
| `number_of_coders` | coders and dongles | integer >= 1 |
| `time_to_burnout` | ms allowed between two compile starts | integer >= 1 |
| `time_to_compile` | ms spent compiling (holding 2 dongles) | integer >= 1 |
| `time_to_debug` | ms spent debugging | integer >= 1 |
| `time_to_refactor` | ms spent refactoring | integer >= 1 |
| `number_of_compiles_required` | the simulation stops when every coder compiled this many times | integer >= 1 |
| `dongle_cooldown` | ms a released dongle stays unavailable | integer >= 0 |
| `scheduler` | arbitration policy of the dongles | `fifo` or `edf` |

Numbers must contain digits only (no sign) and at most 9 digits. Any invalid
input prints `Error: invalid arguments` on the error output and exits with 1.

Example:

```
./codexion 5 800 200 100 100 7 50 edf
```

### Output

Every state change is one line: `timestamp_in_ms coder_id message`.

```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
300 1 is refactoring
801 2 burned out
```

The simulation ends either when a coder burns out (the `burned out` line is
the last line printed) or, silently, when every coder has compiled at least
`number_of_compiles_required` times.

### Source files

| File | Role |
|---|---|
| `src/main.c` | argument check, start and join of all threads |
| `src/parse.c` | argument validation and conversion |
| `src/init.c`, `src/cleanup.c` | creation and destruction of the shared state |
| `src/time.c` | millisecond clock and precise sleep |
| `src/log.c` | serialized logging, plus the helpers that update shared state under the log mutex |
| `src/coder.c` | life cycle of one coder |
| `src/dongle.c` | taking and releasing a dongle, cooldown |
| `src/heap.c` | priority queue used for fifo and edf |
| `src/monitor.c` | burnout detection and end of simulation |
| `src/codexion.h` | structures and prototypes |

## Resources

References:

- `man pthread_create`, `man pthread_join`, `man pthread_mutex_lock`,
  `man pthread_cond_wait`, `man pthread_cond_broadcast`, `man gettimeofday`
- POSIX Threads Programming tutorial (Lawrence Livermore National Laboratory)
- E. G. Coffman, M. J. Elphick, A. Shoshani, "System Deadlocks" (1971): the
  four conditions for deadlock
- C. L. Liu, J. W. Layland, "Scheduling Algorithms for Multiprogramming in a
  Hard-Real-Time Environment" (1973): earliest deadline first scheduling
- T. H. Cormen et al., "Introduction to Algorithms": binary heaps and
  priority queues

### How AI was used

I used an AI assistant as a tutor and reviewer while building the
project step by step.

- **Concepts and design:** it explained the ideas before each step (threads,
  mutexes, condition variables, deadlock, heaps).
- **Debugging:** it helped me read the output of `gdb` and `valgrind`. For
  example, it helped me understand a freeze caused by a duplicated line in the
  coder loop, and a memory leak when initialization fails.
- **Tests:** it suggested the test commands and the expected outputs. I ran
  them myself and compared the results.

## Blocking cases handled

| Case | How it is handled |
|---|---|
| **Deadlock** | Every coder takes the dongle with the lower memory address first, so the last coder in the circle reverses his order. This breaks the circular-wait condition of Coffman's conditions, so a cycle of waiting coders cannot form. A coder who waits for his second dongle holds only a dongle with a lower number than the one he waits for. |
| **Starvation** | Waiting coders are not served in random order. Each dongle keeps a heap of requests: `fifo` serves the oldest request, `edf` serves the coder with the earliest deadline (`last_compile_start + time_to_burnout`). Equal keys are broken by the lower coder id, so the order is fully deterministic. |
| **Dongle cooldown** | A released dongle gets a `free_at` timestamp (release time plus `dongle_cooldown`). The coder who wins the dongle reserves it, then sleeps the remaining cooldown outside the lock, so nobody else is blocked while he waits. |
| **Precise burnout detection** | The monitor thread checks every coder about every millisecond. The stop flag and the `burned out` line are written under the same lock, so no other line can appear after the burnout message. In the runs I made, the message appeared 1 ms after the deadline, well inside the 10 ms limit. |
| **Mixed log lines** | All output goes through `print_log`, which holds the log mutex for the whole line. |
| **Threads asleep at the end** | Coders waiting in a dongle queue sleep on a condition variable. When the simulation stops, the monitor broadcasts on every dongle (while holding its mutex), and the waiting coders leave without taking the dongle. |
| **Single coder** | With one coder there is one dongle. He takes it once, prints `has taken a dongle`, and waits for the monitor to report his burnout. He never waits for himself. |
| **Allocation failure** | If an allocation fails during initialization, everything built so far (mutexes, condition variables, heaps, arrays) is destroyed before the program exits. |
| **End of simulation** | After the stop flag is set, coders do not start a new compile and no more lines are printed. |

## Thread synchronization mechanisms

Only `pthread_mutex_t` and `pthread_cond_t` are used. There is no custom event
implementation.

| Primitive | Protects |
|---|---|
| `log_mutex` (one) | the output, the `stop` flag, each coder's `last_start` and `compiles` counter |
| dongle `mutex` (one per dongle) | `in_use`, `free_at`, the request sequence number and the heap of that dongle |
| dongle `cond` (one per dongle) | puts coders to sleep while the dongle is busy or it is not their turn |

### Examples of races and how they are prevented

**Two coders ask for the same dongle.** The check "is it free and am I at the
top of the queue?" and the change `in_use = 1` happen under the dongle mutex,
so they form one indivisible action. A coder who is not at the top of the heap
waits with `pthread_cond_wait` inside a `while` loop and re-checks the
condition after every wake-up. A release broadcasts, and only the coder at the
top of the heap passes the check.

**The monitor reads `last_start` while a coder writes it.** Both use the log
mutex. The monitor reads the clock, compares and sets the stop flag in one
critical section, so it never sees a half-written value.

**A coder falls asleep just after the stop signal.** The monitor locks each
dongle mutex before broadcasting. A coder is either already asleep (and gets
woken) or not yet inside the wait (and sees the stop flag when he checks the
condition under the same mutex). The wake-up cannot get lost.

**Two lines printed at the same time.** `print_log` takes the log mutex before
`printf` and releases it after, so a line is never cut by another thread.

### Communication between coders and the monitor

The coders never talk to each other. They write their own `last_start` and
`compiles` under the log mutex. The monitor only reads these values (also
under the log mutex) and is the only thread that sets the stop flag. Coders
read the stop flag through `is_stopped`, which locks the same mutex.

### Checks

The program was checked with `valgrind --leak-check=full` (no leaks, no
errors), including a forced allocation failure during initialization.
*This project has been created as part of the 42 curriculum by aalvarad*

# Philosophers

A simulation of the dining philosophers problem, written in C with POSIX
threads. Five philosophers (or one, or two hundred) sit around a table, each
one thinking, eating and sleeping in turn. There are exactly as many forks as
there are philosophers, and eating requires two of them. Nobody may starve,
and nobody may wait forever.

The point of the project is not the philosophers. The point is concurrency:
several threads touching the same memory at the same time, and the discipline
required to make that safe.

---

## The story

Imagine a round table. One bowl of spaghetti per philosopher, and one fork
laid between each pair of neighbours. A philosopher needs both the fork on
their left and the fork on their right before they can eat.

Each philosopher lives the same simple life, over and over: think, pick up two
forks, eat, put the forks down, sleep. That is all.

Three things can go wrong, and the whole project is about preventing them.

**Everyone grabs their left fork at the same instant.** Every fork is now
held, every philosopher is waiting for a second fork that will never come, and
the table freezes forever. This is a **deadlock**. The fix in this project is
asymmetry: philosophers with an even number reach left first, philosophers
with an odd number reach right first. The circle of waiting is broken, so the
table can never lock up.

**One philosopher is always too slow and never gets a turn.** This is
**starvation**, and here it is literal, because a philosopher who does not eat
within `time_to_die` milliseconds of their last meal dies. Giving the odd and
even groups a slight offset at startup keeps meals spread out evenly.

**Two philosophers write to the same variable at the same time.** Nothing
freezes and nothing crashes, but the value becomes nonsense, and the bug shows
up once in a thousand runs. This is a **data race**, and it is the real enemy
of the project, because it is invisible. Every piece of memory shared between
threads in this program (the stop flag, the timestamp of each philosopher's
last meal, the meal counter, and the standard output itself) is protected by
its own mutex. A mutex is a token: only the thread holding it may touch the
resource, and everyone else waits their turn.

One last character sits at the table without eating: the **monitor**. It is
the main thread, and its only job is to walk around the table once per
millisecond, check whether anyone has gone too long without a meal, check
whether everyone has eaten their fill, and call the end of the simulation when
either is true. It never eats, so it never competes for a fork, and it is the
only one allowed to declare a death.

---

## How the code is organised

The program runs on `nb_philo + 1` threads: one per philosopher, plus the main
thread acting as monitor.

| File | What it does |
|---|---|
| `main.c` | Parses, initialises, runs, cleans up. Four calls and nothing else. |
| `parsing.c` | Turns `argv` strings into numbers, rejecting anything that is not a strictly positive integer within range. Nothing starts until the input is proven valid. |
| `time.c` | `get_time_ms` reads the clock in milliseconds. `ft_usleep` sleeps in 500 microsecond slices instead of one long sleep, so a philosopher wakes up promptly when the simulation ends. |
| `init.c` | Allocates the forks and the philosophers, initialises every mutex, and hands each philosopher their two fork addresses. The last philosopher's right fork wraps around to fork zero, which is what closes the circle. |
| `stop.c` | `is_stopped` and `set_stop`, the only two ways to read or write the shared stop flag, both of them behind a mutex. Every thread asks these functions instead of touching the variable. |
| `print.c` | The single exit for every message. It holds the print mutex, checks the simulation is still running, and only then writes one line. This is why two messages can never be interleaved, and why nothing is ever printed after a death. |
| `simulation.c` | Records the start time, launches one thread per philosopher, runs the monitor in the main thread, then joins every thread back. |
| `routine.c` | The life of a philosopher: take forks in parity order, eat, update the last meal timestamp and the meal counter under their own mutex, sleep, think, repeat until the simulation stops. The one philosopher case is handled separately, because both of their forks are the same mutex and locking it twice would deadlock against itself. |
| `monitor.c` | `check_death` and `all_ate_enough`, plus the loop that calls them every millisecond and ends the simulation. |
| `cleanup.c` | Destroys every mutex, then frees the two allocations. In that order, because destroying a mutex inside memory that has already been freed is a use after free. |

Four mutexes in total, each guarding exactly one thing:

- `forks[i]`, one per fork, so no two philosophers hold the same fork
- `meal_lock`, one per philosopher, guarding their `last_meal` and `meals_eaten`
- `print_lock`, guarding standard output
- `stop_lock`, guarding the stop flag

---

## Instructions

### Build

```bash
make
```

Compiles with `-Wall -Wextra -Werror` and links with `-pthread`.
`make clean`, `make fclean` and `make re` behave as expected.

### Run

```bash
./philo nb_philosophers time_to_die time_to_eat time_to_sleep [must_eat]
```

All times are in milliseconds. The optional fifth argument is the number of
meals after which the simulation stops on its own. Without it, the simulation
ends only when a philosopher dies, which means it may run forever, and that is
the expected behaviour.

### Test commands

The commands below assume a Linux environment.

Invalid input, each of these must be refused without crashing:

```bash
./philo
./philo 5
./philo 5 800 200
./philo 0 800 200 200
./philo -5 800 200 200
./philo 5 800 200 200 0
./philo 5 80a 200 200
./philo 2147483648 800 200 200
```

One philosopher, who can never gather two forks and must die around
`time_to_die`:

```bash
./philo 1 800 200 200
```

A death is expected, exactly one `died` line, and it must be the last line
printed:

```bash
./philo 4 310 200 100
./philo 5 300 200 200
./philo 3 200 150 150
```

No death is expected, leave these running as long as you like:

```bash
./philo 5 800 200 200
./philo 4 410 200 200
./philo 2 800 200 200
```

The simulation must end on its own, with no death:

```bash
./philo 5 800 200 200 7
./philo 4 800 200 200 3
./philo 2 800 200 200 1
```

Stress, no death and no freeze:

```bash
./philo 200 800 200 200
```

### Memory and concurrency checks

Memory leaks and invalid accesses:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200 7
```

Expected result: `All heap blocks were freed, no leaks are possible`.

Data races and lock ordering:

```bash
valgrind --tool=helgrind ./philo 5 800 200 200 7
```

Expected result: `ERROR SUMMARY: 0 errors`.

## Resources

- *C pour les nuls*, the reference I kept at hand for C fundamentals
  throughout the project
- [Threads in C, video course](https://www.youtube.com/watch?v=o_GbRujGCnM)
- [Mutexes and synchronisation, video course](https://www.youtube.com/watch?v=mvZKu0DfFLQ)
- `man pthread_create`, `man pthread_mutex_init`, `man gettimeofday`
- Large language models were used throughout this project as a study tool, to
  build a deep understanding of threads, mutexes and data races, and to be
  able to reason about why each lock exists rather than simply placing it
  where it works. Every line of code in this repository was written by hand.

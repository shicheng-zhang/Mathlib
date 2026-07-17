CC = gcc
CFLAGS = -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wfloat-equal -fno-fast-math -ffp-contract=off -Iinclude/mathlib -MMD -MP
LDLIBS = -lm
SRC = src/minimax.c src/exp_log.c src/linalg.c src/core.c src/cordic.c src/trig.c src/fft.c src/payne_hanek.c src/tensor.c src/complex.c src/cpu_dispatch.c

DEPS = $(SRC:.c=.d) tests/*.d

.PHONY: all clean test

all: test_main test_core test_trig test_linalg test_dsp fuzz_god_mode fuzz_boundary

test_main: tests/test.c $(SRC)
	$(CC) $(CFLAGS) -o test_main tests/test.c $(SRC) $(LDLIBS)

test_core: tests/test_core.c $(SRC)
	$(CC) $(CFLAGS) -o test_core tests/test_core.c $(SRC) $(LDLIBS)

test_trig: tests/test_trig.c $(SRC)
	$(CC) $(CFLAGS) -o test_trig tests/test_trig.c $(SRC) $(LDLIBS)

test_linalg: tests/test_linalg.c $(SRC)
	$(CC) $(CFLAGS) -o test_linalg tests/test_linalg.c $(SRC) $(LDLIBS)

test_dsp: tests/test_dsp.c $(SRC)
	$(CC) $(CFLAGS) -o test_dsp tests/test_dsp.c $(SRC) $(LDLIBS)

fuzz_god_mode: tests/fuzz_god_mode.c $(SRC)
	$(CC) $(CFLAGS) -o fuzz_god_mode tests/fuzz_god_mode.c $(SRC) $(LDLIBS)

fuzz_boundary: tests/fuzz_boundary_gauntlet.c $(SRC)
	$(CC) $(CFLAGS) -o fuzz_boundary tests/fuzz_boundary_gauntlet.c $(SRC) $(LDLIBS)

test: all
	@echo "Running all tests..."
	@for t in test_main test_core test_trig test_linalg test_dsp fuzz_god_mode fuzz_boundary; do ./$$t || exit 1; done

clean:
	rm -f test_main test_core test_trig test_linalg test_dsp fuzz_god_mode fuzz_boundary $(DEPS) src/*.o

-include $(DEPS)

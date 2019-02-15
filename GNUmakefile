
VPATH = .:include

# CFLAGS += -DAMREX_BOUND_CHECK

a.out: test.cpp AMReX_Array.H
	g++ -Iinclude $^ -O3 -std=c++14 $(CFLAGS) -o $@

clean:
	$(RM) a.out

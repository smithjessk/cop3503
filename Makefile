CC = clang++
CFLAGS = -std=c++11

ASSIGNMENTS = pa1

.PHONY: assignments $(ASSIGNMENTS)

assignments: $(ASSIGNMENTS)

$(ASSIGNMENTS): %: 
	$(CC) $(CFLAGS) -o $@/bin/$@ $@/src/$@.cpp

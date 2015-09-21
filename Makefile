CC = clang++
CFLAGS = -Wall

ASSIGNMENTS = pa1

.PHONY: assignments $(ASSIGNMENTS)

assignments: $(ASSIGNMENTS)

$(ASSIGNMENTS): %:
	mkdir -p $@/bin/
	$(CC) $(CFLAGS) -o $@/bin/$@ $@/src/$@.cpp

clean:
	for assignment in $(ASSIGNMENTS) ; do \
		rm -rf $$assignment/bin; \
	done
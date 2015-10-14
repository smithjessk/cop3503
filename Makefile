CLANG = clang++
GPP = g++
CFLAGS = -Wall

ASSIGNMENTS = pa1 pa2

.PHONY: assignments $(ASSIGNMENTS)

assignments: $(ASSIGNMENTS)

$(ASSIGNMENTS): %:
	mkdir -p $@/bin/
	$(CLANG) $(CFLAGS) -o $@/bin/$@ $@/src/$@.cpp

compileForUF:
	for assignment in $(ASSIGNMENTS) ; do \
		mkdir -p $$assignment/bin/; \
		$(GPP) $(CFLAGS) -o $$assignment/bin/$$assignment $$assignment/src/$$assignment.cpp; \
	done

clean:
	for assignment in $(ASSIGNMENTS) ; do \
		rm -rf $$assignment/bin; \
	done
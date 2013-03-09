TARGETS = stir shake
include ../Makefile.head

stir : stir.c
	@$(CC) $(CFLAGS) -o $@ $^

shake : stir.c
	@$(CC) $(CFLAGS) -DSHAKE -o $@ $^

include ../Makefile.tail

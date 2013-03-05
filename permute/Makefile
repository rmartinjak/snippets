include ../Makefile.include

default : test

test : permute.c
	@$(CC) $(CFLAGS) -DPERMUTE_TEST -o $@ $<
	@./$@
	@rm $@

.PHONY : test
.PRECIOUS : test

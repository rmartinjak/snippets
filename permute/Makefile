include ../Makefile.include

default : test

test : permute_test.c
	@$(CC) $(CFLAGS) -o $@ $<
	@./$@
	@rm $@

.PHONY : test
.PRECIOUS : test

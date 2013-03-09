TARGETS = test
include ../Makefile.head

test : permute_test.c
	@$(CC) $(CFLAGS) -o $@ $<
	@./$@
	@rm $@

.PHONY : test
.PRECIOUS : test

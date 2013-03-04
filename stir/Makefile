CC = cc
CFLAGS += -O2 -std=c89 -pedantic -Wall -Wextra

all: stir shake

stir : stir.c
	@$(CC) $(CFLAGS) -o $@ $^

shake : stir.c
	@$(CC) $(CFLAGS) -DSHAKE -o $@ $^

clean :
	@rm -f stir shake

.PHONY : clean

CC=gcc
CFLAGS=-Wall -pedantic
LIBS=-lws2_32

SRCDIR=src
ODIR=src/obj

_DEPS=client.h
DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ=main.o client.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

bot: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *.exe
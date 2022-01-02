CC := gcc
CFLAGS := -Wvla -Os  -s -Wall -Werror -g -std=gnu11

TARGET := vm_x2017

LIBDIR := lib
SRCDIR := src
BUILDDIR := build

LIBS := -I${LIBDIR} -lm

SRCFILES := $(wildcard ${SRCDIR}/*.c)
OBJFILES := $(patsubst ${SRCDIR}/%.c, ${BUILDDIR}/%.o, ${SRCFILES})

objdump_x2017: ${SRCDIR}/objdump_x2017.c
	echo "making objdump_x2017"
	${CC} ${CFLAGS} ${LIBS}  -o  $@ $^ milestone/main.c

${TARGET}: ${OBJFILES}

	${CC} ${CFLAGS} -o $@ $^



${BUILDDIR}/%.o : ${SRCDIR}/%.c
	${CC} ${CFLAGS} ${LIBS} -c -o $@ $^

tests:
	./test.sh

run_tests:
	./test.sh

clean:
	rm build/*
	rm vm_x2017
	rm objdump_x2017
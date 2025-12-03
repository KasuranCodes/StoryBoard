PROG = x11
SRC = ${PROG}.c
OBJ = ${SRC:.c=.o}

CC = cc
INCS = -I/usr/include/X11 -IHeaders
LIBS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra -O2 ${INCS} -Wno-missing-field-initializers -pedantic-errors -Wno-unused-function -Wno-unused-variable

all: ${PROG}

${PROG}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}
	-rm ${OBJ}

%.o: %.c
	${CC} -c $< ${CFLAGS}


clean:
	-rm ${OBJ} ${PROG}

.PHONY: all clean
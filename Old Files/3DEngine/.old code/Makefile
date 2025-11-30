PROG = x11
# PROG = test3
SRC = ${PROG}.c
OBJ = ${SRC:.c=.o}

CC = cc
INCS = -I/usr/include/X11 -IHeaders -IHeaders/sokol
LIBS = -lX11 -lXi -lXcursor -lEGL -lGLESv2 -ldl -lpthread -lm 
LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra -O0 ${INCS} -Wno-missing-field-initializers -pedantic-errors -Wno-unused-function -Wno-unused-variable

all: ${PROG}

${PROG}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}
	-rm ${OBJ}

%.o: %.c
	${CC} -c $< ${CFLAGS}


clean:
	-rm ${OBJ} ${PROG}

.PHONY: all clean
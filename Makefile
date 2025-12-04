PROG = storyboard
SRC = storyboard.c
OBJ = ${SRC:.c=.o}

CC = cc
INCS = -Isrc -IHeaders
LIBS = -lglfw -lXi -lGL -lGLU -ldl -lpthread -lm 
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
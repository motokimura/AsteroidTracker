# Macros
PRG	= asteroid_tracker
OBJ	= main.o tf.o planet.o earth.o tracker.o asteroid_tracker.o asteroid_despatch.o pass.o

INCLUDE	= -I eigen-eigen-6b38706d90a9

CFLAGS	= -g -Wall
LFLAGS	= -lm

CC	= g++ $(CFLAGS)

# Primary target
$(PRG) : $(OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(LFLAGS)

# Suffix rule
.cpp.o :
	$(CC) -c $< $(INCLUDE)

# Target for cleaning files
clean :
	rm -f $(PRG) $(OBJ)

# Dependencies
main.o: main.cpp asteroid_despatch.h
tf.o: tf.cpp tf.h
planet.o: planet.cpp planet.h tf.h
earth.o: earth.cpp earth.h planet.h
tracker.o: tracker.cpp tracker.h earth.h tf.h
asteroid_tracker.o: asteroid_tracker.cpp asteroid_tracker.h planet.h tracker.h tf.h
asteroid_despatch.o: asteroid_despatch.cpp asteroid_despatch.h planet.h asteroid_tracker.h tracker.h tf.h
pass.o: pass.cpp pass.h tracker.h

IDIR = ./include
SDIR = ./source
ODIR = ./obj
LDIR = ./lib
BDIR = ./bin

CC=clang++
CFLAGS=-I$(IDIR) -stdlib=libc++ -std=c++11
LIBS=-lm

_DEPS = car.h accController.h cruiseController.h pidController.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = car.o accController.o cruiseController.o pidController.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(info $$_DEPS is [${_DEPS}])
$(info $$DEPS is [${DEPS}])
$(info $$_OBJ is [${_OBJ}])
$(info $$OBJ is [${OBJ}])

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

AdaptiveCruiseControl: $(OBJ) $(SDIR)/main.cpp
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
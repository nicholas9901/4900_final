DIR_BUILD = build
DIR_SRC   = src
EXE       = $(DIR_BUILD)/traffic_sim
OBJS      = $(DIR_BUILD)/main.o 
HEADERS   = $(DIR_SRC)/prototypes.h $(DIR_SRC)/definitions.h 
LD        = clang
FLAGS     = -g

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $(FLAGS) $(OBJS) -o $@

$(DIR_BUILD)/main.o: $(DIR_SRC)/main.c $(HEADERS)
	$(LD) -c $(FLAGS) $< -o $@

clean:
	rm -f $(EXE) *.o

.PHONY: all default
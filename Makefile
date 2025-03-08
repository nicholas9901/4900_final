CC              = clang
DIR_SRC         = src
DIR_BUILD_GUI   = build_gui
DIR_BUILD_NOGUI = build_nogui
EXE             = traffic_sim
HEADERS         = $(DIR_SRC)/prototypes.h $(DIR_SRC)/definitions.h
OBJS            = main.o \
                  vector.o \
                  vehicle.o \
                  intersection.o \
				  instructions.o
OBJS_GUI        = $(addprefix $(DIR_BUILD_GUI)/, $(OBJS)) $(DIR_BUILD_GUI)/draw.o
OBJS_NOGUI      = $(addprefix $(DIR_BUILD_NOGUI)/, $(OBJS))
FLAGS           = -g
DFLAGS_GUI      = -D GUI=1
DFLAGS_NOGUI    = -D GUI=0
LIBS			= -lSDL2main -lSDL2 

all: gui nogui

gui: $(DIR_BUILD_GUI)/$(EXE)

$(DIR_BUILD_GUI)/$(EXE): $(OBJS_GUI)
	$(CC) $(FLAGS) $(OBJS_GUI) $(LIBS) -o $@

nogui: $(DIR_BUILD_NOGUI)/$(EXE)

$(DIR_BUILD_NOGUI)/$(EXE): $(OBJS_NOGUI)
	$(CC) $(FLAGS) $(OBJS_NOGUI) -o $@

$(DIR_BUILD_GUI)/main.o: $(DIR_SRC)/main.c $(HEADERS) $(filter-out $(DIR_BUILD_GUI)/main.o, $(OBJS_GUI))
	$(CC) -c $(DFLAGS_GUI) $(FLAGS) $< -o $@

$(DIR_BUILD_NOGUI)/main.o: $(DIR_SRC)/main.c $(HEADERS) $(filter-out $(DIR_BUILD_NOGUI)/main.o, $(OBJS_NOGUI))
	$(CC) -c $(DFLAGS_NOGUI) $(FLAGS) $< -o $@

$(DIR_BUILD_GUI)/%.o: $(DIR_SRC)/%.c $(HEADERS)
	$(CC) -c $(DFLAGS_GUI) $(FLAGS) $< -o $@

$(DIR_BUILD_NOGUI)/%.o: $(DIR_SRC)/%.c $(HEADERS)
	$(CC) -c $(DFLAGS_NOGUI) $(FLAGS) $< -o $@

clean: clean_gui clean_nogui

clean_gui:
	rm -f $(DIR_BUILD_GUI)/$(EXE) $(DIR_BUILD_GUI)/*.o

clean_nogui:
	rm -f $(DIR_BUILD_NOGUI)/$(EXE) $(DIR_BUILD_NOGUI)/*.o

.PHONY: all gui nogui clean clean_gui clean_nogui
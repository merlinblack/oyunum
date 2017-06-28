# Nigels wizzzy "simple" makefile
# ###################################################################
#
ifeq ($(shell uname -s),Darwin)
	LUA=lua5.3
else
	LUA=lua
endif
CXX = c++
ALLEGRO_LIBS = allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5
CXXFLAGS += -std=c++14 -g
CXXFLAGS += -Isrc -Imanualbind/src
CXXFLAGS += $(shell pkg-config --cflags $(LUA) $(ALLEGRO_LIBS) )
LDFLAGS  += $(shell pkg-config   --libs $(LUA) $(ALLEGRO_LIBS) )
EXECUTABLE := game
INSTALLABLE_SCRIPTS := script.lua dozipfile.lua
SOURCES := game.cpp debug.cpp
OBJDIR := obj
DEPDIR := deps
INSTALLDIR := $(HOME)/oyunum
VPATH := src

# ###################################################################

.SILENT: $(DEPS) $(DEPDIR) $(OBJDIR) clean

OBJECTS=$(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPS=$(addprefix $(DEPDIR)/,$(SOURCES:.cpp=.d))

.PHONY: all
all:	$(OBJDIR) $(SOURCES) $(DEPS) gitversioning.h $(EXECUTABLE)

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp $(EXECUTABLE) $(INSTALLABLE_SCRIPTS) $(INSTALLDIR)

$(EXECUTABLE):	$(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

# Squeeky
# ########
.PHONY: clean
clean:
	echo 'Cleaning'
	rm -f $(OBJECTS) $(EXECUTABLE) gitversioning.h
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)

# Object files
# #############
#
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Dependency files
# #################
#
# Make sure gitversioning.h is made before trying to make
# any dependency file

$(DEPDIR)/%.d: %.cpp gitversioning.h | $(DEPDIR)
	@echo "Caching dependency info for: $<"
	@$(CC) $(CXXFLAGS) -MF"$@" -MG -MP -MM -MT"$(OBJDIR)/$(notdir $(<:.cpp=.o)) $@" "$<"

$(DEPDIR):
	mkdir -p $(DEPDIR)

ifeq (0,$(words $(findstring clean, $(MAKECMDGOALS))))
	# Don't make these, only to clean them out again.
-include $(DEPS)
endif

# gitversioning.h is remade when either git tags or heads change.
gitversioning.h: .git/refs/tags .git/refs/heads .git/HEAD
	@./gitversioning.sh

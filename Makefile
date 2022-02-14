CC       := gcc
LIBDIR   := lib
SRCDIR   := src
CFLAGS   := -Wall -I$(SRCDIR) -std=c99
OS       := Windows
# OS can be "Windows", "Linux", "OSX" or "Other" without double quotes
ifneq ($(OS), Windows)
$(error Operating system target "$(OS)" is unimplemented)
endif

# Libraries

NB_OBJS  := $(patsubst $(SRCDIR)/NB/%.c,obj/%.o,$(wildcard $(SRCDIR)/NB/*.c))
NGE_OBJS := $(patsubst $(SRCDIR)/NGE/%.c,obj/%.o,$(wildcard $(SRCDIR)/NGE/*.c))

obj/%.o: $(SRCDIR)/NB/%.c $(wildcard $(SRCDIR)/NB/*.h)
	$(CC) $(CFLAGS) -c -o $@ $<
obj/%.o: $(SRCDIR)/NGE/%.c $(wildcard $(SRCDIR)/NGE/*.h)
	$(CC) $(CFLAGS) -c -o $@ $<
$(LIBDIR)/libnb.a:$(NB_OBJS)
	ar r $@ $^
$(LIBDIR)/libnge.a:$(NGE_OBJS)
	ar r $@ $^

# Tools

TOOL_BINS := 

tools/%.exe: $(SRCDIR)/tools/%.c $(wildcard $(SRCDIR)/*/*.h)
	$(CC) $(CFLAGS) -o $@ $<

# Game assets

RESDIR     := assets

# -------- Windows OS --------

$(RESDIR)/assets.rc: $(filter-out %.rc,$(wildcard $(RESDIR)/*))
	echo 1 24 \"$(RESDIR)/$(NAME).exe.manifest\" > $@
	for f in $(subst $(RESDIR)/,,$^) ; do \
		echo $$f RCDATA \"$(RESDIR)/$$f\" >> $@; \
	done

obj/assets_rc.o: $(RESDIR)/assets.rc
	windres $< $@

# ----------------------------

# Project1Client

NAME     := Project1Client
ifeq ($(OS), Windows)
OBJS     := $(patsubst $(SRCDIR)/$(NAME)/%.c,obj/%.o,$(wildcard $(SRCDIR)/$(NAME)/*.c) $(wildcard $(SRCDIR)/$(NAME)/content/*.c)) obj/assets_rc.o
else
OBJS     := $(patsubst $(SRCDIR)/$(NAME)/%.c,obj/%.o,$(wildcard $(SRCDIR)/$(NAME)/*.c) $(wildcard $(SRCDIR)/$(NAME)/content/*.c))
endif
LIBS     := $(LIBDIR)/libnge.a $(LIBDIR)/libnb.a $(LIBDIR)/libglew32.a
ELIB     := -lopengl32 -lgdi32

obj/%.o: $(SRCDIR)/$(NAME)/%.c $(wildcard $(SRCDIR)/$(NAME)/*.h)
	$(CC) $(CFLAGS) -c -o $@ $<
game/$(NAME).exe: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L./$(LIBDIR)/ $(patsubst %,-l:%,$(LIBS)) $(ELIB)

# Misc.

.PHONY: clean all_bins

all_bins: game/$(NAME).exe $(TOOL_BINS)
.DEFAULT_GOAL := all_bins

clean:
	$(RM) $(RESDIR)/assets.rc
	$(RM) $(filter-out .gitignore,$(wildcard game/*))
	$(RM) $(filter-out .gitignore obj/content,$(wildcard obj/*))
	$(RM) $(filter-out .gitignore,$(wildcard obj/content/*))
	$(RM) lib/libnb.a lib/libnge.a
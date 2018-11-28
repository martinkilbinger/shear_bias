# Makefile for get_shapes. This is a modified Makefile from the
# shapelens package.

# Makefile assumes PREFIX (root path for installing files) to be set.
# If it is not set, it will point to ~/.local/.
# The path to TMV is taken from TMV_PREFIX, which defaults to PREFIX.
# Additional compilation flags can be specified with SPECIAL_FLAGS, 
# linker flags with SPECIAL_LIBS

LIBNAME = shapelens
INCLPATH = include
LIBPATH = $(HOME)/.local/lib
PROGSRCPATH = .
PROGPATH = ../build/bin

########
# setting defaults and other environment variables
########
ifndef PREFIX
	PREFIX = $(HOME)/.local
endif

ifndef TMV_PREFIX
	TMV_PREFIX = $(PREFIX)
endif

# which OS
UNAME := $(shell uname)

# compilation flags
CC = g++
CFLAGS = -ansi $(SPECIAL_FLAGS) -I$(HOME)/.local/include
ifneq ($(UNAME),Linux)
	CFLAGS += -bind_at_load -I/opt/local/include
endif

# linker flags
#TMVLINK := $(shell cat ${TMV_PREFIX}/share/tmv/tmv-link)
TMVLINK = -L$(HOME)/astro/others/software/tmv/lib -ltmv
LIBS = $(TMVLINK) -lcfitsio -L/opt/local/lib

ifneq (,$(findstring HAS_WCSLIB,$(SPECIAL_FLAGS)))
	LIBS += -lwcs
endif

# archiver flags
AR = ar
ARFLAGS = -sr
ifeq ($(UNAME),Linux)
	SHAREDFLAGS = -shared -fPIC 
	LIBEXT = so
else
	SHAREDFLAGS = -dynamiclib -fPIC
	LIBEXT = dylib
endif

########
# target section
########
PROGS = $(wildcard $(PROGSRCPATH)/*.cc)
PROGSOBJECTS = $(PROGS:$(PROGSRCPATH)/%.cc=$(PROGPATH)/%)

all: progs

.PHONY: clean

clean: 
	rm -f $(PROGSOBJECTS)

progs: $(PROGPATH) $(PROGSOBJECTS)

$(PROGPATH):
	mkdir -p $(PROGPATH)

install: progs
	mkdir -p $(PREFIX)/bin
	cp $(PROGSOBJECTS) $(PREFIX)/bin/

$(PROGPATH)/%: $(PROGSRCPATH)/%.cc
	$(CC) $(CFLAGS) $(SPECIAL_LIBS) -L$(LIBPATH) $< -o $@ -l$(LIBNAME) $(LIBS)
	install_name_tool $@ -change lib/libshapelens.dylib $(LIBPATH)/libshapelens.dylib


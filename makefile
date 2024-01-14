# .SUFFIXES:
# .SUFFIXES: .c .prg
VPATH = src:src/includes

CC			=c:/Users/chris/oscar64/bin/oscar64.exe

CFLAGS=-g -O0 -n -pp 
#-v2
#-e

# OBJDIR=bin
# vpath %.c src


#$(OBJDIR) obj/%.o : %.c ku.h kudefines.h kuglobals.h kufns.h 
#  $(CC) -c $(CPPFLAGS) -o="$@.prg" $<
# ./bin/%.prg: ./src/%.c
# 	$(CC) -c $(CPPFLAGS) -o=bin/$@.prg src/$<
#	$(CC) $(CFLAGS) $<

#
# Only compile Tumult.c, but have freshness dependencies on the other cc files, all .h files, all assets & sprites
#
DEPS=src/includes/*.h assets/* assets/sprites/*

bin/Tumult.prg: Tumult.c prefs.c file_io.c prefs.c tumult_prefs.c 
	echo prereqs are:$^;(date)>make-out.log ; $(CC) $(CFLAGS) $(INCLUDE:%=-i=%) -o="$@" $^ 
#>>make-out.log


#all: snake.prg lander.prg maze3d.prg missile.prg breakout.prg connectfour.prg hscrollshmup.prg
#all: bin/Tumult.prg bin/staticsprite.prg bin/bitmapcolorimage.prg bin/staticsprite.prg bin/hires_pic_test.prg
all: bin/Tumult.prg bin/bitmapcolorimage.prg

# snake.prg: snake.c
# 	$(CC) $<

# missile.prg: missile.c
# 	$(CC) $(CFLAGS) -O3 $<

# hscrollshmup.prg: hscrollshmup.c
# 	$(CC) $(CFLAGS) -O2 $<

clean:
	$(RM) bin/*.asm bin/*.int bin/*.lbl bin/*.map bin/*.prg bin/*.bcs  bin/*.dbj 
# CFLAGS		=-n
# BIN			=bin
# SRC			=src

# OBJECT_FILES=$(addprefix $(OBJ)/, $(OBJECTS))

# $(BIN)/%.prg: $(SRC)/%.c
# #	$(CC) $(CFLAGS) $<
# 	oscar64 -o="%.prg" -g -O0 $<

# all: $(BIN)/Tumult.prg $(BIN)/staticsprite.prg
# # all: joycontrol.prg multiplexer.prg creditroll.prg sprmux32.prg sprmux64.prg

# # joycontrol.prg: joycontrol.c
# # 	$(CC) $<

# # sprmux32.prg: sprmux32.c
# # 	$(CC) $(CFLAGS) $< -O2 -dVSPRITES_MAX=32 -dNUM_IRQS=28

# # clean:
# # 	$(RM) *.asm *.int *.lbl *.map *.prg *.bcs

# #Start of the makefile

# VPATH = ./src:./src/includes:./bin

# OUTPUT_OPTION = -o="bin/$@" -g -O0

# CXXFLAGS += -Wall -g -I./header

# Target = $(notdir $(CURDIR)).exe

# Objects := $(notdir $(patsubst %.c,%.o,$(wildcard src/*.cpp)))



# all: $(Target)

# $(Target): $(Objects)
#      $(CXX) $(CXXFLAGS) -o $(Target) $(addprefix objects/,$(Objects))


# #Beware of -f. It skips any confirmation/errors (e.g. file does not exist)

# .PHONY: clean
# clean:
#      rm -f $(addprefix objects/,$(Objects)) $(Target)

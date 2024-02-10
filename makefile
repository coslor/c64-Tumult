VPATH = src:src/includes;src/tests/includes

CC			=c:/Users/chris/oscar64/bin/oscar64.exe

CFLAGS=-g -O0 -n -v2 -dLOG_LEVEL=100 -dCLC_LOGGING
#-v2
#-e

#
# 
#
MY_DEPS2=Tumult.c logger.c  file_io.c tumult_prefs.c new_prefs.c 
TEST_DEPS=tests/test_logger.c tests/test_runner.c logger.c
# assets/* assets/sprites/* 

VICE=C:/Users/chris/WinVice/GTK3VICE-3.5-win64/bin/x64sc.exe

MAKE_LOG=make-out.log
MAKE_TESTS_LOG=make-tests-out.log

all:tumult tests

bin/%.prg: $(MY_DEPS2)
	(date) >$(MAKE_LOG) ; $(CC) $(CFLAGS) $(INCLUDE:%=-i=%) -o="$@" $^ >>MAKE_LOG

bin/tests/%.prg: $(TEST_DEPS)
	(date) >$(MAKE_TESTS_LOG) ; $(CC) $(CFLAGS) $(INCLUDE:%=-i=%) -o="$@" $^ >>MAKE_TESTS_LOG

tests: bin/tests/test_logger.prg

tumult: bin/Tumult.prg

clean:
	$(RM) bin/*.asm bin/*.int bin/*.lbl bin/*.map bin/*.prg bin/*.bcs  bin/*.dbj bin/tests/*

run: tumult
	$(VICE) bin/Tumult.prg

run_tests: tests
	$(VICE) bin/tests/test_logger.prg

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

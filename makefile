CFLAGS = -c -Iinc/
SRC_FILES = $(wildcard src/*.c)
OUTS = $(SRC_FILES:src/%.c=%.out) # each source has a corresponding executable

vpath %.c src
vpath %.h inc

.PRECIOUS: obj/%.o # prevent auto removal of object files
# By default make all executables
# to get a specific one, pass its name to make for exmple (make wc.out)
all : ${OUTS} stat

# pattern rule for making executables out of objects
%.out : obj/%.o obj/help.o
	gcc $^ -o $@

#pattern for making objects from sources
obj/%.o : %.c
	@echo "Building for $<"
	gcc ${CFLAGS} $< -o $@

stat : ${OUTS}
	@echo "\nDone building"
	@echo > stat

clean : 
	rm -f obj/*.o *.out

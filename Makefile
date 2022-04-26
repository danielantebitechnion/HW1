CC = gcc
OBJS = AsciiArtTool.o RLEList.o main.o
EXEC = prog
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
    $(CC) $(DEBUG_FLAG) $(OBJS) -o $@
AsciiArtTool.o : AsciiArtTool.c AsciiArtTool.h RLEList.h
  $(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
RLEList.o : RLEList.c RLEList.h
  $(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
main.o : main.c AsciiArtTool.c 
  $(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
  rm -f $(OBJS) $(EXEC)

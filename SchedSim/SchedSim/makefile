CC=gcc
CFLAGS=
LDFLAGS=-lncurses
SOURCES=main.c Process.c PCB.c CPU.c Scheduler.c IODevice.c KBDDevice.c SharedMem.c SysTimer.c SysClock.c PCBQueue.c LinkedBlockingQueue.c LinkedQueueNode.c GUI.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=SchedSim

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(ODIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $<

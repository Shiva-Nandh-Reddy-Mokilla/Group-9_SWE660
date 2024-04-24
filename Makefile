CC = gcc
CFLAGS = -pthread
LDFLAGS = 

SRCS = main.c mq.c lcd.c ph_sensor.c
OBJS = $(SRCS:.c=.o)

TARGET = fish_tank_monitor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

mq.o: mq.c
	$(CC) $(CFLAGS) -c mq.c

lcd.o: lcd.c
	$(CC) $(CFLAGS) -c lcd.c

ph_sensor.o: ph_sensor.c
	$(CC) $(CFLAGS) -c ph_sensor.c

clean:
	rm -f $(OBJS) $(TARGET)

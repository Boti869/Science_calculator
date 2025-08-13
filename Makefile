CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = main.c \
      calcs/help.c calcs/basic-calc.c calcs/Science-calc.c \
      electrics-c/electrical-laws.c electrics-c/initLaws.c
OBJ = $(SRC:.c=.o)
TARGET = mycalc

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

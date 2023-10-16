# A MAKEFILE TO BUILD OUR MYSYNC PROJECT

PROJECT = mysync
HEADERS = $(PROJECT).h
OBJ		= mysync.o flags.o processfile.o sync.o glob.o

C11		= cc -std=c11
CFLAGS 	= -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean: 
	rm -f $(PROJECT) $(OBJ)
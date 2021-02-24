# Defining the makefile
CC = gcc # Assigning gcc to a variable 'CC'
DEPS = headers.h # Assigning headers.h to a variable 'DEPS'
all_sources = project.c # Assigning project.c to variable all_sources
all_objs = $(all_sources:.c=.o) #Similarly the variable all_objs is assigned
# Whenever we use a variable, $ symbol must be used before mentioning that variable
my_project: $(all_objs)
		$(CC) -o my_project $(all_sources) # This command runs when we execute 'make my_project' or 'make' which compiles the files  
%.o: %.c $(DEPS)
	$(CC) -c $(all_sources) # This command creates the object file for the corresponding .c file 

clean:
	rm my_project *.o # This command removes all the .o files which runs when we execute 'make clean'

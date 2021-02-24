CC = gcc
DEPS = headers.h
all_sources = project.c 
all_objs = $(all_sources:.c=.o)

my_project: $(all_objs)
		$(CC) -o my_project $(all_sources)
%.o: %.c $(DEPS)
	$(CC) -c $(all_sources)

clean:
	rm my_project *.o
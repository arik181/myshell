# Change Me
NAME	= myshell
ADIR	= arik182

# Constant Variables
COMPILER	= gcc
ASSEMBLER	= as
LINKER		= ld

# Flags
CURSES		= -lcurses
NC		= -lncurses
DEBUG		= -g
#OPT		= -O2
#OPT		= -O3
OBJECT		= -c
LINK		= -lc -dynamic-linker /lib/ld-linux.so.2
PROFILE		= -pg

### Compile the application ###

$(NAME) : $(NAME).c $(NAME).h 
	$(COMPILER) -o $(NAME) $(NAME).c $(OPT) 

### Special Cases ###
	
debug : debugtarget
	ctags $(NAME).c && gdb ./$(NAME)

profile : profiletarget
	echo "Profiling..."
	rm -rf report.out
	rm -rf gmon.out
	echo >> report.out
	echo >> report.out
	echo "--------------------------------------------------------------" >> report.out
	echo >> report.out
	echo "Profile of ./$(NAME)" >> report.out
	echo >> report.out
	./$(NAME) -s $(TESTFILE1) $(CPFILE) >> report.out
	echo >> report.out
	gprof --brief --no-graph ./$(NAME) >> report.out
	vim report.out

debugtarget : $(NAME).c $(NAME).h 
	$(COMPILER) $(DEBUG) -o $(NAME) $(NAME).c 

profiletarget : $(NAME).c $(NAME).h 
	$(COMPILER) $(PROFILE) -o $(NAME) $(NAME).c 

clean : $(NAME)
	rm $(NAME) && rm *.o 

dump: $(NAME)
	objdump -d $(NAME) > objcode

homework: $(NAME) profile
	echo "Cleaning up..."
	rm -rf $(NAME)
	rm -rf *~
	rm -rf $(ADIR)
	echo "Creating directory..."
	mkdir $(ADIR)
	echo "Copying targets..."
	cp Makefile $(NAME).c $(NAME).h report.out typescript.out $(ADIR)/
	tar cvf $(ADIR).tar $(ADIR)
	echo "Done."


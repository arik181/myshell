# Change Me
NAME	= myshell
NAME2	= mysleep
REQ1	= list
ADIR	= arik182

# Constant Variables
COMPILER	= gcc -Wall
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

$(NAME) : $(NAME).c $(NAME).h $(REQ1).c $(REQ1).h
	$(COMPILER) -o $(NAME) $(NAME).c $(REQ1).c $(OPT) 

run : $(NAME).c $(NAME).h $(REQ1).c $(REQ1).h
	$(COMPILER) -o $(NAME) $(NAME).c $(REQ1).c $(OPT) 
	./$(NAME)

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

debugtarget : $(NAME).c $(NAME).h $(REQ1).h
	$(COMPILER) $(DEBUG) -o $(NAME) $(NAME).c $(REQ1).c

profiletarget : $(NAME).c $(NAME).h 
	$(COMPILER) $(PROFILE) -o $(NAME) $(NAME).c 

clean : $(NAME)
	rm $(NAME) && rm *.o 

dump: $(NAME)
	objdump -d $(NAME) > objcode

tex : 	
	latex $(NAME).tex
	dvipdfm $(NAME).dvi

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

$(NAME2) : $(NAME2).c
	$(COMPILER) -o $(NAME2) $(NAME2).c $(OPT) 

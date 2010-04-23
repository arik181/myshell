# Change Me
QSH		= qsh
SLEEP	= mysleep
ADIR	= arik182

# Files
SRC		= defs.h input.h main.h proc.h redir.h util.h list.h input.c main.c \
	  		proc.c redir.c util.c list.c 
TEXSRC 	= designdoc.tex 
PDF 	= designdoc.pdf 

# Flags
NC			= -lncurses
DEBUG		= -g
OPT			= -O2
OBJECT		= -c
PROFILE		= -pg
CFLAGS		= $(DEBUG) 
LDFLAGS		= -lc -dynamic-linker /lib/ld-linux.so.2

### Compile the application ###
all : $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

debug : $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)
	ctags $(SRC)
	gdb ./$(NAME)

$(NAME) : $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

$(NAME2) : $(SRC)
	$(CC) $(CFLAGS) -o $(NAME2) $(SRC)

### Special Cases ###
clean : $(NAME)
	rm $(NAME) && rm *.o 

test : $(NAME) tests
	script -f -c 'cat ./tests | ./$(NAME)'

homework : $(NAME) typescript
	echo "Cleaning up..."
	rm -rf $(NAME)
	rm -rf $(ADIR)
	echo "Creating directory..."
	mkdir $(ADIR)
	echo "Copying targets..."
	cp Makefile $(SRC) report.out typescript $(ADIR)/
	tar cvf $(ADIR).tar $(ADIR)
	echo "Done."

UNAME		= arik181
TITLE		= CS333 Project 1
ADDRESS1	= cs333acc@cs.pdx.edu
ADDRESS2	= $(UNAME)@gmail.com
DIR			= $(UNAME)/
TARFILE		= $(UNAME).tar.gz

tar : homework	
	rm -rf $(TARFILE)
	tar czvf $(TARFILE) $(DIR)

mail : tar 
	mpack -a -s '$(TITLE) tar archive' $(TARFILE) $(ADDRESS1)
	mpack -a -s '$(TITLE) tar archive' $(TARFILE) $(ADDRESS2)

tex : $(TEXSRC)
	pdflatex $(TEXSRC)
	cp -f $(PDF) ~/public_html/


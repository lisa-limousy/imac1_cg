CC       =  gcc
CFLAGS   = -Wall -O2 -g
LIB      = -lSDL -lGLU -lGL -lm  
INCLUDES = 

RM       = rm -f
BIN      = minimal ex3 ex4 ex5 ex6
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all : $(BIN)

% : %.c
	@$(CC) $(CFLAGS) $< $(LIB) $(INCLUDES) -o $@
	@echo $(CC) $< -o $@

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN) 

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)



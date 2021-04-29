MAINDIR	 								= src/main/
MODULESDIR 								= src/modules/
DATASETSMOD								= datasets_mod/
UTILSDIR								= src/utils/
TCPMOD	 								= tcp_mod/
CONFIGMOD								= config_mod/
OUTPUTMOD								= output_mod/
LIBSDIR									= libs/
JSMNLIBS								= jsmn/

DATASETSMODDIR 							= $(MODULESDIR)$(DATASETSMOD)
TCPMODDIR								= $(MODULESDIR)$(TCPMOD)
CONFIGMODDIR							= $(MODULESDIR)$(CONFIGMOD)
JSMNLIBSDIR								= $(LIBSDIR)$(JSMNLIBS)
PARTITIONINGMODDIR						= $(MODULESDIR)$(PARTITIONINGMOD)
OUTPUTMODDIR							= $(MODULESDIR)$(OUTPUTMOD)

MAINOBJ									= $(MAINDIR)main.c
DATASETSOBJ								= $(DATASETSMODDIR)datasets.c
UTILSOBJ								= $(UTILSDIR)utils.c
TCPOBJ									= $(TCPMODDIR)tcp.c
JSMNOBJ									= $(JSMNLIBSDIR)jsmn.c
CONFIGOBJ								= $(CONFIGMODDIR)config.c
CLOGGEROBJ								= $(UTILSDIR)clogger.c
OUTPUTOBJ								= $(OUTPUTMODDIR)output.c
STACKOBJ								= $(UTILSDIR)stack.c

CC 										= gcc
CFLAGS   								= -Wall -g
LDFLAGS  								= -lm
IDFLAGS	 								= -I$(MAINDIR) -I$(DATASETSMODDIR) -I$(TCPMODDIR) -I$(CONFIGMODDIR) -I$(UTILSDIR) -I$(JSMNLIBSDIR) -I$(OUTPUTMODDIR)
OBJFILES 								= $(MAINOBJ) $(DATASETSOBJ) $(TCPOBJ) $(UTILSOBJ) $(CLOGGEROBJ) $(JSMNOBJ) $(CONFIGOBJ) $(OUTPUTOBJ) $(STACKOBJ) 
EXEC   									= bin/TCP-sequential.run

all: $(EXEC)
$(EXEC):
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJFILES) $(IDFLAGS) $(LDFLAGS)

clean:
	@rm -f $(EXEC)
mrproper: clean
	@rm -f $(EXEC)
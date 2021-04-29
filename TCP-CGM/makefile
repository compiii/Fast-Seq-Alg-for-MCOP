MAINDIR	 								= src/main/
MODULESDIR 								= src/modules/
DATASETSMOD								= datasets_mod/
UTILSDIR								= src/utils/
TCPMOD	 								= tcp_mod/
CONFIGMOD								= config_mod/
OUTPUTMOD								= output_mod/
LIBSDIR									= libs/
JSMNLIBS								= jsmn/
KENGNETMAX								= kengne_tmax/
KENGNETINT								= kengne_tint/
KECHID									= kechid/

DATASETSMODDIR 							= $(MODULESDIR)$(DATASETSMOD)
TCPMODDIR								= $(MODULESDIR)$(TCPMOD)
CONFIGMODDIR							= $(MODULESDIR)$(CONFIGMOD)
JSMNLIBSDIR								= $(LIBSDIR)$(JSMNLIBS)
OUTPUTMODDIR							= $(MODULESDIR)$(OUTPUTMOD)
KENGNETMAXDIR							= $(TCPMODDIR)$(KENGNETMAX)
KENGNETINTDIR							= $(TCPMODDIR)$(KENGNETINT)
KECHIDDIR								= $(TCPMODDIR)$(KECHID)

MAINOBJ									= $(MAINDIR)main.c
DATASETSOBJ								= $(DATASETSMODDIR)datasets.c
UTILSOBJ								= $(UTILSDIR)utils.c
TCPOBJ									= $(TCPMODDIR)tcp.c
JSMNOBJ									= $(JSMNLIBSDIR)jsmn.c
CONFIGOBJ								= $(CONFIGMODDIR)config.c
CLOGGEROBJ								= $(UTILSDIR)clogger.c
OUTPUTOBJ								= $(OUTPUTMODDIR)output.c
STACKOBJ								= $(UTILSDIR)stack.c
KENGNETMAXOBJ							= $(KENGNETMAXDIR)kengne_tmax.c
KENGNETINTOBJ							= $(KENGNETINTDIR)kengne_tint.c
KECHIDOBJ								= $(KECHIDDIR)kechid.c

CC 										= mpicc
CFLAGS   								= -Wall -g
LDFLAGS  								= -lm
IDFLAGS	 								= -I$(MAINDIR) -I$(DATASETSMODDIR) -I$(TCPMODDIR) -I$(CONFIGMODDIR) -I$(UTILSDIR) -I$(JSMNLIBSDIR) -I$(OUTPUTMODDIR) -I$(KENGNETMAXDIR) -I$(KENGNETINTDIR) -I$(KECHIDDIR)
OBJFILES 								= $(MAINOBJ) $(DATASETSOBJ) $(TCPOBJ) $(UTILSOBJ) $(CLOGGEROBJ) $(JSMNOBJ) $(CONFIGOBJ) $(OUTPUTOBJ) $(STACKOBJ) $(KENGNETMAXOBJ) $(KENGNETINTOBJ) $(KECHIDOBJ)
EXEC   									= bin/TCP-CGM.run

all: $(EXEC)
$(EXEC):
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJFILES) $(IDFLAGS) $(LDFLAGS)

clean:
	@rm -f $(EXEC)
mrproper: clean
	@rm -f $(EXEC)
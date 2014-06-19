C =			c
C_COMPILER =		cc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	gcc
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1
OBJ =			o
LINK =			gcc -Wall -o 
LINK_OPTS =		-L.
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		ar cr 
LIBRARY_LINK_OPTS =
LIB_SUFFIX =			a
DLL_SUFFIX =			dll
EXE =

CXX = gcc
CXXFLAGS = $(INCLUDES) -g -O0 -Wall -fmessage-length=0 -fPIC -D__STDC_CONSTANT_MACROS -Wno-deprecated-declarations -DCURL_STATICLIB -shared
CFLAGS = $(CXXFLAGS)
CC	= gcc
CPPFLAGS = /I. /DHAVE_PTW32_CONFIG_H
XCFLAGS = /W3 /MD /nologo
#CFLAGS	= /O2 /Ob2 $(XCFLAGS)
CFLAGSD	= /Z7 $(XCFLAGS)


DLL=libDialClient.$(DLL_SUFFIX)
ALL = $(DLL)
all:	$(ALL)

LIBS += -lstdc++ -lIphlpapi -lpthread -lcurl -lws2_32 -lwldap32

# dial_client source
DIAL_CLIENT_OBJ = dial_client_main.$(OBJ) get_ipaddress.$(OBJ) DialServer.$(OBJ) DialDiscovery.$(OBJ)

ifeq ($(VERBOSE), no)
ECHO   = printf "$(1)\t%s\n" $(2)
BRIEF  = CC CXX AR RM
MSG    = $@
$(foreach VAR,$(BRIEF), \
    $(eval override $(VAR) = @$$(call ECHO,$(VAR),$$(MSG)); $($(VAR))))
endif


$(DLL): $(DIAL_CLIENT_OBJ)
	$(CXX) -o $(DLL) $(DIAL_CLIENT_OBJ) $(LIBS)
	
.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<       

.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

dial_client_main.$(CPP):	get_ipaddress.h
get_ipaddress.$(CPP):	get_ipaddress.h
DialServer.$(CPP):	DialServer.h
DialDiscovery.$(CPP):	DialDiscovery.h

	
clean:
	$(RM) -f $(DIAL_CLIENT_OBJ) $(LIB)

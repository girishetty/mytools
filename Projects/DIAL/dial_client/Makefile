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


CXX = gcc
CXXFLAGS = $(INCLUDES) -g -O0 -Wall -fmessage-length=0 -fPIC -D__STDC_CONSTANT_MACROS -Wno-deprecated-declarations -DCURL_STATICLIB
CFLAGS = $(CXXFLAGS)

# dial_client source
DIAL_CLIENT = dial_client_main.o get_ipaddress.o DialServer.o DialDiscovery.o 		

LIBS += -lstdc++ -lIphlpapi -lpthread -lcurl -lws2_32 -lwldap32

DIAL_CLIENT_TARGET = dial_client

ifeq ($(VERBOSE), no)
ECHO   = printf "$(1)\t%s\n" $(2)
BRIEF  = CC CXX AR RM
MSG    = $@
$(foreach VAR,$(BRIEF), \
    $(eval override $(VAR) = @$$(call ECHO,$(VAR),$$(MSG)); $($(VAR))))
endif

$(DIAL_CLIENT_TARGET): $(DIAL_CLIENT)
	$(CXX) -o $(DIAL_CLIENT_TARGET) $(DIAL_CLIENT) $(LIBS)

all:	$(DIAL_CLIENT_TARGET)

clean:
	$(RM) -f $(DIAL_CLIENT) $(DIAL_CLIENT_TARGET)

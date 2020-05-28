C =			c
C_COMPILER =		cc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	gcc
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1
OBJ =			o
LINK =			gcc -Wall -o 
LINK_OPTS =		-L.

CXX = gcc
CXXFLAGS = $(INCLUDES) -g -O0 -Wall -Wno-deprecated-declarations

# VulcanDictionary source
VULCAN_DICTIONARY = main.o AlphabetTable.o DictionaryReader.o

LIBS += -lstdc++ 

VULCAN_DICTIONARY_TARGET = VulcanDictionary

ifeq ($(VERBOSE), no)
ECHO   = printf "$(1)\t%s\n" $(2)
BRIEF  = CC CXX AR RM
MSG    = $@
$(foreach VAR,$(BRIEF), \
    $(eval override $(VAR) = @$$(call ECHO,$(VAR),$$(MSG)); $($(VAR))))
endif

$(VULCAN_DICTIONARY_TARGET): $(VULCAN_DICTIONARY)
	$(CXX) -o $(VULCAN_DICTIONARY_TARGET) $(VULCAN_DICTIONARY) $(LIBS)

all:	$(VULCAN_DICTIONARY_TARGET)

clean:
	$(RM) -f $(VULCAN_DICTIONARY) $(VULCAN_DICTIONARY_TARGET)

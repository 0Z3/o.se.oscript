BASENAME=oscript
G4=$(BASENAME:=.g4)

CC=clang
CPP=clang++

LIBOSE_DIR=../libose

# this produces a var called $LD_UNDEF_FLAGS
include $(LIBOSE_DIR)/ose_linker_flags.mk

MOD_CPP_FILES=\
	$(BASENAME)VisitorImpl\
	$(BASENAME)Lexer\
	$(BASENAME)Parser\
	$(BASENAME)_parser

MOD_C_FILES=\
	ose_$(BASENAME)\
	$(BASENAME)_exec\
	$(BASENAME)_lib\
	$(BASENAME)_types\
	$(BASENAME)_print\

ANTLR4_DIR=antlr4
ANTLR4_RUNTIME_DIR=$(ANTLR4_DIR)/runtime/Cpp
ANTLR4_INCLUDE_DIR=$(ANTLR4_RUNTIME_DIR)/run/usr/local/include/antlr4-runtime
ANTLR4_LIB_DIR=$(ANTLR4_RUNTIME_DIR)/run/usr/local/lib

INCLUDES=-I. -I$(LIBOSE_DIR) -I$(ANTLR4_INCLUDE_DIR)

DEFINES=-DHAVE_OSE_ENDIAN_H \
	-DOSE_CONF_PROVIDE_TYPE_DOUBLE\
	-DOSE_CONF_PROVIDE_TYPE_TIMETAG\
	-DOSE_GETPAYLOADITEMLENGTH_HOOK=osevm_getPayloadItemLength_hook \
	-DOSE_GETPAYLOADITEMSIZE_HOOK=osevm_getPayloadItemSize_hook \
	-DOSE_PPRINTPAYLOADITEM_HOOK=osevm_pprintPayloadItem_hook

CFLAGS_DEBUG=-Wall -DOSE_CONF_DEBUG -O0 -glldb -fPIC
CFLAGS_RELEASE=-Wall -O3 -fPIC

CPPFLAGS_DEBUG=-Wall -DOSE_CONF_DEBUG -O0 -glldb --std=c++17 -fPIC
CPPFLAGS_RELEASE=-Wall -O3 --std=c++17 -fPIC

ifeq ($(shell uname), Darwin)
LDFLAGS=-fvisibility=hidden -shared \
	-Wl,-exported_symbol,_ose_main \
	-flat_namespace \
	$(LD_UNDEF_FLAGS)
else
LDFLAGS=-fvisibility=hidden -shared \
	-flat_namespace \
	$(LD_UNDEF_FLAGS)
endif

release: CFLAGS=$(CFLAGS_RELEASE)
release: CPPFLAGS=$(CPPFLAGS_RELEASE)
release: ose_$(BASENAME).so

debug: CFLAGS=$(CFLAGS_DEBUG)
debug: CPPFLAGS=$(CPPFLAGS_DEBUG)
debug: ose_$(BASENAME).so

$(MOD_CPP_FILES:=.o): %.o: %.cpp $(ANTLR4_INCLUDE_DIR)
	$(CPP) -c $(CPPFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

$(MOD_C_FILES:=.o): %.o: %.c $(ANTLR4_INCLUDE_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

ose_$(BASENAME).so: $(MOD_C_FILES:=.o) $(MOD_CPP_FILES:=.o)
	$(CPP) $(LDFLAGS) -o o.se.$(BASENAME).so $^ $(ANTLR4_LIB_DIR)/libantlr4-runtime.a

$(LIBOSE_DIR)/sys/ose_endian.h:
	cd $(LIBOSE_DIR) && $(MAKE) sys/ose_endian.h

$(ANTLR4_INCLUDE_DIR):
	./make_antlr4_Cpp_runtime.sh

ifndef ANTLR4
ANTLR4=/usr/local/lib/antlr-4.9.3-complete.jar
endif

.PHONY: antlr
antlr: 
	java -jar $(ANTLR4) -package $(BASENAME) -Dlanguage=Cpp -visitor -no-listener $(BASENAME).g4

.PHONY: clean
clean:
	rm -rf *.o *.so *.interp *.tokens

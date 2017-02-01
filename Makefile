#
# Makefile for HAL
#

RELIC_PTH=/usr/local
GMP_PTH=/usr/local
FLINT_PTH=/usr/local

DIR_PTH =.
IDIR=include
ODIR=obj
SDIR=src

CC=gcc

PREFIX=''

ifeq ($(PREFIX),'')
        PREFIX=/usr/local
endif

# Paths to internal directories

_UTIL_PTH=utils
_TEST_PTH=test
IUTIL_PTH=$(DIR_PTH)/$(IDIR)/$(_UTIL_PTH)
OUTIL_PTH=$(DIR_PTH)/$(ODIR)/$(_UTIL_PTH)
SUTIL_PTH=$(DIR_PTH)/$(SDIR)/$(_UTIL_PTH)

# FGP scheme paths
_FGP_PTH=fgp
_FGP_TOOL_PTH=fgp/tools
IFGP_PTH=$(DIR_PTH)/$(IDIR)/$(_FGP_PTH)
IFGP_TOOL_PTH=$(DIR_PTH)/$(IDIR)/$(_FGP_TOOL_PTH)
OFGP_PTH=$(DIR_PTH)/$(ODIR)/$(_FGP_PTH)
OFGP_TOOL_PTH=$(DIR_PTH)/$(ODIR)/$(_FGP_TOOL_PTH)
SFGP_PTH=$(DIR_PTH)/$(SDIR)/$(_FGP_PTH)
SFGP_TOOL_PTH=$(DIR_PTH)/$(SDIR)/$(_FGP_TOOL_PTH)

# CF13 scheme paths
_CF13_PTH=cf13
_CF13_TOOL_PTH=cf13/tools
ICF13_PTH=$(DIR_PTH)/$(IDIR)/$(_CF13_PTH)
ICF13_TOOL_PTH=$(DIR_PTH)/$(IDIR)/$(_CF13_TOOL_PTH)
OCF13_PTH=$(DIR_PTH)/$(ODIR)/$(_CF13_PTH)
OCF13_TOOL_PTH=$(DIR_PTH)/$(ODIR)/$(_CF13_TOOL_PTH)
SCF13_PTH=$(DIR_PTH)/$(SDIR)/$(_CF13_PTH)
SCF13_TOOL_PTH=$(DIR_PTH)/$(SDIR)/$(_CF13_TOOL_PTH)

# test examples paths
OTEST_PTH=test/obj
STEST_PTH=test
SFUNC_PTH=test/func


# Libraries
LIB_PTH = -L$(LCL_PTH)/lib -L$(GMP_PTH)/lib -L$(RELIC_PTH)/lib -L$(FLINT_PTH)/lib
LIBS = -lgmp -lrelic_s -lflint 
LFLAGS = $(LIB_PTH) $(LIBS) 

# Dependencies

# Libraries
_GMP_DEPS = gmp.h
_RELIC_DEPS = relic.h
_FLINT_DEPS = fmpz.h fmpz_poly.h fq.h fq_poly.h

GMP_DEPS = $( patsubst %, $(GMP_PTH)$(IDIR)/%, $(_GMP_DEPS) )
RELIC_DEPS = $( patsubst %, $(RELIC_PTH)$(IDIR)/%, $(_RELIC_DEPS) )
FLINT_DEPS = $( patsubst %, $(FLINT_PTH)$(IDIR)/flint/%, $(_FLINT_DEPS) )

# HAL utils
_UTIL_DEPS = error_hdl.h utils.h bn_ext.h types.h prf.h rng.h
UTIL_DEPS = $( patsubst %, $(IUTIL_PTH)/%, $(_UTIL_DEPS) )

# FGP scheme deps
_FGP_DEPS = fgp_authenticate.h fgp_verify.h fgp_keygen.h fgp.h
_FGP_TOOL_DEPS = fgp_keys.h fgp_tag.h fgp_verification_keys.h fgp_tools.h fgp_msg.h fgp_prf.h
FGP_DEPS = $( patsubst %, $(IFGP_PTH)/%, $(_FGP_DEPS) )
FGP_TOOL_DEPS = $( patsubst %, $(IFGP_TOOL_PTH)%, $(_FGP_TOOL_DEPS) )

# CF13 scheme deps
_CF13_DEPS = cf13_authenticate.h cf13_verify.h cf13_keygen.h cf13.h
_CF13_TOOL_DEPS = cf13_keys.h cf13_tag.h cf13_verification_keys.h cf13_tools.h cf13_msg.h cf13_prf.h
CF13_DEPS = $( patsubst %, $(ICF13_PTH)/%, $(_CF13_DEPS) )
CF13_TOOL_DEPS = $( patsubst %, $(ICF13_TOOL_PTH)%, $(_CF13_TOOL_DEPS) )

LIB_DEPS = $(UTIL_DEPS) $(FGP_DEPS) $(FGP_TOOL_DEPS) $(CF13_DEPS) $(CF13_TOOL_DEPS)

DEPS = $(LIB_DEPS) $(GMP_DEPS) $(RELIC_DEPS) $(FLINT_DEPS) 

IFLAGS = -I$(GMP_PTH)/$(IDIR) -I$(FLINT_PTH)/$(IDIR)/flint -I$(RELIC_PTH)/$(IDIR)  -I$(IFGP_TOOL_PTH) -I$(IUTIL_PTH) -I$(IFGP_PTH) -I$(ICF13_TOOL_PTH) -I$(ICF13_PTH)


# Object files

# test examples objects
_OBJ_SAMPLE_EXEC = sample_test.o
_OBJ_SAMPLE2_EXEC = sample_test2.o
_OBJ_FUNC_EXEC = create_functions.o
_OBJ_CFSAMPLE_EXEC = cf_test.o

OBJ_SAMPLE_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_SAMPLE_EXEC))
OBJ_SAMPLE2_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_SAMPLE2_EXEC))
OBJ_FUNC_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_FUNC_EXEC))
OBJ_CFSAMPLE_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_CFSAMPLE_EXEC))

# HAL utils objects
_OBJ_UTIL = error_hdl.o bn_ext.o types.o prf.o rng.o
OBJ_UTIL = $(patsubst %,$(OUTIL_PTH)/%,$(_OBJ_UTIL))

# FGP scheme objects
_OBJ_FGP = fgp_authenticate.o fgp_verify.o fgp_keygen.o fgp.o
_OBJ_FGP_TOOL = fgp_keys.o fgp_tag.o fgp_verification_keys.o fgp_msg.o fgp_prf.o

OBJ_FGP = $(patsubst %,$(OFGP_PTH)/%,$(_OBJ_FGP))
OBJ_FGP_TOOL = $(patsubst %,$(OFGP_TOOL_PTH)/%,$(_OBJ_FGP_TOOL))

# CF13 scheme objects
_OBJ_CF13 = cf13_authenticate.o cf13_verify.o cf13_keygen.o cf13.o
_OBJ_CF13_TOOL = cf13_keys.o cf13_tag.o cf13_verification_keys.o cf13_msg.o cf13_prf.o

OBJ_CF13 = $(patsubst %,$(OCF13_PTH)/%,$(_OBJ_CF13))
OBJ_CF13_TOOL = $(patsubst %,$(OCF13_TOOL_PTH)/%,$(_OBJ_CF13_TOOL))

OBJ_LIB = $(OBJ_UTIL) $(OBJ_FGP) $(OBJ_FGP_TOOL) $(OBJ_CF13_TOOL) $(OBJ_CF13)


all : hal-crypto lib
 
$(OUTIL_PTH)/%.o: $(SUTIL_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OFGP_TOOL_PTH)/%.o: $(SFGP_TOOL_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OFGP_PTH)/%.o: $(SFGP_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OCF13_TOOL_PTH)/%.o: $(SCF13_TOOL_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OCF13_PTH)/%.o: $(SCF13_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTEST_PTH)/sample_test.o: $(STEST_PTH)/sample_test.c
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTEST_PTH)/sample_test2.o: $(STEST_PTH)/sample_test2.c $(SFUNC_PTH)/function.h
	$(CC) -c -o $@ $< $(IFLAGS) -Itest/func $(LFLAGS)

$(OTEST_PTH)/create_functions.o: $(SFUNC_PTH)/create_functions.c
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTEST_PTH)/cf_test.o: $(STEST_PTH)/cf_test.c
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(SFUNC_PTH)/function.o: $(SFUNC_PTH)/function.c $(SFUNC_PTH)/function.h
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

test: $(OBJ_SAMPLE_EXEC) $(OBJ_FUNC_EXEC)
	$(CC) -o sampleTest $(OBJ_SAMPLE_EXEC) -lhal-crypto -L/lib $(IFLAGS) $(LFLAGS) 
	mv sampleTest test
	$(CC) -o createFunction $(OBJ_FUNC_EXEC) -lhal-crypto -L/lib $(IFLAGS) $(LFLAGS)
	mv createFunction test

cftest: $(OBJ_CFSAMPLE_EXEC)
	$(CC) -o cfTest $(OBJ_CFSAMPLE_EXEC) -lhal-crypto -L/lib $(IFLAGS) $(LFLAGS)
	mv cfTest test

test2: $(OBJ_SAMPLE2_EXEC) $(SFUNC_PTH)/function.o
	$(CC) -o sampleTest2 $(OBJ_SAMPLE2_EXEC) $(SFUNC_PTH)/function.o -lhal-crypto -L/lib $(IFLAGS) -Itest/func $(LFLAGS) 
	mv sampleTest2 test

hal-crypto: $(LIB_DEPS) $(OBJ_LIB)

lib: $(LIB_DEPS) $(OBJ_LIB)
	rm -f lib/libhal-crypto.a
	ar -cvq lib/libhal-crypto.a $(OBJ_LIB)

install:
	mkdir -p $(PREFIX)/include/hal-crypto
	mkdir -p $(PREFIX)/lib
	rm -f $(PREFIX)/include/hal-crypto/*.h
	rm -f $(PREFIX)/lib/libhal-crypto.a
	cp -v include/utils/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/fgp/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/fgp/tools/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/cf13/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/cf13/tools/*.h $(PREFIX)/include/hal-crypto/
	cp -v lib/libhal-crypto.a $(PREFIX)/lib/

.PHONY: clean

clean:
	rm -f $(OTEST_PTH)/*.o $(OUTIL_PTH)/*.o $(OFGP_PTH)/*.o $(OFGP_TOOL_PTH)/*.o $(OCF13_PTH)/*.o $(OCF13_TOOL_PTH)/*.o
	rm -f test/sampleTest test/sampleTest2 test/createFunction test/cfTest
	rm -f lib/libhal-crypto.a

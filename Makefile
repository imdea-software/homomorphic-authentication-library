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

_UTIL_PTH=utils
_TEST_PTH=test
_HMC_PTH=fgp
_TOOL_PTH=fgp/tools

OTEST_PTH=test/obj
STEST_PTH=test
SFUNC_PTH=test/func

IUTIL_PTH=$(DIR_PTH)/$(IDIR)/$(_UTIL_PTH)
IHMC_PTH=$(DIR_PTH)/$(IDIR)/$(_HMC_PTH)
ITOOL_PTH=$(DIR_PTH)/$(IDIR)/$(_TOOL_PTH)

OUTIL_PTH=$(DIR_PTH)/$(ODIR)/$(_UTIL_PTH)
OHMC_PTH=$(DIR_PTH)/$(ODIR)/$(_HMC_PTH)
OTOOL_PTH=$(DIR_PTH)/$(ODIR)/$(_TOOL_PTH)

SUTIL_PTH=$(DIR_PTH)/$(SDIR)/$(_UTIL_PTH)
SHMC_PTH=$(DIR_PTH)/$(SDIR)/$(_HMC_PTH)
STOOL_PTH=$(DIR_PTH)/$(SDIR)/$(_TOOL_PTH)

LIB_PTH = -L$(LCL_PTH)/lib -L$(GMP_PTH)/lib -L$(RELIC_PTH)/lib -L$(FLINT_PTH)/lib
LIBS = -lgmp -lrelic_s -lflint 
LFLAGS = $(LIB_PTH) $(LIBS) 

_GMP_DEPS = gmp.h
_RELIC_DEPS = relic.h
_FLINT_DEPS = fmpz.h fmpz_poly.h fq.h fq_poly.h

_HMC_DEPS = fgp_authenticate.h fgp_verify.h fgp_keygen.h fgp.h
_UTIL_DEPS = error_hdl.h utils.h bn_ext.h types.h prf.h
_TOOL_DEPS = fgp_keys.h fgp_tag.h fgp_verification_keys.h fgp_tools.h fgp_msg.h fgp_prf.h

GMP_DEPS = $( patsubst %, $(GMP_PTH)$(IDIR)/%, $(_GMP_DEPS) )
RELIC_DEPS = $( patsubst %, $(RELIC_PTH)$(IDIR)/%, $(_RELIC_DEPS) )
FLINT_DEPS = $( patsubst %, $(FLINT_PTH)$(IDIR)/flint/%, $(_FLINT_DEPS) )

HMC_DEPS = $( patsubst %, $(IHMC_PTH)/%, $(_HMC_DEPS) )
UTIL_DEPS = $( patsubst %, $(IUTIL_PTH)/%, $(_UTIL_DEPS) )
TOOL_DEPS = $( patsubst %, $(ITOOL_PTH)%, $(_TOOL_DEPS) )

LIB_DEPS = $(HMC_DEPS) $(UTIL_DEPS) $(TOOL_DEPS) 

DEPS = $(LIB_DEPS) $(GMP_DEPS) $(RELIC_DEPS) $(FLINT_DEPS) 

IFLAGS = -I$(GMP_PTH)/$(IDIR) -I$(FLINT_PTH)/$(IDIR)/flint -I$(RELIC_PTH)/$(IDIR)  -I$(ITOOL_PTH) -I$(IUTIL_PTH) -I$(IHMC_PTH)

_OBJ_SAMPLE_EXEC = sample_test.o
_OBJ_SAMPLE2_EXEC = sample_test2.o
_OBJ_FUNC_EXEC = create_functions.o

_OBJ_UTIL = error_hdl.o bn_ext.o types.o prf.o
_OBJ_HMC = fgp_authenticate.o fgp_verify.o fgp_keygen.o fgp.o 
_OBJ_TOOL = fgp_keys.o fgp_tag.o fgp_verification_keys.o fgp_msg.o fgp_prf.o

OBJ_SAMPLE_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_SAMPLE_EXEC))
OBJ_SAMPLE2_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_SAMPLE2_EXEC))
OBJ_FUNC_EXEC = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_FUNC_EXEC))

OBJ_UTIL = $(patsubst %,$(OUTIL_PTH)/%,$(_OBJ_UTIL))
OBJ_HMC = $(patsubst %,$(OHMC_PTH)/%,$(_OBJ_HMC))
OBJ_TOOL = $(patsubst %,$(OTOOL_PTH)/%,$(_OBJ_TOOL))

OBJ_LIB = $(OBJ_UTIL) $(OBJ_TOOL) $(OBJ_HMC) 

all : hal-crypto lib
 
$(OUTIL_PTH)/%.o: $(SUTIL_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTOOL_PTH)/%.o: $(STOOL_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OHMC_PTH)/%.o: $(SHMC_PTH)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTEST_PTH)/sample_test.o: $(STEST_PTH)/sample_test.c 
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OTEST_PTH)/sample_test2.o: $(STEST_PTH)/sample_test2.c $(SFUNC_PTH)/function.h
	$(CC) -c -o $@ $< $(IFLAGS) -Itest/func $(LFLAGS)

$(OTEST_PTH)/create_functions.o: $(SFUNC_PTH)/create_functions.c
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(SFUNC_PTH)/function.o: $(SFUNC_PTH)/function.c $(SFUNC_PTH)/function.h
	$(CC) -c -o $@ $< $(IFLAGS) $(LFLAGS)

test: $(OBJ_SAMPLE_EXEC) $(OBJ_FUNC_EXEC)
	$(CC) -o sampleTest $(OBJ_SAMPLE_EXEC) -lhal-crypto -L/lib $(IFLAGS) $(LFLAGS) 
	mv sampleTest test
	$(CC) -o createFunction $(OBJ_FUNC_EXEC) -lhal-crypto -L/lib $(IFLAGS) $(LFLAGS)
	mv createFunction test

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
	cp -v include/fgp/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/utils/*.h $(PREFIX)/include/hal-crypto/
	cp -v include/fgp/tools/*.h $(PREFIX)/include/hal-crypto/
	cp -v lib/libhal-crypto.a $(PREFIX)/lib/

.PHONY: clean

clean:
	rm -f $(OTEST_PTH)/*.o $(OHMC_PTH)/*.o $(OTOOL_PTH)/*.o $(OUTIL_PTH)/*.o
	rm -f test/sampleTest test/sampleTest2 test/createFunction

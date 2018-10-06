# Jarosław Rymut
SHELL := /bin/sh

# pliki
# HELPERS nie tworzą plików wykonywalnych
HELPERS :=
FILES := $(filter-out $(HELPERS),$(wildcard *.cpp))

####################
# zależności
stack.o: stack.hpp

# wszystko potrzebuje biblioteki
# $(FILES:.cpp=.x): private LDLIBS += -L. -l#####
# wszystko zależy od nagłówka i biblioteki
# $(FILES:.cpp=.o): #####.hpp | lib####.so

####################
# testy
N := 300
TESTS := stack
# zależność od generatora
test.stdstack.txt test.stack.txt: test.genList.txt

####################
# numer zestawu
zestaw := $(notdir $(lastword $(CURDIR)))
# nazwa spakowanego pliku
tar := Rymut_$(zestaw).tar.gz

####################
# flagi
CFLAGS := -O3 -Wall -Wextra
# wiatmy w 2012
# -Wpedantic dostępne od gcc 4.8
ifeq ($(shell $(CC) -dumpversion),4.7)
	CFLAGS += -pedantic
else
	CFLAGS += -Wpedantic
endif
# PIC > PIE > no-pie
CFLAGS += -fPIC
LDFLAGS += -pie
# ścieżka do bibliotek
LDLIBS += -Wl,-R.
# link time optimization
CFLAGS += -flto
# standardy
CXXFLAGS := -std=c++11 $(CFLAGS)
CFLAGS += -std=c11

####################
# private jest dostępne od GNU make 3.82
ifneq (3.82,$(firstword $(sort $(MAKE_VERSION) 3.82)))
  $(error "*** PLEASE USE AT LEAST GNU MAKE 3.82 FROM 2010 ***")
endif

####################
# wszystko zależy od Makefile
$(FILES:.cpp=.o): Makefile
$(HELPERS:.cpp=.o): Makefile

# make może automatycznie usunąć te pliki
.INTERMEDIATE: $(HELPERS:.cpp=.o) $(FILES:.cpp=.o)

####################
# domyślne cele
.DEFAULT_GOAL := all
.PHONY: all
all: $(FILES:.cpp=.x)

.PHONY: clean
clean: mostlyclean
	$(RM) $(FILES:.cpp=.x) $(tar) lib*

.PHONY: mostlyclean
mostlyclean:
	$(RM) test.*.txt gen*.x

.PHONY: tar
tar:
# poprawka na błąd 'file changed as we read it'
	[ -f $(tar) ] || touch $(tar)
	tar -caf ./$(tar) --exclude=*.o --exclude=*.x --exclude=*.tar.* --exclude=test.*.txt -C .. $(zestaw)

# kopia wbudowanej reguły, z nowym rozszerzeniem
%.x: %.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

lib%.so: private LDFLAGS += -shared
lib%.so: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

####################
# inne cele
.PHONY: check
check:
	@clang-tidy \
		-checks='bugprone-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-parameters,-readability-braces-around-statements,-readability-named-parameter' \
		-config="{CheckOptions: [ \
			{ key: readability-identifier-naming.NamespaceCase, value: CamelCase }, \
			{ key: readability-identifier-naming.ClassCase, value: CamelCase  }, \
			{ key: readability-identifier-naming.StructCase, value: CamelCase  }, \
			{ key: readability-identifier-naming.FunctionCase, value: lower_case }, \
			{ key: readability-identifier-naming.VariableCase, value: lower_case }, \
			{ key: readability-identifier-naming.GlobalConstantCase, value: UPPER_CASE } \
		]}" \
		$(FILES) $(HELPERS) \
		-- $(filter-out -flto,$(CXXFLAGS))
# lto does something bad to clang-tidy

.PHONY: format
format:
	@clang-format -i -style=file $(FILES) $(HELPERS)

####################
# testy
gen%.x: gen%.cxx
	g++ -std=c++11 -O2 -o $@ $^

test.gen%.txt: gen%.x
	./$(filter %.x,$^) $(N) >$@

test.%.txt: %.x
	./$(filter %.x,$^) <$(filter test.gen%,$^) >$@

TESTS_TARGETS := $(addprefix test_,$(TESTS))
.PHONY: $(TESTS_TARGETS)
$(TESTS_TARGETS): test_%: test.%.txt test.std%.txt
	@diff -sq $^

.PHONY: test
test: $(TESTS_TARGETS)

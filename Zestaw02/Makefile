# Copyright (C) 2018 Jarosław Rymut
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>
SHELL := /bin/sh

# pliki
# HELPERS nie tworzą plików wykonywalnych
HELPERS :=
FILES := $(filter-out $(HELPERS),$(wildcard *.cpp))

####################
# zależności
ArrayList.o: ArrayList.hpp
LinkedList.o: LinkedList.hpp
CursorList.o: CursorList.hpp

# wszystko potrzebuje biblioteki
# $(FILES:.cpp=.x): private LDLIBS += -L. -l#####
# wszystko zależy od nagłówka i biblioteki
# $(FILES:.cpp=.o): #####.hpp | lib####.so

####################
# testy
N := 350
# zależność od generatora
test.stdList.txt: test.genList.txt
test.ArrayList.txt: test.genList.txt
test.LinkedList.txt: test.genList.txt
test.stdVector.txt: test.genList.txt
# plik do porównania
test_stdList: test.stdVector.txt
test: test_stdList

test_ArrayList: test.stdList.txt
test: test_ArrayList

test_LinkedList: test.stdList.txt
test: test_LinkedList

test_stdVector: test.ArrayList.txt
test: test_stdVector

####################
# numer zestawu
zestaw := $(notdir $(lastword $(CURDIR)))
# nazwa spakowanego pliku
tar := Rymut_$(zestaw).tar.gz

####################
# flagi
CFLAGS := -O3 -pipe -Wall -Wextra -Weffc++
# wiatmy w 2012
# -Wpedantic dostępne od gcc 4.8
ifeq ($(shell $(CC) -dumpversion),4.7)
	CFLAGS += -pedantic
else
	CFLAGS += -Wpedantic
endif
# PIC > PIE > no-pie
CFLAGS += -fPIC
# and for linker:
LDFLAGS += -pie
# ścieżka do bibliotek
LDLIBS += -Wl,-R.
# link time optimization
CFLAGS += -flto
# standardy
CXXFLAGS := -std=c++14 $(CFLAGS)
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
	$(RM) test.*.txt gen*.x rapidcheck.x

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

# usuń
%: %.o
%: %.cpp
.SUFFIXES:
.SUFFIXES: .o .cpp

####################
# inne cele
.PHONY: check
check:
	@clang-tidy \
		-checks="bugprone-*,\
	cppcoreguidelines-*,\
	clang-analyzer-*,\
	misc-*,\
	modernize-*,\
	performance-*,\
	portability-*,\
	readability-*,\
	-misc-unused-parameters,\
	-readability-braces-around-statements,\
	-readability-named-parameter,\
	-readability-implicit-bool-conversion,\
	-cppcoreguidelines-no-malloc,\
	-cppcoreguidelines-owning-memory,\
	-cppcoreguidelines-pro-bounds-*"\
		-header-filter='.*,-rapidcheck.hpp' \
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
.PHONY: test
test:
	@echo "[32mAll tests are successfull![0m"

gen%.x: gen%.cxx
	g++ -std=c++11 -O2 -o $@ $^

.PHONY: FORCE
FORCE:
test.gen%.txt: gen%.x FORCE
	./$(filter %.x,$^) $(N) >$@

test.%.txt: %.x
	time ./$(filter %.x,$^) <$(filter test.gen%,$^) >$@

test_%: test.%.txt
	@diff -sq $^

####################
# rapidcheck
ifneq (,$(wildcard ../rapidcheck/CMakeLists.txt))

.PHONY: rapidcheck
rapidcheck: rapidcheck.x
	./rapidcheck.x

check: CXXFLAGS += -I../rapidcheck/include
rapidcheck.x: CXXFLAGS += -I../rapidcheck/include
rapidcheck.x: ../rapidcheck/build/librapidcheck.a
rapidcheck.o: ArrayList.hpp
test: rapidcheck

../rapidcheck/build/librapidcheck.a: | ../rapidcheck/build/
	cd ../rapidcheck/build && cmake ..
	$(MAKE) --directory=../rapidcheck/build

../rapidcheck/build/:
	cd ../rapidcheck && mkdir build

endif

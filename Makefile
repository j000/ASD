# Copyright (C) 2018 Jarosław Rymut
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>
SHELL := /bin/sh

# pliki
# HELPERS nie tworzą plików wykonywalnych
CHECKS := $(wildcard *check.cpp)
TESTS := $(wildcard *test.cpp)
HELPERS := $(CHECKS) $(TESTS)
FILES := $(filter-out $(HELPERS),$(wildcard *.cpp))

####################
ifneq (,$(wildcard zestaw.mk))
include zestaw.mk
endif

####################
# numer zestawu
zestaw := $(notdir $(lastword $(CURDIR)))
# nazwa spakowanego pliku
tar := Rymut_$(zestaw).tar.gz

####################
# flagi
CFLAGS := -O3 -pipe -Wall -Wextra -Weffc++ -Wconversion
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

CPPFLAGS := -MMD -MP

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
	$(RM) test.*.txt gen*.x $(HELPERS:.cpp=.x) $(FILES:.cpp=.d) $(HELPERS:.cpp=.d)

.PHONY: tar
tar:
# poprawka na błąd 'file changed as we read it'
	@[ -f $(tar) ] || touch $(tar)
	tar -caf ./$(tar) \
		--dereference \
		--exclude=*.d \
		--exclude=*.o \
		--exclude=*.x \
		--exclude=*.tar.* \
		--exclude=test.*.txt \
		-C .. \
		$(zestaw)

# kopia wbudowanej reguły, z nowym rozszerzeniem
%.x: %.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

lib%.so: private LDFLAGS += -shared
lib%.so: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -M $<

ifneq (,$(wildcard $(FILES:.cpp=.d)))
include $(FILES:.cpp=.d)
endif

# usuń domyślne reguły
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
	-clang-analyzer-cplusplus.NewDeleteLeaks,\
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
		$(FILES) \
		-- $(filter-out -flto,$(CXXFLAGS))
# lto does something bad to clang-tidy

.PHONY: format
format:
	@clang-format -i -style=file $(FILES) $(HELPERS)

####################
# testy
N := 350

.PHONY: test
test:
	@printf "[32mAll tests are successfull![0m\n"

gen%.x: gen%.cxx
	g++ -std=c++11 -O2 -o $@ $^

.PHONY: FORCE
FORCE:
test.gen%.txt: gen%.x FORCE
	./$(filter %.x,$^) $(N) >$@

test.%.txt: %.x
	/usr/bin/time ./$(filter %.x,$^) <$(filter test.gen%,$^) >$@

test_%: test.%.txt
	@diff -sq $^

%test: %test.x
	./$^

test: $(subst .cpp,,$(TESTS))

####################
# rapidcheck
ifneq (,$(wildcard ../rapidcheck/CMakeLists.txt))
  RAPIDCHECK := ../rapidcheck
endif
ifneq (,$(wildcard ./rapidcheck/CMakeLists.txt))
  RAPIDCHECK := ./rapidcheck
endif
ifneq (,$(RAPIDCHECK))

RC_PARAMS:="max_success=10000"

# .PHONY: rapidcheck
%check: %check.x
	RC_PARAMS=${RC_PARAMS} ./$^

check: CXXFLAGS += -isystem $(RAPIDCHECK)/include
%check.x: CXXFLAGS += -isystem $(RAPIDCHECK)/include
%check.x: LOADLIBES += $(RAPIDCHECK)/build/librapidcheck.a
test: $(subst .cpp,,$(CHECKS))

$(RAPIDCHECK)/build/librapidcheck.a: | $(RAPIDCHECK)/build/
	cd $(RAPIDCHECK)/build && cmake ..
	$(MAKE) --directory=$(RAPIDCHECK)/build

$(RAPIDCHECK)/build/:
	cd $(RAPIDCHECK) && mkdir build

endif

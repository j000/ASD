# Copyright (C) 2018 Jarosław Rymut
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>

# zależności
# wszystko potrzebuje biblioteki
# $(FILES:.cpp=.x): private LDLIBS += -L. -l#####
# wszystko zależy od nagłówka i biblioteki
# $(FILES:.cpp=.o): #####.hpp | lib####.so

test.gen%.txt: gen%.x FORCE
	./gen$*.x $(N) >$@

test.%.160000.txt: %.x
	./$*.x 160000 >$@

test.sort.txt: test.genRandom.txt
test.sort.txt:
	sort -n < $^ > $@

####################
# zależność od generatora
test.heapSortIter.txt: test.genRandom.txt
test.heapSortRec.txt: test.genRandom.txt
test.mergeSort.txt: test.genRandom.txt

####################
# plik do porównania
test_heapSortIter: test.sort.txt
test: test_heapSortIter

test_heapSortRec: test.sort.txt
test: test_heapSortRec

test_mergeSort: test.sort.txt
test: test_mergeSort

####################
result.%.txt: benchmark.x test.%.160000.txt
	./$(filter %.x,$^) < $(filter test.%,$^) | tee $@

plot.%.png: gnu.plot result.%.txt
	gnuplot -e 'set output "$@"' -e 'filename="$(filter result.%.txt,$^)"' gnu.plot

mostlyclean: clean_plot
.PHONY: clean_plot
clean_plot:
	$(RM) plot.*.png result.*.txt

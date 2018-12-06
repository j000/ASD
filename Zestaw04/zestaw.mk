# Copyright (C) 2018 Jarosław Rymut
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>

# zależności
# wszystko potrzebuje biblioteki
# $(FILES:.cpp=.x): private LDLIBS += -L. -l#####
# wszystko zależy od nagłówka i biblioteki
# $(FILES:.cpp=.o): #####.hpp | lib####.so

.INTERMEDIATE: genRandom.x
test.genRandom.txt: genRandom.x FORCE
	./genRandom.x $(N) $(N)00 >$@

test.sort.txt: test.genRandom.txt
test.sort.txt:
	sort -n < $^ > $@

####################
# zależność od generatora
test.stdSort.txt: test.genRandom.txt
test.bubbleSort.txt: test.genRandom.txt
test.insertionSort.txt: test.genRandom.txt
test.countingSort.txt: test.genRandom.txt
test.selectionSort.txt: test.genRandom.txt
test.radixSort.txt: test.genRandom.txt

####################
# plik do porównania
test_stdSort: test.sort.txt
test: test_stdSort

test_bubbleSort: test.sort.txt
test: test_bubbleSort

test_insertionSort: test.sort.txt
test: test_insertionSort

test_countingSort: test.sort.txt
test: test_countingSort

test_selectionSort: test.sort.txt
test: test_selectionSort

test_radixSort: test.sort.txt
test: test_radixSort

####################
result.txt: benchmark.x
	./$^ | tee $@

plot.png: gnu.plot result.txt
	gnuplot -e 'set output "$@"' -e 'filename="result.txt"' gnu.plot

mostlyclean: clean_plot
.PHONY: clean_plot
clean_plot:
	$(RM) plot.png result.txt

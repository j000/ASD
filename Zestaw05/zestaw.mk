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
test.heapSortIter.txt: test.genRandom.txt
test.heapSortRec.txt: test.genRandom.txt

####################
# plik do porównania
test_heapSortIter: test.sort.txt
test: test_heapSortIter

test_heapSortRec: test.sort.txt
test: test_heapSortRec

####################
result.txt: benchmark.x
	./$^ | tee $@

plot.png: gnu.plot result.txt
	gnuplot -e 'set output "$@"' gnu.plot

mostlyclean: clean_plot
.PHONY: clean_plot
clean_plot:
	$(RM) plot.png result.txt

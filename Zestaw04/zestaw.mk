
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

####################
# zależność od generatora
test.stdSort.txt: test.genRandom.txt

####################
# plik do porównania

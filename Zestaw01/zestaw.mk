# Copyright (C) 2018 Jarosław Rymut
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>

# zależności
# wszystko potrzebuje biblioteki
# $(FILES:.cpp=.x): private LDLIBS += -L. -l#####
# wszystko zależy od nagłówka i biblioteki
# $(FILES:.cpp=.o): #####.hpp | lib####.so

####################
# zależność od generatora
test.stack.txt: test.genList.txt
test.stdstack.txt: test.genList.txt
test.queue.txt: test.genList.txt
test.stdqueue.txt: test.genList.txt

####################
# plik do porównania
test_stack: test.stdstack.txt
test: test_stack

test_queue: test.stdqueue.txt
test: test_queue

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
test.stdList.txt: test.genList.txt
test.ArrayList.txt: test.genList.txt
test.LinkedList.txt: test.genList.txt
test.CursorList.txt: test.genList.txt
test.stdVector.txt: test.genList.txt
test.myVector.txt: test.genList.txt

####################
# plik do porównania
test_stdList: test.stdVector.txt
test: test_stdList

test_ArrayList: test.stdList.txt
test: test_ArrayList

test_LinkedList: test.stdList.txt
test: test_LinkedList

test_CursorList: test.stdList.txt
test: test_CursorList

test_stdVector: test.ArrayList.txt
test: test_stdVector

test_myVector: test.stdVector.txt
test: test_myVector

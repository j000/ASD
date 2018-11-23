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
test.mergeStdVector.txt: test.genList.txt
test.uniqueStdVector.txt: test.genList.txt
test.mergeStdList.txt: test.genList.txt
test.uniqueStdList.txt: test.genList.txt
test.mergeArrayList.txt: test.genList.txt
test.uniqueArrayList.txt: test.genList.txt
test.mergeLinkedList.txt: test.genList.txt

####################
# plik do porównania
test_mergeStdVector: test.mergeStdList.txt
test: test_mergeStdVector

test_uniqueStdVector: test.uniqueStdList.txt
test: test_uniqueStdVector

test_mergeArrayList: test.mergeStdList.txt
test: test_mergeArrayList

test_uniqueArrayList: test.uniqueStdList.txt
test: test_uniqueArrayList

test_mergeLinkedList: test.mergeStdList.txt
test: test_mergeLinkedList

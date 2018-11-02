// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#include "LinkedList.hpp"

#include <ios>
#include <iostream>

int main(int, char**)
{
	using ::std::cin;
	using ::std::cout;
	using ::std::endl;

	::std::ios_base::sync_with_stdio(false);

	LinkedList<int> list;

	unsigned int n;
	cin >> n;
	while (n > 0) {
		char command;
		cin >> command;
		switch (command) {
		case 'F': {
			int number;
			cin >> number;
			list.push_front(number);
			break;
		}
		case 'B': {
			int number;
			cin >> number;
			list.push_back(number);
			break;
		}
		case 'f': {
			if (list.empty()) {
				cout << "EMPTY" << endl;
			} else {
				cout << list.pop_front() << endl;
			}
			break;
		}
		case 'b': {
			if (list.empty()) {
				cout << "EMPTY" << endl;
			} else {
				cout << list.pop_back() << endl;
			}
			break;
		}
		case 'C': {
			int number;
			cin >> number;
			if (list.find(number)) {
				cout << "TRUE" << endl;
			} else {
				cout << "FALSE" << endl;
			}
			break;
		}
		case 'S': {
			cout << list.size() << endl;
			break;
		}
		}
		--n;
	}
}

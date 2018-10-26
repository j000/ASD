// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#include <algorithm>
#include <ios>
#include <iostream>
#include <vector>

int main(int, char**)
{
	using ::std::cin;
	using ::std::cout;
	using ::std::endl;

	::std::ios_base::sync_with_stdio(false);

	::std::vector<int> list;
	list.reserve(8);

	int n;
	cin >> n;
	while (n > 0) {
		char command;
		cin >> command;
		switch (command) {
		case 'F': {
			int number;
			cin >> number;
			// list.push_front(number);
			list.insert(list.begin(), number);
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
				cout << list.front() << endl;
				// list.pop_front();
				list.erase(list.begin());
			}
			break;
		}
		case 'b': {
			if (list.empty()) {
				cout << "EMPTY" << endl;
			} else {
				cout << list.back() << endl;
				list.pop_back();
			}
			break;
		}
		case 'C': {
			int number;
			cin >> number;
			if (::std::find(list.begin(), list.end(), number) != list.end()) {
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

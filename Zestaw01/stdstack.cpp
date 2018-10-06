// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#include <ios>
#include <iostream>
#include <stack>

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	std::stack<int> stack;

	int n;
	std::cin >> n;
	while (n > 0) {
		char command;
		std::cin >> command;
		switch (command) {
		case 'A': {
			int number;
			std::cin >> number;
			stack.push(number);
			break;
		}
		case 'D': {
			if (stack.empty())
				std::cout << "EMPTY" << std::endl;
			else {
				std::cout << stack.top() << std::endl;
				stack.pop();
			}
			break;
		}
		case 'S': {
			std::cout << stack.size() << std::endl;
			break;
		}
		}
		--n;
	}
}

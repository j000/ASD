// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#include <ios>
#include <iostream>
#include <random>
#include <stack>

char closing(char c)
{
	switch (c) {
	case '[':
		return ']';
	case '(':
	default:
		return ')';
	}
}

int fail()
{
	std::cout << "FAIL" << std::endl;
	return 1;
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	std::stack<char> stack;

	char tmp;

	while (std::cin >> tmp) {
		switch (tmp) {
		case '[':
		case '(': {
			stack.push(closing(tmp));
			break;
		}
		case ']':
		case ')': {
			if (stack.empty() || tmp != stack.top())
				return fail();
			stack.pop();
			break;
		}
		default:
			return fail();
		}
	}

	if (!stack.empty())
		return fail();

	std::cout << "OK" << std::endl;
	return 0;
}

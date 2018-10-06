#include <ios>
#include <iostream>
#include <queue>

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	std::queue<int> queue;

	int n;
	std::cin >> n;
	while (n > 0) {
		char command;
		std::cin >> command;
		switch (command) {
		case 'A': {
			int number;
			std::cin >> number;
			queue.push(number);
			break;
		}
		case 'D': {
			if (queue.empty())
				std::cout << "EMPTY" << std::endl;
			else {
				std::cout << queue.front() << std::endl;
				queue.pop();
			}
			break;
		}
		case 'S': {
			std::cout << queue.size() << std::endl;
			break;
		}
		}
		--n;
	}
}

// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#include <iomanip>
#include <ios>
#include <iostream>
#include <random>
#include <stack>
#include <stdexcept>
#include <vector>

using std::size_t;

void usage(char* name)
{
	using std::cout;
	using std::endl;
	cout << "Usage: " << name << " pairs [errors]" << endl;
	cout << "\t" << std::left << std::setw(12) << "pairs"
		 << "- how many pairs of brackets to create" << endl
		 << "\t" << std::left << std::setw(12) << "errors"
		 << "- how many errors to introduce" << endl;
}

double get_random()
{
	thread_local static std::mt19937 generator {std::random_device {}()};
	thread_local static std::uniform_real_distribution<> distribution {0., 1.};
	return distribution(generator);
}

class ErrorGenerator {
public:
	bool check()
	{
		m_current += m_probability;
		if (m_max_errors && get_random() < m_current) {
			m_current = 0;
			--m_max_errors;
			return true;
		}
		return false;
	}
	ErrorGenerator(double probability, unsigned max_errors = 1)
		: m_probability(probability), m_max_errors(max_errors)
	{
	}

private:
	double m_probability {0.1};
	double m_current {0};
	unsigned m_max_errors {1};
};

class ExpressionTree {
	enum class Bracket : char { Round, Square };

	class ExpressionTreeNode {
		friend class ExpressionTree;

	public:
		ExpressionTreeNode() noexcept
		{
			if (get_random() >= 0.5)
				m_bracket = Bracket::Square;
		}

		explicit ExpressionTreeNode(Bracket bracket) noexcept
		{
			m_bracket = bracket;
		}

		// the rule of five...
		~ExpressionTreeNode() = default;
		ExpressionTreeNode(const ExpressionTreeNode&) = default;
		ExpressionTreeNode& operator=(const ExpressionTreeNode&) = default;
		ExpressionTreeNode(ExpressionTreeNode&&) = default;
		ExpressionTreeNode& operator=(ExpressionTreeNode&&) = default;

		char open()
		{
			switch (m_bracket) {
			case Bracket::Square:
				return '[';
			case Bracket::Round:
			default: // just in case...
				return '(';
			}
		}

		char close()
		{
			switch (m_bracket) {
			case Bracket::Square:
				return ']';
			case Bracket::Round:
			default: // just in case...
				return ')';
			}
		}

	private:
		enum Bracket m_bracket { Bracket::Round };
		size_t m_children {};
		size_t m_sibling {};
	};

public:
	ExpressionTree(size_t pairs = 1, unsigned errors = 0) noexcept
		: m_error {0.5 * errors / (pairs - 1), errors}
	{
		if (pairs == 0)
			pairs = std::floor(get_random() * 10000);

		for (decltype(pairs) i = 0; i < pairs; ++i) {
			m_tree.emplace_back();
			set_parent(i, std::floor(get_random() * i));
		}
	}

	// the rule of five...
	ExpressionTree() = delete;
	~ExpressionTree() = default;
	ExpressionTree(const ExpressionTree&) = default;
	ExpressionTree& operator=(const ExpressionTree&) = default;
	ExpressionTree(ExpressionTree&&) = default;
	ExpressionTree& operator=(ExpressionTree&&) = default;

	ExpressionTreeNode* root() { return &m_tree[0]; }

	auto size() { return m_tree.size(); }

	friend std::ostream& operator<<(std::ostream& out, ExpressionTree& tree)
	{
		out << tree.to_string();
		return out;
	}

	std::string to_string()
	{
		// FIXME change to stack-based implementation
		std::string output {};
		output += m_tree[0].open();
		output += print_helper(m_tree[0].m_children);
		output += m_tree[0].close();
		return output;
	}

	void generate(size_t pairs)
	{
		for (decltype(pairs) i = 0; i < pairs; ++i) {
			m_tree.emplace_back(Bracket::Square);
			set_parent(i, std::floor(get_random() * i));
		}
	}

private:
	std::vector<ExpressionTreeNode> m_tree {};
	ErrorGenerator m_error;

	std::string print_helper(size_t node)
	{
		if (!node)
			return std::string {};

		std::string out {};
		if (!m_error.check())
			out += m_tree[node].open();
		out += print_helper(m_tree[node].m_children);
		// if (!m_error.check())
			out += m_tree[node].close();
		return out += print_helper(m_tree[node].m_sibling);
	}

	void set_parent(size_t children, size_t parent)
	{
		if (!m_tree[parent].m_children) {
			m_tree[parent].m_children = children;
			return;
		}

		parent = m_tree[parent].m_children;
		while (m_tree[parent].m_sibling)
			parent = m_tree[parent].m_sibling;

		m_tree[parent].m_sibling = children;
	}
};

int main(int argc, char** argv)
{
	std::ios_base::sync_with_stdio(false);
	if (argc < 2 || argc > 3) {
		usage(argv[0]);
		exit(1);
	}

	unsigned pairs = std::stoul(argv[1]);
	unsigned errors = 0;

	if (argc == 3)
		errors = std::stoul(argv[2]);

	ExpressionTree tree {pairs, errors};
	std::cout << tree << std::endl;
}

//Напишете функция, 
//която приема указател към корена на двоично дърво и 
//връща указател към най - голямото му(с най - много елементи) 
//двоично наредено поддърво.
//По - долу са дадени прототипът на функцията и представянето на възлите на дървото.

#include <vector>
#include <algorithm>
#include <utility>

struct Node {
	int value;

	Node* left;
	Node* right;
};

bool is_leaf(Node* node) {
	return !node;
}

//броя елементи в дървото
int count(Node* node) {
	return is_leaf(node) ? 0 : 1 + count(node->left) + count(node->right);
}

//[min, max] е интервалът на допустимите елементи в поддървото,
//този интервал се определя от елементите в пътя до поддървото.
//Използваме го, за да проверим дали е изпълнена инвариантата:
//Всеки елемент е по-малък или равен на всички елементи в дясното си поддърво и
//по-голям или равен на всички елементи в лявото си поддърво
//INT_max == std::numeric_limits<int>::min()
bool is_bst(Node* node, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) {

	/*if (!is_leaf(node)) {
	return false;
	}

	if (node->value < min) {
	return false;
	}

	if (node->value > max) {
	return false;
	}

	if (!is_bst(node->left, min, node->value)) {
	return false;
	}

	if (!is_bst(node->right, node->value, max)) {
	return false;
	}

	return true;*/

	return is_leaf(node) ||
		(node->value >= min &&
			node->value <= max &&
			is_bst(node->left, min, node->value) &&
			is_bst(node->right, node->value, max));
}

//O(N^2)
Node* get_largest_bst_subtree(Node* root) {
	if (is_bst(root)) {
		return root;
	}
	Node* left = get_largest_bst_subtree(root->left);
	Node* right = get_largest_bst_subtree(root->right);

	return count(left) > count(right) ? left : right;
}

//struct Pair {
//	Node* first;
//	int second;
//};
//В first пазим указател към корена на поддървото
//В second пазим броя елементи в това поддърво
std::pair<Node*, int> get_largest_bst_subtree_help(Node* root) {
	if (is_leaf(root)) {
		return std::make_pair(root, 0);
	}
	std::pair<Node*, int> left = get_largest_bst_subtree_help(root->left);
	std::pair<Node*, int> right = get_largest_bst_subtree_help(root->right);

	/*
		за да можем да подобрим отговора,
		трябва най-голямото наредено дърво в лятото поддърво
		да има за корен лявото дете на текущия елемент.
		аналогично за дясното дете.

		остава да проверим, дали можем да "закачим" текущия елемент,
		към тези две дървета.
		За да ги обединим, трябва да са изпълнени условията,
		корена на лявото поддърво да е по-малък или равен на текущия елемент,
		корена на дясното поддърво да е по-голям или равен на текущия елемент.
	*/

	if (left.first == root->left &&
		right.first == root->right &&
		(is_leaf(root->left) || root->left->value <= root->value) &&
		(is_leaf(root->right) || root->right->value >= root->value)) {
		return std::make_pair(root, left.second + right.second + 1);
	}
	return left.second < right.second ? right : left;
}

//O(N)
Node* get_largest_bst_subtree_smart(Node* root) {
	return get_largest_bst_subtree_help(root).first;
}
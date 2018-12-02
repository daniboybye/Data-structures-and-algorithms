//Имплементирайте функция, 
//която получава указател към корена на двоично дърво от цели числа(не - наредено) 
//и две цели числа A и B.
//Ако A и B се срещат в дървото, 
//функцията трябва да върне указател към най - близкия им общ родител.
//Ако такъв не съществува, върнете nullptr.
//Считаме, че всеки елемент в дървото се среща само по веднъж.
//По - долу са дадени прототипът на функцията и представянето на възлите на дървото.

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

struct Node {
	int value;
	Node *left, *right;
};

bool is_leaf(Node* node) {
	return !node;
}

//обхождаме дървото с dfs О(N),
//използваме рекурсия, вместо стек
//пазим си изминатия път в path
//резултатъв в path  е пътя от корена до елементът със стойност value
bool find_path(Node* root, int value, std::vector<Node*>& path) {
	if (is_leaf(root)) {
		return false;
	}
	path.push_back(root);

	/*if (root->value == value) {
		return true;
	}

	if (find_path(root->left, value, path)) {
		return true;
	}

	if (find_path(root->right, value, path)) {
		return true;
	}*/

	if (root->value == value || find_path(root->left, value, path) || find_path(root->right, value, path)) {
		return true;
	}

	path.pop_back();
	return false;
}

//О(N)
Node* lca(Node* root, int a, int b) {
	std::vector<Node*> path_a, path_b;

	find_path(root, a, path_a);
	find_path(root, b, path_b);

	//ако а или b не се среща в дървото
	if (path_a.empty() || path_b.empty()) {
		return nullptr;
	}

	//последният общ елемент в пътищата от корена до двата елемента е LeastCommonAncestor
	size_t i = 0;
	for (; i < std::min(path_a.size(), path_b.size()) && path_a[i] == path_b[i]; ++i);

	return path_a[i - 1];
}
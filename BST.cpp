#include <iostream>
#include "BST.h"

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) :
parent(parent), left(left), right(right), keyValuePair(key, value) {}
BinarySearchTree::Node::Node(const Node &other) : parent(nullptr), left(nullptr), right(nullptr), keyValuePair(other.keyValuePair)
{
    if (other.left != nullptr)
    {
        left = new Node(*(other.left));
        left->parent = this;
    }
    if (other.right != nullptr)
    {
        right = new Node(*(other.right));
        right->parent = this;
    }
}
bool BinarySearchTree::Node::operator==(const Node &other) const { return keyValuePair == other.keyValuePair; }
void BinarySearchTree::Node::output_node_tree() const
{
    if (left != nullptr) { left->output_node_tree(); }
    std::cout<<'('<<keyValuePair.first<<", "<<keyValuePair.second<<')'<<std::endl;
    if (right != nullptr)
    {
        if (right->keyValuePair.first != UINT32_MAX) { right->output_node_tree(); }
    }
}
void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
    if (key < keyValuePair.first)
    {
        if (left == nullptr) { left = new Node(key, value, this); }
        else { left->insert( key, value); }
    }
    else
    {
        if (right == nullptr) { right = new Node(key, value, this); }
        else { right->insert( key, value); }
    }
}
void BinarySearchTree::Node::erase(const Key &key)
{
    Node* currentNode = this;
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    Node* newNode = currentNode->right;
    if (newNode == nullptr)
    {
        if (currentNode->left != nullptr)
        {
            newNode = currentNode->left;
            if (currentNode->parent != nullptr)
            {
                if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = newNode; }
                else { currentNode->parent->right = newNode; }
            }
            newNode->parent = currentNode->parent;
        }
        else
        {
            if (currentNode->parent != nullptr)
            {
                if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = nullptr; }
                else { currentNode->parent->right = nullptr; }
            }
        }
    }
    else
    {
        while (newNode->left != nullptr) { newNode = newNode->left; }
        if (newNode->parent->left == newNode)
        {
            newNode->parent->left = newNode->right;
            if (newNode->right != nullptr) { newNode->right->parent = newNode->parent; }
        }
        if (currentNode->parent != nullptr)
        {
            if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = newNode; }
            else { currentNode->parent->right = newNode; }
        }
        newNode->parent = currentNode->parent;
        newNode->left = currentNode->left;
        if(currentNode->right != newNode) { newNode->right = currentNode->right; }
    }
    delete currentNode;
}
void BinarySearchTree::Node::clear()
{
    if (left != nullptr)
    {
        left->clear();
        delete left;
        left = nullptr;
    }
    if (right != nullptr)
    {
        right->clear();
        delete right;
        right = nullptr;
    }
}
size_t BinarySearchTree::Node::height() const
{
    size_t h_l;
    size_t h_r;
    if (left != nullptr) { h_l = left->height(); }
    else { h_l = 0; }
    if (right != nullptr) { h_r = right->height(); }
    else { h_r = 0; }
    return 1 + (h_r > h_l ? h_r : h_l);
}
BinarySearchTree::Node* BinarySearchTree::Node::findMax()
{
    Node* currentNode = this;
    while (currentNode->right != nullptr && currentNode->right->keyValuePair.first != UINT32_MAX)
    {
        currentNode = currentNode->right;
    }
    return currentNode;
}

/*===============================================================================================================================*/

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) { *this = other; }
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other)
{
    if (this == &other) { return *this; }
    if (_root != nullptr)
    {
        _root->clear();
        delete _root;
    }
    _root = nullptr;
    if (other._root != nullptr)
    {
        _root = new Node(*(other._root));
    }
    _size = other._size;
    return *this;
}
BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept { *this = std::move(other); }
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
    if (this == &other) { return *this; }
    if (_root != nullptr)
    {
        _root->clear();
        delete _root;
    }
    _size = other._size;
    _root = other._root;
    other._size = 0;
    other._root = nullptr;
    return *this;
}
BinarySearchTree::~BinarySearchTree()
{
    if (_root != nullptr)
    {
        _root->clear();
        delete _root;
    }
}
void BinarySearchTree::insert(const Key &key, const Value &value)
{
    if (_root == nullptr)
    {
        _root = new Node(key, value, nullptr, nullptr, nullptr);
        _root->right = new Node(UINT32_MAX, 0, _root->findMax());
    }
    else
    {
        Node* temp = _root->findMax()->right;
        _root->findMax()->right = nullptr;
        _root->insert(key, value);
        _root->findMax()->right = temp;
        temp->parent = _root->findMax();
    }
    _size++;
}
void BinarySearchTree::erase(const Key &key)
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return; }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    Node* newNode = currentNode->right;
    while(currentNode->right != nullptr && currentNode->right->keyValuePair.first == key)
    {
        Node* temp = currentNode->right;
        currentNode->right = currentNode->right->right;
        if (temp->right != nullptr)
        {
            temp->right->parent = currentNode;
        }
        delete temp;
        _size--;
        newNode = currentNode->right;
    }
    if (newNode == nullptr)
    {
        if (currentNode->left != nullptr)
        {
            newNode = currentNode->left;
            if (currentNode->parent != nullptr)
            {
                if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = newNode; }
                else { currentNode->parent->right = newNode; }
            }
            newNode->parent = currentNode->parent;
        }
        else
        {
            if (currentNode->parent != nullptr)
            {
                if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = nullptr; }
                else { currentNode->parent->right = nullptr; }
            }
        }
    }
    else
    {
        while (newNode->left != nullptr) { newNode = newNode->left; }
        if (newNode->parent->left == newNode)
        {
            newNode->parent->left = newNode->right;
            if (newNode->right != nullptr) { newNode->right->parent = newNode->parent; }
        }
        if (currentNode->parent != nullptr)
        {
            if (key < currentNode->parent->keyValuePair.first) { currentNode->parent->left = newNode; }
            else { currentNode->parent->right = newNode; }
        }
        newNode->parent = currentNode->parent;
        newNode->left = currentNode->left;
        if(currentNode->right != newNode) { newNode->right = currentNode->right; }
    }
    if (currentNode == _root) { _root = newNode; }
    delete currentNode;
    _size--;
}
size_t BinarySearchTree::size() const { return _size; }
void BinarySearchTree::output_tree() { if (_root != nullptr) { _root->output_node_tree(); }}
size_t BinarySearchTree::max_height() const
{
    if (_root != nullptr) { return _root->height(); }
    else { return 0; }
}
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { throw std::out_of_range("Дерево пустое."); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    return ConstIterator(currentNode);
}
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { throw std::out_of_range("Дерево пустое."); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    return Iterator(currentNode);
}
BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return ConstIterator(nullptr); }
    while (currentNode->left != nullptr)
    {
        currentNode = currentNode->left;
    }
    return ConstIterator(currentNode);
}
BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return ConstIterator(nullptr); }
    while (currentNode->right != nullptr && currentNode->right->keyValuePair.first != UINT32_MAX)
    {
        currentNode = currentNode->right;
    }
    return ConstIterator(currentNode);
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return ConstIterator(nullptr); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    Value value = currentNode->keyValuePair.second;
    Node* temp = currentNode;
    while (currentNode->right != nullptr && currentNode->right->keyValuePair.first == key)
    {
        if (currentNode->right->keyValuePair.second < value)
        {
            value = currentNode->right->keyValuePair.second;
            temp = currentNode;
        }
        currentNode = currentNode->right;
    }
    return ConstIterator(temp);
}
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return ConstIterator(nullptr); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    Value value = currentNode->keyValuePair.second;
    Node* temp = currentNode;
    while (currentNode->right != nullptr && currentNode->right->keyValuePair.first == key)
    {
        if (currentNode->right->keyValuePair.second > value)
        {
            value = currentNode->right->keyValuePair.second;
            temp = currentNode;
        }
        currentNode = currentNode->right;
    }
    return ConstIterator(temp);
}
BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return Iterator(nullptr); }
    while (currentNode->left != nullptr)
    {
        currentNode = currentNode->left;
    }
    return Iterator(currentNode);
}
BinarySearchTree::Iterator BinarySearchTree::end() { return Iterator(_root->findMax()->right); }
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { return ConstIterator(nullptr); }
    while (currentNode->left != nullptr)
    {
        currentNode = currentNode->left;
    }
    return ConstIterator(currentNode);
}
BinarySearchTree::ConstIterator BinarySearchTree::cend() const { return ConstIterator(_root->findMax()->right); }
std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key)
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { throw std::out_of_range("Дерево пустое."); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    Iterator first(currentNode);
    while (currentNode->keyValuePair.first == key)
    {
        Node* temp = currentNode;
        currentNode = currentNode->right;
        if (currentNode == nullptr) { return std::make_pair(first, ++Iterator(temp)); }
    }
    return std::make_pair(first, Iterator(currentNode));
}
std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const
{
    Node* currentNode = _root;
    if (currentNode == nullptr) { throw std::out_of_range("Дерево пустое."); }
    while (currentNode->keyValuePair.first != key)
    {
        if (key < currentNode->keyValuePair.first) { currentNode = currentNode->left; }
        else { currentNode = currentNode->right; }
        if (currentNode == nullptr) { throw std::out_of_range("Ключ отсутствует в дереве."); }
    }
    ConstIterator first(currentNode);
    while (currentNode->keyValuePair.first == key)
    {
        Node* temp = currentNode;
        currentNode = currentNode->right;
        if (currentNode == nullptr) { return std::make_pair(first, ++ConstIterator(temp)); }
    }
    return std::make_pair(first, ConstIterator(currentNode));
}

/*===============================================================================================================================*/

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}
std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() { return _node->keyValuePair; }
const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const { return _node->keyValuePair; }
std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() { return &_node->keyValuePair; }
const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const { return &_node->keyValuePair; }
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (_node == nullptr || _node->keyValuePair.first == UINT32_MAX) { return *this; }
    if (_node->right != nullptr)
    {
        _node = _node->right;
        if (_node->keyValuePair.first == UINT32_MAX) { return *this; }
        else
        {
            while (_node->left != nullptr)
            {
                _node = _node->left;
            }
        }
    }
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->right)
        {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    if (_node == nullptr) { return *this; }
    if (_node->keyValuePair.first == UINT32_MAX)
    {
        _node = _node->parent;
        return *this;
    }
    if (_node->left != nullptr)
    {
        _node = _node->left;
        while (_node->right != nullptr)
        {
            _node = _node->right;
        }
    }
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->left)
        {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    Iterator tmp = *this;
    --(*this);
    return tmp;
}
bool BinarySearchTree::Iterator::operator==(const Iterator &other) const { return _node == other._node; }
bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const { return _node != other._node; }
BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}
const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const { return _node->keyValuePair; }
const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const { return &_node->keyValuePair; }
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (_node == nullptr || _node->keyValuePair.first == UINT32_MAX) { return *this; }
    if (_node->right != nullptr)
    {
        _node = _node->right;
        if (_node->keyValuePair.first == UINT32_MAX) { return *this; }
        else
        {
            while (_node->left != nullptr)
            {
                _node = _node->left;
            }
        }
    }
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->right)
        {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (_node == nullptr) { return *this; }
    if (_node->keyValuePair.first == UINT32_MAX)
    {
        _node = _node->parent;
        return *this;
    }
    if (_node->left != nullptr)
    {
        _node = _node->left;
        while (_node->right != nullptr)
        {
            _node = _node->right;
        }
    }
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->left)
        {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}
bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const { return _node == other._node; }
bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const { return _node != other._node; }

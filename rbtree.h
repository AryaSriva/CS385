/*******************************************************************************
 * Name        : rbtree.h
 * Author      : Aryaman Srivastava
 * Version     : 1.0
 * Date        : 11/13/23
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
using namespace std;
// Forward declaration
template <typename K, typename V>
class RedBlackTree;

/**
 * tree_exception class
 * Demonstrates how you can write your own custom exceptions in C++.
 */
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_{message} { }

    ~tree_exception() throw() { }

    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};

template <typename K, typename V>
class RedBlackTreeIterator {
public:
    /**
     * Constructor
     */
    RedBlackTreeIterator() : node_ptr{nullptr}, tree{nullptr} { }

    /**
     * Equality operator. Compares node pointers.
     */
    bool operator==(const RedBlackTreeIterator &rhs) const {
        return node_ptr == rhs.node_ptr;
    }

    /**
     * Inequality operator. Compares node pointers.
     */
    bool operator!=(const RedBlackTreeIterator &rhs) const {
        return node_ptr != rhs.node_ptr;
    }

    /**
     * Dereference operator. Returns a reference to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>& operator*() const {
        return *node_ptr;
    }

    /**
     * Dereference operator. Returns a pointer to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>* operator->() const {
        return node_ptr;
    }

    /**
     * Preincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator& operator++() {
        Node<K, V> *p;

        if (node_ptr == nullptr) {
            // ++ from end(). Get the root of the tree.
            node_ptr = tree->root_;

            // Error, ++ requested for an empty tree.
            if (node_ptr == nullptr)
                throw tree_exception(
                        "RedBlackTreeIterator operator++(): tree empty");

            // Move to the smallest value in the tree, which is the first node
            // in an inorder traversal.
            while (node_ptr->left != nullptr) {
                node_ptr = node_ptr->left;
            }
        } else {
            if (node_ptr->right != nullptr) {
                // Successor is the leftmost node of right subtree.
                node_ptr = node_ptr->right;

                while (node_ptr->left != nullptr) {
                    node_ptr = node_ptr->left;
                }
            } else {
                // Have already processed the left subtree, and
                // there is no right subtree. Move up the tree,
                // looking for a parent for which node_ptr is a left child,
                // stopping if the parent becomes nullptr (or in this case,
                // root_parent_. A non-null parent is the successor. If parent
                // is nullptr, the original node was the last node inorder, and
                // its successor is the end of the list.
                p = node_ptr->parent;
                while (p != nullptr && node_ptr == p->right) {
                    node_ptr = p;
                    p = p->parent;
                }

                // If we were previously at the rightmost node in
                // the tree, node_ptr = nullptr, and the iterator specifies
                // the end of the list.
                node_ptr = p;
            }
        }

        return *this;
    }

    /**
     * Postincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator operator++(int) {
        RedBlackTreeIterator tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    }

private:
    // node_ptr is the current location in the tree. We can move
    // freely about the tree using left, right, and parent.
    // tree is the address of the RedBlackTree object associated
    // with this iterator. It is used only to access the
    // root pointer, which is needed for ++ and --
    // when the iterator value is end().
    Node<K, V> *node_ptr;
    RedBlackTree<K, V> *tree;
    friend class RedBlackTree<K, V>;

    /**
     * Constructor used to construct an iterator return value from a tree
     * pointer.
     */
    RedBlackTreeIterator(Node<K, V> *p, RedBlackTree<K, V> *t) :
        node_ptr(p), tree(t) { }
};

template<typename K, typename V>
class RedBlackTree : public Tree {
public:
    typedef RedBlackTreeIterator<K, V> iterator;

    /**
     * Constructor to create an empty red-black tree.
     */
    explicit RedBlackTree() : root_{nullptr}, size_{0} { }

    /**
     * Constructor to create a red-black tree with the elements from the
     * vector.
     */
    explicit RedBlackTree(std::vector< std::pair<K, V> > &elements) :
                                            root_(nullptr), size_(0) {
        insert_elements(elements);
    }

    /**
     * Destructor.
     */
    ~RedBlackTree() {
        delete_tree(root_);
    }

    /**
     * Inserts elements from the vector into the red-black tree.
     * Duplicate elements are not inserted.
     */
    void insert_elements(std::vector< std::pair<K, V> > &elements) {
        for (size_t i = 0, len = elements.size(); i < len; ++i) {
            try {
                insert(elements[i].first, elements[i].second);
            } catch (const tree_exception &te) {
                std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }

    /**
     * Inserts a key-value pair into the red black tree.
     * const iterator &it indicates where to start the search for the place to
     * insert the node. If it == end(), the search starts at the root.
     * Must throw a tree_exception if attempting to insert a key that is
     * already present in the tree.
     */
    void insert(const iterator &it, const std::pair<K, V> &key_value) {
        const K& key = key_value.first;
        Node<K, V> *x, *y;
        if (it != end()) {
            x = it.node_ptr;
            y = x->parent;
        } else {
            x = root_;
            y = nullptr;
        }
        if (find(key) != end()) { //check if we are trying to insert a duplicate key and throw a tree_exception
            ostringstream s;
            s << "Attempt to insert duplicate key '" << key << "'.";
            throw tree_exception(s.str());
            return;
        }
        while (x != nullptr) { //find where to insert the new node
            y = x;
            if (key < x->key()) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        Node<K, V> *z = new Node(key, key_value.second);
        z->parent = y; 
        if (y == nullptr) { //check if the new node is going to be the root
            root_ = z;
        } else if (key < y->key()) { //check if the new node should be the left or right child in accordance to BST property
            y->left = z;
        } else {
            y->right = z;
        }
        z->left = nullptr; //make the new node a leaf node and increment the size of the tree
        z->right = nullptr;
        z->color = RED;
        size_++;
        insert_fixup(z); //rebalance the tree
    }

    /**
     * Inserts a key-value pair into the red-black tree.
     */
    void insert(const K &key, const V &value) {
        iterator e = end();
        insert(e, std::pair<K, V>(key, value));
    }

    /**
     * Returns an ASCII representation of the red-black tree.
     */
    std::string to_ascii_drawing() {
        BinaryTreePrinter<K, V> printer(root_);
        return printer.to_string();
    }

    /**
     * Returns the height of the red-black tree.
     */
    int height() const {
        return height(root_);
    }

    /**
     * Returns the number of nodes in the red-black tree.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Returns the leaf count of the red-black tree.
     */
    size_t leaf_count() const {
        return leaf_count(root_);
    }

    /**
     * Returns the internal node count of the red-black tree.
     */
    size_t internal_node_count() const {
        return internal_node_count(root_);
    }

    /**
     * Returns the diameter of the red-black tree rooted at node.
     * Diameter is defined as the length of the longest path between
     * two (non-null) leaves in the tree. The path does not necessarily have to
     * pass through the root.
     */
    size_t diameter() const {
        // TODO
        return diameter(root_); //call the diameter helper function with the root as the parameter
    }

    /**
     * Returns the max width of the red-black tree, i.e. the largest number of
     * nodes on any level.
     */
    size_t max_width() const {
        size_t max_width = 0;
        for (int i = 0, h = height(root_) + 1; i < h; ++i) {
            size_t w = width(root_, i);
            if (w > max_width) {
                max_width = w;
            }
        }
        return max_width;
    }

    /**
     * Returns the successful search cost, i.e. the average number of nodes
     * visited to find a key that is present.
     */
    double successful_search_cost() const {
        return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
    }

    /**
     * Returns the unsuccessful search cost, i.e. the average number of nodes
     * visited to find a key that is not present.
     */
    double unsuccessful_search_cost() const {
        return (double)sum_null_levels() / null_count();
    }

    /**
     * Searches for item. If found, returns an iterator pointing
     * at it in the tree; otherwise, returns end().
     */
    iterator find(const K &key) {
        Node<K, V> *x = root_;
        while (x != nullptr) {
            const K& current_key = x->key();
            if (key == current_key) {
                break; // Found!
            } else if (key < current_key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(x, this);
    }

    /**
     * Returns an iterator pointing to the first item in order.
     */
    iterator begin() {
        Node<K, V> *curr = root_;

        // if the tree is not empty, the first node
        // in order is the farthest node left from root
        if (curr != nullptr) {
            while (curr->left != nullptr) {
                curr = curr->left;
            }
        }

        // build return value using private constructor
        return iterator(curr, this);
    }

    /**
     * Returns an iterator pointing just past the end of the tree data.
     */
    iterator end() {
        return iterator(nullptr, this);
    }

private:
    Node<K, V> *root_;
    size_t size_;
    friend class RedBlackTreeIterator<K, V>;

    /**
     * Deletes all nodes from the red-black tree.
     */
    void delete_tree(Node<K, V> *n) {
        if(n == nullptr) { //return if the node is null as we don't have anything to delete
            return;
        } else if (n->right == nullptr && n->left == nullptr) { //if the node is a leaf just delete the node and return
            delete n;
            return;
        } else { //otherwise, recurisvely visit the left and right subtrees, deleting nodes as we go
            delete_tree(n->left);
            delete_tree(n->right);
            delete n;
            return;
        }
    }

    /**
     * Fixup method described on p. 316 of CLRS.
     */
    void insert_fixup(Node<K, V> *z) {
        while (z->parent != nullptr && z->parent->parent != nullptr && z->parent->color == RED) { //check that the node has a parent and grandparent
        //check if the parent is the grandparent's left or right child and balance accordingly, performing rotations as needed
            if (z->parent == z->parent->parent->left) { 
                Node<K, V> *y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) { 
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(z->parent->parent);
                }
            } else {
                Node<K, V> *y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    left_rotate(z->parent->parent);
                }
            }
        }
        // Last line below
        root_->color = BLACK;
    }

    /**
     * Left-rotate method described on p. 313 of CLRS.
     */
    void left_rotate(Node<K, V> *x) {
        Node<K, V> *y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    /**
     * Right-rotate method described on p. 313 of CLRS.
     */
    void right_rotate(Node<K, V> *x) {
        Node<K, V> *y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    }

    /**
     * Returns the height of the red-black tree starting at node.
     * A null node starts at height -1.
     */
    int height(Node<K, V> *node) const {
        if (node == nullptr) { //height of null node is -1
            return -1;
        } else {
            //calculate the height of the left and right subtrees and return the max of these two values(height of the tree)
            int left = 1 + height(node->left); 
            int right = 1 + height(node->right); 
            return max(left, right); 
        }
    }

    /**
     * Returns the count of leaves in the red-black tree starting at node.
     * For this method, a leaf is a non-null node that has no children.
     */
    size_t leaf_count(Node<K, V> *node) const {
        if (node == nullptr) { //if the node is null, it isn't a leaf
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) { //if the left and right subtrees are null, the node is a leaf
            return 1;
        } else {
            //calculate the number of leaves in the left and right subtrees and return their sum
            int left = leaf_count(node->left); 
            int right = leaf_count(node->right);
            return left + right;
        }
    }

    /**
     * Returns the count of internal nodes in the red-black tree starting at
     * node.
     * An internal node has at least one child.
     */
    size_t internal_node_count(Node<K, V> *node) const {
        if (node == nullptr) { //if the node is null, it isn't an internal node
            return 0;
        } else if (node->left == nullptr && node->right == nullptr) { //check if the node is a leaf
            return 0;
        } else { 
            //calculate the number of internal nodes in the left and right subtrees, and return their sum + 1(if the node isn't a leaf, it is an internal node)
            int left = internal_node_count(node->left);
            int right = internal_node_count(node->right);
            return 1 + left + right;
        }
    }

    /**
     * Helper method to assist in the computation of tree diameter.
     */
    int diameter(Node<K, V> *node) const {
        if (node == nullptr) { //if the node is null, the diameter is 0
            return 0;
        } else {
            /*calculate the heights and diameters of the left/right subtrees, 
            then return the max between the height of the tree + 1(height starts at -1) and the diameters of the left/right subtree*/
            int leftHeight = height(node->left);
            int rightHeight = height(node->right);
            int leftDiameter = diameter(node->left);
            int rightDiameter = diameter(node->right);
            return max(leftHeight + rightHeight + 2, max(leftDiameter, rightDiameter));
        }
    }

    /**
     * Returns the width of the red-black tree at the designated level.
     * Width is defined as the number of nodes residing at a level.
     */
    size_t width(Node<K, V> *node, size_t level) const {
        if (node == nullptr) { //if the node is null, the width is 0
            return 0;
        } else if (level == 0) { //if the level is 0, it is a leaf node
            return 1;
        } else {
            //calculate the widths of the left and right subtrees and return their sum
            int left = width(node->left, level - 1);
            int right = width(node->right, level - 1);
            return left + right;
        }
    }

    size_t null_count() const {
        return null_count(root_);
    }

    /**
     * Returns the count of null pointers in the red-black tree starting at
     * node.
     */
    size_t null_count(Node<K, V> *node) const {
        if (node == nullptr) { //return 1 if the node is null
            return 1;
        } else {
            //count the number of null nodes in the left and right subtrees and return their sum
            int left = null_count(node->left);
            int right = null_count(node->right);
            return left + right;
        }
    }

    size_t sum_levels() const {
        return sum_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each non-null node in the red-black
     * tree starting at node.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 1*0 + 2*1 + 1*2 = 4.
     */
    size_t sum_levels(Node<K, V> *node, size_t level) const {
        if (node == nullptr) { //if the node is null, return 0
            return 0;
        } else {
            //calculate the sum of the levels of the non-null nodes of the left and right subtrees, adding the level as we go to the summation
            int left = sum_levels(node->left, level+1);
            int right = sum_levels(node->right, level+1);
            return level + left + right;
        }
    }

    size_t sum_null_levels() const {
        return sum_null_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each null node in the red-black tree
     * starting at node.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * 10 <- level 2
     *       / \
     *       * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     */
    size_t sum_null_levels(Node<K, V> *node, size_t level) const {
        if (node == nullptr) { //if the node is null, return its level
            return level;
        } else {
            //calculate the sum of the levels of the null-nodes of the left and right subtrees and return their sum
            int left = sum_null_levels(node->left, level+1);
            int right = sum_null_levels(node->right, level+1);
            return left + right; 
        }
    }
};

#endif /* RBTREE_H_ */

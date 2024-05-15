// STUDENT CODE
// NAME: Michael Lee
// San Diego State University
// CS210/CS496: Data Structures
//
// File: BinarySearchTree.h
//
// A concrete implementation of a basic binary search tree. This structure is
// useful only for education and illustration of the BAD things about growth.
//
// Additional Reading:
// https://en.wikipedia.org/wiki/Binary_search_tree

#ifndef PROG2_BINARYSEARCHTREE_H
#define PROG2_BINARYSEARCHTREE_H

#include <memory>

#include "Map.h"

namespace sdsu {
    template<typename KEY, typename VALUE>
    class BinarySearchTree : public Map<KEY,VALUE> {

        // The BST links together BSTNode objects. The outside world never
        // needs to use these, but this data structure will use them to build
        // the search tree.
        struct BSTNode {
            // The BST structures itself around the KEY's ranking. The key is
            // so important, it must be unique in the BST.
            KEY key;

            // The value will almost certainly not be a void*, but this allows
            // us to store ANYTHING, for we can cast a void* into something else
            // void* is an address to anything . . . not nothingness. Values
            // in a map may be repeated, and values don't appear in a Set.
            VALUE value;

            std::shared_ptr<BSTNode> childL;
            std::shared_ptr<BSTNode> childR;

            BSTNode(){};
            BSTNode(KEY item) : key(item), childL(nullptr), childR(nullptr){};

            std::pair<std::shared_ptr<KEY*>&, int> keyset(std::shared_ptr<KEY*> &arr){
                std::pair<std::shared_ptr<KEY*>&,int> toRet(arr);
            };

            // This is something like Java's toString method.
            // This is an IN-ORDER traversal.
            friend std::ostream& operator<<(std::ostream& os,  BSTNode const &node) {
                if( node.childL != nullptr ) os << *node.childL;
                os << std::setw(8) << node.key << " ";
                if( node.childR != nullptr ) os << *node.childR;
                return os;
            }
        };

        std::shared_ptr<BSTNode> root;
        int count; // keep track of number of nodes in tree

        // this function may help when you write other functions. Sometimes you
        // want to literally work with the node holding the key, and not
        // just the keys and values themselves. Your design will decide if you
        // need something like this or not.
        // BSTNode &getNode(const KEY &key){}

        // This is a PRIVATE version of the insert function that no one on
        // the outside can call. I find it useful for something like
        // the public version to kick things off, and this does the real
        // work.
        bool insert(const KEY &key, std::shared_ptr<BSTNode> &start){
            bool inserted = false; // keep track of whether the key was inserted or not
            //empty tree
            if (start == nullptr){
                start = std::make_shared<BSTNode>(key);
                count++;
                inserted = true;
                return inserted;
            }
            std::shared_ptr<BSTNode> current = start;
            while (current != nullptr){ // nodes are present in the tree
                if (current->key > key){
                    if (current->childL != nullptr){ // current node has a left child
                        current = current->childL; // update the left child of current to become the new current node
                        continue;
                    }
                    current->childL = std::make_shared<BSTNode>(key); // create link to new node for left child
                    count++;
                    inserted = true;
                    break;
                }
                if (current->key < key) {
                    if (current->childR != nullptr){ // current node has a right child
                        current = current->childR; // update the right child of current to become the new current node
                        continue;
                    }
                    current->childR = std::make_shared<BSTNode>(key); // create link to new node for right child
                    count++;
                    inserted = true;
                    break;
                }
                if (current->key == key){ // key we are trying to insert already exists
                    inserted = false;
                    break;
                }
            }
            return inserted;
        }

        bool remove(const KEY &key, std::shared_ptr<BSTNode> parent, std::shared_ptr<BSTNode> current, bool wasLeft){
            if (parent == nullptr){ // removing the root
                // root has 1 child
                if (current->childL == nullptr){ // child is right
                    root = current->childR;
                    count--;
                    return true;
                }
                else if (current->childR == nullptr){ // child is left
                    root = current->childL;
                    count--;
                    return true;
                }
                else{ // root has 2 children
                    KEY toReplace = getInOrderPre(current);
                    root->key = toReplace;
                    deletePredecessorNode(current);
                    count--;
                    return true;
                }
            }
            else{ // removing key that is not the root
                // node to be removed has no children
                if (current->childL == nullptr && current->childR == nullptr){
                    if (wasLeft){
                        parent->childL = nullptr;
                        count--;
                        return true;
                    } else{
                        parent->childR = nullptr;
                        count--;
                        return true;
                    }
                }
                // node to be removed has no left child
                else if (current->childL == nullptr){
                    if (wasLeft){
                        parent->childL = current->childR;
                        count--;
                        return true;
                    } else{
                        parent->childR = current->childR;
                        count--;
                        return true;
                    }
                }
                // node to be removed has no right child
                else if (current->childR == nullptr){
                    if (wasLeft){
                        parent->childL = current->childL;
                        count--;
                        return true;
                    } else{
                        parent->childR = current->childL;
                        count--;
                        return true;
                    }
                }
                // node to be removed has 2 children
                else{
                    KEY toReplace = getInOrderPre(current);
                    current->key = toReplace;
                    deletePredecessorNode(current);
                    count--;
                    return true;
                }
            }
        }

        // returns in-order predecessor of current key (largest key in the tree that is smaller than the current key)
        KEY getInOrderPre(std::shared_ptr<BSTNode> current) const {
           current = current->childL; // predecessor MUST be in left subtree from root
           while(current->childR != nullptr) { // iterate until rightmost node in left subtree is reached
               current = current->childR;
           }
           return current->key;
        }

        // gets rid of the leftover predecessor node
        void deletePredecessorNode(std::shared_ptr<BSTNode> current) {
            std::shared_ptr<BSTNode> pre;
            std::shared_ptr<BSTNode> cur;
            pre = current;
            cur = current->childL;
            if (cur->childR == nullptr && cur->childL == nullptr) { // predecessor is the left child of current node and has no children
                pre->childL = nullptr; // set parent->childL to nullptr
            } else if (cur->childR == nullptr && cur->childL != nullptr) { // predecessor is the left child of current node and has one child
                pre->childL = cur->childL;
            } else { // predecessor is not the left child of current node
                while (cur->childR != nullptr) {   // iterate until rightmost node in left subtree is reached
                    pre = cur; // parent becomes current node
                    cur = cur->childR; // current becomes the right child of current node
                }
                if (cur->childL == nullptr) { // predecessor has no children
                    pre->childR = nullptr;
                } else { // predecessor node has a left child
                    pre->childR = cur->childL;
                }
            }
        }

    public:
        BinarySearchTree() {
            // initialize
            root = nullptr;
            count = 0;
        }

        BinarySearchTree(const BinarySearchTree<KEY,VALUE> &other) {

        }

        ~BinarySearchTree() override {

        }

        bool contains(const KEY &key) const override {
            std::shared_ptr<BSTNode> cur = root;
            // empty tree
            if(count == 0) return false;

            // current node's key is the key we are searching for
            if(cur->key == key) return true;

            while (cur->key != key){ // still searching for desired key
                if(cur->key > key){ // desired key is in left subtree
                    if(cur->childL == nullptr) return false; // key not found
                    cur = cur->childL; // traverse left
                }
                else if(cur->key < key){ // desired key is in right subtree
                    if(cur->childR == nullptr) return false; // key not found
                    cur = cur->childR; // traverse right
                }
            }
            return true;
        }

        void clear() override {
            root == nullptr;
            count = 0;
        }

        virtual VALUE &get(const KEY &key) override {
            std::shared_ptr<BSTNode> cur = root;
            while (cur != nullptr){ // nodes are present
                if (cur->key == key){ // we found the key we were searching for
                    VALUE &val = cur->value;
                    return val;
                }
                else if (cur->key < key){ // desired key is in right subtree
                    cur = cur->childR;
                }
                else{ // desired key is in left subtree
                    cur = cur->childL;
                }
            }
        }

        //SET insert
        bool insert(const KEY &key) override {
            return insert(key, root); // private insert above
        }

        //MAP insert
        VALUE insert(const KEY &key, const VALUE &val) override {
            //empty tree
            if (root == nullptr){
                root = std::make_shared<BSTNode>(key);
                root->value = val;
                count++;
                return val;
            }
            std::shared_ptr<BSTNode> current = root;
            VALUE oldValue; // will contain the value of the already present key
            while (current != nullptr) { // nodes are present in the tree
                if (current->key > key) {
                    if (current->childL != nullptr) { // current node has a left child
                        current = current->childL; // update the left child of current to become the new current node
                        continue;
                    }
                    current->childL = std::make_shared<BSTNode>(key); // create link to new node for left child
                    current->childL->value = val; // set the value of current's left child = val parameter
                    count++;
                    return val;
                }
                if (current->key < key) {
                    if (current->childR != nullptr) { // current node has a right child
                        current = current->childR; // update the right child of current to become the new current node
                        continue;
                    }
                    current->childR = std::make_shared<BSTNode>(key); // create link to new node for right child
                    current->childR->value = val; // set the value of current's left child = val parameter
                    count++;
                    return val;
                }
                if (current->key == key) { // if the key is already present, it returns the previous value
                    oldValue = current->value;
                    current->value = val;
                    return oldValue;
                }
            }
        }

        std::pair<std::shared_ptr<KEY*>,int> keys() override{
            // Gives the caller an in-order array of keys.
            //
            // return: two items grouped together in a pair. The first is an array
            // filled with keys, and the second is the size of that array.
            KEY* raw = new KEY[size()];
            std::shared_ptr<KEY*> arr = std::make_shared<KEY*>(raw);

            // Todo: Extra Credit Students fill up the arr[] with the keys in-order

            std::pair<std::shared_ptr<KEY*>,int> toRet(arr,size());
            return toRet;
        };

        virtual std::pair<std::shared_ptr<VALUE*>,int> values() override {
            // Gives the caller a display of values that correspond to
            // the in-order key traversal. That is, someone could request
            // the keyset and the values, and keyset[1] corresponds to values[1].
            //
            // return: two items grouped together in a pair. The first is an array
            // filled with values, and the second is the size of that array.
            VALUE* raw = new VALUE[size()];
            std::shared_ptr<VALUE*> arr = std::make_shared<VALUE*>(raw);

            // Todo: Students fill up the arr[] with the values in-order
            // Todo: with respect to their keys (not each other). Extra Credit

            std::pair<std::shared_ptr<VALUE*>,int> vals(arr,size());
            return vals;
        };

        bool remove(const KEY &key) override {
            // empty tree
            if (count == 0) return false;

            // only 1 node in tree
            if (count == 1) {
                if (root->key == key){
                    root = nullptr;
                    count = 0;
                    return true;
                }
                return false;
            }

            std::shared_ptr<BSTNode> curNode = root; // current node
            std::shared_ptr<BSTNode> pre = nullptr; // parent node
            bool exist = true; // states if key is present in tree or not
            bool wasLeft = false; // key to be removed on the left of the parent node

            // more than 1 node
            while (curNode->key != key) { // still searching for desired key
                if (curNode->key > key) { // desired key is in left subtree
                    if (curNode->childL == nullptr){ // key not found
                        exist = false;
                        break;
                    }
                    wasLeft = true;
                    pre = curNode;
                    curNode = curNode->childL;
                }
                else{ // desired key is in right subtree
                    if (curNode->childR == nullptr){ // key not found
                        exist = false;
                        break;
                    }
                    wasLeft = false;
                    pre = curNode;
                    curNode = curNode->childR;
                }
            }
            if (exist == true){ // if key is present, return call to private remove function
                return remove(key, pre, curNode, wasLeft);
            } return false;
        }

        int size() const override {
            return count;
        }

        VALUE& operator[](std::size_t idx){
            return get(idx);
        }

        friend std::ostream& operator<<(std::ostream& os, sdsu::BinarySearchTree<KEY,VALUE> const &bst) {
            if( bst.root != nullptr ) return os << "[" << (*bst.root) << "]";
            return os;
        }
    };
}
#endif //PROG2_BINARYSEARCHTREE_H

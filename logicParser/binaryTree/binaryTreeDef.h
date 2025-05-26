#ifndef BINARY_TREE_DEF_H_
#define BINARY_TREE_DEF_H_

#include <cstdio>
#include <cstdlib>

enum class binaryTreeError {
    NO_ERRORS             =        0,
    TREE_NULL_POINTER     =  1 <<  0,
    NODE_NULL_POINTER     =  1 <<  1,
    ROOT_NULL_POINTER     =  1 <<  2,
    NODE_USED             =  1 <<  3,
    ALLOCATION_ERROR      =  1 <<  4,
};

enum class linkDirection {
    LEFT   = 1 << 0,
    RIGHT  = 1 << 1,
    PARENT = 1 << 2
};

template<typename DT>
struct node {
    DT        data   =      {};
    node<DT> *left   = nullptr;
    node<DT> *right  = nullptr;
    node<DT> *parent = nullptr;
};

template<typename DT>
struct binaryTree {
    node<DT> *root = nullptr;
};

template<typename DT>
binaryTreeError treeInitialize         (binaryTree<DT> *tree);
template<typename DT>
binaryTreeError treeDestruct           (binaryTree<DT> *tree);
template<typename DT>
binaryTreeError nodeInitialize         (node<DT> **currentNode);
template<typename DT>
binaryTreeError nodeLink               (binaryTree<DT> *tree, node<DT> *currentNode, linkDirection direction);
template<typename DT>
binaryTreeError nodeDestruct           (binaryTree<DT> *tree, node<DT> **node);
template<typename DT>
node<DT>       *emplaceNode            (node<DT> currentNode);

#endif // BINARY_TREE_DEF_H_
#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "binaryTreeDef.h"
#include "customWarning.h"

template<typename DT>
inline binaryTreeError treeInitialize(binaryTree<DT> *tree) {
    customWarning(tree, binaryTreeError::TREE_NULL_POINTER);

    if (nodeInitialize(&tree->root) != binaryTreeError::NO_ERRORS) {
        return binaryTreeError::ROOT_NULL_POINTER;
    }

    tree->root->left  = nullptr;
    tree->root->right = nullptr;

    return binaryTreeError::NO_ERRORS;
}

template<typename DT>
inline binaryTreeError treeDestruct(binaryTree<DT> *tree) {
    customWarning(tree, binaryTreeError::TREE_NULL_POINTER);

    if (tree->root) {
        nodeDestruct(tree, &(tree->root));
    }

    return binaryTreeError::NO_ERRORS;
}

template<typename DT>
inline binaryTreeError nodeInitialize(node<DT> **currentNode) {
    *currentNode = new node<DT>;

    customWarning((*currentNode), binaryTreeError::NODE_NULL_POINTER);

    return binaryTreeError::NO_ERRORS;
}

template<typename DT>
inline binaryTreeError nodeLink(binaryTree<DT> *tree, const node<DT> *currentNode, const linkDirection direction) {
    customWarning(tree,        binaryTreeError::TREE_NULL_POINTER);
    customWarning(currentNode, binaryTreeError::NODE_NULL_POINTER);

    node<DT> **childNode = nullptr;

    switch (direction) {
        case linkDirection::LEFT:
        {
            childNode = &(currentNode->left);
            break;
        }

        case linkDirection::RIGHT:
        {
            childNode = &(currentNode->right);
            break;
        }

        case linkDirection::PARENT:
        default:
        {
            return binaryTreeError::NO_ERRORS;
            break;
        }
    }

    if (*childNode) {
        return binaryTreeError::NODE_USED;
    }

    node<DT> *newNode = {};
    nodeInitialize(&newNode);

    newNode->parent = currentNode;
    newNode->left   =     nullptr;
    newNode->right  =     nullptr;
    *childNode      =     newNode;

    DUMP_(tree);

    return binaryTreeError::NO_ERRORS;
}

template<typename DT>
inline binaryTreeError nodeDestruct(binaryTree<DT> *tree, node<DT> **node) {
    customWarning(tree,    binaryTreeError::TREE_NULL_POINTER);
    customWarning((*node), binaryTreeError::NODE_NULL_POINTER);

    if ((*node)->left) {
        nodeDestruct(tree, &((*node)->left));
    }

    if ((*node)->right) {
        nodeDestruct(tree, &((*node)->right));
    }

    delete *node;

    return binaryTreeError::NO_ERRORS;
}

template<typename DT>
node<DT> *emplaceNode(node<DT> currentNode) {
    node<DT> *newNode = nullptr;

    nodeInitialize(&newNode);
    customWarning(newNode, nullptr);

    *newNode = currentNode;

    if (newNode->left) {
        newNode->left->parent = newNode;
    }

    if (newNode->right) {
        newNode->right->parent = newNode;
    }

    return newNode;
}

#endif // BINARY_TREE_H_
/* Impl Binary Search Tree, refer to:
 * 1. https://leetcode.com/problems/delete-node-in-a-bst
 * 2. https://leetcode.com/problems/insert-into-a-binary-search-tree
 * 3. https://leetcode.com/problems/search-in-a-binary-search-tree/
 */
#pragma once
#include <stack>
#include <vector>

namespace impl
{
struct tree_node
{
    int val;
    tree_node *left, *right;
    tree_node(int v = 0) : val(v), left(nullptr), right(nullptr)
    {
    }
};

class bstree
{
  private:
    // Return the root of tree after deleting key
    tree_node *remove(tree_node *node, int val)
    {
        if (node == nullptr)
            return node;

        if (val > node->val)
            node->right = remove(node->right, val);
        else if (val < node->val)
            node->left = remove(node->left, val);
        else
        {
            // we need to delete curretn 'node'. Rather than deleting,
            // we replace it with its precessor (or successor).
            bool l = node->left, r = node->right;
            if (!l && !r) // leaf node, has no precessor (or successor)
            {
                delete node;
                return nullptr;
            }
            else if (l ^ r) // has one child
            {
                auto child = l ? node->left : node->right;
                delete node;
                return child;
            }
            else // has two children
            {
                auto successor = mostLeft(node->right);
                node->val = successor->val;
                node->right = remove(node->right, successor->val);
                return node;
            }
        }
        return node;
    }
    tree_node *mostLeft(tree_node *p)
    {
        while (p->left)
            p = p->left;
        return p;
    }

  public:
    tree_node *root;
    bstree() : root(nullptr)
    {
    }
    auto search(int val)
    {
        auto p = root;
        while (p)
        {
            if (p->val < val)
                p = p->right;
            else if (p->val > val)
                p = p->left;
            else
                break;
        }
        return p;
    }

    // return the inserted node
    tree_node *insert(int val)
    {
        if (root == nullptr)
            return (root = new tree_node(val));

        tree_node *pre = nullptr, *p = root;
        auto node = new tree_node(val);
        while (p)
        {
            pre = p;
            if (p->val < val)
                p = p->right;
            else if (p->val > val)
                p = p->left;
            else
            {
                // If 'val' has existed in bst
                delete node;
                return nullptr;
            }
        }
        return (val < pre->val) ? (pre->left = node) : (pre->right = node);
    }

    tree_node *remove(int val)
    {
        return root = remove(root, val);
    }

    // Get sequence by in-order traversal
    std::vector<int> flattern()
    {
        std::vector<int> seq;
        std::stack<tree_node *> stk;
        auto p = root;

        while (!stk.empty() || p)
        {
            if (p)
                stk.push(p), p = p->left;
            else
            {
                p = stk.top(), stk.pop();
                seq.emplace_back(p->val);
                p = p->right;
            }
        }
        return seq;
    }
};
} // namespace impl
/* Implement a nested single linked list, which is similar to list in python.
 * e.g. [1, [2, 3, [4]], [5, [6, [7]]]]
 */

#pragma once
#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <stack>

namespace impl
{
class nested_node
{
  public:
    bool is_list; // Denote current node is a list or not
    nested_node *next;
    union {
        uint64_t value;
        nested_node *list;
    }; // if is_list = true, then this 'list' is valid

    static int num_nodes;  // To test nested_list::destroy() is work or not

    nested_node(uint64_t val, bool islist = false) : value(val), is_list(islist), next(nullptr)
    {
        num_nodes++;
    }

    virtual ~nested_node()
    {
        num_nodes--;
    };
};

int nested_node::num_nodes = 0;

class nested_list
{
  private:
    nested_node *head; // dummy head node

    // Insert a node after the 'pos'.
    // 'data' could be a value of a node, also could be a pointer of list
    // 'pos' must be a node of current nested_list object
    nested_node *insert(nested_node *pos, bool is_list, uint64_t data)
    {
        assert(pos != nullptr);
        auto node = new nested_node(data, is_list);
        node->next = pos->next;
        pos->next = node;
        return node;
    }

    void destroy(nested_node *node)
    {
        if (node == nullptr)
            return;
        nested_node *p = node, *next = nullptr;
        while (p != nullptr)
        {
            if (p->is_list)
                destroy(node->list);
            else
            {
                next = p->next;
                delete p;
                p = next;
            }
        }
    }

    nested_node *deep_copy(nested_node *node)
    {
        if (node == nullptr)
            return nullptr;
        auto ptr = new nested_node(node->value, node->is_list);
        if (node->is_list)
            ptr->list = deep_copy(node->list);
        ptr->next = deep_copy(node->next);
        return ptr;
    }

    void print(nested_node *node)
    {
        if (node == nullptr)
            return;
        std::cout << "[";
        for (auto p = node; p != nullptr; p = p->next)
        {
            if (!p->is_list)
                std::cout << p->value;
            else
                print(p->list);
            if (p->next)
                std::cout << ", ";
        }
        std::cout << "]";
    }

  public:
    nested_list() : head(new nested_node(-1))
    {
    }

    // std::move ctor, to support return a nested_list object in a function call
    nested_list(const nested_list &&list)
    {
        head = list.head;
    }

    virtual ~nested_list()
    {
        destroy(head);
    }

    void push_front(uint64_t val)
    {
        insert(head, false, val);
    }

    void push_front(const nested_list &list)
    {
        assert(this != &list && head != list.head);
        nested_node *copied = deep_copy(list.head->next);
        insert(head, true, (uint64_t)copied);
    }

    void display()
    {
        print(head->next);
        std::cout << '\n';
    }

    // Make nested list flattern
    std::vector<uint64_t> flattern()
    {
        std::vector<uint64_t> res;
        std::stack<nested_node *> stk;
        for (auto p = head->next; p != nullptr; p = p->next)
            stk.emplace(p);
        
        while (!stk.empty())
        {
            auto node = stk.top();
            stk.pop();

            if (node->is_list)
            {
                for (auto p = node->list; p != nullptr; p = p->next)
                    stk.emplace(p);
            }
            else
            {
                res.emplace_back(node->value);
            }
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};
} // namespace impl
#include "red-black-tree.hpp"
#include <iostream>

template <typename K>
struct node_of : node
{
    node_of(const K& k, node_of* parent = nullptr) : node(red,parent), key(k) {}

    void inorder(void (*f)(const node_of&)) const
    {
        if (left)  left_child()->inorder(f);
        f(*this);
        if (right) right_child()->inorder(f);
    }

    const node_of* left_child()  const { return static_cast<const node_of*>(left); }
          node_of* left_child()        { return static_cast<      node_of*>(left); }
    const node_of* right_child() const { return static_cast<const node_of*>(right); }
          node_of* right_child()       { return static_cast<      node_of*>(right); }

    using node::insert_case1;
    using node::delete_case1;

    K key;
};

template <typename K>
struct red_black_tree
{
    typedef node_of<K> node_type;
    node_type* root;

    red_black_tree() : root(nullptr) {}

    node_type* find(const K& key)
    {
        node_type* p = root;

        while (p)
        {
            if (key == p->key)
                return p;
            else
            if (key < p->key)
                p = p->left_child();
            else
                p = p->right_child();
        }

        return nullptr;
    }
    node_type* insert(const K& key)
    {
        node_type* q;

        if (node_type* p = root)
            for (;;)
            {
                if (key == p->key)
                    return p;
                else
                if (key < p->key)
                    if (p->left)
                        p = p->left_child();
                    else
                    {
                        p->left = q = new node_type(key,p);
                        break;
                    }
                else
                    if (p->right)
                        p = p->right_child();
                    else
                    {
                        p->right = q = new node_type(key,p);
                        break;
                    }
            }
        else
            root = q = new node_type(key);

        q->insert_case1(*reinterpret_cast<node**>(&root));
        return q;
    }

    void remove(const K& key)
    {
        node* n = find(key);
        node::remove(*reinterpret_cast<node**>(&root),n);
    }

};

template <typename T>
void show(const node_of<T>& n)
{
    std::cout << n.key << ',';
}

int main()
{
    red_black_tree<int> rbt;
    int values[] = {7,3,2,8,3,0,9,1,5};
    for (size_t i = 0, n = sizeof(values)/sizeof(values[0]); i < n; ++i)
        rbt.insert(values[i]);
    rbt.root->inorder(&show<int>);
    for (size_t i = 0, n = sizeof(values)/sizeof(values[0]); i < n; ++i)
        rbt.remove(values[i]);
    rbt.root->inorder(&show<int>);
}

#include <algorithm>
#include <mach7/config.hpp>                // Mach7 configuration

/// The source code for the class is adopted from http://en.wikipedia.org/wiki/Red-black_tree
class node
{
public:

    enum node_color { red, black } m_color;
    node* parent;
    node* left;
    node* right;

    node(
        node_color k = red,
        node* p = nullptr,  
        node* l = nullptr, 
        node* r = nullptr
    ) : 
        m_color(k),
        parent(p), 
        left(l), 
        right(r)
    {}
    virtual ~node() { if (left) delete left; if (right) delete right; }
    node_color color() const { return this ? m_color : black; }
    node* grandparent() const { return this && parent ? parent->parent : nullptr; }
    node* sibling() const { return this == parent->left ? parent->right : parent->left; }
    node* uncle() const
    {
        if (node* g = grandparent())
            return parent == g->left ? g->right : g->left;
        else
            return nullptr; // No grandparent means no uncle
    }
    const node* maximum_node() const
    {
        const node* p = this;
        while (p->right) p = p->right;
        return p;
    }
          node* maximum_node()
    {
        node* p = this;
        while (p->right) p = p->right;
        return p;
    }
    static void swap_nodes(node*& root, node*& n1, node*& n2)
    {
        std::swap(n1->parent,n2->parent);
        std::swap(n1->left,  n2->left);
        std::swap(n1->right, n2->right);

        if (n1->parent) (n1->parent->left  == n2 ? n1->parent->left  : n1->parent->right) = n1; else { XTL_ASSERT(n2 == root); root = n1; }
        if (n2->parent) (n2->parent->left  == n1 ? n2->parent->left  : n2->parent->right) = n2; else { XTL_ASSERT(n1 == root); root = n2; }
        if (n1->left)   (n1->left->parent  == n2 ? n1->left->parent  : n1->right->parent) = n1;
        if (n2->left)   (n2->left->parent  == n1 ? n2->left->parent  : n2->right->parent) = n2;
        if (n1->right)  (n1->right->parent == n2 ? n1->right->parent : n1->left->parent ) = n1;
        if (n2->right)  (n2->right->parent == n1 ? n2->right->parent : n2->left->parent ) = n2;

        //std::swap(n1,n2);
    }
    static void replace_node(node*& root, node* oldn, node* newn)
    {
        if (!oldn->parent)
            root = newn;
        else
            if (oldn == oldn->parent->left)
                oldn->parent->left = newn;
            else
                oldn->parent->right = newn;
        if (newn) 
            newn->parent = oldn->parent;
    }
    static void remove(node*& root, node* n)
    {
        if (!n) return;  /* Key not found, do nothing */

        if (n->left && n->right)
        {
            /* Copy key/value from predecessor and then delete it instead */
            node* p = n->left->maximum_node();
            swap_nodes(root, n, p);
        }

        XTL_ASSERT(n->left == NULL || n->right == NULL);
        node* child = n->right ? n->right : n->left;

        if (n->color() == black) 
        {
            n->m_color = child->color();
            n->delete_case1(*reinterpret_cast<node**>(&root));
        }

        replace_node(root, n, child);

        if (!n->parent && child)
            child->m_color = black;

        delete n;
    }

protected:

    void rotate_left(node*& root) 
    {
        node* r = right;
        replace_node(root, this, r);
        right = r->left;
        if (r->left) 
        {
            r->left->parent = this;
        }
        r->left = this;
        parent = r;
    }
    void rotate_right(node*& root) 
    {
        node* l = left;
        replace_node(root, this, l);
        left = l->right;
        if (l->right)
        {
            l->right->parent = this;
        }
        l->right = this;
        parent = l;
    }
    void insert_case1(node*& root)
    {
        if (!parent)
            m_color = black;
        else
            insert_case2(root);
    }
    void insert_case2(node*& root)
    {
        if (parent->color() == black)
            return; /* Tree is still valid */
        else
            insert_case3(root);
    }
    void insert_case3(node*& root)
    {
        node* u = uncle();

        if (u && u->color() == red) 
        {
            parent->m_color = black;
            u->m_color = black;
            node* g = grandparent();
            g->m_color = red;
            g->insert_case1(root);
        } 
        else
            insert_case4(root);
    }
    void insert_case4(node*& root)
    {
        node* g = grandparent();

        if (this == parent->right && parent == g->left) 
        {
            parent->rotate_left(root);
            left->insert_case5(root);
        }
        else 
        if (this == parent->left  && parent == g->right)
        {
            parent->rotate_right(root);
            right->insert_case5(root);
        }
    }
    void insert_case5(node*& root)
    {
        node* g = grandparent();

        parent->m_color = black;
        g->m_color = red;
        if (this == parent->left)
            g->rotate_right(root);
        else
            g->rotate_left(root);
    }
    void delete_case1(node*& root) { if (parent) delete_case2(root); }
    void delete_case2(node*& root)
    {
        node* s = sibling();

        if (s->color() == red) {
            parent->m_color = red;
            s->m_color = black;
            if (this == parent->left)
                parent->rotate_left(root);
            else
                parent->rotate_right(root);
        }
        delete_case3(root);
    }
    void delete_case3(node*& root)
    {
        node* s = sibling();

        if (parent->color()   == black &&
            s->color()        == black &&
            s->left->color()  == black &&
            s->right->color() == black) 
        {
            s->m_color = red;
            parent->delete_case1(root);
        } 
        else
            delete_case4(root);
    }
    void delete_case4(node*& root)
    {
        node* s = sibling();

        if (parent->color()   == red   &&
            s->color()        == black &&
            s->left->color()  == black &&
            s->right->color() == black)
        {
            s->m_color = red;
            parent->m_color = black;
        }
        else
            delete_case5(root);
    }
    void delete_case5(node*& root)
    {
        node* s = sibling();

        if  (s->color() == black) 
        { /* this if statement is trivial,
          due to case 2 (even though case 2 changed the sibling to a sibling's child,
          the sibling's child can't be red, since no red parent can have a red child). */
            /* the following statements just force the red to be on the left of the left of the parent,
            or right of the right, so case six will rotate correctly. */
            if (this == parent->left     &&
                s->right->color() == black &&
                s->left->color()  == red)
            { /* this last test is trivial too due to cases 2-4. */
                s->m_color = red;
                s->left->m_color = black;
                s->rotate_right(root);
            }
            else
                if (this == parent->right    &&
                    s->left->color()  == black &&
                    s->right->color() == red)
                {/* this last test is trivial too due to cases 2-4. */
                    s->m_color = red;
                    s->right->m_color = black;
                    s->rotate_left(root);
                }
        }
        delete_case6(root);
    }
    void delete_case6(node*& root)
    {
        node* s = sibling();

        s->m_color = parent->color();
        parent->m_color = black;

        if (this == parent->left)
        {
            s->right->m_color = black;
            parent->rotate_left(root);
        }
        else
        {
            s->left->m_color = black;
            parent->rotate_right(root);
        }
    }
};
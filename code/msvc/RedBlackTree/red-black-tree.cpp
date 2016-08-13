//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include "red-black-tree.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/all.hpp>

using namespace mch;

template <typename K>
struct node_of : node
{
    node_of(const K& k, node_of* parent = nullptr) : node(red,parent), key(k) {}
    node_of(node_color c, node_of* l, const K& k, node_of* r) : node(c,0,l,r), key(k) {}

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

namespace mch ///< Mach7 library namespace
{
template <typename T> struct bindings<node_of<T>> { 
    CM(0,node_of<T>::m_color); 
    CM(1,node_of<T>::left); 
    CM(2,node_of<T>::key); 
    CM(3,node_of<T>::right); 
};
} // of namespace mch

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

    //type color = R | B
    //type 'a tree = E | T of color * 'a tree * 'a * 'a tree
    //
    //let balance = function  
    //  | B, T (R, T (R,a,x,b), y, c), z, d  
    //  | B, T (R, a, x, T (R,b,y,c)), z, d  
    //  | B, a, x, T (R, T (R,b,y,c), z, d)  
    //  | B, a, x, T (R, b, y, T (R,c,z,d)) -> T (R, T (B,a,x,b), y, T (B,c,z,d))  
    //  | col, a, x, b                      -> T (col, a, x, b) 
    node_type* balance(node::node_color color, node_type* left, const K& key, node_type* right)
    {
        const node::node_color B = node::node_color::black;
        const node::node_color R = node::node_color::red;
        typedef node_type T;
        node::node_color col;
        var<T*> a, b, c, d;
        var<K&> x, y, z;

        Match(color, left, key, right)
        {
        Case(B, C<T>(R, C<T>(R, a, x, b), y, c), z, d) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
        Case(B, C<T>(R, a, x, C<T>(R, b, y, c)), z, d) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
        Case(B, a, x, C<T>(R, C<T>(R, b, y, c), z, d)) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
        Case(B, a, x, C<T>(R, b, y, C<T>(R, c, z, d))) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
        Case(col, a, x, b)                             return new T(col, a, x, b);
        }
        EndMatch

/*
        if (color == B)
        {
            //if (match(color,left,key,right).
            //    with((B, C<T>(R, C<T>(R, a, x, b), y, c), z, d)
            //      || (B, C<T>(R, a, x, C<T>(R, b, y, c)), z, d)
            //      || (B, a, x, C<T>(R, C<T>(R, b, y, c), z, d))
            //      || (B, a, x, C<T>(R, b, y, C<T>(R, c, z, d)))
            //    ))
            //    return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));

            Match(left,right)
            {
            Case(C<T>(R, C<T>(R, a, x, b), y, c), d) z = key; return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            Case(C<T>(R, a, x, C<T>(R, b, y, c)), d) z = key; return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            Case(a, C<T>(R, C<T>(R, b, y, c), z, d)) x = key; return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            Case(a, C<T>(R, b, y, C<T>(R, c, z, d))) x = key; return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            }
            EndMatch

            //Match(color,left,key,right)
            //{
            //Case(B, C<T>(R, C<T>(R, a, x, b), y, c), z, d) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            //Case(B, C<T>(R, a, x, C<T>(R, b, y, c)), z, d) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            //Case(B, a, x, C<T>(R, C<T>(R, b, y, c), z, d)) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            //Case(B, a, x, C<T>(R, b, y, C<T>(R, c, z, d))) return new T(R, new T(B,a,x,b), y, new T(B,c,z,d));
            //}
            //EndMatch
        }
        else
            return new T(c, a, x, b);
*/
    } 

    node_type* ins(node_type* s, const K& x)
    {
        var<node::node_color> c;
        var<node_type*>  a, b;
        var<K&>          y;

        Match(s)
        {
        Case(val(nullptr))
            return new node_type(node::red,nullptr,x,nullptr);
        Case(C<node_type>(c,a,y,b))
	        if (x < y) return balance(c, ins(a,x), y, b); else 
            if (x > y) return balance(c, a, y, ins(b,x)); else
	        return s;
        }
        EndMatch
    }

    node_type* insert(const K& x, node_type* s)
    {
        node_type* r = ins(s,x);
        r->m_color = node::black;
        return r;
    }
};

template <typename T>
void show(const node_of<T>& n)
{
    std::cout << n.key << ',';
}

std::fstream f;

template <typename T>
void dot(const node_of<T>& n)
{
    f << '\"' << &n << "\"[color=" << (n.m_color == node::red ? "red" : "black") << ", label=" << n.key << "]" << std::endl;
    //if (n.parent) f << '\"' << &n << "\" -> \"" << n.parent << "\" [label=P]" << std::endl;
    if (n.left)   f << '\"' << &n << "\" -> \"" << n.left   << "\" [label=L,color=" << (n.left->parent  == &n ? "blue" : "red") << "]" << std::endl;
    if (n.right)  f << '\"' << &n << "\" -> \"" << n.right  << "\" [label=R,color=" << (n.right->parent == &n ? "blue" : "red") << "]" << std::endl;
}

template <typename T>
void print_dot(const red_black_tree<T>& rbt)
{
    static int i = 0;
    std::stringstream ss;
    ss << "rbt" << std::setw(2) << std::setfill('0') << i++ << ".dot";
    f.open(ss.str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    if (f)
    {
        f << "digraph RBT {" << std::endl;
        if (rbt.root) rbt.root->inorder(&dot<T>);
        f << "}" << std::endl;
        f.close();
    }
}

int main()
{
    red_black_tree<int> rbt;
    int values[] = {7,3,2,8,3,0,9,1,5};
    const size_t n = sizeof(values)/sizeof(values[0]);
    for (size_t i = 0; i < n; ++i)
    {
        print_dot(rbt);
        rbt.insert(values[i]);
    }
    rbt.root->inorder(&show<int>);
    print_dot(rbt);
    return 0;
    for (size_t i = 0; i < n; ++i)
        rbt.remove(values[i]);
    rbt.root->inorder(&show<int>);
}

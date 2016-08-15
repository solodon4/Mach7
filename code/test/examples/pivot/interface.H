//
// This file is part of The Pivot framework.
// Written by Gabriel Dos Reis <gdr@cs.tamu.edu>
// 

#ifndef IPR_INTERFACE_INCLUDED
#define IPR_INTERFACE_INCLUDED

#include <utility>
#include <iterator>
#include <stdexcept>

namespace ipr {
   // IPR is designed to be regular, fully general enough to
   //    (1) represent programs written in full Standard C++
   //        -- except macros -- Standard C, and possibly Fortran
   //        and other C-like languages;
   //    (2) be at the basis of program manipulations;
   //    (3) be ahead of time, i.e. accomodate future possible
   //        extensions to C++, e.g. "concepts".
   //
   // The "meta-language" implemented by IPR is expression-based.
   // IPR nodes "climbing" is based on the Visitor Design Pattern.
   //
   // IPR class nodes come in two flavors:
   //     (a) the interface classes, found in this header;
   //     (b) the implementation classes, found in <ipr/impl>.
   // The primary reason for this separation is, we don't want clients
   // programs to critically depend on the particular implementations
   // details of the moment.  Also, it is our intent that the interfaces
   // could be implemented in various ways depending on the particular
   // constraints of the target tools.
   // 
   // A third header file <ipr/traversal> offers a set of traversal
   // facilities.  At the moment, it contains only trivial visitor classes.
   // A fourth header, <ipr/io>, contains interfaces to input/output
   // operations.  At the moment, only output in form of XPR is supported.
   //
   // The interface classes are non-mutating, i.e. there is no way
   // a client program could modify a node through its interface.
   // In particular, all member functions of the interface classes
   // are const member functions.
   //
   // There is an additional requirement on IPR nodes:  type nodes
   // shall have maximal sharing, i.e. be unified.  The design choices
   // behind that decision are discussed in the paper "Representing
   // C++ Directly, Compactly and Efficiently."
   //
   // Warning:  If you add any new node type, you need to update
   //           "node-category.def" for the numerical mapping.

   struct Visitor;               // base class for all IPR vistor classes

   struct Node;                  // universal base class for all IPR nodes
   
   struct Annotation;            // node annotations
   struct Region;                // declarative region
   struct Comment;               // C-style and BCPL-style comments
   struct String;                // literal string 

   struct Mapping;               // function
   struct Overload;              // overload set
   struct Parameter_list;        // function/template parameter list
   struct Scope;                 // declarations in a region

   // ------------------------------------------
   // -- general constructors                 --
   // ------------------------------------------
   struct Linkage;               // general language linkage
   struct Expr;                  // general expressions
   struct Classic;               // general classic expressions
   struct Name;                  // general names
   struct Type;                  // general types
   struct Udt;                   // general user-defined types
   struct Stmt;                  // general statements
   struct Decl;                  // general declarations

   // ------------------------------------------
   // -- results of name constructor constants --
   // ------------------------------------------
   struct Identifier;            // identifier                          foo
   struct Operator;              // C++ operator name             operator+
   struct Conversion;            // conversion function name   operator int
   struct Scope_ref;             // qualified name                     s::m
   struct Template_id;           // C++ template-id                  S<int>
   struct Type_id;               // C++ type-id                  const int*
   struct Ctor_name;             // constructor name                   T::T
   struct Dtor_name;             // destructor name                   T::~T
   struct Rname;                 // de Bruijn (index, position),
                                 // find better name

   // -------------------------------------------
   // -- results of type constructor constants --
   // -------------------------------------------
   struct Array;                 // array type
   struct As_type;               // coerce-to type
   struct Class;                 // user-defined type - class
   struct Decltype;              // strict type of a declaration/expression
   struct Enum;                  // user-defined type - enum
   struct Function;              // function type
   struct Namespace;             // user-defined type - namespace
   struct Pointer;               // pointer type
   struct Ptr_to_member;         // pointer-to-member type
   struct Product;               // product type - not ISO C++ type
   struct Qualified;             // cv-qualified types
   struct Reference;             // reference type
   struct Rvalue_reference;      // rvalue-reference type -- C++0x
   struct Sum;                   // sum type - not ISO C++ type
   struct Template;              // type of a template - Not ISO C++ type
   struct Union;                 // user-defined type - union

   // --------------------------------------------------------
   // -- results of nullar expression constructor constants --
   // --------------------------------------------------------
   struct Phantom;               // for arrays of unknown bound

   // -------------------------------------------------------
   // -- results of unary expression constructor constants --
   // -------------------------------------------------------
   struct Address;               // address-of                          &a
   struct Array_delete;          // array delete-expression     delete[] p
   struct Complement;            // bitwise complement                  ~m
   struct Delete;                // delete-expression             delete p
   struct Deref;                 // dereference expression              *p
   struct Expr_list;             // expression list        
   struct Expr_sizeof;           // sizeof an expression         sizeof *p
   struct Expr_typeid;           // typeid of an expression    typeid (*p)
   struct Id_expr;               // an identifier used in an expression
   struct Label;                 // a label - target of a goto-statement
                                 //         or entry of a switch-statement
   struct Not;                   // logical negation                 !cond
   struct Paren_expr;            // parenthesized expression           (a)
   struct Post_decrement;        // post-increment                     p++
   struct Post_increment;        // post-decrement                     p--
   struct Pre_decrement;         // pre-increment                      ++p
   struct Pre_increment;         // pre-decrement                      --p
   struct Throw;                 // throw expression           throw Bad()
   struct Type_sizeof;           // sizeof a type             sizeof (int)
   struct Type_typeid;           // typeidof a type           typeid (int)
   struct Unary_minus;           // unary minus                         -a
   struct Unary_plus;            // unary plus                          +a

   // --------------------------------------------------------
   // -- results of binary expression constructor constants --
   // --------------------------------------------------------
   struct And;                   // logical and                    a && b
   struct Array_ref;             // array member selection         a[i]
   struct Arrow;                 // indirect member selection      p->m
   struct Arrow_star;            // indirect member indirection    p->*m
   struct Assign;                // assignment                     a = b
   struct Bitand;                // bitwise and                    a & b
   struct Bitand_assign;         // in-place bitwise and           a &= b
   struct Bitor;                 // bitwise or                     a | b
   struct Bitor_assign;          // in-place bitwise or            a |= b
   struct Bitxor;                // bitwise exclusive or           a ^ b
   struct Bitxor_assign;         // in-place bitwise exclusive or  a ^= b
   struct Call;                  // function call                  f(u, v)
   struct Cast;                  // C-style class                  (T) e
   struct Comma;                 // comma-operator                 a, b
   struct Const_cast;            // const-cast             const_cast<T&>(v)
   struct Datum;                 // object construction             T(v)
   struct Div;                   // division                       a / b
   struct Div_assign;            // in-place division              a /= b
   struct Dot;                   // direct member selection        x.m
   struct Dot_star;              // direct member indirection      x.*pm
   struct Dynamic_cast;          // dynamic-cast         dynamic_cast<T&>(v)
   struct Equal;                 // equality comparison            a == b
   struct Greater;               // greater comparison             a > b
   struct Greater_equal;         // greater-or-equal comparison    a >= b
   struct Less;                  // less comparison                a < b
   struct Less_equal;            // less-equal comparison          a <= b
   struct Literal;               // literal expressions            3.14
   struct Lshift;                // left shift                     a << b
   struct Lshift_assign;         // in-place left shift            a <<= b
   struct Member_init;           // member initialization          : m(v)
   struct Minus;                 // subtraction                    a - b
   struct Minus_assign;          // in-place subtraction           a -= b
   struct Modulo;                // modulo arithmetic              a % b
   struct Modulo_assign;         // in=place modulo arithmetic     a %= b
   struct Mul;                   // multiplication                 a * b
   struct Mul_assign;            // in-place multiplication        a *= b
   struct Not_equal;             // not-equality comparison        a != b
   struct Or;                    // logical or                     a || b
   struct Plus;                  // addition                       a + b
   struct Plus_assign;           // in-place addition              a += b
   struct Reinterpret_cast;      // reinterpret-cast  reinterpret_cast<T>(v)
   struct Rshift;                // right shift                    a >> b
   struct Rshift_assign;         // in-place right shift           a >>= b
   struct Static_cast;           // static-cast            static_cast<T>(v)

   // --------------------------------------------------------
   // -- result of trinary expression constructor constants --
   // --------------------------------------------------------
   struct Conditional;           // conditional                   p ? a : b
   struct New;                   // new-expression              new (p) T(v)

   // -----------------------------------------------
   // -- result of statement constructor constants --
   // -----------------------------------------------
   struct Block;                 // brace-enclosed statement sequence
   struct Break;                 // break-statement
   struct Continue;              // continue-statement
   struct Ctor_body;             // constructor-body
   struct Do;                    // do-statement
   struct Expr_stmt;             // expression-statement
   struct Empty_stmt;            // empty statement -- particular Expr_stmt
   struct For;                   // for-statement
   struct For_in;                // structured for-statement
   struct Goto;                  // goto-statement
   struct Handler;               // exception handler statement
   struct If_then;               // if-statement
   struct If_then_else;          // if-else-statement
   struct Labeled_stmt;          // labeled-statement
   struct Return;                // return-statement
   struct Switch;                // switch-statement
   struct While;                 // while-statement

   // -------------------------------------------------
   // -- result of declaration constructor constants --
   // -------------------------------------------------
   struct Alias;                 // alias-declaration
   struct Asm;                   // asm-declaration
   struct Base_type;             // base-class in class inheritance
   struct Enumerator;            // enumerator in enumeration-declaration
   struct Field;                 // field in union or class declaration
   struct Bitfield;              // bitfield
   struct Fundecl;               // function-declaration
   struct Named_map;             // template-declaration
   struct Parameter;             // function or template parameter
   struct Typedecl;              // declaration for a type
   struct Var;                   // variable declaration
   struct Using_directive;       // using-directive

   // ------------------------
   // -- distinguished node --
   // ------------------------
   struct Unit;                  // translation unit

   // --------------------------------
   // -- Special built-in constants --
   // --------------------------------
   struct Void;                  // "void"
   struct Bool;                  // "bool"
   struct Char;                  // "char"
   struct sChar;                 // "signed char"
   struct uChar;                 // "unsigned char";
   struct Wchar_t;               // "wchar_t";
   struct Short;                 // "short"
   struct uShort;                // "unsigned short"
   struct Int;                   // "int"
   struct uInt;                  // "unsigned int"
   struct Long;                  // "long"
   struct uLong;                 // "unsigned long"
   struct Long_long;             // "long long"
   struct uLong_long;            // "unsigned long long"
   struct Float;                 // "float"
   struct Double;                // "double"
   struct Long_double;           // "long double"
   struct Ellipsis;              // "..."

   struct C_linkage;             // "C" linkage
   struct Cxx_linkage;           // "C++" linkage

   // ----------------------------------
   // -- built-ins, but not constants --
   // ----------------------------------
   struct Global_scope;          // global namespace -- "::"


                                // -- Category_code --
   // A list of numerical codes in one-to-one correspondance with
   // IPR node "interface types".  In a sufficiently expressive and
   // efficient type system, we would not need to manually maintain
   // this list.  This can be seen as an optimization of the traditional
   // double-dispatch process for determining the dynamic type of a node.
   enum Category_code {
#include <ipr/node-category>
   };

   // Routines to report statistics about a run of a program.
   namespace stats {
      int all_nodes_count();    // count of all nodes 
      int node_count(Category_code); // count of nodes of a given category

      // >>>> Yuriy Solodkyy: 2010/06/17
      // Support for measuring how much memory IPR datastructures take
      #ifdef IPR_TRACK_MEMORY_SIZE
      extern int ipr_mem_size;  // size in bytes taken by all IPR datastructures
      #endif // IPR_TRACK_MEMORY_SIZE
      // <<<< Yuriy Solodkyy: 2010/06/17 
   }

                                // -- Various Location Types --
   // C++ constructs span locations.  There are at least four flavours of
   // locations:
   //       (a) physical source location;
   //       (b) logical source location;
   //       (c) physical unit location; and
   //       (d) logical unit location.
   // Physical and logical source locations are locations as witnessed
   // at translation phase 2 (see ISO C++, §2.1/1).
   // Physical and logical unit locations are locations as manifest when
   // looking at a translation unit, at translation phase 4.
   // 
   // Many IPR nodes will have a source and unit locations.  Instead of
   // storing a source file and unit name as a string in every
   // location data type, we save space by mapping file-names and
   // unit-names to IDs (integers).  That mapping is managed by the Unit
   // instance.
   struct Basic_location {
      int line;
      int column;

      Basic_location() : line(0), column(0) { }
   };

   struct Source_location : Basic_location {
      int file;                 // ID of the file
      Source_location() : file(0) { }
   };

   struct Unit_location : Basic_location {
      int unit;                 // ID of the unit
      Unit_location() : unit(0) { }
   };


   template<Category_code Cat, class T = Expr>
   struct Category : T {
   protected:
      Category(Category_code c = Cat) : T(c) { }
   };

                                // -- Sequence<> --
   // Often, we use a notion of sequence to represent intermediate
   // abstractions like base-classes, enumerators, catch-clauses,
   // parameter-type-list, etc.  A "Sequence" is a collection abstractly
   // described by its "begin()" and "end()" iterators.  It is made
   // abstract because it may admit different implementations depending
   // on concrete constraints.  For example, a scope (a sequence of
   // declarations, that additionally supports look-up by name) may
   // implement this interface either as a associative-array that maps
   // Names to Declarations (with no particular order) or as
   // vector<Declaration*> (in order of their appearance in programs), or
   // as a much more elaborated data structure.
   template<class T>
   struct Sequence  {
      struct Iterator;          // An iterator is a pair of (sequence,
                                // position).  The position indicates
                                // the particular value referenced in
                                // the sequence.

      // Provide STL-style interface, for use with some STL algorithm
      // helper classes.  Sequence<> is an immutable sequence.
      typedef T value_type;
      typedef const T& reference;
      typedef const T* pointer;
      typedef Iterator iterator;

      virtual int size() const = 0;
      virtual Iterator begin() const;
      virtual Iterator end() const;

      Iterator position(int) const;
      const T& operator[](int) const;

   protected:
      virtual const T& get(int) const = 0;
   };

                                // -- Sequence<>::Iterator --
   // This iterator class is as much as abstract as it could be, for
   // useful purposes.  It forwards most operations to the "Sequence"
   // class it provides a view for.
   template<class T>
   struct Sequence<T>::Iterator
      : std::iterator<std::bidirectional_iterator_tag, const T>  {

      Iterator() : seq(0), index(0) {}

      Iterator(const Sequence* s, int i) : seq(s), index(i) { }

      const T& operator*() const
      { return seq->get(index); }

      const T* operator->() const
      { return &seq->get(index); }

      Iterator& operator++()
      {
         ++index;
         return *this; 
      }

      Iterator& operator--()
      {
        --index;
        return *this;
      }

      Iterator operator++(int)
      {
         Iterator tmp = *this;
         ++index;
         return tmp;
      }

      Iterator operator--(int)
      {
        Iterator tmp = *this;
        --index;
        return tmp;
      }

      bool operator==(Iterator other) const
      { return seq == other.seq && index == other.index; }

      bool operator!=(Iterator other) const
      { return !(*this == other); }

   private:
      const Sequence* seq;
      int index;
   };

   template<class T>
   inline typename Sequence<T>::Iterator
   Sequence<T>::position(int i) const
   { return Iterator(this, i); }

   template<class T>
   inline typename Sequence<T>::Iterator
   Sequence<T>::begin() const
   { return Iterator(this, 0); }

   template<class T>
   inline typename Sequence<T>::Iterator
   Sequence<T>::end() const
   { return Iterator(this, size()); }

   template<class T>
   inline const T&
   Sequence<T>::operator[](int p) const
   { return get(p); }


                                // -- Unary<> --
   // A unary-expression is a specification of an operation that takes
   // only one operand, an expression.  By extension, a unary-node is a
   // node category that is essentially determined only by one node,
   // its "operand".  Usually, such an operand node is a classic expression.
   // Occasionally, it can be a type (e.g. sizeof (T)), we don't want to
   // loose that information, therefore we add a template-parameter
   // (the second) to indicate the precise type of the operand.  The first
   // template-parameter designates the actual node subcategory this class
   // provides interface for.
   template<class Cat, class Operand = const Expr&>
   struct Unary : Cat {
      typedef Operand Arg_type;
      virtual Operand operand() const = 0;
      Unary() {}
      Unary(Category_code c) : Cat(c) {}
   };

   
                                // -- Binary<> --
   // In full generality, a binary-expression is an expression that
   // consists in (a) an operator, and (b) two operands.  In Standard
   // C++, the two operands often are of the same type (and if they are not
   // they are implicitly converted).  In IPR, they need not be
   // of the same type.  This generality allows representations of
   // cast-expressions which are conceptually binary-expressions -- they
   // take a type and an expression.  Also, a function call is
   // conceptually a binary-expression that applies a function to
   // a list of arguments.  By extension, a binary node is any node that
   // is essentially dermined by two nodes, its "operands".
   // As for Unary<> nodes, we indicate the operands' type information
   // through the template-parameters First and Second.
   template<class Cat, class First = const Expr&, class Second = const Expr&>
   struct Binary : Cat {
      typedef First Arg1_type;
      typedef Second Arg2_type;
      virtual First first() const = 0;
      virtual Second second() const = 0;
      Binary() {}
      Binary(Category_code c) : Cat(c) {}
   };

                                // -- Ternary<> --
   // Similar to Unary<> and Binary<> categories.  This is for
   // ternary-expressions, or more generally for ternary nodes.
   // An example of a ternary node is a Conditional node.
   template<class Cat, class First = const Expr&,
            class Second = const Expr&, class Third = const Expr&>
   struct Ternary : Cat { 
      typedef First Arg1_type;
      typedef Second Arg2_type;
      typedef Third Arg3_type;
      virtual First first() const = 0;
      virtual Second second() const = 0;
      virtual Third third() const = 0;
      Ternary() {}
      Ternary(Category_code c) : Cat(c) {}
   };

                                // -- Quaternary --
   // Similar to Unary<>, Binary<> and Ternary<>.  Quaternary nodes are,
   // however rare in diversity.  Examples include function types.
   template<class Cat, class First = const Product&,
            class Second = const Type&, class Third = const Type&,
            class Fourth = const Linkage&>
   struct Quaternary : Cat {
      typedef First Arg1_type;
      typedef Second Arg2_type;
      typedef Third Arg3_type;
      typedef Fourth Arg4_type;
      virtual First first() const = 0;
      virtual Second second() const = 0;
      virtual Third third() const = 0;
      virtual Fourth fourth() const = 0;
      Quaternary() {}
      Quaternary(Category_code c) : Cat(c) {}
   };

                                // -- Node --
   // Universal base class of all IPR nodes, in the traditional
   // OO design sense.  Its primary purpose is to provide a hook
   // for the Visitor Design Pattern.
   struct Node {
      // This class does not have a declared virtual destructor
      // because we don't plan to have Nodes manage resources, and
      // therefore no deletion through pointers to this base class.

      const int node_id;        // unique node-identifier in a translation
                                // unit.  An integer data member is prefered
                                // over the address of the actual node
                                // for simplicity and persistency reasons.

      
      const Category_code category; // the category the complete node object
                                // belongs to.  In a sufficiently expressive
                                // and efficient type system, we would not
                                // need this member, for it could be read
                                // directly from the type of the object.
      

      // Hook for visitor classes.
      virtual void accept(Visitor&) const = 0;

// >>>> Yuriy Solodkyy: 2010/06/17
// Support for measuring how much memory IPR datastructures take
#ifdef IPR_TRACK_MEMORY_SIZE
      void* operator new  (size_t n)
      {
          stats::ipr_mem_size += n;
          return ::new char[n];
      }

      void* operator new  (size_t n, void* p)
      {
          return p; // Placement new, just return the argument
      }

      void* operator new[](size_t n)
      {
          stats::ipr_mem_size += n;
          return ::new char[n];
      }

      void* operator new[](size_t n, void* p)
      {
          return p; // Placement new, just return the argument
      }

      void  operator delete  (void* p)
      {
          return ::delete p;
      }

      void  operator delete  (void*, void*)
      {
          // Placement delete, do nothing
      }

      void  operator delete[](void* p)
      {
          return ::delete p;
      }

      void  operator delete[](void*, void*)
      {
          // Placement delete, do nothing
      }
#endif // IPR_TRACK_MEMORY_SIZE
// <<<< Yuriy Solodkyy: 2010/06/17 

   protected:
      // It is an error to create a complete object of this type.
      Node(Category_code);      // Used by derived classes.
   };

                                // -- String --
   // Strings in IPR are immutable, and therefore unified.  The constitute
   // the basic building block of many nodes.
   struct String : Category<string_cat, Node> {
      typedef const char* iterator;
      virtual int size() const = 0;
      virtual iterator begin() const = 0;
      virtual iterator end() const  = 0;
   };

                                // -- Comment --
   // This node represents comments, either C-style or BCPL-style.  Notice
   // that the comment delimiters are part of Comment::text.
   struct Comment : Unary<Category<comment_cat, Node>, const String&> {
      Arg_type text() const { return operand(); }
   };

                                // -- Linkage --
   // This node represent language linkage, e.g., "C" in `extern "C"'.
   // Ths ISO standard mandates at least two language linkages: "C" and
   // "C++".  If those were the only language linkage used in practice,
   // then, we would have just used a 2-valued enum.  However, some C++
   // implementations do have support for things like "Java" language
   // linkage and whatnot.  Consequently, the most general representation
   // is adopted.  From ISO C++ point of view, language linkage applies
   // only to function names, variable names, and function types.
   struct Linkage : Unary<Category<linkage_cat, Node>, const String&> {
      Arg_type language() const { return operand(); }
   };

                                // -- Annotation --
   // A pair "(name, value)" used to communicate information
   // between external tools that use IPR.
   struct Annotation : Binary<Category<annotation_cat, Node>,
                              const String&, const Literal&> {
      Arg1_type name() const { return first(); }
      Arg2_type value() const { return second(); }
   };

                                // -- Region --
   // A Region node represents a region of program text.  It is mostly
   // useful for capturing the notion of scope (in Standard C++ sense).
   // In IPR, we're using a generalized notion of Scope (a sequence of
   // declarations).  The  notion of Region helps making precise when
   // some implicit actions like cleanup-ups happen, or nesting of scopes.
   // The sequence of declarations appearing in a Region makes up the
   // Scope of that region.
   struct Region : Category<region_cat, Node> {
      typedef std::pair<Unit_location, Unit_location> Location_span;
      virtual const Location_span& span() const = 0;
      virtual const Region& enclosing() const = 0;
      virtual const Scope& bindings() const = 0;
      virtual const Expr& owner() const = 0;
   };

                                // -- Expr --
   // An expression is a sequence of operators and operands that specifies
   // a computation.  Such a computation can be static (constant,
   // object, function, type, template, namespace, concept) or dynamic
   // (object, function).  Every expression has a type.
   struct Expr : Node {
      virtual const Type& type() const = 0;

   protected:
      Expr(Category_code c) : Node(c)
      { }
   };

                                // -- Classic --
   // Classic expressions are those constructed with operators
   // directly expressible in standard C++.  Most of those operators
   // can be overloaded and given user-defined implementation. For
   // instantce,  cout << 2, involves a user-defined operator.  The IPR
   // representation is not a function call but, something like
   // 
   //                        << ---> implementation
   //                       /  \.
   //                    cout   2
   //
   // where we record the fact that the left-shift operator has a
   // user-defined menaning.  In IPR, we take the general apporach that
   // all operators can be given user-defined meanings.  Consequently,
   // we define this class to represent the idea that an expression
   // has a user-supplied meaning.
   struct Classic : Expr {
      // This is true if and only if, this is an overloaded operator
      // with user-defined declaration.
      virtual bool has_impl_decl() const = 0;

      // The declaration for the corresponding operator function.
      virtual const ipr::Decl& impl_decl() const = 0;

   protected:
      Classic(Category_code c) : Expr(c)
      { }
   };

                                // -- Name --
   // Standard C++ says that 'a name is a use of identifier to designate
   // an entity'.  In IPR we take the view that a name is a symbol
   // (or combination thereof) interpreted in their most abstract sense,
   // i.e. whose meaning depends on binding contexts (Scopes).
   // That definition accounts for the following (in the standard C++ sense)
   //    - unqualified-id                    -- Identifier
   //    - qualified-id                      -- Scope_ref
   //    - operator-function-id              -- Operator
   //    - conversion-function-id            -- Conversion
   //    - template-id                       -- Template_id
   //    - type-id                           -- Type_id
   // 
   // Most names are introduced by declarations into scope.
   // A Name is an Expression.  That obviates the needs for a
   // special node to turn names into Expressions, for the purpose
   // of symbolic interpretation.   
   struct Name : Expr {
      // At the moment, this class is empty because there is no
      // interesting operation that could be provided here without
      // imposing too much of implementation details.

   protected:
      Name(Category_code c) : Expr(c)
      { }
   };

                                // -- Identifier --
   // An identifier is a sequence of alphanumeric characters starting
   // with either a letter or an underbar ('_').
   struct Identifier : Unary<Category<identifier_cat, Name>, const String&> {
      // The character sequence of this identifier
      Arg_type string() const { return operand(); }
   };

                                // -- Operator --
   // For a function operator "operator @", "opname()" is the "@"
   // sub-part.  Notice that for the array forms of allocation
   // and deallocation operator functions, this is respectively
   // "new[]" and "delete[]", with no space before, betweeen, and after
   // the square brackets.
   struct Operator : Unary<Category<operator_cat, Name>, const String&> {
      Arg_type opname() const { return operand(); }
   };

                                // -- Conversion --
   // A conversion-function-id is the name of a user-defined
   // conversion function.
   struct Conversion : Unary<Category<conversion_cat, Name>, const Type&> {
      // The type this conversion-function converts values to.
      Arg_type target() const { return operand(); }
   };

                                // -- Scope_ref --
   // A qualified name of the form "scope::member".
   struct Scope_ref : Binary<Category<scope_ref_cat, Name> > {
      Arg1_type scope() const  { return first(); }
      Arg2_type member() const { return second(); }
   };

                                // -- Template_id --
   // A Template_id is a name of the form
   //    template-expr<template-argument-list>
   // which 'applies' a template to a template-argument list.
   struct Template_id : Binary<Category<template_id_cat, Name>,
                               const Name&, const Expr_list&> {
      Arg1_type template_name() const { return first(); }
      Arg2_type args() const { return second(); }
   };


                                // -- Ctor_name --
   // Standard C++ is not very consistent about constructor
   // name. On one hand, it says constructors do not have names;
   // but on another hand, there are clearly cases where they
   // act as if they had name.  For example, "S::S" designates
   // constructors of class S.  Similarly, if class S has a template
   // constructor then explicit or partial specializations need ways
   // to refer to particular set of specializations.  By consistency
   // with other declarations, and symmetry with destrcutors, we have
   // introduced this node class.
   struct Ctor_name : Unary<Category<ctor_name_cat, Name>, const Type&> {
      Arg_type object_type() const { return operand(); }
   };

                                // -- Dtor_name --
   // This node represent a destructor name of the form "~T", where T
   // is a type.
   struct Dtor_name : Unary<Category<dtor_name_cat, Name>, const Type&> {
      Arg_type object_type() const { return operand(); }
   };

                                // -- Rname --
   // This is the abstract, alpha-renaming independent, representation of
   // a parameter (of either a function or a template) in its de Brujin
   // level form.  Here we include type in the abstract-name.
   // The "level" is the nesting-depth, starting from 1, of the parameter.
   // the "position" is its position, starting from 0, in the
   // parameter-list at a given level.
   struct Rname : Ternary<Category<rname_cat, Name>, const Type&, int, int> {
      // Although the type() of an Rname is its first operand, we don't define
      // that function here, for it would involve a two indirections to
      // to give the result.  Consequently we define it in the implemenation
      // part.
      // pmp: swapped level and position to return 2nd and 3rd
      Arg2_type level() const { return second(); }
      Arg3_type position() const { return third(); }
   };

                                // -- Overload --
   // An overload-set is an expression whose value is the set of all
   // declarations for a name in a given scope.  An overload-set supports
   // look-up by type.  The result of such lookup is the sequence of
   // declarations of that name with that type.
   struct Overload : Sequence<Decl>, Category<overload_cat> {
      // Bring Sequence<Decl>::operator[] into
      // scope before overloading.
      using Sequence<Decl>::operator[];
      virtual const Sequence<Decl>& operator[](const Type&) const = 0;
   };

                                // -- Scope -- 
   // A "declaration" is a type specification for a name. A "Scope" is
   // a "sequence" of declarations, that additionally supports "lookup"
   // by "Name".  A name may have more than one declarations in a given
   // scope; such a name is said "overloaded".  The result of
   // looking up a name in a scope is a set of all declarations, called
   // an "overload set", for that name in that scope.  An "overload set"
   // is a "sequence" of declarations, that additionaly supports
   // lookup by "Type".
   struct Scope : Category<scope_cat> {
      typedef Sequence<Decl>::Iterator Iterator;

      // The sequence of declarations this scope contain.
      virtual const Sequence<Decl>& members() const = 0;

      // The region that determine this scope.
      // virtual const Region& region() const = 0;
      
      // Look-up by name returns the overload-set of all declarations,
      // for the subscripting name, contained in this scope.
      virtual const Overload& operator[](const Name&) const = 0;

      // How may declarations are there in this Scope.
      int size() const { return members().size(); }

      Iterator begin() const { return members().begin(); }
      Iterator end() const { return members().end(); }
      const Decl& operator[](int i) const { return members()[i]; }
      
      // The enclosing scope, if any of this scope.
      // virtual const Scope& enclosing() const = 0;
//       const Scope& enclosing() const
//       {
//          return region().enclosing().bindings();
//       }

      // The owner (e.g. class, namespace, ...) of this scope.  This
      // is a convenient function, given Region::owner.
//      const Expr& owner() const { return region().owner(); }
   };

                                // -- General types -- 
   // A type is a collection of constraints and operations that preserve
   // some invariants.  Since a Type is also an Expression, it has a type.
   // A type of a type is what we call a "concept".  A type in IPR has a
   // much broader significance than Standard C++ types (henceforth called
   // "classic type").  In particular, in IPR, namespace is a type.
   // Simirlary, an overload-set has a type.
   struct Type : Expr {
      // cv-qualifiers are actually type operators.  Much of these operators
      // currently make sense only for classic type -- although it might
      // be interesting to explore the notion of pointer to overload-set
      // or reference to such an expression.
      enum Qualifier { 
         None             = 0,
         Const            = 1 << 0,
         Volatile         = 1 << 1,
         Restrict         = 1 << 2        // not standard C++
      };

      // A type can be decomposed into two canonical components:
      //   (1) its cv-qualifiers;
      //   (2) the cv-unqualified part, also called the "main variant".
//      virtual Qualifier qualifiers() const = 0;
//      virtual const Type& main_variant() const = 0;

      // All types have have names.
      virtual const Name& name() const = 0;

   protected:
      Type(Category_code c) : Expr(c)
      { }
   };

   // These overloads for Type::Qualifier are necessary because there
   // is no implicit conversion from integers to enums in C++.
   inline Type::Qualifier
   operator|(Type::Qualifier a, Type::Qualifier b)
   {
      return Type::Qualifier(int(a) | int(b));
   }

   inline Type::Qualifier&
   operator|=(Type::Qualifier& a, Type::Qualifier b)
   {
      return a = Type::Qualifier(int(a) | int(b));
   }

   inline Type::Qualifier
   operator&(Type::Qualifier a, Type::Qualifier b)
   {
      return Type::Qualifier(int(a) & int(b));
   }

   inline Type::Qualifier&
   operator&=(Type::Qualifier& a, Type::Qualifier b)
   {
      return a = Type::Qualifier(int(a) & int(b));
   }

   inline Type::Qualifier
   operator^(Type::Qualifier a, Type::Qualifier b)
   {
      return Type::Qualifier(int(a) ^ int(b));
   }

   inline Type::Qualifier&
   operator^=(Type::Qualifier& a, Type::Qualifier b)
   {
      return a = Type::Qualifier(int(a) ^ int(b));
   }
                                // -- Type_id --
   // This node is used for elaborated expressions that designate types.
   // For example, "const T*" is a Type_id , so is "int (T&)".
   struct Type_id : Unary<Category<type_id_cat, Name>, const Type&> {
      Arg_type type_expr() const { return operand(); }
   };

                                // -- Array --
   // An array-type describes object expressions that designate C-style
   // homogenous object containers that meet the random-access
   // requirements.  When an array-type is declared with unspecified
   // bound, "bound()" returns a null-expression.
   // An alternate design choice would have been to have a predicate
   // "has_unknown_bound()", which when true would make "bound()" throw
   // an exception if accessed.
   struct Array : Binary<Category<array_cat, Type>, const Type&> {
      Arg1_type element_type() const { return first(); }
      Arg2_type bound() const        { return second(); }
   };

                                // -- As_type --
   // This node represents the use of a general expression as
   // a type.  Such situation arises in cases where a declaration
   // node can be used to designate a type, as in
   // \code
   //    struct S;
   //    typedef int count;
   //    typename T::size_type s = 90;
   //    template<typename T, T t> ...
   // \endcode
   // Some C++ implementations define "extended built-in" types with
   // language linkage, e.g. `extern "Java"' or `extern "Fortran"', for
   // interoperating with other languages.  The current representation
   // includes language linkage as integral part of As_type.
   struct As_type : Binary<Category<as_type_cat, Type>,
                           const Expr&, const Linkage& > {
      Arg1_type expr() const { return first(); }
      Arg2_type lang_linkage() const { return second(); }
   };

                                // -- Decltype --
   // This node represents query for the "generalized declared type"
   // of an expression.  Likely C++0x extension.
   struct Decltype : Unary<Category<decltype_cat, Type>, const Expr&> {
      Arg_type expr() const { return operand(); }
   };

                                // -- Function --
   // This node class represents a Type that describes an expression
   // that refers to a function.  In full generality, a template
   // is a also a function (it describes Type-valued functions);
   // however, we've made a special node for template.  ISO C++ specifies
   // that function types have language linkages and two function types
   // with different language linkages are different.
   struct Function : Quaternary<Category<function_cat, Type> > {
      // Parameter-type-list of a function of this type.  In full
      // generality, this also describes template signature.
      Arg1_type source() const { return first(); }

      // return-type
      Arg2_type target() const { return second(); }

      // list of exception types a function of this type may throw
      Arg3_type throws() const { return third(); }

      // The language linkage for this function type.  For most function
      // types, it is C++ -- for it is the default.
      Arg4_type lang_linkage() const { return fourth(); }
   };

                                // -- Pointer --
   // A pointer-type is type that describes an Address node.
   struct Pointer : Unary<Category<pointer_cat, Type>, const Type&> {
      // The type of the entity whose address an object of this
      // type may hold. 
      Arg_type points_to() const { return operand(); }
   };

                                // -- Product --
   // A Product represents a Cartesian product of Types.  Pragmatically,
   // it may be viewed as a Sequence of Types.  It is a Type.
   struct Product : Unary<Category<product_cat, Type>, const Sequence<Type>&> {
      Arg_type elements() const { return operand(); }
      int size() const { return elements().size(); }
      const Type& operator[](int i) const { return elements()[i]; }
   };

                                // -- Ptr_to_member --
   // This is for pointer-to-member type, e.g. int A::* or void (A::*)().
   // A pointer to member really is not a pointer type, it is much closer
   // a pair of a type and offset that usual pointer types.
   struct Ptr_to_member : Binary<Category<ptr_to_member_cat, Type>,
                                 const Type&, const Type&> {
      Arg1_type containing_type() const { return first(); }
      Arg2_type member_type() const { return second(); }
   };

                                // -- Qualified --
   // A cv-qualified type.  Representing a cv-qualified type with all
   // available information attainable in at most one indirection is
   // very tricky. Consequently, we currently represents a cv-qualified
   // type with a separate node as a binary operator.  Notice that we
   // maintain the invariant
   //     Qualified(cv2, Qualified(cv1, T)) = Qualified(cv1 | cv2, T)
   // In particular, the Qualified::main_variant is never a Qualified node.
   // We also maintain the invariant that Qualified::qualifiers is never
   // Type::None, consequently it is an error to attempt to create such a node.
   struct Qualified : Binary<Category<qualified_cat, Type>,
                             ipr::Type::Qualifier, const Type&> {
      Arg1_type qualifiers() const { return first(); }
      Arg2_type main_variant() const { return second(); }
   };

                                // -- Reference --
   // A reference-type describes an expression that acts like an alias
   // for a object or function.  However, unlike a pointer-type, it is
   // not an object-type.
    struct Reference : Unary<Category<reference_cat, Type>, const Type&> {
       // The type of the object or function an expression of this
       // type refers to.  
       Arg_type refers_to() const { return operand(); }
    };

                                // -- Rvalue_reference --
   // An rvalue-reference-type to support move semantics in C++0x.
    struct Rvalue_reference : Unary<Category<rvalue_reference_cat, Type>,
                                    const Type&> {
       // The type of the object or function an expression of this
       // type refers to.  
       Arg_type refers_to() const { return operand(); }
    };

                                // -- Sum --
   // A Sum type represents a distinct union of types.  This is currently
   // used only for exception specification in Function type.
   struct Sum : Unary<Category<sum_cat, Type>, const Sequence<Type>&> {
      Arg_type elements() const { return operand(); }
      int size() const { return elements().size(); }
      const Type& operator[](int i) const { return elements()[i]; }
   };

                                // -- Template --
   // This represents the type of a template declaration.  In the near future,
   // when "concepts" are integrated, it will become a Ternary node where the
   // third operand will represent the "where-clause".
   struct Template : Binary<Category<template_cat, Type>,
                            const Product&, const Type&> {
      // The constraints or types of the template-parameters.
      const Product& source() const { return first(); }
      // The type of the instantiation result.
      const Type& target() const { return second(); }
   };

                                // -- Udt --
   // Base class for user-defined types Nodes -- factor our common properties.
   struct Udt : Type {
      // The region delimited by the definition of this Udt.
      virtual const Region& region() const = 0;
      const Scope& scope() const { return region().bindings(); }

   protected:
      // It is an error to create a node of this type.
      Udt(Category_code c) : Type(c) // Used by derived classes.
      { }
   };

                                // -- Namespace --
   // The type of a Standard C++ namespace.  A namespace is primarily
   // interesting because it is a sequence of declarations that can be
   // given a name.
   struct Namespace : Udt {
      typedef Decl Member;      // -- type of members of this type.
      const Sequence<Decl>& members() const { return scope().members(); }
   // >>>> 2011-08-10 by Yuriy
   // Added for experiments with closed case of pattern matching
   protected:
      Namespace(Category_code c = namespace_cat) : Udt(c) {}
   // <<<< 2011-08-10 by Yuriy
   };

                                // -- Class --
   struct Class : Category<class_cat, Udt> {
      typedef Decl Member;      // -- type of members of this type.
      const Sequence<Decl>& members() const { return scope().members(); }
      virtual const Sequence<Base_type>& bases() const = 0;
   };

                                // -- Union --
   struct Union : Category<union_cat, Udt> {
      typedef Decl Member;      // -- type of members of this type.
      const Sequence<Decl>& members() const { return scope().members(); }
   };

                                // -- Enum --
   // An enumration is an object-type whose members are named constants
   // the definitions of which as part of the definition of the enumeration
   // itself.  By historical accident, enumerators are not "properly scoped".
   struct Enum : Category<enum_cat, Udt> {
      typedef Enumerator Member;      // -- type of members of this type.
      virtual const Sequence<Enumerator>& members() const = 0;
   };


                                // -- Phantom --
   // This nodes represents the "bound" of an array type with
   // unknown bound, as in "unsigned char charset[];"
   // We do not unify Phantom expressions, as two arrays with
   // unknown bounds may not designate the same type.
   struct Phantom : Category<phantom_cat, Expr> { };

                                // -- Address --
   // Address-of expression -- "&expr"
   struct Address : Unary<Category<address_cat, Classic> > { };

                                // -- Array_delete --
   // Array-form of delete-expression --  "delete[] p"
   struct Array_delete : Unary<Category<array_delete_cat,Classic> > {
      const Expr& storage() const { return operand(); }
   };

                                // -- Complement --
   // Complement-expression -- "~expr"
   struct Complement : Unary<Category<complement_cat,Classic> > { };

                                // -- Delete --
   // Delete-expression -- "delete p"
   struct Delete : Unary<Category<delete_cat,Classic> > {
      const Expr& storage() const { return operand(); }
   };

                                // -- Deref --
   // Dereference-expression -- "*expr"
   struct Deref : Unary<Category<deref_cat, Classic> > { };

                                // -- Paren_expr --
   // Parenthesized-expressions -- "(expr)".  This might seem purely
   // syntactical but it also has semantic implications, like when an
   // argument-dependent lookup should be done or not, or on the accuracy
   // of an expression evaluation.
   struct Paren_expr : Unary<Category<paren_expr_cat, Classic> > {
      const Expr& expr() const { return operand(); }
   };

                                // -- Expr_list --
   // A sequence of expressions "(e1, e2, ..., eN)".  This form of expression
   // is a tuple of expressions.  In particular, an Expr_list is different
   // from a Comma expression -- where each sub-expression is evaluated,
   // discarded except the last one.  The type of an Expr_list
   // is a Product.
   struct Expr_list : Unary<Category<expr_list_cat>, const Sequence<Expr>&> {
      Arg_type elements() const { return operand(); }
      int size() const { return elements().size(); }
      const Expr& operator[](int i) const { return elements()[i]; }
   };

                                // -- Expr_sizeof --
   // sizeof-expression -- "sizeof (expr)"
   struct Expr_sizeof : Unary<Category<expr_sizeof_cat, Classic> > { };

                                // -- Expr_typeid --
   // typeid-expression -- "typeid (expr)"
   struct Expr_typeid : Unary<Category<expr_typeid_cat, Classic> > { };

                                // -- Id_expr --
   // This node represents use of a name to designate an entity.
   // FIXME: Explain how useful this is and when it is used.
   struct Id_expr : Unary<Category<id_expr_cat, Name>, const Name&> {
      virtual const Decl& resolution() const = 0;
      Arg_type name() const { return operand(); }
   };

                                // -- Label --
   // label-expression.  Appears in goto-statements.
   struct Label : Unary<Category<label_cat>, const Identifier&> {
      Arg_type name() const { return operand(); }
   };

                                // -- Not --
   // logical-not-expression -- "!expr"
   struct Not : Unary<Category<not_cat, Classic> > { };

                                // -- Post_decrement --
   // post-decrement-expression -- "expr--".
   struct Post_decrement : Unary<Category<post_decrement_cat,Classic> > { };
   
                                // -- Post_increment --
   // post-increment-expression -- "expr++".
   struct Post_increment : Unary<Category<post_increment_cat, Classic> > { };

                                // -- Pre_decrement --
   // pre-decrement-expression -- "--expr".
   struct Pre_decrement : Unary<Category<pre_decrement_cat, Classic> > { };

                                // -- Pre_increment --
   // pre-increment-expression -- "++expr".
   struct Pre_increment : Unary<Category<pre_increment_cat, Classic> > { };

                                // -- Throw --
   // A node that represents a C++ expression of the form `throw ex'.
   // As a special case, when exception() is a null-expression, then
   // this node represents the flow-control "throw;", which actually
   // could be represented by a dedicated statement node, Rethrow for
   // instance.
   struct Throw : Unary<Category<throw_cat, Classic> > {
      const Expr& exception() const { return operand(); }
   };

                                // -- Type_sizeof --
   // sizeof-expression -- "sizeof (type)"
   struct Type_sizeof : Unary<Category<type_sizeof_cat, Classic>, const Type&> { };

                                // -- Type_typeid --
   // typeid-expression -- "typeid (type)"
   struct Type_typeid : Unary<Category<type_typeid_cat, Classic>, const Type&> { };

                                // -- Unary_minus --
   // unary-minus-expression -- "-expr"
   struct Unary_minus : Unary<Category<unary_minus_cat, Classic> > { };

                                // -- Unary_plus --
   // unary-plus-expression -- "+expr"
   struct Unary_plus : Unary<Category<unary_plus_cat, Classic> > { };

                                // -- Member_selection<> --
   // This class factorizes the commonalities of various object member
   // selection operation.
   template<Category_code Cat>
   struct Member_selection : Binary<Category<Cat, Classic> > {
      const Expr& base() const { return this->first(); }
      const Expr& member() const { return this->second(); }
   };

                                // -- Cast_expr<> --
   // This classes factorizes the commonalities of various cast-expressions
   template<Category_code Cat>
   struct Cast_expr : Binary<Category<Cat, Classic>, const Type&> {
      // The type() of a cast expression is its first() operand.  However,
      // we do not define (override) that member here, as it implies two
      // indirections to get the information.  Therefore, it is better
      // to define it at the implementation side where we have all the bits
      // necessary to make it a single indirection.

      const Expr& expr() const { return this->second(); }
   };

                                // -- Plus --
   // Addition-expression -- "a + b"
   struct Plus : Binary<Category<plus_cat, Classic> > { };

                                // -- Plus_assign --
   // In-place addition-expression -- "a += b"
   struct Plus_assign : Binary<Category<plus_assign_cat, Classic> > { };

                                // -- And --
   // Logical-and-expression -- "a && b"
   struct And : Binary<Category<and_cat, Classic> > { };

                                // -- Array_ref --
   // This is for an expression that designate a particular slot
   // in an array expression `array[slot]'.
   struct Array_ref : Member_selection<array_ref_cat> { };

                                // -- Arrow --
   // This node type models object member selection, based on pointer,
   // using the arrow-notation.  See `Dot'.
   struct Arrow : Member_selection<arrow_cat> { };

                                // -- Arrow_star --
   // Member selection through pointer to member -- "p->*"
   struct Arrow_star : Member_selection<arrow_star_cat> { };

                                // -- Assign --
   // Assignment-expression -- "a = b"
   struct Assign : Binary<Category<assign_cat, Classic> > { };

                                // -- Bitand --
   // Bit-and-expression -- "a & b"
   struct Bitand : Binary<Category<bitand_cat, Classic> > { };

                                // -- Bitand_assign --
   // In-place bit-and-expression -- "a &= b"
   struct Bitand_assign : Binary<Category<bitand_assign_cat, Classic> > { };

                                // -- Bitor --
   // Bit-or expression -- "a | b"
   struct Bitor : Binary<Category<bitor_cat, Classic> > { };

                                // -- Bitor_assign --
   // In-place bit-or-expression -- "a |= b"
   struct Bitor_assign : Binary<Category<bitor_assign_cat, Classic> > { };

                                // -- Bitxor --
   // Exclusive bit-or-expression -- "a ^ b"
   struct Bitxor : Binary<Category<bitxor_cat, Classic> > { };

                                //  -- Bitxor_assign --
   // In-place exclusive bit-or-expression -- "a ^= b"
   struct Bitxor_assign : Binary<Category<bitxor_assign_cat, Classic> > { };

                                //  -- Cast --
   // An expression of the form "(type) expr", representing the
   // old-style/C-style cast.
   struct Cast : Cast_expr<cast_cat> { };

                                // -- Call --
   // A function call with an argument list.  Notice that in the abstract
   // this is not really different from an Template_id.  However, having
   // a separate node means less cluter in codes.
   struct Call : Binary<Category<call_cat, Classic>,
                         const Expr&, const Expr_list&> {
      Arg1_type function() const  { return first(); }
      Arg2_type args() const { return second(); }
   };

                                // -- Comma --
   // comma-expression -- "a, b"
   struct Comma : Binary<Category<comma_cat, Classic> > { };

                                // -- Const_cast --
   // const_cast-expression -- "const_cast<type>(expr)".
   struct Const_cast : Cast_expr<const_cast_cat> { };

                                // -- Datum --
   // An expression of the form "T(e1, e2, .. en)" where "T"
   // is a type and "ei"s are expressions.  This is not a function
   // call -- although syntactically it looks like so.  "T" will
   // be the `type()' of this expression.
   struct Datum : Binary<Category<datum_cat, Classic>,
                          const Type&, const Expr_list&> {
      // See comments for the various cast operators regarding type().

      Arg2_type args() const { return second(); }
   };

                                // -- Div --
   // Division-expression -- "a / b"
   struct Div : Binary<Category<div_cat, Classic> > { };

                                //  -- Div_assign --
   // In-place division-expression -- "a /= b"
   struct Div_assign : Binary<Category<div_assign_cat, Classic> > { };

                                // -- Dot --
   // This node type represents a member selection on object using
   // the dot-notation "object.member": both "object" and "member"
   // can be general expressions.
   struct Dot : Member_selection<dot_cat> { };

                                // -- Dot_star --
   // An expression of the form "object.*pm".
   struct Dot_star : Member_selection<dot_star_cat> { };

                                // -- Dynamic_cast --
   // An expression of the from "dynamic_cast<type>(expr)".
   struct Dynamic_cast : Cast_expr<dynamic_cast_cat> { };

                                // -- Equal --
   // Equality-comparison-expression -- "a == b"
   struct Equal : Binary<Category<equal_cat, Classic> > { };

                                // -- Greater --
   // greater-comparison-expression -- "a > b"
   struct Greater : Binary<Category<greater_cat, Classic> > { };

                                // -- Greater_equal --
   // greater-or-equal-comparison-expression -- "a >= b"
   struct Greater_equal : Binary<Category<greater_equal_cat, Classic> > { };

                                // -- Less --
   // less-comparison-expression -- "a < b"
   struct Less : Binary<Category<less_cat, Classic> > { };

                                // -- Less_equal --
   // less-equal-comparison-expression -- "a <= b"
   struct Less_equal : Binary<Category<less_equal_cat, Classic> > { };
   
                                // -- Literal --
   // An IPR literal is just like a standard C++ literal. 
   struct Literal : Binary<Category<literal_cat, Classic>,
                           const Type&, const String&> {
      // See comments for the cast operators regarding type().

      // The texttual representation of this literal as it appears
      // in the program text.
      Arg2_type string() const { return second(); }
   };

                                // -- Mapping --
   // This node represents a parameterized expression.
   // Its type is a Function in case of parameterized classic expression,
   // and Template otherwise.
   struct Mapping : Category<mapping_cat> {
      virtual const Parameter_list& params() const = 0;
      virtual const Type& result_type() const = 0;
      virtual bool  has_result() const = 0;
      virtual const Expr& result() const = 0;

      // Mappings may have nested mappings (e.g. member templates of
      // templates)
      // The depth of a template declaration is its nesting nevel.
      virtual int depth() const = 0;
   };

                                // -- Member_init --
   // Node that represent a member initlization, in constructor
   // definition.
   struct Member_init : Binary<Category<member_init_cat> > {
      Arg1_type member() const { return first(); }
      Arg2_type initializer() const { return second(); }
   };

                                // -- Minus --
   // Subtraction-expression -- "a - b"
   struct Minus : Binary<Category<minus_cat, Classic> > { };

                                //  -- Minus_assign --
   // In-place subtraction-expression -- "a -= b".
   struct Minus_assign : Binary<Category<minus_assign_cat, Classic> > { };

                                // -- Modulo --
   // Modulo-expression -- "a % b"
   struct Modulo : Binary<Category<modulo_cat, Classic> > { };

                                // -- Modulo_assign --
   // In-place modulo-expression -- "a %= b"
   struct Modulo_assign : Binary<Category<modulo_assign_cat, Classic> > { };
   
                                // -- Mul --
   // Multiplication-expression -- "a * b"
   struct Mul : Binary<Category<mul_cat, Classic> > { };

                                // -- Mul_assign --
   // In-place multiplication-expression -- "a *= b"
   struct Mul_assign : Binary<Category<mul_assign_cat, Classic> > { };

                                // -- Not_equal --
   // Inequality-comparison-expression -- "a != b"
   struct Not_equal : Binary<Category<not_equal_cat, Classic> > { };

                                // -- Or --
   // logical-or-expression -- "a || b"
   struct Or : Binary<Category<or_cat, Classic> > { };

                                // -- Reinterpret_cast --
   // An expression of the form "reinterpret_cast<type>(expr)"
   struct Reinterpret_cast : Cast_expr<reinterpret_cast_cat> { };

                                // -- Lshift --
   // left-shift-expression -- "a << b"
   struct Lshift : Binary<Category<lshift_cat, Classic> > { };

                                // -- Lshift_assign --
   // In-place left-shift-expression -- "a <<= b".
   struct Lshift_assign : Binary<Category<lshift_assign_cat, Classic> > { };

                                // -- Rshift --
   // Right-shift-expression -- "a >> b"
   struct Rshift : Binary<Category<rshift_cat, Classic> > { };

                                // -- Rshift_assign --
   // In-place right-shift-expression -- "a >>= b"
   struct Rshift_assign : Binary<Category<rshift_assign_cat, Classic> > { };

                                // -- Static_cast --
   // An expression of the form "static_cast<type>(expr)".
   struct Static_cast : Cast_expr<static_cast_cat> { };

                                // -- New --
   // This node represents a new-expression:
   //    ::_opt new new-placement_opt new-type-id new-initializer_opt
   //    ::_opt new new-placement_opt ( type-id ) new-initializer_opt
   struct New : Ternary<Category<new_cat, Classic>,
                        const Expr_list&, const Type&, const Expr_list&> {
      // use_placement is true when this new-expression uses a placement.
      bool use_placement() const { return first().size() != 0; }
      Arg1_type placement() const { return first(); }
      
      Arg2_type allocated_type() const { return second(); }
      
      // has_initializer is true when the new-expression specifies
      // an initializer-list
      bool has_initializer() const { return third().size() != 0; }
      Arg3_type initializer() const { return third(); }
   };

                                // -- Conditional --
   // This represents the C++ ternary operator ?:.  In principle,
   // it is redundant with both If_then_else and Swicth nodes
   // from semantics point of view.  However, if we want to preserve
   // the syntax, then it is necessary to have it.
   struct Conditional : Ternary<Category<conditional_cat, Classic> > {
      Arg1_type condition() const { return first(); }
      Arg2_type then_expr() const { return second(); }
      Arg3_type else_expr() const { return third(); }
   };

                                // -- Parameter_list --
   // FIXME: Parameter_list should have its category ste properly.
   struct Parameter_list : Region, Sequence<Parameter> { };

                                // -- Stmt -- 
   // The view that a statement is kind of expression does not follow from
   // Standard C++ specification.  It is a design choice we made based on
   // the following facts:
   //    1. We need to have an IPR node that can represent both a
   //       declaration and an expression.  That need is examplified
   //       by constructs like
   //            if (std::cin >> c) { /* .... */ }
   //       and
   //            if (Call* c = dynamic_cast<Call*>(e)) { /* ... */ }
   //       In the first case we have an expression and in the second
   //       case we have a declaration.  Therefore, we have a diagram
   //       that loooks like
   // 
   //              ,----> E.D. <----,
   //              |                |
   //          Expression      Declaration
   // 
   //
   //    2. Standard C++ says some declarations are statements, and when
   //       a declaration declares a variable with initialization, the
   //       initializer usually requests code execution.  Therefore, we
   //       chose to say that a declaration is a statement and we get
   //       a similar diagram
   // 
   //              ,----> D.S. <----,
   //              |                |
   //         Declaration       Statement
   // 
   //
   // When combined, the two diagrams yield
   // 
   //              ,----> E.D. <---- ----> D.S <----,
   //              |                |               |
   //          Expression      Declaration      Statement
   // 
   //
   // Somehow, we have to satisfy that equation.  In a previous design
   // we chose what seemed to be the simplest, i.e. deriving a Statement
   // from an Expr and a Declaration from a Statement.  Doing so
   // however led to numerous questions about some very basic operations on
   // expressions like `arity()' or `type()'.
   //
   // In a second design, we derived a Statement from Node and a
   // Decl from both Expr and Statement.  This led to complications in the
   // class hierarchy and unnecessary complexities.  Therefore, we're back to
   // the view that a statement is an expression, with some simplifications.
   struct Stmt : Expr {
      // The location of this statement in its unit.
      virtual const Unit_location& unit_location() const = 0;
      virtual const Source_location& source_location() const = 0;
      // virtual bool has_source_location() const = 0;
      virtual const Sequence<Annotation>& annotation() const = 0;

   protected:
      Stmt(Category_code c) : Expr(c)
      { }
   };

                                // -- Expr_stmt --
   // This node class represents an expression statement, e.g.
   //    std::cout << "Hello World" << std::endl;
   // "expr()" is the Expression to evaluate.
   struct Expr_stmt : Unary<Category<expr_stmt_cat, Stmt> > {
      Arg_type expr() const { return operand(); }
      Expr_stmt(Category_code c = expr_stmt_cat) : Unary<Category<expr_stmt_cat, Stmt> >(c) {}
   };

                                // -- Labeled_stmt --
   // This node is for labeled-statement:
   //   - identifier : statement
   //   - case constant-expression : statement
   //   - default : statement
   // IPR represents that "label()" as an expressions so that it can
   // handle "identifier", "case cst-expr" and "default", where cst-expr
   // is an abritrary constant-expression.
   struct Labeled_stmt : Binary<Category<labeled_stmt_cat, Stmt>,
                                const Expr&, const Stmt&> {
      const Expr& label() const { return first(); }
      const Stmt& stmt() const { return second(); }
   };

                                // -- Block --
   // A Block is any sequence of statements bracketed by curly braces.
   struct Block : Category<block_cat, Stmt> {
      virtual const Scope& members() const = 0;
      virtual const Sequence<Stmt>& body() const = 0;
      virtual const Sequence<Handler>& handlers() const = 0;
   };

                                // -- Ctor_body --
   // This node represents a construtor "body" consisting of:
   //      (a) member-initializer list, if present; and
   //      (b) the actual body of the constructor
   struct Ctor_body : Binary<Category<ctor_body_cat, Stmt>,
                             const Expr_list&, const Block&> {
      Arg1_type inits() const { return first(); }
      Arg2_type block() const { return second(); }
   };

                                // -- If_then
   // A classic if-statement without the "else" part.
   struct If_then : Binary<Category<if_then_cat, Stmt>,
                           const Expr&, const Stmt&> {
      Arg1_type condition() const { return first(); }
      Arg2_type then_stmt() const { return second(); }
   };

                                // -- If_then_else --
   // A classic full is-statement.
   struct If_then_else : Ternary<Category<if_then_else_cat, Stmt>,
                                 const Expr&, const Stmt&, const Stmt&> {
      Arg1_type condition() const { return first(); }
      Arg2_type then_stmt() const { return second(); }
      Arg3_type else_stmt() const { return third(); }
   };

                                // -- Switch --
   // A classic switch-statement.
   struct Switch : Binary<Category<switch_cat, Stmt>,
                          const Expr&, const Stmt&> {
      Arg1_type condition() const { return first(); }
      Arg2_type body() const { return second(); }
   };

                                // -- While --
   // A classic while-statement
   struct While : Binary<Category<while_cat, Stmt>,
                         const Expr&, const Stmt&> {
      Arg1_type condition() const { return first(); }
      Arg2_type body() const { return second(); }
   };

                                // -- Do --
   // A classic do-statement
   struct Do : Binary<Category<do_cat, Stmt>, const Expr&, const Stmt&> {
      Arg1_type condition() const { return first(); }
      Arg2_type body() const { return second(); }
   };

                                // -- For --
   // A classic for-statement
   struct For : Category<for_cat, Stmt> {
      virtual const Expr& initializer() const = 0;
      virtual const Expr& condition() const = 0;
      virtual const Expr& increment() const = 0;
      virtual const Stmt& body() const = 0;
   };

                                // -- For_in --
   struct For_in : Category<for_in_cat, Stmt> {
      virtual const Var& variable() const = 0;
      virtual const Expr& sequence() const = 0;
      virtual const Stmt& body() const = 0;
   };

                                // -- Break --
   // A classic break-statement.
   struct Break : Category<break_cat, Stmt> {
      virtual const Stmt& from() const = 0;
   };

                                // -- Continue --
   // A classic continue-statement.
   struct Continue : Category<continue_cat, Stmt> {
      virtual const Stmt& iteration() const = 0;
   };

                                // -- Goto --
   // A classic goto-statement.
   struct Goto : Unary<Category<goto_cat, Stmt> > {
      Arg_type target() const { return operand(); }
   };

                                // -- Return --
   // A classic return-statement.
   struct Return : Unary<Category<return_cat, Stmt> > {
      Arg_type value() const { return operand(); }
   };

                                // -- Handler --
   // This represents a catch-clause.  Notice that there is no node
   // for "try" as every Block is implicitly a try-block.  Ideally, we
   // should have every expression as a "try-block".
   struct Handler : Binary<Category<handler_cat, Stmt>,
                           const Decl&, const Block&> {
      Arg1_type exception() const { return first(); }
      Arg2_type body() const { return second(); }
   };

                                // -- Substitution --
   // A "Substitution" is a mapping of a parameter to an expression
   // (its value).  It is mainly convenient for representing individual
   // template-argument used to instantiate a template.  Thus, a
   // template-argument list is a sequence of Substitutions.
   struct Substitution {
      Substitution(const Parameter& p, const Expr& x)
            : var(&p), expr(&x) { }

      const Parameter& param() const { return *var; }
      const Expr& value() const { return *expr; }

   private:
      const Parameter* var;
      const Expr* expr;
   };

                                // -- Decl --
   // Only declaration-statements are statements.  But we find it simpler
   // just to take the general rule that a declaration is a statement.
   // However it should be observed that declarations like
   //   (a) function-definition
   //   (b) template-declaration
   //   (c) explicit-instantiation
   //   (d) explicit-specialization
   //   (e) linkage-specification
   //   (f) namespace-definition
   // are not statement in Standard C++ sense.
   // 
   // We take the view that all declarations can be parameterized.
   // Therefore, declaration nodes potentially are instantiations of a
   // a template given by "pattern()", with a set of template-arguments,
   // given by "substitutions()".  A declaration for which we have
   // an empty substitutions() is either
   //     (a) a primary template; or
   //     (b) a non-template declaration.
   struct Decl : Stmt {
      enum Specifier {
         None = 0,
         Auto       = 1 << 0,
         Register   = 1 << 1,
         Static     = 1 << 2,
         Extern     = 1 << 3,
         Mutable    = 1 << 4,
         StorageClass  = Auto | Register | Static | Extern | Mutable,

         Inline     = 1 << 5,
         Virtual    = 1 << 6,   // also used as storage class specifier
                                // for virtual base subobjects
         Explicit   = 1 << 7,
         Pure       = 1 << 8,
         FunctionSpecifier = Inline | Virtual | Explicit | Pure,
      
         Friend     = 1 << 9,
         Typedef    = 1 << 10,

         Public     = 1 << 11,
         Protected  = 1 << 12,
         Private    = 1 << 13,
         AccessProtection = Public | Protected | Private,

         Export     = 1 << 14,  // for exported template declarations.
         Constexpr  = 1 << 15   // C++ 0x
      };

      virtual Specifier specifiers() const = 0;
      virtual const Linkage& lang_linkage() const = 0;

      virtual const Name& name() const = 0;

      // The region where the declaration really belongs to.  This region is
      // the same for all declarations.
      virtual const Region& home_region() const = 0;

      // The region where this declaration appears -- purely lexical.
      // For many declarations, this is the same as the home region.
      // Exceptions are invisible friends, member functions defined
      // outside of their enclosing class or namespaces.
      virtual const Region& lexical_region() const = 0;

      virtual bool has_initializer() const = 0;
      virtual const Expr& initializer() const = 0;

      virtual const Named_map& generating_map() const = 0;
      virtual const Sequence<Substitution>& substitutions() const = 0;

      virtual int position() const = 0;

      // This is the first seen declaration for name() in a given
      // translation unit.  The master declaration is therefore the
      // first element of the declaration-set.
      virtual const Decl& master() const = 0;

      virtual const Sequence<Decl>& decl_set() const = 0;

   protected:
      Decl(Category_code c) : Stmt(c)
      { }
   };

   inline Decl::Specifier
   operator|(Decl::Specifier a, Decl::Specifier b)
   { return Decl::Specifier(int(a) | int(b)); }

   inline Decl::Specifier&
   operator|=(Decl::Specifier& a, Decl::Specifier b)
   { return a = Decl::Specifier(int(a) | int(b)); }

   inline Decl::Specifier
   operator&(Decl::Specifier a, Decl::Specifier b)
   { return Decl::Specifier(int(a) & int(b)); }

   inline Decl::Specifier&
   operator&=(Decl::Specifier& a, Decl::Specifier b)
   { return a = Decl::Specifier(int(a) & int(b)); }

   inline Decl::Specifier
   operator^(Decl::Specifier a, Decl::Specifier b)
   { return Decl::Specifier(int(a) ^ int(b)); }

   inline Decl::Specifier&
   operator^=(Decl::Specifier& a, Decl::Specifier b)
   { return a = Decl::Specifier(int(a) ^ int(b)); }

                                // -- Named_map --
   // This represents a parameterized declaration.  If its
   // template-parameter list is empty, that means that it is
   // either an explicit specialization -- if result() has a
   // non-empty substitutions() -- or a really non-template
   // declaration  -- if result().substitutions() is empty.
   // The list of specializations of this template (either
   // partial or explicit) is given by specializations().
   struct Named_map : Category<named_map_cat, Decl> {
      virtual const Named_map& primary_named_map() const = 0;
      virtual const Sequence<Decl>& specializations() const = 0;
      virtual const Mapping& mapping() const = 0;

      const Parameter_list& params() const { return mapping().params(); }
      const Expr& result() const { return mapping().result(); }

      virtual const Named_map& definition() const = 0;
   };

                                // -- Enumerator --
   // This represents a classic enumerator.
   struct Enumerator : Category<enumerator_cat, Decl> {
      const Type& type() const { return membership(); }
      virtual const Enum& membership() const = 0;
   };

                                // -- Asm --
   // An asm-declaration.  While this sort of declaration does not
   // actually introduce any name, it was made a declaration in C++,
   // so that it could appear at non-local scopes -- where general
   // declarations live.  Its name is "asm" and its type is "void".
   struct Asm : Category<asm_cat, Decl> {
      virtual const String& text() const = 0;
   };

                                // - Alias --
   // This represent a alias declaration (e.g. typedef, namespace-alias,
   // template alias, ...).
   // An alias is always initialized -- with what it is an alias for.
   // The type of an Alias expression is that of its initializer.
   struct Alias : Category<alias_cat, Decl> {
      const Type& type() const { return initializer().type(); }
   };

                                // -- Base_type --
   // Each base-specifier in a base-clause morally declares an unnamed
   // subobject.  We represent that subobject declaration by a Base_type.
   // For consistency with other non-static members, the name of that
   // subobject is pretended to be the same as that of the base-class
   // so that when it appears in a member-initializer list, it can
   // conveniently be thought of as initialization of that subobject.
   struct Base_type : Category<base_type_cat, Decl> {
      // A base-object is, by definition, unnamed.  However, it
      // is convenient to refer to it by the name of the corresponding
      // type -- in C++ tradition.
      const Name& name() const { return type().name(); }
   };

                                // -- Parameter --
   // A parameter is uniquely characterized by its position in
   // a parameter list.
   struct Parameter : Category<parameter_cat, Decl> {
      virtual const Parameter_list& membership() const = 0;
      const Expr& default_value() const { return initializer(); }
   };

                                // -- Fundecl --
   // This node represents a function declaration. Notice that the
   // exception specification is actually made part of the function
   // type.  For convenience, it is represented here too.
   struct Fundecl : Category<fundecl_cat, Decl> {
      virtual const Udt& membership() const = 0;
      virtual const Mapping& mapping() const = 0;

      const Parameter_list& parameters() const
      { return mapping().params(); }

      virtual const Fundecl& definition() const = 0;
   };

                                // -- Var --
   // This represents a variable declaration.  It is also used to
   // represent a static data member.
   struct Var : Category<var_cat, Decl> {
   };

                                // -- Field --
   // This node represents a nonstatic data member.  
   struct Field : Category<field_cat, Decl> {
      virtual const Udt& membership() const = 0;
   };

                                // -- Bitfield --
   // A bit-field data member.
   struct Bitfield : Category<bitfield_cat, Decl> {
      virtual const Expr& precision() const = 0;
      virtual const Udt& membership() const = 0;
   };

                                // -- Typedecl --
   struct Typedecl : Category<typedecl_cat, Decl> {
      virtual const Udt& membership() const = 0;
      virtual const Typedecl& definition() const = 0;
   };

                                // -- Unit --
   struct Unit : Category<unit_cat, Node> {
      virtual const ipr::Global_scope& get_global_scope() const = 0;

      virtual const Void& get_void() const = 0;
      virtual const Bool& get_bool() const = 0;
      virtual const Char& get_char() const = 0;
      virtual const sChar& get_schar() const = 0;
      virtual const uChar& get_uchar() const = 0;
      virtual const Wchar_t& get_wchar_t() const = 0;
      virtual const Short& get_short() const = 0;
      virtual const uShort& get_ushort() const = 0;
      virtual const Int& get_int() const = 0;
      virtual const uInt& get_uint() const = 0;
      virtual const Long& get_long() const = 0;
      virtual const uLong& get_ulong() const = 0;
      virtual const Long_long& get_long_long() const = 0;
      virtual const uLong_long& get_ulong_long() const = 0;
      virtual const Float& get_float() const = 0;
      virtual const Double& get_double() const = 0;
      virtual const Long_double& get_long_double() const = 0;
      virtual const Ellipsis& get_ellipsis() const = 0;
      virtual const Type& get_class() const = 0;
      virtual const Type& get_union() const = 0;
      virtual const Type& get_enum() const = 0;
      virtual const Type& get_namespace() const = 0;

      virtual const Linkage& get_cxx_linkage() const = 0;
      virtual const Linkage& get_c_linkage() const = 0;
   };

                                // -- Built-in type constants --

   struct Primitive : As_type { };

   struct Void : As_type { };   

   struct Bool : As_type { };  

   struct Char : As_type { };
   struct sChar : As_type { };
   struct uChar : As_type { };

   struct Wchar_t : As_type { };

   struct Short : As_type { };
   struct uShort : As_type { };

   struct Int : As_type { };
   struct uInt : As_type { };

   struct Long : As_type { };
   struct uLong : As_type { };

   struct Long_long : As_type { };
   struct uLong_long : As_type { };

   struct Float : As_type { };
   struct Double : As_type { };
   struct Long_double : As_type { };

   struct Ellipsis : As_type { };

   struct Global_scope : Namespace { Global_scope() : Namespace(global_scope_cat) {} }; // 2011-08-10 YS: Added for pattern matching purposes

   struct Empty_stmt : Expr_stmt { Empty_stmt() : Expr_stmt(empty_stmt_cat) {} };

   struct C_linkage : Linkage { };
   struct Cxx_linkage : Linkage { };

                                // -- Visitor --
   struct Visitor {
      virtual void visit(const Node&) = 0;

      virtual void visit(const Annotation&);
      virtual void visit(const Region&);
      virtual void visit(const Comment&);
      virtual void visit(const String&);
      virtual void visit(const Linkage&);

      virtual void visit(const Overload&);
      virtual void visit(const Parameter_list&);
      virtual void visit(const Scope&);

      virtual void visit(const Expr&) = 0;
      virtual void visit(const Classic&);

      // -- results of name constructor constants --
      
      virtual void visit(const Name&);
      virtual void visit(const Identifier&);
      virtual void visit(const Operator&);
      virtual void visit(const Conversion&);
      virtual void visit(const Scope_ref&);
      virtual void visit(const Template_id&);
      virtual void visit(const Type_id&);
      virtual void visit(const Ctor_name&);
      virtual void visit(const Dtor_name&);
      virtual void visit(const Rname&);

      // -- results of type constructor constants --

      virtual void visit(const Type&) = 0;
      virtual void visit(const Array&);
      virtual void visit(const As_type&);
      virtual void visit(const Class&);
      virtual void visit(const Decltype&);
      virtual void visit(const Enum&);
      virtual void visit(const Function&);
      virtual void visit(const Namespace&);
      virtual void visit(const Pointer&);
      virtual void visit(const Ptr_to_member&);
      virtual void visit(const Product&);
      virtual void visit(const Qualified&);
      virtual void visit(const Reference&);
      virtual void visit(const Rvalue_reference&);
      virtual void visit(const Sum&);
      virtual void visit(const Template&);
      virtual void visit(const Union&);
      virtual void visit(const Udt&);

      // -- results of nullar expression constructor constants --

      virtual void visit(const Phantom&);

      // -- results of unary expression constructor constants --

      virtual void visit(const Address&);
      virtual void visit(const Array_delete&); 
      virtual void visit(const Complement&); 
      virtual void visit(const Delete&); 
      virtual void visit(const Deref&);
      virtual void visit(const Expr_list&);
      virtual void visit(const Expr_sizeof&);
      virtual void visit(const Expr_typeid&);
      virtual void visit(const Id_expr&);
      virtual void visit(const Label&);
      virtual void visit(const Not&);
      virtual void visit(const Paren_expr&);
      virtual void visit(const Post_decrement&);
      virtual void visit(const Post_increment&); 
      virtual void visit(const Pre_decrement&); 
      virtual void visit(const Pre_increment&); 
      virtual void visit(const Throw&);
      virtual void visit(const Type_sizeof&); 
      virtual void visit(const Type_typeid&); 
      virtual void visit(const Unary_minus&); 
      virtual void visit(const Unary_plus&);

      // -- results of binary expression constructor constants --

      virtual void visit(const And&); 
      virtual void visit(const Array_ref&);
      virtual void visit(const Arrow&); 
      virtual void visit(const Arrow_star&); 
      virtual void visit(const Assign&);
      virtual void visit(const Bitand&);
      virtual void visit(const Bitand_assign&);
      virtual void visit(const Bitor&);
      virtual void visit(const Bitor_assign&);
      virtual void visit(const Bitxor&);
      virtual void visit(const Bitxor_assign&);
      virtual void visit(const Cast&); 
      virtual void visit(const Call&); 
      virtual void visit(const Comma&);
      virtual void visit(const Const_cast&); 
      virtual void visit(const Datum&); 
      virtual void visit(const Div&);
      virtual void visit(const Div_assign&);
      virtual void visit(const Dot&);
      virtual void visit(const Dot_star&); 
      virtual void visit(const Dynamic_cast&); 
      virtual void visit(const Equal&); 
      virtual void visit(const Greater&); 
      virtual void visit(const Greater_equal&); 
      virtual void visit(const Less&); 
      virtual void visit(const Less_equal&); 
      virtual void visit(const Literal&);
      virtual void visit(const Lshift&);
      virtual void visit(const Lshift_assign&);
      virtual void visit(const Member_init&);
      virtual void visit(const Minus&);
      virtual void visit(const Minus_assign&);
      virtual void visit(const Modulo&);
      virtual void visit(const Modulo_assign&);
      virtual void visit(const Mul&);
      virtual void visit(const Mul_assign&);
      virtual void visit(const Not_equal&); 
      virtual void visit(const Or&); 
      virtual void visit(const Plus&);
      virtual void visit(const Plus_assign&);
      virtual void visit(const Reinterpret_cast&); 
      virtual void visit(const Rshift&);
      virtual void visit(const Rshift_assign&);
      virtual void visit(const Static_cast&); 

      // -- result of trinary expression constructor constants --

      virtual void visit(const Conditional&);
      virtual void visit(const New&);
      virtual void visit(const Mapping&);

      // -- result of statement constructor constants --

      virtual void visit(const Stmt&) = 0;
      virtual void visit(const Block&);
      virtual void visit(const Break&);
      virtual void visit(const Continue&);
      virtual void visit(const Ctor_body&);
      virtual void visit(const Do&);
      virtual void visit(const Expr_stmt&);
      virtual void visit(const Empty_stmt&);
      virtual void visit(const For&);
      virtual void visit(const For_in&);
      virtual void visit(const Goto&);
      virtual void visit(const Handler&);
      virtual void visit(const If_then&);
      virtual void visit(const If_then_else&);
      virtual void visit(const Labeled_stmt&);
      virtual void visit(const Return&);
      virtual void visit(const Switch&);
      virtual void visit(const While&);

      // -- result of declaration constructor constants --

      virtual void visit(const Decl&) = 0;
      virtual void visit(const Alias&);
      virtual void visit(const Base_type&);
      virtual void visit(const Bitfield&);
      virtual void visit(const Enumerator&);
      virtual void visit(const Field&);
      virtual void visit(const Fundecl&);
      virtual void visit(const Named_map&);
      virtual void visit(const Parameter&);
      virtual void visit(const Typedecl&);
      virtual void visit(const Var&);
      virtual void visit(const Asm&);

      // -- distinguished node --

      virtual void visit(const Unit&);

      // -- Special built-in constants --

      virtual void visit(const Void&);
      virtual void visit(const Bool&);
      virtual void visit(const Char&);
      virtual void visit(const sChar&);
      virtual void visit(const uChar&);
      virtual void visit(const Short&);
      virtual void visit(const uShort&);
      virtual void visit(const Int&);
      virtual void visit(const uInt&);
      virtual void visit(const Long&);
      virtual void visit(const uLong&);
      virtual void visit(const Long_long&);
      virtual void visit(const uLong_long&);
      virtual void visit(const Float&);
      virtual void visit(const Double&);
      virtual void visit(const Long_double&);
      virtual void visit(const Ellipsis&);

      virtual void visit(const C_linkage&);
      virtual void visit(const Cxx_linkage&);

      // -- built-ins, but not constants --

      virtual void visit(const Global_scope&);
   };
}

#endif // IPR_INTERFACE_INCLUDED

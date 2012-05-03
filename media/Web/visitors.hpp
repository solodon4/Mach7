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

// -------------------------------------------
// -- results of name constructor constants --
// -------------------------------------------
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

// -------------------------------------------
// -- results of type constructor constants --
// -------------------------------------------
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

      
// --------------------------------------------------------
// -- results of nullar expression constructor constants --
// --------------------------------------------------------
      virtual void visit(const Phantom&);

// -------------------------------------------------------
// -- results of unary expression constructor constants --
// -------------------------------------------------------
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

// --------------------------------------------------------
// -- results of binary expression constructor constants --
// --------------------------------------------------------
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

// --------------------------------------------------------
// -- result of trinary expression constructor constants --
// --------------------------------------------------------
      virtual void visit(const Conditional&);
      virtual void visit(const New&);
      virtual void visit(const Mapping&);

// -----------------------------------------------
// -- result of statement constructor constants --
// -----------------------------------------------
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

// -------------------------------------------------
// -- result of declaration constructor constants --
// -------------------------------------------------
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

// ------------------------
// -- distinguished node --
// ------------------------
      virtual void visit(const Unit&);

// --------------------------------
// -- Special built-in constants --
// --------------------------------
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

// ----------------------------------
// -- built-ins, but not constants --
// ----------------------------------
      virtual void visit(const Global_scope&);
   };

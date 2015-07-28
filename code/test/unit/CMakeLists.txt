include_directories(../..)

# these are all compiled the same way
set(PROGRAMS algebraic category cppcon-matching cppcon-visitors example01 example02 example03 example04 exp exp2 expr expr_meta filter memoized_cast morton non_unique_problem non_unique_workaround pattern_composition predicate prolog prolog-pat prolog-pat2 regex shape4 shape5 shape6 shape7 shape8 type_switch2 type_switch3 type_switchN type_switchN-decl type_switchN-patterns virpat-shapes xtl)

foreach(program ${PROGRAMS})
  add_executable(${program} ${program}.cpp)
endforeach(program)

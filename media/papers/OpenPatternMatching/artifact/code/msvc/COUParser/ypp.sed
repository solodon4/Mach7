#
# Sed script to preprocess clean yacc grammar to dump the rules
# by SolodOn (solodon@mail.com)
# Version 1.0
# From 2006-07-11
#
# Usage: sed -f ypp.sed file.y
#
# NOTE: For the script to work rules should be formatted as following:
#
#       X
#          : A
#          | B
#          | C
#          | D
#          ;
#


#
# Hold the head of the rule
#
/^[ \t]*[A-Za-z_][A-Za-z_0-9]*[ \t]*$/{
h
}

#
# Append the head to the end of current line and do replacement with pasting head
#
/^[ \t]*[|:].*/{
G
s/^\([ \t]*\)\([|:]\)\(.*\)\n\(.*\)/\1\2\3\t { cerr << __LINE__ << "\\t\4 -> \3" << endl; }/g
}

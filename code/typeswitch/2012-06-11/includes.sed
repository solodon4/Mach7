#
# Generates GraphViz .dot file for inclusion hierarchy
#
# Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
# Version 1.0 from 15th March 2012
#
# Usage: grep "#include" *.hpp | sed -f includes.sed > includes.dot
#    or: grep -R --include=*.hpp "#include" * | sed -f includes.sed > includes.dot
#  then: dot.exe -Tpng -oincludes.png includes.dot
#

s/^/"/g
s/:/"/g
s/\/\/.*//g
s/"[A-Za-z0-9_\.\/-]*\//"/g
s/\(<.*>\)/\1\n\1 [color="red",shape="box"]/g
s/#include/ -> /g
1 { s/^/digraph Includes {\nrankdir=TB;\nratio=0.44;\n/g }
$ { s/$/\n\}\n/g }

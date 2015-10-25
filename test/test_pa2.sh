#!/bin/bash

cd ../

make pa2
pa2/bin/pa2 best <<EOF
1
a
128

2
a

4
EOF

cd test
#!/bin/bash

cd ../

make pa2
pa2/bin/pa2 best <<EOF
1
A
1

1
B
1

2
A

4
EOF

cd test
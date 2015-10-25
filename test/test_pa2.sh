#!/bin/bash

cd ../

make pa2
pa2/bin/pa2 worst <<EOF
1
P1
8

1
P2
7

1
P3
9

3

4

2
P2

3

4

1
P4
3

4

1
P1
5

1
P5
1000000

5
EOF

cd test
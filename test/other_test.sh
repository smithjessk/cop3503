#!/bin/bash

cd ../

make pa2
pa2/bin/pa2 best <<EOF

1
a
1

1
b
1

4

2
a

4

1
a
1
	
4

EOF
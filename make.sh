CC="g++"
CFLAGS="-std=c++11 -g -I."
$CC -c $CFLAGS ThreadPool.cpp
$CC -c $CFLAGS Server.cpp
$CC -c $CFLAGS util.cpp
$CC -c $CFLAGS main.cpp
$CC *.o -o main


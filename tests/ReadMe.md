
# LA
A few tests have been done for LA, it does not cover all cases so far
for the tests you can run it from a file or from a terminal. 

###For reading from a file (enter your code in input.txt):
```
cd test/LA
make
./test_scanner
```

###For reading from stdin:
```
cd test/LA
make USE_STDIN=1
./test_scanner
```

>To end: ctrl + D

## Cleanup (removal of object and executable files)
```
make clean
```
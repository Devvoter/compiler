
# LA
Testing. You can test code from a file(past code in input.txt and LA will makes tokens) or if 
you want to test some specific lexem you can run it without file and write lexem into terminal. 

### For reading from a file (one more: enter your code in input.txt):
```
cd test/LA
make
./test_scanner
```

### For reading from stdin:
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
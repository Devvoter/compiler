
# LA
Testing. You can test code from a file(past code in input.txt and LA will makes tokens) or if 
you want to test some specific lexem you can run it without file and write lexem into terminal. 

### For reading from a file (one more: enter your code in input.txt):
```
cd test/LA
make USE_FILE=1
./test_scanner
```

### For reading from stdin:
```
cd test/LA
make
./test_scanner
```
>To end: ctrl + D

Or

```
cd test/LA
make
./test_scanner < file.txt
```
## Cleanup (removal of object and executable files)
```
make clean
```


# Stack
Added some tests for stack.

```
cd test/stack
make
./test_stack
```

## Cleanup (removal of object and executable files)
```
make clean
```
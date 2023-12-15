#This is my makefile, which isn't related to my zybook submission

build: main.c
    gcc main.c -o main.exe

test: test.c
    gcc test.c -o test.exe

#the < operator is used to send dictionary.txt as input when executing main.exe
run: build
    .\main.exe < 5 dictionary.txt cabci abate

run_test: test
    ./test.exe

clean:
    del -f main.exe
    del -f test.exe
#Jiayi Chen jc4489 Lab 6 part2 Makefile

CC=gcc
CXX=g++

INCLUDES=
CFLAGS=-g -Wall
LDFLAGS=-g -Wall

LDLIBS=

.PHONY: default
default: http-client

http-client: 

.PHONY: vg
vg:
	valgrind --leak-check=yes ./http-client www.gnu.org 80 /software/make/manual/make.html

.PHONY: vgv
vgv:
	valgrind -v ./http-client www.gnu.org 80 /software/make/manual/make.html

.PHONY: clean
clean:
	rm -rf *.o http-client make.html make.html~

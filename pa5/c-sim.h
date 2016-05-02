#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct offset{
  int data;
  struct offset *next;
};

struct block{
  struct offset *off;
  int tag;
  int t;
  struct block *next;
};

struct set{
  struct block *b;
  struct set *next;
};

struct cache{
  struct set *s;
};

void read(int data, int bo, int in, int tag, struct cache *c);

void write(int data, int bo, int in, int tag, struct cache *c);

int get1s(int numofbits);

int getbits(int temp);

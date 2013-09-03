#include "linkedlist.h"
#include "loader.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static List wordList;

static int8_t processToken(Token token) {
  addEntry(wordList, getEnd(wordList), (void *)token);
  return 1;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    int i;
    for (i = 0; i < argc; ++i) {
      printf("%s\n", argv[i]);
    }
  } else {
    wordList = createList();
    loadFromFile(argv[1], &processToken);
    printf("\n----------------------------------------------\n");
    Iterator iter;
    for (iter = getBegin(wordList); iter; moveNext(&iter)) {
      Token str = (Token)getCurrent(iter);
      printf("\t[%s]\n", str);
    }
    
    destroyList(wordList);
  }
  
  return 0;
}

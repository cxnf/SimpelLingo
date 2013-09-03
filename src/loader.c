#include "loader.h"

#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>

int8_t loadFromFile(const char * path, loaderCallback fnCallback) {
  FILE * file = fopen(path, "r");
  if (!file) {
    return 0;
  }

  List list = createList();
  int iRead;
  while ((iRead = fgetc(file)) != EOF) {
    switch (iRead) {
    case ';':
      {
	int iSize;
	if ((iSize = getSize(list)) > 0) {
	  char * token = (char *)malloc(sizeof(char) * (iSize + 1));
	  int i;
	  Iterator iter;
	  for (i = 0, iter = getBegin(list); i < iSize && iter; ++i, moveNext(&iter)) {
	    token[i] = *(char *)getCurrent(iter);
	  }
	  token[i] = '\0';
	  clearList(list);
	  (*fnCallback)((Token)token);
	}
      }
    default:
      {
	if ((iRead >= 'a' && iRead <= 'z') || (iRead >= 'A' && iRead <= 'Z')) {
	  char * buf = (char *)malloc(sizeof(char));
	  buf[0] = iRead;
	  addEntry(list, getEnd(list), (void *)buf);
	} else {
	  clearList(list);
	}
      }
    }
  }
  
  fclose(file);
  return 1;
}

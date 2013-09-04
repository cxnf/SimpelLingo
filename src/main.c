#include "list.h"
#include "tokenizer.h"
#include "game.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static List wordList;

static int8_t processToken(enum TokenType ttType, Token token) {
  switch (ttType) {
  case TTText:
    addEntry(wordList, getEnd(wordList), (void *)token);
    return 1;

  default:
    printf("Unsupported token type\n");
    return 0;
  }
}

int main(int argc, char ** argv) {
  if (argc < 3) {
    printf("Error: Not enough arguments.\n");
    return 1;
  } else {
    wordList = createList();

    if (!strcmp(argv[1], "--build-list")) {
      if (argc < 4) {
	printf("Specify input list(s).\n");
      } else {
	int i;
	for (i = 3; i < argc; ++i) {
	  parseFile(argv[i], &processToken);
	}
	FILE * file = fopen(argv[2], "w");
	if (file) {
	  Iterator iter;
	  for (iter = getBegin(wordList); iter; moveNext(&iter)) {
	    const char * ccaText = (const char *)getCurrent(iter);
	    if (strlen(ccaText) == 5) {
	      fprintf(file, "%s;\n", ccaText);
	    }
	  }
	  fclose(file);
	}
      }
    } else if (!strcmp(argv[1], "--run-game")) {

      srand(time(NULL));
      printf("Guess the word! (or use Ctrl-C to quit)\n");
      parseFile(argv[2], &processToken);
      if (getSize(wordList)) {
	enum MatchResults mrResult = startMatch(rand() % getSize(wordList), wordList);
	printf("MR: [%d]\n", mrResult);
      } else {
	printf("Error: World list empty.\n");
      }
    } else {
      printf("Error: Invalid mode.\n");
    }
    
    destroyList(wordList);
  }
  return 0;
}

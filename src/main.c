#include "list.h"
#include "tokenizer.h"
#include "game.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static List wordList;                             //!< Word list containing all allowed words.

/*! \brief Processes token for tokenizer.
  Appends token to world list.
  \param ttType Type of the token to process.
  \param token Token data.
  \return 0 to cancel on unsupported token type, otherwise 1.
*/
static int8_t processToken(enum TokenType ttType, Token token) {
  switch (ttType) {
  case TTText:
    if (strlen(token) == 5) {
      addEntry(wordList, getEnd(wordList), (void *)token);
    }
    return 1;

  default:
    printf("Unsupported token type\n");
    return 0;
  }
}

/*! \brief Builds world list.
  Parses input list, filtering it for valid words, and writes it to the output list.
  \param caOutList Path to output list.
  \param caInList Path to input list.
  \return 0 on success, else error code.
*/
static int8_t buildWordList(char * caOutList, char * caInList) {
  if (parseFile(caInList, &processToken)) {
    return 1;
  }
  FILE * file = fopen(caOutList, "w");
  if (file) {
    Iterator iter;
    for (iter = getBegin(wordList); iter; moveNext(&iter)) {
      const char * ccaText = (const char *)getCurrent(iter);
      fprintf(file, "%s;\n", ccaText);
    }
    fclose(file);
  } else {
    return 1;
  }
  return 0;
}

/*! \brief Runs a game.
  Initializes resources and starts a new match.
  \param caInList Path to word list.
  \return 0 on success, else error code.
*/
static int8_t runGame(char * caInList) {
  srand(time(NULL));
  printf("Guess the word! (or use Ctrl-C to quit)\n ^ appears below correct characters.\n * appears below characters in wrong location.\n");
  if (parseFile(caInList, &processToken)) {
    return 1;
  }
  if (getSize(wordList)) {
    enum MatchResults mrResult = startMatch(rand() % getSize(wordList), wordList);
    switch (mrResult) {
    case MRWin:
      printf("You won the game.\n");
      break;
    case MRLose:
      printf("You lost the game.\n");
      break;
    default:
      printf("Game resulted in run-time error: [%d]\n", mrResult);
      break;
    }
  } else {
    printf("Error: Word list empty.\n");
    return 1;
  }
  return 0;
}

/*! \brief Entry point.
  Program entry point, calls all systems.
  \param argc Amount of arguments passed from command line.
  \param argv Arguments passed from command line.
  \return 0 on supported run type, otherwise error code.
*/
int main(int argc, char ** argv) {
  wordList = createList();
  int rc = 0;
  
  switch (argc) {
  case 0:
  case 1:
    rc = runGame("list");
    break;

  case 3:
  case 4:
    if (!strcmp(argv[1], "--build-list")) {
      if (argc < 4) {
	printf("Specify input list(s).\n");
	rc = 1;
      } else {
	rc = buildWordList(argv[2], argv[3]);
      }
    } else if (!strcmp(argv[1], "--run-game")) {
      rc = runGame(argv[2]);
    } else {
      printf("Error: Invalid mode.\n");
    }
    break;
    
  default:
    printf("Error: Invalid amount of arguments.\n");
    rc = 1;
  }

  destroyList(wordList);
  return rc;
}

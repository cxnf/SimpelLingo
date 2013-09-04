#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNTER_START 5

typedef const char * CWord;
typedef char * Word;

// ----------------- Struct definitions -----------------------------------
struct GameContext {
  uint8_t uiRemainingRounds;
  CWord cwWord;
  Word wTip;
};

// ----------------- Local Function declarations --------------------------
static inline int8_t fetchInput(Word wBuffer, struct GameContext * pgcContext);
static inline int8_t isMatchOn(struct GameContext * pgcContext);
static inline int8_t guess(Word wBuffer, struct GameContext * pgcContext);
static inline int8_t isAllowed(Word wBuffer, List lWords);


// ----------------- Global Function definitions --------------------------
enum MatchResults startMatch(uint32_t uiIndex, List lWords) {
  Iterator iter = getEntry(lWords, uiIndex);
  struct GameContext gcContext = { COUNTER_START, (CWord)getCurrent(iter), (Word)malloc(sizeof(char) * 6) };
  memset(gcContext.wTip, (int)'_', sizeof(char) * 5);
  gcContext.wTip[5] = '\0';
  Word wBuffer = (Word)malloc(sizeof(char) * 6);

  while (isMatchOn(&gcContext)) {
    printf("%s\n", gcContext.wTip);
    while (!fetchInput(wBuffer, &gcContext)) {
      if (strlen(wBuffer) != 5) {
	printf("Input less then 5 characters\n");
      } else {
	printf("Input contains illegal character(s), use a-z or A-Z\n");
      }
    }
    if (isAllowed(wBuffer, lWords)) {
      if (!strcmp(gcContext.cwWord, wBuffer)) {
	return MRWin;
      }
      if (!guess(wBuffer, &gcContext)) {
	printf("Failed to match strings\n");
	return MRRunError;
      }
    } else {
      printf("'%s' not a word\n", wBuffer);
    }
  }

  return MRLose;
}


// ----------------- Local Function definitions ---------------------------
static inline int8_t fetchInput(Word wBuffer, struct GameContext * pgcContext) {
  int8_t i;
  printf("%d>", pgcContext->uiRemainingRounds);
  scanf("%5s", wBuffer);
  for (i = 0; i < 5; ++i) {
    if (wBuffer[i] >= 'a' && wBuffer[i] <= 'z') {
      // this char is ok, leave it
    } else if (wBuffer[i] >= 'A' && wBuffer[i] <= 'Z') {
      wBuffer[i] = tolower(wBuffer[i]);
    } else {
      return 0;
    }
  }
  return 1;
}

static inline int8_t isMatchOn(struct GameContext * pgcContext) {
  return pgcContext->uiRemainingRounds;
}

static inline int8_t guess(Word wBuffer, struct GameContext * pgcContext) {
  int8_t i;
  printf("  ");
  for (i = 0; i < 5; ++i) {
    if (wBuffer[i] == pgcContext->cwWord[i]) {
      printf("^");
      pgcContext->wTip[i] = pgcContext->cwWord[i];
    } else {
      CWord cwContains = strchr(pgcContext->cwWord, wBuffer[i]);
      if (cwContains) {
	printf("+");
      } else {
	printf(" ");
      }
    }
  }
  printf("\n");
  --pgcContext->uiRemainingRounds;
  return 1;
}

static inline int8_t isAllowed(Word wBuffer, List lWords) {
  Iterator iter;
  for (iter = getBegin(lWords); iter; moveNext(&iter)) {
    if (!strcmp((CWord)getCurrent(iter), wBuffer)) {
      return 1;
    }
  }
  return 0;
}

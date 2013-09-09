#include "tokenizer.h"

#include "list.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

// ----------------- Struct definitions -----------------------------------

/*! \struct ParseContext
  \brief Context of tokenizer.
*/
struct ParseContext {
  FILE * file;                                    //!< File parsed by tokenizer.
  List list;                                      //!< List to mimic dynamic string.
  int iLine;                                      //!< Line number of tokenizer.
  int iColumn;                                    //!< Column number of tokenizer.
  parserCallback fnCallback;                      //!< Callback for external token processing.
};


// ----------------- Local Function declarations --------------------------

/*! \brief Report error.
  Reports an error to stdin.
  \param ccaMessage Message to print.
  \param cppcContext Context of the tokenizer.
*/
static inline void reportError(const char * ccaMessage, const struct ParseContext * cppcContext);
/*! \brief Free memory.
  Frees all memory allocated by tokenizer for current context.
  \param ppcContext Context to free.
*/
static inline void cleanUp(struct ParseContext * ppcContext);
/*! \brief Report error and clean up.
  Reports an error to stdin and frees memory.
  \param ccaMessage Message to print.
  \param ppcContext Context of the tokenizer.
*/
static inline void reportAndClean(const char * ccaMessage, struct ParseContext * ppcContext);
/*! \brief Parse new line char.
  Parses a new line char to support for '\n', '\n\r' and '\r'.
  \param ppcContext Context of tokenizer.
  \return Parse result.
*/
static enum ParseResults parseNewLine(struct ParseContext * ppcContext);
/*! \brief Parse expression.
  Parses an expression.
  \param ppcContext Context of tokenizer.
  \return Parse result.
*/  
static enum ParseResults parseExpression(struct ParseContext * ppcContext);


// ----------------- Global Function definitions --------------------------
enum ParseResults parseFile(const char * path, parserCallback fnCallback) {
  struct ParseContext pcContext;
  pcContext.file = fopen(path, "r");
  if (!pcContext.file) {
    return 0;
  }
  
  pcContext.list = createList();
  pcContext.iLine = 1;
  pcContext.iColumn = 0;
  pcContext.fnCallback = fnCallback;
  enum ParseResults result = ROk;

  int iRead;
  while ((iRead = fgetc(pcContext.file)) != EOF) {
    ++pcContext.iColumn;
    
    switch (iRead) {
    case '\n':
      {
	fpos_t pPos;
	fgetpos(pcContext.file, &pPos);
	int iTmpRead = fgetc(pcContext.file);
	if (iTmpRead == EOF) {
	  // reportAndClean("Unexpected end of file", &pcContext);
	  cleanUp(&pcContext);
	  return ROk;
	}
	if (iTmpRead != '\r') {
	  fsetpos(pcContext.file, &pPos);
	}
      }
    case '\r':
      if ((result = parseNewLine(&pcContext))) {
	reportAndClean("Unexpected newline", &pcContext);
	return result;
      }
      break;

    case ';':
      if ((result = parseExpression(&pcContext))) {
	reportAndClean("Unexpected ';'", &pcContext);
	return result;
      }
      break;

    default:
      if ((iRead >= 'a' && iRead <= 'z') || (iRead >= 'A' && iRead <= 'Z')) {
	char * buf = (char *)malloc(sizeof(char));
	*buf = (char)iRead;
	addEntry(pcContext.list, getEnd(pcContext.list), (void *)buf);
      } else if (isspace((char)iRead)) {
	
	if (getSize(pcContext.list) > 0) {
	  reportAndClean("Missing ';'", &pcContext);
	  return RErrMissingToken;
	}
      }
      break;
    }
  }

  cleanUp(&pcContext);
  return ROk;
}


// ----------------- Local Function definitions ---------------------------
static inline void reportError(const char * ccaMessage, const struct ParseContext * cppcContext) {
  printf("Errror: %s at line %d[%d]\n", ccaMessage, cppcContext->iLine, cppcContext->iColumn);
}
static inline void cleanUp(struct ParseContext * ppcContext) {
  destroyList(ppcContext->list);
  fclose(ppcContext->file);
}
static inline void reportAndClean(const char * ccaMessage, struct ParseContext * ppcContext) {
  reportError(ccaMessage, ppcContext);
  cleanUp(ppcContext);
}

static enum ParseResults parseNewLine(struct ParseContext * ppcContext) {
  if (getSize(ppcContext->list) > 0) {
    return RErrInvalidToken;
  }
  ++ppcContext->iLine;
  ppcContext->iColumn = 0;
  return ROk;
}

static enum ParseResults parseExpression(struct ParseContext * ppcContext) {
  int iSize;
  if ((iSize = getSize(ppcContext->list)) > 0) {
    char * token = (char *)malloc(sizeof(char) * (iSize + 1));
    int i;
    Iterator iter;
    for (i = 0, iter = getBegin(ppcContext->list); i < iSize && iter; ++i, moveNext(&iter)) {
      token[i] = *(char *)getCurrent(iter);
    }
    token[i] = '\0';
    clearList(ppcContext->list);
    if (!(*ppcContext->fnCallback)(TTText, (Token)token)) {
      return RErrCanceled;
    }
    return ROk;
  } else {
    return RErrInvalidToken;
  }  
}

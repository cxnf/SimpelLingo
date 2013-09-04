#pragma once

/*! \file tokenizer.h
  \brief Basic text stream tokenizer.
*/

#include <stdint.h>

/*! \enum TokenType
  \brief Supported token types.
*/
enum TokenType {
  TTText
};

/*! \enum ParseResults
  \brief Return codes of parse functions.
*/
enum ParseResults {
  ROk = 0,                                        //<! No errors.
  RErrCanceled,                                   //<! Parse operation canceled by callback.
  RErrInvalidToken,                               //<! Invalid or unexpected token found.
  RErrMissingToken                                //<! Missing token.
};

typedef const char * Token;
typedef int8_t (*parserCallback)(enum TokenType, Token);

/*! \brief Generate token stream from file stream.
  Generates a token stream from a file stream.
  The file is read as a text file.
  Generated tokens are send directly to the callback.
  When the callback returns 0, the parse operation cancels.
  \param path Relative or absolute path to a text file, file must exists.
  \param fnCallback Pointer to function called when a new token is available.
  \return ROk on success, error code otherwise.
*/
enum ParseResults parseFile(const char * path, parserCallback fnCallback);

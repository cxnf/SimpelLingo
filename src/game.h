#pragma once

/*! \file game.h
  \brief Game manager.
*/

#include "list.h"
#include <stdint.h>

/*! \enum MatchResults
  \brief Results of a match.
*/
enum MatchResults {
  MRWin,                                          //!< Won match.
  MRLose,                                         //!< Lost match.
  MRRunError                                      //!< Match resulted in an error.
};

/*! \brief Starts a match.
  Starts a match and blocks until match has a result.
  \param uiIndex Index of word to guess in 'lWords'.
  \param lWords List of words containing all allowed input words.
  \return Result of the match.
*/
enum MatchResults startMatch(uint32_t uiIndex, List lWords);

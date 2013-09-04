#pragma once

/*! \file game.h
  \brief Game manager.
*/

#include "list.h"
#include <stdint.h>

enum MatchResults {
  MRWin,
  MRLose,
  MRRunError
};

enum MatchResults startMatch(uint32_t uiIndex, List lWords);

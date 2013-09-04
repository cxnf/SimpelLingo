#include "list.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// ----------------- Struct definitions -----------------------------------

/*! \struct _list_
  \brief Implementation of 'List' type.
  Lists hold only hold iterators to their first and last entry.
  A list maintains a counter to determine the amount of entries it holds.
  Outside of compilation unit list may only be refered to by pointers.
*/
struct _list_ {
  Iterator first;                                 //<! Iterator to first entry.
  Iterator last;                                  //<! Iterator to last entry.
  uint32_t uiSize;                                //<! Entry counter.
};

/*! \struct _iterator_
  \brief Implementation of 'Iterator' type.
  Iterators are actually wrappers around void pointers to implement a doubly linked list (with next/prev field).
  Iterators outside of compilation unit are allows pointers to iterators, these iterators are never actually changed.
  Instead the pointer is changed to point to another iterator.
*/
struct _iterator_ {
  Iterator next;                                  //<! Iterator to next entry, or NULL when this is the last entry.
  Iterator prev;                                  //<! Iterator to previous entry, or NULL when this is the first entry.
  void * pItem;                                   //<! Pointer to actual data, may only be NULL for the 'NULL iterator'.
  List container;                                 //<! Pointer to container, or NULL when pointer is in an invalid state.
};


// ----------------- Local Variables --------------------------------------
static struct _iterator_ _NULL_iterator_ = { NULL, NULL, NULL, NULL }; //<! NULL iterator, allows functions to return a valid pointer to an invalid iterator without creating potential memory leaks.


// ----------------- Local Function declarations --------------------------

/*! \brief Get the 'NULL iterator'.
  Returns a valid pointer to an invalid iterator without requirement to allocate a new iterator.
  \return Invalid iterator.
*/
static inline Iterator getNullIterator();


// ----------------- Global Function definitions --------------------------
List createList() {
  List list = (List)malloc(sizeof(struct _list_));
  list->first = NULL;
  list->last = NULL;
  list->uiSize = 0;
  return list;
}

void clearList(List list) {
  if (!list) {
    return;
  }
  Iterator current = list->first;
  Iterator next = NULL;
  while (current) {
    next = current->next;
    free(current->pItem);
    free(current);
    current = next;
  }
  list->first = NULL;
  list->last = NULL;
  list->uiSize = 0;
}

void destroyList(List list) {
  if (!list) {
    return;
  }
  clearList(list);
  free(list);
}

Iterator getBegin(List list) {
  if (!list) {
    return getNullIterator();
  }
  return list->first;
}
Iterator getEnd(List list) {
  if (!list) {
    return getNullIterator();
  }
  return list->last;
}

uint32_t getSize(List list) {
  if (!list) {
    return 0;
  }
  return list->uiSize;
}

Iterator getEntry(List list, uint32_t uiIndex) {
  if (!list) {
    return getNullIterator();
  }
  uint32_t i;
  Iterator iter;
  if (list->uiSize / 2 < uiIndex) {
    for (i = list->uiSize - 1, iter = getEnd(list); i > uiIndex && iter; --i, movePrevious(&iter));
  } else {
    for (i = 0, iter = getBegin(list); i < uiIndex && iter; ++i, moveNext(&iter));
  }
  if (i != uiIndex || !iter) {
    return getNullIterator();
  } else {
    return iter;
  }
}

int8_t addEntry(List list, Iterator iterator, void * pItem) {
  if (!list || !pItem) {
    return 0;
  }
  if (!iterator) {
    iterator = getNullIterator();
  } else if (list != iterator->container) {
    return 0;
  }
  Iterator newEntry = (Iterator)malloc(sizeof(struct _iterator_));
  newEntry->prev = NULL;
  newEntry->next = NULL;
  newEntry->container = list;
  newEntry->pItem = pItem;
  ++list->uiSize;
  if (!iterator->next) {
    list->last = newEntry;
  } else {
    newEntry->next = iterator->next;
    newEntry->next->prev = newEntry;
  }
  if (!iterator->pItem) {
    list->first = newEntry;
  } else {
    newEntry->prev = iterator;
    iterator->next = newEntry;
  }
  return 1;
}

int8_t removeEntry(List list, Iterator iterator) {
  if (!list || !iterator) {
    return 0;
  }
  if (list != iterator->container || !iterator->pItem) {
    return 0;
  }
  --list->uiSize;
  if (!iterator->prev) {
    list->first = iterator->next;
  } else {
    iterator->prev->next = iterator->next;
  }
  if (!iterator->next) {
    list->last = iterator->prev;
  } else {
    iterator->next->prev = iterator->prev;
  }
  free(iterator->pItem);
  free(iterator);
  return 1;
}

void * getCurrent(Iterator iterator) {
  if (!iterator) {
    return NULL;
  }
  return iterator->pItem;
}

int8_t moveNext(Iterator * pIterator) {
  if (!pIterator) {
    return 0;
  }
  Iterator iterator = *pIterator;
  if (!iterator) {
    return 0;
  }
  *pIterator = iterator->next;
  return 1;
}
int8_t movePrevious(Iterator * pIterator) {
  if (!pIterator) {
    return 0;
  }
  Iterator iterator = *pIterator;
  if (!iterator) {
    return 0;
  }
  *pIterator = iterator->prev;
  return 1;
}

int8_t isBegin(Iterator iterator) {
  if (!iterator) {
    return 1;
  }
  if (!iterator->pItem) {
    return 1;
  }
  return !iterator->prev || iterator->container->first == iterator;
}
int8_t isEnd(Iterator iterator) {
  if (!iterator) {
    return 1;
  }
  if (!iterator->pItem) {
    return 1;
  }
  return !iterator->next || iterator->container->last == iterator;
}


// ----------------- Local Function definitions ---------------------------
static inline Iterator getNullIterator() {
  return &_NULL_iterator_;
}

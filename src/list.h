#pragma once

/*! \file list.h
  \brief Doubly linked list.
*/

#include <stdint.h>

typedef struct _list_ * List;
typedef struct _iterator_ * Iterator;

// ----------------- List functions ---------------------------------------

/*! \brief Creates a new list.
  Creates a new list and returns it.
  Each list created by this function must be destroyed by 'destroyList(List)' to avoid memory leaks.
  \return Created list.
*/
List createList();
/*! \brief Clears a list.
  Clears a list, destroying all entries.
  All iterators to entries in the list are no longer valid.
  \param list List to clear.
*/
void clearList(List list);
/*! \brief Destroys a list.
  Destroys a list and all entries.
  All iterators to entries in the list are no longer valid.
  \param list List to destroy.
*/
void destroyList(List list);

/*! \brief Get iterator to begin of list.
  Returns an iterator to the first entry in the list.
  \param list List to get iterator from.
  \return Iterator to first entry.
*/
Iterator getBegin(List list);
/*! \brief Get iterator to end of list.
  Returns an iterator to the last entry in the list.
  \param list List to get iterator from.
  \return Iterator to last entry.
*/
Iterator getEnd(List list);

/*! \brief Get size of a list.
  Returns the total amount of entries in the list.
  \param list List to get size of.
  \return Size of the list.
*/
uint32_t getSize(List list);

/*! \brief Get entry at given location.
  Returns an iterator to the entry at given index in the list.
  \param list List to get iterator from.
  \param uiIndex Index of entry, if equal or greater to size 'getEnd(List)' is returned.
  \return Iterator to entry at given index or last entry.
*/
Iterator getEntry(List list, uint32_t uiIndex);

/*! \brief Add entry to list.
  Adds an entry to the list after 'iterator'.
  When 'iterator' is set to NULL, the entry is inserted at the beginning of the list.
  \param list List to add entry to.
  \param iterator Entry preceding entry to add.
  \param pItem Pointer to data of new entry.
  \return 1 on success, else 0.
*/
int8_t addEntry(List list, Iterator iterator, void * pItem);
/*! \brief Remove entry from list.
  Removes an entry to the list pointed to by 'iterator', destroying removed entry.
  Iterators pointing to removed entry are invalid.
  \param list List to removed entry from.
  \param iterator Entry to remove.
  \return 1 on success, else 0.
*/
int8_t removeEntry(List list, Iterator iterator);


// ----------------- Iterator functions -----------------------------------

/*! \brief Get data of iterator.
  Returns a pointer to the data of an entry.
  \param iterator Entry to get data from.
  \return Pointer to data or NULL when iterator is invalid.
*/
void * getCurrent(Iterator iterator);

/*! \brief Move iterator to next entry.
  Moves the iterator to the next entry in its list.
  The iterator is set to NULL when it left list bounds.
  \param pIterator Pointer to iterator.
  \return 1 if iterator points to an entry in the list, 0 if it went out of bounds.
*/
int8_t moveNext(Iterator * pIterator);
/*! \brief Move iterator to previous entry.
  Moves the iterator to the previous entry in its list.
  The iterator is set to NULL when it left list bounds.
  \param pIterator Pointer to iterator.
  \return 1 if iterator points to an entry in the list, 0 if it went out of bounds.
*/
int8_t movePrevious(Iterator * pIterator);

/*! \brief Is first element of list.
  Returns a value indicating 'iterator' points to first element in its list.
  \param iterator Iterator to check.
  \return 1 if first entry, 0 otherwise.
*/
int8_t isBegin(Iterator iterator);
/*! \brief Is last element of list.
  Returns a value indicating 'iterator' points to last element in its list.
  \param iterator Iterator to check.
  \return 1 if last entry, 0 otherwise.
*/
int8_t isEnd(Iterator iterator);

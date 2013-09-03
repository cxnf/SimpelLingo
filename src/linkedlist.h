/*!
  \file Doubly linked list.
*/

#include <stdint.h>

typedef struct _list_ * List;
typedef struct _iterator_ * Iterator;

List createList();
void clearList(List list);
void destroyList(List list);

Iterator getBegin(List list);
Iterator getEnd(List list);

uint32_t getSize(List list);

Iterator getEntry(List list, uint32_t uiIndex);

int8_t addEntry(List list, Iterator iterator, void * pItem);

int8_t removeEntry(List list, Iterator iterator);

void * getCurrent(Iterator iterator);

int8_t moveNext(Iterator * pIterator);
int8_t movePrevious(Iterator * pIterator);

int8_t isBegin(Iterator Iterator);
int8_t isEnd(Iterator iterator);

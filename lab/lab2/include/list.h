#ifndef __LIST_H__
#define __LIST_H__

#include "assert.h"
#include "kernel.h"


struct ListHead {
	struct ListHead *prev, *next;
};
typedef struct ListHead ListHead;

#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) - (int)(&((type*)0)->member)))



#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)


#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))


static inline void
list_add(ListHead *prev, ListHead *next, ListHead *data) {
	assert(data != NULL);
	data->prev = prev;
	data->next = next;
	if (prev != NULL) prev->next = data;
	if (next != NULL) next->prev = data;
}

static inline void
list_add_before(ListHead *list, ListHead *data) {
	list_add(list->prev, list, data);
}

static inline void
list_add_after(ListHead *list, ListHead *data) {
	list_add(list, list->next, data);
}

static inline void
list_del(ListHead *data) {
	assert(data != NULL);
	ListHead *prev = data->prev;
	ListHead *next = data->next;
	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;
}

static inline void
list_init(ListHead *list) {
	list->prev = list->next = list;
}

static inline boolean
list_empty(ListHead *list) {
	return list == list->next;
}

#define list_foreach(ptr, head) \
	for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#endif


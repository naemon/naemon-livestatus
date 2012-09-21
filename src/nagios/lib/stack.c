#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "lnag-utils.h"

struct stack {
	int (*compare)(const void *, const void *);
	void **list;
	unsigned int alloc, pos;
	int is_sorted;
};

void stack_sort(stack *s) {
	/* ignore empty and already sorted lists */
	if(!s || !s->list || !s->pos || s->is_sorted) {
		return;
	}

	qsort(s->list, s->pos, sizeof(void *), s->compare);
	s->is_sorted = 1;
}

/*
 * The engine of the "sorted list lookup". Arrives at the right
 * conclusion by bisecting its way around inside the sorted list.
 */
int stack_find_pos(stack *s, const void *key) {
	int value;
	unsigned int high, low = 0;

	if(!s || !s->pos || !s->list) {
		return -1;
	}

	if(!s->is_sorted)
		stack_sort(s);

	high = s->pos;
	while(high - low > 0) {
		unsigned int mid = low + ((high - low) / 2);

		value = s->compare(&key, &s->list[mid]);
		if(value > 0) {
			low = mid + 1;
			continue;
		}
		else if(value < 0) {
			high = mid;
			continue;
		}
		else {
			return mid;
		}
	}

	return -1;
}

void *stack_find(stack *s, const void *key) {
	int slot = stack_find_pos(s, key);

	if(slot < 0)
		return NULL;
	return s->list[slot];
}

static int stack_grow(stack *s, unsigned int hint) {
	void *ptr;

	if(!hint)
		return 0;

	ptr = realloc(s->list, (s->alloc + hint) * sizeof(void *));
	if(!ptr)
		return -1;
	s->list = ptr;
	s->alloc += hint;
	return 0;
}

int stack_push(stack *s, void *item) {
	if(s->pos >= s->alloc - 1 && stack_grow(s, alloc_nr(s->alloc)) < 0) {
		return -1;
	}

      	/*
      	 * No sorting is required when there's either only
      	 * one item in the list, or when the list is added
      	 * to in sequential order.
      	 */
	s->list[s->pos] = item;
	if(s->is_sorted && s->pos
	   && s->compare(&s->list[s->pos - 1], &s->list[s->pos]) > 0) {
		s->is_sorted = 0;
	}
	s->pos++;
	return 0;
}

void *stack_pop(stack *s) {
	void *item;

	if(!s->pos)
		return NULL;
	s->pos--;
	item = s->list[s->pos];
	s->list[s->pos] = NULL;
	return item;
}

stack *stack_init(unsigned int hint, int (*cmp)(const void *, const void *)) {
	stack *s;

	s = calloc(1, sizeof(*s));
	if(!s)
		return NULL;
	if(hint)
		stack_grow(s, hint);

	s->compare = cmp;

	return s;
}

int stack_set_list(stack *s, void **list, unsigned int items, int sorted) {
	if(!s || !list || !items)
		return -1;

	s->list = list;
	s->pos = items;
	s->alloc = 0;
	if(!sorted) {
		stack_sort(s);
	}
	return 0;
}

void stack_free_items(stack *s) {
	unsigned int i;

	if(!s || !s->list)
		return;
	for(i = 0; i < s->pos; i++)
		free(s->list[i]);
	s->pos = 0;
}

void *stack_destroy(stack *s, int items_too) {
	if(!s)
		return NULL;

	if(items_too)
		stack_free_items(s);

	if(s->list)
		free(s->list);
	s->list = NULL;
	free(s);

	return NULL;
}

unsigned int stack_entries(stack *s) {
	if(!s)
		return 0;

	return s->pos;
}

void *stack_get_list(stack *s) {
	if(!s)
		return NULL;

	return s->list;
}

void stack_foreach(stack *s, void *arg, int (*cb)(void *, void *)) {
	int i;

	if(!s || !s->list || !s->pos)
		return;

	for(i = 0; i < s->pos; i++) {
		cb(arg, s->list[i]);
	}
}

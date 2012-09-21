/**
 * @file stack.h
 *
 * @brief stack implementation (as an array, obviously)
 */

#ifndef include_stack_h__
#define include_stack_h__
#include <stdlib.h>

/** flag passable to stack_destroy() */
#define STACK_FREE_ITEMS 1

/** forward declaration */
struct stack;
/** opaque type used for stack operations */
typedef struct stack stack;

/**
 * Locates the position of a particular entry in the stack.
 * @param s The stack object
 * @param key The key object, as passed to the sort/compare function
 * @return The position of the slot on success, -1 on errors
 */
extern int stack_find_pos(stack *s, const void *key);

/**
 * Locates and returns the object stashed at a particular place in
 * the stack
 * @param s The stack
 * @param key The key object, as passed to the sort/compare function
 * @return The object of desire on success, NULL on errors
 */
extern void *stack_find(stack *s, const void *key);

/**
 * Sorts the stack object, making it suitable for lookups.
 * In order to avoid checking the sorted-ness of a list when
 * doing lookups on it, the user has to tell the API when to
 * sort the lists. This is the call to make for making sure that
 * happens. This function uses qsort(3) to sort the list.
 * @param s The unsorted sorted-list object
 */
extern void stack_sort(stack *sl);

/**
 * Adds an item to the tail of the stack
 * This marks the list as unsorted and forces a sort next time a
 * lookup in the list is done. stack_add(sl, item) is a macro alias
 * for this function.
 * @param s The stack to add to
 * @param item The item to add
 * @return 0 on success, -1 on errors
 */
extern int stack_push(stack *s, void *item);

/** @see stack_push */
#define stack_add(sl, item) stack_push(sl, item)

/**
 * Pops the latest added item from the stack
 * If the list is sorted, the item sorted to the last position is
 * popped instead.
 * @param s The stack to pop from
 * @return The last item in the list on success. NULL on errors
 */
extern void *stack_pop(stack *sl);

/**
 * Initializes a stack object
 * @param hint Initial slot allocation
 * @param cmp Comparison function, used for sorting and finding
 *            items in the stack
 * @return An initialized stack object on success. NULL on errors
 */
extern stack *stack_init(unsigned int hint, int (*cmp)(const void *, const void *));

/**
 * Set the list of items to operate on
 * Many applications will store lists of items that they pre-sort and
 * create manually with great efficiency. In those cases the application
 * can set the list they want to search using this function.
 * The list isn't copied and its items shouldn't be released, so one
 * should avoid calling stack_release() on a stack object where
 * the list is stack-allocated. Such lists must be free()'d manually.
 * @param s The stack object to attach the list to
 * @param list The list to use for sorted lookups
 * @param items The number of items in the list
 * @param sorted Should be 0 if the list needs sorting and 1 otherwise
 * @return 0 on success, -1 if s is NULL, -2 if s already has a list
 */
extern int stack_set_list(stack *s, void **list, unsigned int items, int sorted);

/**
 * free()'s memory associated with the stack table.
 * Note that the stack object itself isn't released. Only the list
 * base pointer. If you've used stack_set_list() to set the list for
 * this stack object, you must be careful to ensure that the
 * list can be free'd without causing segmentation violations or
 * memory leaks.
 * @param s The stack object
 */
extern void stack_release(stack *sl);

/**
 * free()'s each list entry of the object, leaving the allocated list
 * entries intact and making it reusable for other objects.
 *
 * Don't use this on statically allocated lists.
 *
 * @param s The stack object
 */
extern void stack_free_items(stack *sl);

/**
 * free()'s the list itself and the stack object. This function
 * returns NULL for convenience to assign it to the stack pointer in
 * the caller. If items_too is non-zero, each list entry is also free()'d.
 *
 * @param s The stack object
 * @param flags bitmask value. Pass stack_FREE_ITEMS to free items
 * @return NULL
 */
extern void *stack_destroy(stack *s, int flags);

/**
 * Get the current number of entries in the given stack
 *
 * @param s The stack object
 * @return The current number of entries in the given list object
 */
extern unsigned int stack_entries(stack *sl);

/**
 * Returns the list pointer of the given stack
 * @param s The stack object
 * @return The list pointer
 */
extern void *stack_get_list(stack *sl);

/**
 * Runs the given callback function for each element of the sorted
 * list.
 * @param s The stack object
 * @param arg Any random pointer, passed as first argument to the callback
 * @param cb The callback function to call for each element
 */
extern void stack_foreach(stack *s, void *arg, int (*cb)(void *, void *));
#endif /* include_stack_h__ */

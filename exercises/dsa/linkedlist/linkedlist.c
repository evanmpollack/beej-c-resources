#include "linkedlist.h"

/**
 * NOTES:
 * 
 * - const void* instead of const void *const for two reasons:
 *  1. pointer constness is lost when stored in node, which doesn't make semantic sense. The pointer can change through the set function.
 *  2. Changes to the address aren't reflected in the caller because the address is passed by value. 
 *      Therefore, the pointer is essentially const inside the function even though it's not stored in read only memory
 * 
 * - No need to check if an an integer is converted to unsigned because unsigned integers wrap around like an odometer in both directions. so -1 is 2^32 - 1 if converted from int to unsigned int on a 64 bit, two's complement machine
 */


struct node {
    // const void* because the container never changes the data. However, it is possible that the pointer is updated to another address if the set function is called.
    const void* data;
    struct node* next;
    struct node* prev;
};

void ll_free(struct linked_list* list) {
    if (list == NULL || list->size == 0)
        return;
    struct node* curr = list->head;
    while (curr != NULL) {
        struct node* next = curr->next;
        free(curr);
        curr = next;
    }
}

// returning small struct (<100 bytes) is apparently idiomatic modern C and modern compilers can optimize around this
struct linked_list ll_create(void) {
    return (struct linked_list) {0, NULL, NULL};
}

static struct node* traverse(const struct linked_list *const list, const unsigned long long index) {
    struct node* curr = list->head;
    for (int i = 0; i < index && curr != NULL; i++) {
        curr = curr->next;
    }
    return curr;
}

static struct node* create_node(const void* data) {
    struct node* node = malloc(sizeof(struct node));
    if (node == NULL) 
        return NULL;
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void ll_add_first(struct linked_list *const list, const void* data) {
    if (list == NULL || list->size == ULLONG_MAX)
        return;
    struct node* new_head = create_node(data);
    if (new_head == NULL)
        return;
    new_head->next = list->head;
    if (list->head != NULL)
        list->head->prev = new_head;
    list->head = new_head;
    list->tail = (list->tail == NULL) ? list->head : list->tail;
    list->size++;
}

void ll_add_last(struct linked_list *const list, const void* data) {
    if (list == NULL || list->size == ULLONG_MAX)
        return;
    struct node* new_tail = create_node(data);
    if (new_tail == NULL)
        return;
    new_tail->prev = list->tail;
    if (list->tail != NULL)
        list->tail->next = new_tail;
    list->tail = new_tail;
    list->head = (list->head == NULL) ? list->tail : list->head;
    list->size++;
}

void ll_add(struct linked_list *const list, const unsigned long long index, const void* data) {
    // check for above 0 for the case where the size is zero and the user want's to add at index 0. This is valid for the first entry only.
    
    // What about case where list->size == LLU_MAX
    if (list == NULL || list->size == ULLONG_MAX || (list->size > 0 && index >= list->size))
        return;
    if (index == 0) {
        ll_add_first(list, data);
        return;
    }
    if (index == list->size) {
        ll_add_last(list, data);
        return;
    }
    struct node* prev = traverse(list, index - 1);
    struct node* next = prev->next;
    struct node* added = create_node(data);
    if (added == NULL)
        return;
    added->prev = prev;
    added->next = next;
    prev->next = added;
    next->prev = added;
    list->size++;
}

const void* ll_remove_first(struct linked_list *const list) {
    if (list == NULL || list->size == 0)
        return NULL;
    struct node* old_head = list->head;
    if (list->head == list->tail)
        list->tail = NULL;
    list->head = list->head->next;
    const void* data = old_head->data;
    free(old_head);
    list->size--;
    return data;
}

const void* ll_remove_last(struct linked_list *const list) {
    if (list == NULL || list->size == 0)
        return NULL;
    struct node* old_tail = list->tail;
    if (list->tail == list->head)
        list->head = NULL;
    list->tail = list->tail->prev;
    const void* data = old_tail->data;
    free(old_tail);
    list->size--;
    return data;
}

const void* ll_remove(struct linked_list *const list, const unsigned long long index) {
    if (list == NULL || list->size == 0 || index >= list->size)
        return NULL;
    if (index == 0)
        return ll_remove_first(list);
    if (index == list->size - 1)
        return ll_remove_last(list);
    struct node* to_remove = traverse(list, index);
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    const void* data = to_remove->data;
    free(to_remove);
    list->size--;
    return data;
}

bool ll_is_empty(const struct linked_list *const list) {
    return list != NULL && list->size == 0;
}

const void* ll_get(const struct linked_list *const list, const unsigned long long index) {
    if (list == NULL || index >= list->size)
        return NULL;
    return traverse(list, index)->data;
}

void ll_set(const struct linked_list *const list, const unsigned long long index, const void* data) {
    if (list == NULL || index >= list->size)
        return;
    traverse(list, index)->data = data;
}

bool ll_contains(const struct linked_list *const list, const void* target, int (*compare)(const void*, const void*)) {
    bool contains = false;
    for (struct node* curr = list->head; curr != NULL; curr = curr->next) {
        if (compare(curr->data, target) != 0)
            continue;
        contains = true;
        break;
    }
    return contains;
}
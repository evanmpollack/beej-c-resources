#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdbool.h>


struct node {
    void* data;
    struct node* next;
    struct node* prev;
};

struct linked_list {
    unsigned long long size;
    struct node* head;
    struct node* tail;
};

void free_linked_list(struct linked_list* list) {
    if (list == NULL || list->size == 0)
        return;
    struct node* curr = list->head;
    while (curr != NULL) {
        struct node* next = curr->next;
        free(curr);
        curr = next;
    }
}

void create(struct linked_list* list) {
    if (list != NULL)
        free_linked_list(list);
    struct linked_list l = {0, NULL, NULL};
    *list = l;
}

static struct node* traverse(const struct linked_list *const list, const unsigned long long index) {
    struct node* curr = list->head;
    for (int i = 0; i < index && curr != NULL; i++) {
        curr = curr->next;
    }
    return curr;
}

static struct node* create_node(const void *const data) {
    struct node* node = malloc(sizeof(struct node));
    if (node == NULL) 
        return NULL;
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void add_first(struct linked_list *const list, const void *const data) {
    if (list == NULL)
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

void add_last(struct linked_list *const list, const void *const data) {
    if (list == NULL)
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

void add(struct linked_list *const list, const unsigned long long index, const void *const data) {
    // No need to check for input index below 0 -> if an int is converted into an unsigned int, it wraps around. For example -1 to unsigned would be 2^32 - 2
    // check for above 0 for the case where the size is zero and the user want's to add at index 0. This is valid for the first entry only.
    if (list == NULL || (list->size > 0 && index >= list->size))
        return;
    if (index == 0) {
        add_first(list, data);
        return;
    }
    if (index == list->size - 1) {
        add_last(list, data);
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

void* remove_first(struct linked_list *const list) {
    if (list == NULL || list->size == 0)
        return NULL;
    struct node* old_head = list->head;
    if (list->head == list->tail)
        list->tail = NULL;
    list->head = list->head->next;
    void* data = old_head->data;
    free(old_head);
    list->size--;
    return data;
}

void* remove_last(struct linked_list *const list) {
    if (list == NULL || list->size == 0)
        return NULL;
    struct node* old_tail = list->tail;
    if (list->tail == list->head)
        list->head = NULL;
    list->tail = list->tail->prev;
    void* data = old_tail->data;
    free(old_tail);
    list->size--;
    return data;
}

void* remove(struct linked_list *const list, const unsigned long long index) {
    if (list == NULL || list->size == 0 || index >= list->size)
        return NULL;
    if (index == 0)
        return remove_first(list);
    if (index == list->size - 1)
        return remove_last(list);
    struct node* to_remove = traverse(list, index);
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    void* data = to_remove->data;
    free(to_remove);
    list->size--;
    return data;
}

bool is_empty(const struct linked_list *const list) {
    return list != NULL && list->size == 0;
}

void* get(const struct linked_list *const list, const unsigned long long index) {
    if (list == NULL || index >= list->size)
        return NULL;
    return traverse(list, index)->data;
}

void set(const struct linked_list *const list, unsigned long long index, void* data) {
    if (list == NULL || index >= list->size)
        return;
    traverse(list, index)->data = data;
}

bool contains(const struct linked_list *const list, const void *const target, int (*compare)(const void*, const void*)) {
    bool contains = false;
    for (struct node* curr = list->head; curr != NULL; curr = curr->next) {
        if (compare(curr->data, target) != 0)
            continue;
        contains = true;
        break;
    }
    return contains;
}
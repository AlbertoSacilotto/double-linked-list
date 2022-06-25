/*----------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class>
 * ---------------------------------------------------------
 * Exercise Number: S02
 * Title:			Doubly Linked List implementation
 * Author:			
 * ----------------------------------------------------------
 * Description:
 * Implementation of a doubly linked list.
 * ----------------------------------------------------------
 */

/*
  Implementation notes:

  1) The 'ListData' struct of this linked list SHALL have
     - a pointer to the head node, 
     - a pointer to the tail node
     - and the size of the list 
    as members!

  2) List list, node, and iterator allocation: 
     Use functions `mem_alloc(…)` and `mem_free(…)`
     declared in `allocator.h`. DO NOT use `malloc(…)` and `free(…)` directly
     as unit tests will fail.

  3) Use 'limits.h' to get maximum and minimum values for numeric types, if needed.

  4) Implement 'list_iterator.h' in this file as well.

  5) Avoid code duplication wherever (reasonably) possible.
     This is valid for implementation of similar functions as well
     as for reoccurring code patterns, such as list iteration.
     Nevertheless, aim for efficiency, e.g. `remove_all` shall traverse 
     the list only once and not use `remove` as defined, because
     the later would start at the beginning of the list for each iteration.
*/

#include "doubly_linked_list_with_iterator.h"
#include "list_types.h"
#include "allocator.h"

/** The type of list nodes */
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct IntListData {
	int size;
	Node* head;
	Node* tail;
} IntListData;

/** The implementation of list node data */
IntList list_obtain() {
    IntList newList = alloc_mem(sizeof(IntListData));
    if (newList != 0) {
        newList->size = 0;
        newList->head = 0;
        newList->tail = 0;
    }
	return newList;
}

void list_release(IntList* p_list) {
	if (p_list == 0 || *p_list == 0) {
		return;
	}  
    if ((*p_list)->size == 0) {
        free_mem(*p_list);
        *p_list = 0;
        return;
    }

    Node* node = (*p_list)->head;
    for(int i = 0; i < (*p_list)->size; ++i) {
        Node* n = node;
        if (node->next) {
            node = node->next;
        }
        free_mem(n);
    }
    (*p_list)->size = 0;
    free_mem(*p_list);
    *p_list = 0;
}

/** The implementation of list data */

bool list_is_valid(IntList list) {
    return (list != 0);
}

bool list_is_empty(IntList list) {
    if (!list_is_valid(list)) {
        return true;
    }
    return list->size == 0;
}

int list_get_size(IntList list) {
    if (list_is_valid(list)) {
        return list->size;
    }
    return 0;
}

bool list_contains(IntList list, int value) {
    if (!list_is_valid(list)) return false;

    Node* node = list->head;
    for (int i = 0; i < list->size; ++i) {
        if (node->data == value) {
            return true;
        }
        node = node->next;
    }
    return false;    
}

int list_get_at(IntList list, unsigned int index) {
    if (!list_is_valid(list)) return 0;
    if (index >= list->size) return 0;

    Node* node = list->head;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }
    return node->data;
}

void list_insert(IntList list, int value) {
    if (!list_is_valid(list)) return;

    Node* newNode = alloc_mem(sizeof(Node));
    newNode->data = value;
    newNode->prev = 0;
    newNode->next = 0;

    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void list_insert_at(IntList list, unsigned int index, int value) {
    if (!list_is_valid(list)) return;
    if (index > list->size) {
        index = list->size;
    }

    Node* newNode = alloc_mem(sizeof(Node));
    newNode->data = value;
    newNode->prev = 0;
    newNode->next = 0;

    Node* node = list->head;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }
    if (list->size == 0) {
        list->tail = newNode;
        list->head = newNode;
    }
    else {
        if (index == list->size) {
            newNode->prev = list->tail;
            list->tail->next = newNode;
        }
        else {
            newNode->prev = node->prev;
            newNode->next = node;
            if (node->prev) {
                node->prev->next = newNode;
            }
            node->prev = newNode;
        }
        if (node == list->head) {
            list->head = newNode;
        }
        if (node == list->tail) {
            list->tail = newNode;
        }
    }
    list->size++;
}

void list_append(IntList list, IntList list_to_append) {
    if (!list_is_valid(list)) return;
    if (!list_is_valid(list_to_append)) return;
    if (list_to_append->size == 0) return;

    if (list->size == 0) {
        list->head = list_to_append->head;
        list->tail = list_to_append->tail;
        list->size = list_to_append->size;
        list_to_append->head = 0;
        list_to_append->tail = 0;
        list_to_append->size = 0;
    }
    else {
        Node* tailFirst = list->tail;
        Node* tailSecond = list_to_append->tail;
        Node* headSecond = list_to_append->head;

        tailFirst->next = headSecond;
        headSecond->prev = tailFirst;
        tailFirst = tailSecond;
        list->size += list_to_append->size;

        headSecond = 0;
        tailSecond = 0;
        list_to_append->size = 0;
    }
}

void list_remove(IntList list, int value) {
    if (!list_is_valid(list)) return;

    Node* node = list->head;
    for (int i = 0; i < list->size; ++i) {
        if (node->data == value) {
            list_remove_at(list, i);
            break;
        }
        node = node->next;
    }
}

void list_remove_all(IntList list, int value) {
    if (!list_is_valid(list)) return;
    if (list->size == 0) return;

    Node* node = list->head;
    int i = 0;
    while (1)
    {
        if (node->data == value) {
            list_remove_at(list, i);
            i = 0;
            node = list->head;
        }
        else {
            node = node->next;
            ++i;
        }
        if (node == 0) break;
    }
}

int list_remove_at(IntList list, unsigned int index) {
    if (list == 0 || index >= list->size) {
        return 0;
    }

    Node* node = list->head;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }

    int ret = node->data;
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    } else if (node == list->tail) {
        list->tail = node->prev;
    }

    free_mem(node);
    list->size--;
    return ret;
}

void list_clear(IntList list) {
    if (list == 0) {
        return;
    }
    if (list->size == 0) {
        list = 0;
        return;
    }

    Node* node = list->head;
    for (int i = 0; i < list->size; ++i) {
        Node* n = node;
        if (node->next) {
            node = node->next;
        }
        free_mem(n);
    }
    list->size = 0;
}
/** The implementation of list iterator data */
typedef struct IntListIteratorData {
    Node* head;
    Node* tail;
    Node* current;
} IntListIteratorData;

IntListIterator list_it_obtain(IntList list) {
    if (!list_is_valid(list)) return 0;
    IntListIterator it = alloc_mem(sizeof(IntListIteratorData));

    if (list->size == 0) {
        it->head = 0;
        it->tail = 0;
        it->current = 0;
    } else {
        it->head = list->head;
        it->current = list->head;
        it->tail = list->tail;
    }
    return it;
}

void list_it_release(IntListIterator* p_it) {
    free_mem(*p_it);
}

bool list_it_is_valid(IntListIterator it) {
    if (it != 0) {
        return it->current != 0;
    }
    return false;
}

bool list_it_next(IntListIterator it) {
    if (!list_it_is_valid(it)) return false;

    if (it->current->next != 0) {
        it->current = it->current->next;
        return true;
    }
    return false;
}

bool list_it_previous(IntListIterator it) {
    if (!list_it_is_valid(it)) return false;

    if (it->current->prev != 0) {
        it->current = it->current->prev;
        return true;
    }
    return false;
}

int list_it_get(IntListIterator it) {
    if (!list_it_is_valid(it)) return;

    return it->current->data;
}

void list_it_set(IntListIterator it, int value) {
    if (!list_it_is_valid(it)) return;

    it->current->data = value;
}


/* ===================================================================== */
/* private list functions */

/* abstract away and generalize also memory allocation for list nodes */
//static <node-type> list_obtain_node(<params>) {
//}
//
//static void list_release_node(<node-type>) {
//}
//
///* optional: implement a function for printing the content of the list - may be useful for debugging */
//void list_dump(char* prefix, IntList list) {
//}

/* ===================================================================== */

/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}


charval_t *new_charval(unsigned char c, int val) {
    charval_t *temp;

    temp = (charval_t *) emalloc(sizeof(charval_t));
    temp->c = c;
    temp->val = val;
    temp->next = NULL;

    return temp;
}


charval_t *add_front(charval_t *list, charval_t *cv) {
    cv->next = list;
    return cv;
}


charval_t *add_end(charval_t *list, charval_t *cv) {
    charval_t *curr;

    if (list == NULL) {
        cv->next = NULL;
        return cv;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = cv;
    cv->next = NULL;
    return list;
}


charval_t *peek_front(charval_t *list) {
    return list;
}


charval_t *remove_front(charval_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}


int index_c(charval_t *list, char ele){
	charval_t *curr = list;
	int count;
	for(count=0;curr != NULL;curr = curr->next,count++){
		if(curr->c==ele){
			return count;
		}
	}
	return -1;
}

//ALWAYS PASSED AS REFERENCE
charval_t* pop_c(charval_t **list, int index){
	charval_t *curr = *list;
	charval_t *prev = NULL;
	int count;
	for(count=0;curr != NULL;curr = curr->next,count++){
		if(index==count){
			if(prev!= NULL){
				prev->next = curr->next;
			}else if(prev == NULL){
				*list=curr->next;
			}

			return curr;	
			
		}
		prev = curr;
	}
	//return null char if not there
	return NULL;
}

charval_t* find_c(charval_t **list, int index){
	charval_t *curr = *list;
	int count;
	for(count=0;curr != NULL;curr = curr->next,count++){
		if(index==count){
			return curr;	
		}
	}
	//return null char if not there
	return NULL;

}

void free_charval(charval_t *cv) {
    assert(cv != NULL);
    free(cv);
}


void apply(charval_t *list,
           void (*fn)(charval_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}


void print_list(charval_t* head){
	/*This function prints a linked list out.*/

	/* In this function, we set up a temporary pointer to keep track of the current node.*/ 
	charval_t* current = head;
	
	/* We then use this pointer to cycle through the list nodes, printing the name and value of each one.*/
	while(current != NULL){
		printf("List char is '%c', value is %d.\n", current->c, current->val);
		
		current = current->next;
	}

	printf("\n");
}


void empty_list(charval_t* head){
	/*This function empties out the linked list, freeing all nodes and storage within.*/
	
	/*Now we're in a pickle. We have to free the current node before moving onto the next one. But if we free the current node, the current node's 'next' pointer is gone. We'll have to create a temporary pointer to store the node that's after the current one!*/
	charval_t* current = head;
	charval_t* next = head;
	
	/* Now, we cycle through the list. At each list, we store the value of 'next', then free the node's 'name' value and then the node itself. Finally, we move on to the node that's marked as the next node (unless that pointer is NULL - then we're done).*/
	while(next != NULL){
		next = current->next;
		free(current);
		
		if(next != NULL){ current = next; }
	}
}	

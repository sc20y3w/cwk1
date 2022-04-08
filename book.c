#include "book_management.h"
#include <stdio.h>
#include <stdlib.h>

BookList *init_book_list() {
    struct _BookList *head;
    head = (struct BookList *) malloc(sizeof(struct _BookList));
    head->list = NULL;
    head->length = 0;
    return head;
}
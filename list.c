#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int cmp_int(const int item1, const int item2){
  return item1-item2;
}

int cmp_chr(const char item1, const char item2){
  return item1-item2;
}

static p_ll create_list_item(void *new_data){
  p_ll new_node;

  if ((new_node = (p_ll)malloc(sizeof(t_ll)))!=NULL){
    new_node->data = new_data;
    new_node->next = NULL;
  }
  return new_node;
}

p_ll find_item_in_list(const p_ll head, const void *item, int (*cmp)()){
  p_ll ptr;
  int notfound;

  for (ptr=head, notfound=1; ptr && notfound; ){
    notfound = cmp(ptr->data,item);
    if (notfound){
      ptr = ptr->next;
    }
  }
  return ptr;
}

int del_list_item(d_ll head, const void *item, void (*free_on_del)(), int(*cmp)()){
  int rv = 0, cv;

  p_ll ptr = *head, tmp;

  /* test for empty list */
  if (ptr){
    /* not an empty list */
    /* the item that we want to delete the first item */
    cv = cmp(ptr->data,item);
    if (cv){
      /* not the first item */
      while (cv && ptr){
        tmp=ptr;
        ptr=ptr->next;
        if (ptr){
          cv = cmp(ptr->data,item);
        }
      }
      if (!cv){
        tmp->next = ptr->next;
        if (free_on_del){
          if (free_on_del != free){
            free_on_del(&ptr->data);
          }
          free(ptr->data);
          ptr->data = NULL;
        }
        free(ptr);
        ptr=NULL;
        rv = 1;
      }
    } else {
      /* found as the first data item */
      (*head) = (*head)->next;
      if (free_on_del){
        if (free_on_del != free){
          free_on_del(&ptr->data);
        }
        free(ptr->data);
        ptr->data = NULL;
      }
      free(ptr);
      ptr = NULL;
      rv = 1;
    }
  }
  return rv;
}


/*
 * this is recursive unordered add to the list.
 */
void add_list_node(d_ll head, void *data){
  /* Unordered list creator */
  d_ll node = head;
  if (*node == NULL){
    *node = create_list_item(data);
  } else {
    add_list_node(&(*node)->next,data);
  }
}

/*
 * this is recursive ordered add to the list.
 */
void add_ordered_list_node(d_ll head, void *data, int (*cmp)()){
  p_ll tmp;
  d_ll node = head;
  int cv;

  if (*node == NULL){
    *node = create_list_item(data);
  } else {
    cv = cmp((*node)->data, data);
    if (cv < 0) {
      add_ordered_list_node(&(*node)->next,data,cmp);
    } else {
      tmp=*node;
      *node=create_list_item(data);
      (*node)->next = tmp;
    }
  }
}


/*
 * this is iterative unordered add to the list.
 */

void add_node_to_list(d_ll head, void *data){
  p_ll ptr, holder;

  holder = *head;
  ptr = create_list_item(data);
  if (NULL == *head){
    *head = ptr;
  } else {
    while (NULL != (*head)->next){
      *head = (*head)->next;
    }
    (*head)->next = ptr;
    (*head) = holder;
  }
}

/*
 * this is iterative ordered add to the list.
 */

void add_node_to_ordered_list(d_ll head, void *data, int (*cmp)()){
  d_ll working = head;
  p_ll holder = *head;
  p_ll ptr, tmp;
  int cv, cv_next, done = 0;

  holder = *head;
  ptr = create_list_item(data);
  if (NULL == *working){
    *working = ptr;
  } else {
    while ( NULL != *working && !done) {
      // second and subsequent elements
      // start at working head
      // compare working with new item.
      cv=cmp((*working)->data,data);
      if (0>cv){
        // current item is bigger than new item
        if (NULL != (*working)->next){
          // move to the next element
          cv_next = cmp((*working)->next->data,data);
          if (0>cv_next){
            *working = (*working)->next;
          } else {
            ptr->next = (*working)->next;
            (*working)->next = ptr;
            done = 1;
            *head = holder;
          }
        } else {
          // at the end of the list as working->next is NULL
          // so it is safe to add new data to the end of the list/
          ptr->next = (*working)->next;
          (*working)->next = ptr;
          done =1;
          *head = holder;
        }
      } else if (0<cv) {
        tmp = *head;
        *working = ptr;
        (*working)->next = tmp;
        done = 1;
        head = working;
        holder = *head;
      }
    }
  }
}

void delete_all(d_ll head, void(*free_on_del)()){
  p_ll tmp;
  d_ll del;

  for (del=head; *del;){
    tmp=(*del)->next;
    if ((*del)->data && free_on_del){
      if (free_on_del!=free){
        free_on_del(&(*del)->data);
      }
      free((*del)->data);
      (*del)->data = NULL;
    }
    free(*del);
    *del=NULL;
    *del=tmp;
  }
}

int display_itemised_list(FILE *file, const p_ll head, int itemiser, int (*printrec)()){
  p_ll ptr;
  int c;

  if (file == stdout){
    printf("List start\n----------\n");
  }
  for (c=0, ptr = head; ptr; ptr=ptr->next){
    printf("%c ",++c+itemiser);
    printrec(file,ptr->data);
  }
  if (file == stdout){
    printf("----------\nList finish\n");
  }
  return c;
}

int display_list(FILE *file, const p_ll head, int (*printrec)()){
  p_ll ptr;
  int c;

  if (file == stdout){
    printf("List start\n----------\n");
  }
  for (c=0, ptr = head; ptr; ptr=ptr->next){
//    if (file == stdout)
//      printf("%3d ",++c);
    printrec(file,ptr->data);
  }
  if (file == stdout){
    printf("----------\nList finish\n");
  }
  return c;
}

void *get_item(p_ll head, int item_num){
  p_ll tmp=head;

  while (0 != item_num && NULL != tmp){
    item_num--;
    tmp = tmp->next;
  }
  return tmp?tmp->data:NULL;
}

int foreach(p_ll head, void (*action)()){
  int c=0;
  for (p_ll ptr = head; ptr; ptr = ptr->next){
    action(ptr->data);
    c++;
  }
  return c;
}

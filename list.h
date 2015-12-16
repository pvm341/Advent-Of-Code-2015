#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

/*
 * Linked list library
 *
 * Linked list that is generic
 *
 * The data member is a void pointer
 * the user must supply functions to cmpare and
 * display the data members
 *
 * int cmp(const void *item1, const void *item2)
 *  return val  <0 where item1 is less than item2
 *             ==0 where item1 equals item2
 *              >0 where item1 is greater then item2
 *  int display (FILE *file, const void* item);
 *   return the number of characters written to the file
 *
 */

struct s_ll {
  void *data;
  struct s_ll *next;
};

typedef struct s_ll t_ll, *p_ll, **d_ll;

int cmp_int(const int item1, const int item2);
int cmp_chr(const char item1, const char item2);

// Recursive routines to add to the list
void add_list_node(d_ll head, void *data);  // O(n)
void add_ordered_list_node(d_ll head, void *data, int (*cmp)()); //O(n/2)
// iterative routines to add to the list
void add_node_to_list(d_ll head, void *data);  //O(1)
void add_node_to_ordered_list(d_ll head, void *data, int (*cmp)()); //O(n/2)

void delete_all(d_ll head, void(*free_on_del)());
int display_list(FILE *file, const p_ll head, int (*printrec)());
int display_itemised_list(FILE *file, const p_ll head, int itemiser, int (*printrec)());
int del_list_item(d_ll head, const void *item, void (*free_on_del)(), int(*cmp)());
int delete_item(d_ll head, const void *item, void(*free_on_del)(), int(*cmp)());
p_ll find_item_in_list(const p_ll head, const void *item, int (*cmp)());
void *get_item(p_ll head, int item_num);
int foreach(p_ll head, void (*action)());
#endif // LIST_H_INCLUDED

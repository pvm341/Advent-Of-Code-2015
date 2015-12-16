#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "list.h"

#define TEST 0
///07752394651

struct s_wire {
  char name[3];
  unsigned short signal;
};

typedef struct s_wire t_wire, *p_wire;

int wirecmp_name(const p_wire wire_a,const p_wire wire_b);
int wirecmp_name2str(const p_wire wire,const char *string);
int print_wire(FILE *file,p_wire awire);
p_wire get_wire(char *name);
char *get_name(char *name, char *p, const char min, const char max);

void act_on_input(char *input);

typedef enum {NOT,OR,AND,LSHIFT,RSHIFT,EOLIST} enOPERATORs;

char *operators[] = {"NOT","OR","AND","LSHIFT","RSHIFT",NULL};


#if TEST
char * testinput[] =  {
                      "123 -> x",
                      "456 -> y",
                      "x AND y -> d",
                      "x OR y -> e",
                      "x LSHIFT 2 -> f",
                      "y RSHIFT 2 -> g",
                      "NOT x -> h",
                      "NOT y -> i",NULL
                      };

#endif
p_ll wires = NULL;

int main(){

#if TEST
  for (int i=0;testinput[i];i++){
    act_on_input(testinput[i]);
  }
#else
  FILE *file;
  char line[81];

  if (NULL != (file=fopen("xmascha7.txt","rt"))){
    fgets(line,80,file);
    while (!feof(file)){
      act_on_input(line);
      printf("%s",line);
       //display_list(stdout,wires,print_wire);
      fgets(line,80,file);
    }
    fclose(file);
  }

#endif // TEST
  display_list(stdout,wires,print_wire);
  delete_all(&wires,free);
  return 0;
}

int value_from_string(char *str){
  int v = 0;


  while (isdigit(*str)){
    v*=10;
    v+= *str -'0';
    str++;
  }
  return v;
}

int in_range(const char check, const char min, const char max){
  return (check >= min && check <= max);
}

char *pass_while_equal(char *p,const char ch){
  while (ch == *p){
    p++;
  }
  return p;
}

char *pass_while_not_equal(char *p,const char ch){
   while (ch != *p){
     p++;
   }
   return p;
}

char *pass_while_in_list(char *p, const char *list){
  while (NULL != strpbrk(p,list)){
    p++;
  }
  return p;
}

char *pass_while_not_in_list(char *p, const char *list){
  while (NULL == strpbrk(p,list)){
    p++;
  }
  return p;
}

char *pass_while_in_range(char *p, const char min, const char max){
  while (min <= *p && max >= *p){
    p++;
  }
  return p;
}

char *pass_while_not_in_range(char *p, const char min, const char max){
  while (!(min <= *p && max >= *p)){
    p++;
  }
  return p;
}

void act_on_input(char *input){
  p_wire sw1, sw2, dw1;
  char name[3], *p;
  char optor[10];
  int signal, opt,oprand;

  if ((strstr(input,"b ") && input[0] == 'b') || (strstr(input," b\n")))
    printf(input);
  // Is this a wire initialisation
  p=input;
  if (isdigit(*p)){
    signal = value_from_string(p);
    p=pass_while_not_equal(p,' ');
    p=pass_while_in_list(p," ->");
    if ('a' <= *p && 'z' >= *p){
      p= get_name(name,p,'a','z');
    }
    dw1 = get_wire(name);
    dw1->signal = signal;
  //Operator first;
  } else if ('A'<= *p && 'Z' >= *p){
    p=get_name(optor,p,'A','Z');
    p=pass_while_equal(p,' ');
    if ('a' <= *p && 'z' >= *p){
      get_name(name,p,'a','z');
      sw1 = get_wire(name);
      if (NULL != sw1){
        p=pass_while_in_list(p," ->");
        p = get_name(name,p,'a','z');
        dw1 = get_wire(name);
        dw1->signal = sw1->signal ^ 0xffff;
      }
    }
    // a wire first
  } else if ('a' <= *p && 'z' >= *p){
    p=get_name(name,p,'a','z');
    sw1 = get_wire(name);
    p=pass_while_equal(p,' ');
    if ('A'<= *p && 'Z' >= *p){
      p=get_name(optor,p,'A','Z');
      p=pass_while_equal(p,' ');
      if ('0' <= *p && '9' >= *p){
        oprand = value_from_string(p);
        p=pass_while_in_range(p,'0','9');
      } else if ('a' <= *p && 'z' >= *p){
        p=get_name(name,p,'a','z');
        sw2 = get_wire(name);
      }
    }
    p=pass_while_in_list(p,"-> ");
    p=get_name(name,p,'a','z');
    dw1=get_wire(name);

    for (opt=NOT;operators[opt] && strcmp(operators[opt],optor);opt++);/* void loop */
    switch(opt){
      case AND:
        dw1->signal = sw1->signal & sw2->signal;
        break;
      case OR:
        dw1->signal = sw1->signal | sw2->signal;
        break;
      case RSHIFT:
        dw1->signal = sw1->signal >> (oprand);
        break;
      case LSHIFT:
        dw1->signal = sw1->signal << (oprand);
        break;
      case NOT:
        dw1->signal = sw1->signal ^ 0xffff;
        break;
      case EOLIST:
        dw1->signal = sw1->signal;
        break;
    }
  }
}


p_wire get_wire(char *name){
  p_ll wire_ptr;
  p_wire wire;

  if (NULL == (wire_ptr = find_item_in_list(wires,name,wirecmp_name2str))){
    printf("New-");
    if (NULL != (wire=(p_wire)malloc(sizeof(t_wire)))){
      strcpy(wire->name,name);
      wire->signal = 0;
      add_node_to_ordered_list(&wires,wire,wirecmp_name);
    } else {
      fprintf(stderr,"NULL malloc %s:,\n",name);
    }
  } else {
    printf("Found-");
    wire = wire_ptr->data;
  }
  printf("%2s: %d ",wire->name,wire->signal);
  return wire;
}

char *get_name(char *name, char *p, const char min, const char max){
  char *n=name;
  while(in_range(*p,min,max)){
    *n++ = *p++;
  }
  *n =0;
  return p;
}

int wirecmp_name(const p_wire wire_a,const p_wire wire_b){
  return strcmp(wire_a->name,wire_b->name);
}

int wirecmp_name2str(const p_wire wire,const char *string){
  return strcmp(wire->name,string);
}

int print_wire(FILE *file,p_wire awire){
  return printf("%2s: %d\n",awire->name,awire->signal);
}


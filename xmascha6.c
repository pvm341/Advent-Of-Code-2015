#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEST 0

typedef enum {TURNOFF, TURNON, TOGGLE} en_ACTIONS;
char *actionstrs[] = {"TURNOFF", "TURNON", "TOGGLE"};
char *searchstrs[] = {"turn off ","turn on ","toggle ", NULL};

struct s_action {
  en_ACTIONS todo;
  int sx, sy, fx, fy;
};

typedef struct s_action t_action;
#if TEST
void action_lights_stage_1(int lights_array[1000][1000], const t_action action);
#else
void action_lights_stage_1(int lights_array[1000][1000], char *actstr);
void action_lights_stage_2(int lights_array[1000][1000], char *actstr);
#endif // TEST
int count_lights(int lights_array[1000][1000]);
int count_brightness(int lights_array[1000][1000]);
t_action decode_line(char *string);
void display_action(t_action action);

int main(){
  int lights[1000][1000];
#if TEST
  t_action action;
  action_lights_stage_1(lights,(t_action){TURNOFF,0,0,999,999});
  action_lights_stage_1(lights,(t_action){TURNON,0,0,2,2});
  printf("there are %d lights on\n",count_lights(lights));
  action_lights_stage_1(lights,(t_action){TURNOFF,0,0,999,999});
  action_lights_stage_1(lights,(t_action){TURNON,499,499,500,500});
  printf("there are %d lights on\n",count_lights(lights));
  action = decode_line("turn on 545,676 through 545,929");
  display_action(action);
  action = decode_line("turn off 316,716 through 413,941");
  display_action(action);
  action = decode_line("toggle 488,826 through 755,971");
  display_action(action);
#else
  FILE *file;
  t_action action;
  char line[50];

  if (NULL != (file=fopen("xmascha6.txt","rt"))){
    action_lights_stage_1(lights,"turn off 0,0 through 999,999");
    fgets(line,49,file);
    while (!feof(file)){
      action_lights_stage_1(lights,line);
      fgets(line,49,file);
    }
    printf("The number of lights in stage one is %d\n",count_lights(lights));
    fclose(file);
  }
  if (NULL != (file=fopen("xmascha6.txt","rt"))){
    action_lights_stage_1(lights,"turn off 0,0 through 999,999");
    fgets(line,49,file);
    while (!feof(file)){
      action_lights_stage_2(lights,line);
      fgets(line,49,file);
    }
    printf("The brightness of lights in stage two is %d\n",count_brightness(lights));

    fclose(file);
  }
#endif // TEST
  return 0;
}

t_action decode_line(char *string){
  t_action action;
  char *p;

  action.todo = TURNOFF;
  while (searchstrs[action.todo] && NULL == (p = strstr(string,searchstrs[action.todo]))){
    action.todo++;
  }
  if (p){
    p+=strlen(searchstrs[action.todo]);
  }
  sscanf(p,"%d,%d through %d,%d",&action.sx,&action.sy,&action.fx,&action.fy);
  //display_action(action);
  return action;
}

void display_action(t_action action){
  printf("%s %d,%d THROUGH %d,%d\n",actionstrs[action.todo],action.sx,action.sy,action.fx,action.fy);
}

#if TEST
void action_lights_stage_1(int lights_array[1000][1000], const t_action action){
#else
void action_lights_stage_1(int lights_array[1000][1000], char *actstr){
  t_action action = decode_line(actstr);
#endif // TEST

  for (int y=action.sy;y<=action.fy;y++){
    for (int x=action.sx;x<=action.fx;x++){
      switch(action.todo){
        case TURNOFF:
          lights_array[x][y] = 0;
          break;
        case TURNON:
          lights_array[x][y] = 1;
          break;
        case TOGGLE:
          if (1 ==lights_array[x][y]) {
            lights_array[x][y] = 0;
          } else {
            lights_array[x][y] = 1;
          }
          break;
      }
    }
  }
}

void action_lights_stage_2(int lights_array[1000][1000], char *actstr){
  t_action action = decode_line(actstr);

  for (int y=action.sy;y<=action.fy;y++){
    for (int x=action.sx;x<=action.fx;x++){
      switch(action.todo){
        case TURNOFF:
          if (lights_array[x][y]){
            lights_array[x][y]--;
          }
          break;
        case TURNON:
          lights_array[x][y]++;
          break;
        case TOGGLE:
          lights_array[x][y] += 2;
          break;
      }
    }
  }
}

int  count_lights(int lights_array[1000][1000]){
  int lights_on = 0;

  for (int y=0;y<1000;y++){
    for (int x=0;x<1000;x++){
       if (lights_array[x][y]){
            lights_on++;
       }
    }
  }
  return lights_on;
}

int  count_brightness(int lights_array[1000][1000]){
  int brightness = 0;

  for (int y=0;y<1000;y++){
    for (int x=0;x<1000;x++){
       brightness += lights_array[x][y];
    }
  }
  return brightness;
}

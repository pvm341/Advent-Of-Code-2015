#include <stdio.h>
#include <stdlib.h>

int find_wrap_size(const int w, const int l, const int h);
int find_ribbon_length(const int w, const int l, const int h);
int min(const int a, const int b);
int mid(const int a, const int b, const int c);

int main()
{
  FILE *file;
  char line[20];
  int w,l,h;
  int wrapping=0;
  int ribbon=0;
  int area,len;

  if (NULL!=(file=fopen("xmascha2.txt","rt"))){
    fgets(line,19,file);
    while(!feof(file)){
      sscanf(line," %dx%dx%d",&w,&l,&h);
      area = find_wrap_size(w,l,h);
      len = find_ribbon_length(w,l,h);
      wrapping += area;
      ribbon += len;
      fgets(line,19,file);
    }
    fclose(file);
    printf("Total area of wrapping is %d sq ft\n",wrapping);
    printf("Total length of ribbon is %d ft\n",ribbon);
  }
  return 0;
}

int find_wrap_size(const int w, const int l, const int h){
  int s, area;
  s=min(w*l,w*h);
  s=min(s,l*h);
  area = (2*w*h) + (2*w*l) + (2*l*h)+s;
  return area;
}

int find_ribbon_length(const int w, const int l, const int h){
  int s, m, length = 0;
  s = min(w,l);
  s = min(s,h);
  m = mid(w,l,h);
  length = w*l*h+(2*s)+(2*m);
  return length;
}

int min(const int a, const int b){
  int ret_val;
  if (a<b)
    ret_val = a;
  else
    ret_val = b;
  return ret_val;
}

int mid(const int a, const int b, const int c){
  int ret_val;

  if (((a == b) && (b == c)) || ((a<=b) && (a>=c)) || ((a>=b) && (a<=c))) {
    ret_val = a;
  } else if (((b<=a) && (b>=c)) || ((b>=a) && (b<=c))) {
    ret_val = b;
  } else if (((c<=a) && (c>=b)) || ((c>=a) && (c<=b))) {
    ret_val = c;
  }
  return ret_val;
}


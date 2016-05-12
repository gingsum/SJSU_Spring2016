// This is a simple test program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "vmms.h"

struct student
{
  int	ID;
  char	name[20];
  struct student *left;
  struct student *right;
};

int main(int argc, char** argv)
{
  int rc = 0;
  char *student_ptr;
  char *list;
  char *list2;


  list = (char*) vmms_malloc(50, &rc);
  student_ptr = (char*) vmms_malloc(100, &rc);
  list2 = (char*) vmms_malloc(200, &rc);

  if (list == NULL)
    return 1;

  strcpy (list, "dummy1");
  strcpy (list+10, "911");

  printf("list address = %8x; Name = %s; ID = %s\n", list, list, (char*)list+10);
  
  rc = vmms_free((char*) student_ptr);

  vmms_print(list,50);
  vmms_memset(list+5,'a',10);
  vmms_memcpy(list, "blah",4);
 // vmms_print(list, 10);
 // system("pause");

  return rc;
}
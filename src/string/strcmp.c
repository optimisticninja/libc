#include <string.h>

int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0') {
          if(str1[i] != str2[i]) {
              failed = 1;
              break;
          }

          i++;
      }

      if((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))
          failed = 1;
  
      return failed;
}

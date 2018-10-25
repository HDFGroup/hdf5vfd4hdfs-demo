
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define DELIM       "\t"

int main(int argc, char *argv[])
{
  char strLastKey[BUFFER_SIZE];
  char strLine[BUFFER_SIZE];
  char *strCurrKey = NULL;
  char *strCurrCod = NULL;

  size_t obj_count[4] = { 0, 0, 0, 0 }; /* #G, #D, #T, #U */

  *strLastKey = '\0';
  *strLine = '\0';

  while(fgets(strLine, BUFFER_SIZE - 1, stdin) )
    {
      strCurrKey  = strtok(strLine, DELIM);
      strCurrCod = strtok(NULL, DELIM);

      if( strLastKey[0] == '\0')
        {
          strcpy(strLastKey, strCurrKey);
        }

      if(strcmp(strCurrKey, strLastKey))
        {
          printf("%s\tG %llu\tD %llu\tT %llu\n",
                 strLastKey, obj_count[0], obj_count[1], obj_count[2]);

          obj_count[0] = (*strCurrCod == 'G') ? 1 : 0;
          obj_count[1] = (*strCurrCod == 'D') ? 1 : 0;
          obj_count[2] = (*strCurrCod == 'T') ? 1 : 0;
          obj_count[3] = (*strCurrCod == '?') ? 1 : 0;
        }
      else
        {
          if (*strCurrCod == 'G')
            ++obj_count[0];
          else if (*strCurrCod == 'D')
            ++obj_count[1];
          else if (*strCurrCod == 'T')
            ++obj_count[2];
          else
            ++obj_count[3];
        }
      strcpy(strLastKey, strCurrKey);
    }

  printf("%s\tG %llu\t D %llu\t T %llu\n",
         strLastKey, obj_count[0], obj_count[1], obj_count[2]);

  return 0;
}

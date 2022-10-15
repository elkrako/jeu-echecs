
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const size_t SIZE = 1024;

extern int count_island(char **world);

int main()
{
   char buffer[SIZE];
   char *world[15];

   FILE *f = fopen("foo.txt", "r");
   assert(f != NULL);

   size_t read = fread(buffer, 1, SIZE, f);
   assert(read < SIZE);

   size_t row = 0;
   world[row] = buffer;
   for (size_t i = 0; i < read; i++) {
      if (buffer[i] == '\n') {
         buffer[i] = '\0';
         world[++row] = &buffer[i + 1];
      }
   }
   world[row] = NULL;

   count_island(world);

   for (size_t i = 0; world[i] != NULL; i++)
      printf("%s\n", world[i]);

   fclose(f);

   return EXIT_SUCCESS;
}
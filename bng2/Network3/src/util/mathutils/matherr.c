#include "mathutils.h"

/* Error handler for math library */

void MATHERR(char *error_text, unsigned int flags)
{
  fprintf(stderr,"Math library run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(EXIT_FAILURE);
}

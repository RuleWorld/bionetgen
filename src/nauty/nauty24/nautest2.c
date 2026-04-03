/* Test for basic nauty and dreadnaut functions (but not nauty itself) */

#include "naututil.h"

long seed;

int
main(int argc, char *argv[])
{
	int i,j,bad;
	setword w;
	int curfile;
	FILE *f;
#ifdef CPUDEFS
	CPUDEFS
#endif

	printf("NAUTYVERSION=%s NAUTYVERSIONID=%d\n",
		NAUTYVERSION,NAUTYVERSIONID);
	printf("MAXN=%d MAXM=%d WORDSIZE=%d NAUTY_INFINITY=%d\n",
		MAXN,MAXM,WORDSIZE,NAUTY_INFINITY);

	printf("defined:");
#ifdef __STDC__
	printf(" __STDC__");
#endif
#ifdef BIGNAUTY
	printf(" BIGNAUTY");
#endif
#ifdef SYS_UNIX
	printf(" SYS_UNIX");
#endif
#ifdef SYS_CRAY
        printf(" SYS_CRAY");
#endif
#ifdef SETWORD_SHORT
	printf(" SETWORD_SHORT");
#endif
#ifdef SETWORD_INT
	printf(" SETWORD_INT");
#endif
#ifdef SETWORD_LONG
	printf(" SETWORD_LONG");
#endif
#ifdef SETWORD_LONGLONG
	printf(" SETWORD_LONGLONG");
#endif
	printf("\n");

#ifdef DOPROMPT
	curfile = 0;
	printf("DOPROMPT(stdin)=%d DOPROMPT(stdout)=%d\n",
		DOPROMPT(stdin),DOPROMPT(stdout));
#else
	printf("DOPROMPT is not defined\n");
#endif

#ifdef CPUTIME
        printf("CPUTIME = %f\n",CPUTIME);
#else
        printf("CPUTIME is not defined\n");
#endif

#ifdef INITSEED
	INITSEED;
        printf("INITSEED: seed=%ld\n",seed);
#else
        printf("INITSEED is not defined\n");
#endif

#ifdef OPENOUT
        OPENOUT(f,"nautest.txt",0);
	fprintf(f,"test\n");
#else
        printf("OPENOUT is not defined\n");
#endif

	exit(0);
} 

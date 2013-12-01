/* shortg.c  version 1.9; B D McKay, Jul 29, 2008. */

#define USAGE \
  "shortg [-qvkdu] [-i# -I#:# -K#] [-fxxx] [-Tdir] [infile [outfile]]"

#define HELPTEXT \
"  Remove isomorphs from a file of graphs.\n\
\n\
    If outfile is omitted, it is taken to be the same as infile\n\
    If both infile and outfile are omitted, input will be taken\n\
            from stdin and written to stdout\n\
\n\
    The output file has a header if and only if the input file does.\n\
\n\
    -s  force output to sparse6 format\n\
    -g  force output to graph6 format\n\
        If neither -s or -g are given, the output format is\n\
        determined by the header or, if there is none, by the\n\
        format of the first input graph.\n\
    -k  output graphs have the same labelling and format as the inputs.\n\
        Otherwise, output graphs have canonical labelling.\n\
        -s and -g are ineffective if -k is given.  If none of -sgk are\n\
        given, the output format is determined by the header or, if there\n\
        is none, by the format of the first input graph.\n\
\n\
    -v  write to stderr a list of which input graphs correspond to which\n\
        output graphs. The input and output graphs are both numbered\n\
        beginning at 1.  A line like\n\
           23 : 30 154 78\n\
        means that inputs 30, 154 and 78 were isomorphic, and produced\n\
        output 23.\n\
\n\
    -d  include in the output only those inputs which are isomorphic\n\
        to another input.  If -k is specified, all such inputs are\n\
        included in their original labelling.  Without -k, only one\n\
        member of each nontrivial isomorphism class is written,\n\
        with canonical labelling.\n\
\n\
    -fxxx  Specify a partition of the point set.  xxx is any\n\
        string of ASCII characters except nul.  This string is\n\
        considered extended to infinity on the right with the\n\
        character 'z'.  One character is associated with each point,\n\
        in the order given.  The labelling used obeys these rules:\n\
         (1) the new order of the points is such that the associated\n\
        characters are in ASCII ascending order\n\
         (2) if two graphs are labelled using the same string xxx,\n\
        the output graphs are identical iff there is an\n\
        associated-character-preserving isomorphism between them.\n\
    -i#  select an invariant (1 = twopaths, 2 = adjtriang(K), 3 = triples,\n\
        4 = quadruples, 5 = celltrips, 6 = cellquads, 7 = cellquins,\n\
        8 = distances(K), 9 = indsets(K), 10 = cliques(K), 11 = cellcliq(K),\n\
       12 = cellind(K), 13 = adjacencies, 14 = cellfano, 15 = cellfano2)\n\
    -I#:#  select mininvarlevel and maxinvarlevel (default 1:1)\n\
    -K#   select invararg (default 3)\n\
\n\
    -u  Write no output, just report how many graphs it would have output.\n\
        In this case, outfile is not permitted.\n\
    -Tdir  Specify that directory \"dir\" will be used for temporary disk\n\
        space by the sort subprocess.  The default is usually /tmp.\n\
    -q  Suppress auxiliary output\n"


/*************************************************************************/

#include "gtools.h" 
#include "nautinv.h"

#if (HAVE_PIPE==0) || (HAVE_WAIT==0)
 #error Forget it, either pipe() or wait() are not available
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#else
#if !HAVE_PID_T
typedef int pid_t;
#endif
#endif

#if !PIPE_DEC
int pipe(int*);
#endif

#if !FDOPEN_DEC
FILE *fdopen(int, const char*);
#endif

#define SORTCOMMAND  SORTPROG,SORTPROG,"-u","+0","-1"
#define VSORTCOMMAND1  SORTPROG,SORTPROG
#define VSORTCOMMAND2  SORTPROG,SORTPROG,"+0","-1","+2"

#define SORTCOMMANDT  SORTPROG,SORTPROG,"-T",tempdir,"-u","+0","-1"
#define VSORTCOMMANDT1  SORTPROG,SORTPROG,"-T",tempdir
#define VSORTCOMMANDT2  SORTPROG,SORTPROG,"-T",tempdir,"+0","-1","+2"

static struct invarrec
{
    void (*entrypoint)(graph*,int*,int*,int,int,int,permutation*,
                      int,boolean,int,int);
    char *name;
} invarproc[]
    = {{NULL, "none"},
       {twopaths,    "twopaths"},
       {adjtriang,   "adjtriang"},
       {triples,     "triples"},
       {quadruples,  "quadruples"},
       {celltrips,   "celltrips"},
       {cellquads,   "cellquads"},
       {cellquins,   "cellquins"},
       {distances,   "distances"},
       {indsets,     "indsets"},
       {cliques,     "cliques"},
       {cellcliq,    "cellcliq"},
       {cellind,     "cellind"},
       {adjacencies, "adjacencies"},
       {cellfano,    "cellfano"},
       {cellfano2,   "cellfano2"}};

#define NUMINVARS ((int)(sizeof(invarproc)/sizeof(struct invarrec)))

/**************************************************************************/

static pid_t
beginsort(FILE **sortin, FILE **sortout, char *tempdir,
          boolean vdswitch, boolean keep)
/* begin sort process, open streams for i/o to it, and return its pid */
{
        int pid;
        int inpipe[2],outpipe[2];

        if (pipe(inpipe) < 0 || pipe(outpipe) < 0)
            gt_abort(">E shortg: can't create pipes to sort process\n");

        if ((pid = fork()) < 0) gt_abort(">E shortg: can't fork\n");

        if (pid > 0)            /* parent */
        {
            close(inpipe[0]);
            close(outpipe[1]);
            if ((*sortin = fdopen(inpipe[1],"w")) == NULL)
                gt_abort(">E shortg: can't open stream to sort process\n");
            if ((*sortout = fdopen(outpipe[0],"r")) == NULL)
                gt_abort(">E shortg: can't open stream from sort process\n");
        }
        else                   /* child */
        {
            SET_C_COLLATION;

            close(inpipe[1]);
            close(outpipe[0]);
            if (dup2(inpipe[0],0) < 0 || dup2(outpipe[1],1) < 0)
                gt_abort(">E shortg: dup2 failed\n");

	    if (tempdir == NULL)
	    {
                if (vdswitch)
                    if (keep) execlp(VSORTCOMMAND2,NULL);
                    else      execlp(VSORTCOMMAND1,NULL);
                else
                    execlp(SORTCOMMAND,NULL);
	    }
	    else
	    {
                if (vdswitch)
                    if (keep) execlp(VSORTCOMMANDT2,NULL);
                    else      execlp(VSORTCOMMANDT1,NULL);
                else
                    execlp(SORTCOMMANDT,NULL);
	    }
            gt_abort(">E shortg: can't start sort process\n");
        }

        return pid;
}

/**************************************************************************/

static void
tosort(FILE *f, char *cdstr, char *dstr, nauty_counter index)
/* write one graph to sort process 
   cdstr = canonical string 
   dstr = optional original string
   index = optional index number */
{
        int i;
	char buff[30];

        for (i = 0; cdstr[i] != '\n'; ++i) {}
	cdstr[i] = '\0';
	writeline(f,cdstr);

        if (dstr != NULL)
        {
            writeline(f," ");
            for (i = 0; dstr[i] != '\n'; ++i) {}
	    dstr[i] = '\0';
	    writeline(f,dstr);
        }

        if (index > 0)
        {
#if LONG_LONG_COUNTERS
            sprintf(buff,"\t%09llu\n",index);
#else
            sprintf(buff,"\t%09lu\n",index);
#endif
            writeline(f,buff);
        }
        else
	    writeline(f,"\n");
}

/**************************************************************************/

static boolean
fromsort(FILE *f, char **cdstr, char **dstr, nauty_counter *index)
/* read one graph from sort process */
{
        int j;
        char *s;

	if ((s = getline(f)) == NULL) return FALSE;

	*cdstr = s;
        for (j = 0; s[j] != ' ' && s[j] != '\t' && s[j] != '\n'; ++j) {}

        if (s[j] == ' ')
        {
	    s[j] = '\0';
	    *dstr = &s[j+1];
            for (++j; s[j] != '\t' && s[j] != '\n'; ++j) {}
        }
        else
            *dstr = NULL;

        if (s[j] == '\t')
        {
#if LONG_LONG_COUNTERS
            if (sscanf(&s[j+1],"%llu",index) != 1)
#else
            if (sscanf(&s[j+1],"%lu",index) != 1)
#endif
                gt_abort(">E shortg: index field corrupted\n");
        }
        else
            *index = 0;
	s[j] = '\0';

        return TRUE;
}

/**************************************************************************/

int
main(int argc, char *argv[])
{
        char *infilename,*outfilename;
        FILE *infile,*outfile;
        FILE *sortin,*sortout;
        int status,loops;
        char *dstr,*cdstr,*prevdstr,*prevcdstr;
        char sw,*fmt;
        boolean badargs,quiet,vswitch,dswitch,keep,format,uswitch;
	boolean iswitch,Iswitch,Kswitch,Tswitch;
	boolean sswitch,gswitch;
        nauty_counter numread,prevnumread,numwritten,classsize;
        int m,n,i,argnum,line;
	int outcode,codetype;
	int inv,mininvarlevel,maxinvarlevel,invararg;
        long minil,maxil;
        pid_t sortpid;
	graph *g;
        char *arg,*tempdir;
#if MAXN
	graph h[MAXN*MAXM];
#else
	DYNALLSTAT(graph,h,h_sz);
#endif

	HELP;

	nauty_check(WORDSIZE,1,1,NAUTYVERSIONID);

        infilename = outfilename = NULL;
        dswitch = format = quiet = vswitch = keep = uswitch = FALSE;
	sswitch = gswitch = Tswitch = FALSE;
	iswitch = Iswitch = Kswitch = FALSE;
	inv = 0;

     /* parse argument list */

        argnum = 0;
	badargs = FALSE;

        for (i = 1; !badargs && i < argc; ++i)
        {
            arg = argv[i];
            if (arg[0] == '-' && arg[1] != '\0')
            {
		++arg;
		while (*arg != '\0')
		{
		    sw = *arg++;
		         SWBOOLEAN('q',quiet)
		    else SWBOOLEAN('v',vswitch)
		    else SWBOOLEAN('k',keep)
		    else SWBOOLEAN('d',dswitch)
		    else SWBOOLEAN('u',uswitch)
		    else SWBOOLEAN('s',sswitch)
		    else SWBOOLEAN('g',gswitch)
                    else SWINT('i',iswitch,inv,"shortg -i")
                    else SWINT('K',Kswitch,invararg,"shortg -K")
                    else SWRANGE('I',":-",Iswitch,minil,maxil,"shortg -I")
		    else if (sw == 'f')
		    {
		        format = TRUE;
		        fmt = arg;
		        break;
		    }
		    else if (sw == 'T')
		    {
		        Tswitch = TRUE;
		        tempdir = arg;
		        break;
		    }
		    else badargs = TRUE;
		}
            }
            else
            {
		++argnum;
                if      (argnum == 1) infilename = arg;
		else if (argnum == 2) outfilename = arg;
                else 		      badargs = TRUE;
            }
        }

	if (strcmp(SORTPROG,"no_sort_found") == 0)
	    gt_abort(">E shortg: no sort program known\n");

	if (uswitch && outfilename != NULL)
	    gt_abort(">E shortg: -u and outfile are incompatible\n");

        if (sswitch && gswitch)
            gt_abort(">E shortg: -s and -g are incompatible\n");

	if (Tswitch && *tempdir == '\0')
	    gt_abort(">E shortg: -T needs a non-empty argument\n");

        if (argnum == 1 && !uswitch) outfilename = infilename;

        if (iswitch && (inv > 15))
            gt_abort(">E shortg: -i value must be 0..15\n");

        if (iswitch && inv == 0) iswitch = FALSE;

        if (iswitch)
        {
            if (Iswitch)
            {
                mininvarlevel = minil;
                maxinvarlevel = maxil;
            }
            else
                mininvarlevel = maxinvarlevel = 1;
            if (!Kswitch) invararg = 3;
        }

	if (badargs)
        {
            fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
            exit(1);
        }

	if (!quiet)
	{
	    fprintf(stderr,">A shortg");
	    if (uswitch || keep || vswitch || format || Tswitch
			|| sswitch || gswitch || iswitch)
	    fprintf(stderr," -");
            if (sswitch) fprintf(stderr,"s");
            if (gswitch) fprintf(stderr,"g");
	    if (keep) fprintf(stderr,"k");
	    if (vswitch) fprintf(stderr,"v");
	    if (dswitch) fprintf(stderr,"d");
	    if (uswitch) fprintf(stderr,"u");
            if (iswitch)
                fprintf(stderr,"i=%s[%d:%d,%d]",invarproc[inv].name,
                        mininvarlevel,maxinvarlevel,invararg);
	    if (format) fprintf(stderr,"f%s",fmt);
	    if (format && Tswitch) fprintf(stderr," -");
	    if (Tswitch) fprintf(stderr,"T%s",tempdir);
	    if (argnum > 0) fprintf(stderr," %s",infilename);
	    if (argnum > 1) fprintf(stderr," %s",outfilename);
	    fprintf(stderr,"\n");
	}

     /* open input file */

        if (infilename && infilename[0] == '-') infilename = NULL;
	infile = opengraphfile(infilename,&codetype,FALSE,1);
	if (!infile) exit(1);
	if (!infilename) infilename = "stdin";

        if (sswitch || !gswitch && (codetype&SPARSE6)) outcode = SPARSE6;
        else                                           outcode = GRAPH6;

#ifdef SIG_IGN
        signal(SIGPIPE,SIG_IGN);        /* process pipe errors ourselves */
#endif

     /* begin sort in a subprocess */

        sortpid = beginsort(&sortin,&sortout,(Tswitch?tempdir:NULL),
						dswitch||vswitch,keep);

     /* feed input graphs, possibly relabelled, to sort process */

        numread = 0;

        while (TRUE)
        {
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    dstr = readg_line;
            ++numread;
	    loops = loopcount(g,m,n);
#if !MAXN
	    DYNALLOC2(graph,h,h_sz,n,m,"shortg");
#endif
	    fcanonise_inv(g,m,n,h,format?fmt:NULL,
		invarproc[inv].entrypoint,mininvarlevel,maxinvarlevel,
		invararg, loops>0);
	    if (outcode == SPARSE6) cdstr = ntos6(h,m,n);
	    else                    cdstr = ntog6(h,m,n);

            tosort(sortin,cdstr,keep ? dstr : NULL,vswitch ? numread : 0);
	    FREES(g);
        }
        fclose(sortin);
        fclose(infile);

     /* open output file */

	if (uswitch)
	    outfilename = "<none>";
        else if (outfilename == NULL || outfilename[0] == '-' || is_pipe)
        {
            outfile = stdout;
            outfilename = "stdout";
        }
        else
        {
            if ((outfile = fopen(outfilename,"w")) == NULL)
            {
                fprintf(stderr,
                    ">E shortg: can't open %s for writing\n",outfilename);
                gt_abort(NULL);
            }
        }

	if (!uswitch && (codetype&HAS_HEADER))
	    if (outcode == SPARSE6) writeline(outfile,SPARSE6_HEADER);
	    else                    writeline(outfile,GRAPH6_HEADER);

        if (!quiet)
#if LONG_LONG_COUNTERS
            fprintf(stderr,
                    ">Z %6llu graphs read from %s\n",numread,infilename);
#else
            fprintf(stderr,
                    ">Z %6lu graphs read from %s\n",numread,infilename);
#endif

     /* collect output from sort process and write to output file */

	prevcdstr = prevdstr = NULL;
        numwritten = 0;
	if (dswitch)
        {
	    classsize = 0;
            while (fromsort(sortout,&cdstr,&dstr,&numread))
            {
                if (classsize == 0 || strcmp(cdstr,prevcdstr) != 0)
		    classsize = 1;
		else
		{
		    ++classsize;
		    if (classsize == 2)
		    {
		 	++numwritten;
			if (!uswitch)
			{
                            writeline(outfile,keep ? prevdstr : prevcdstr);
                    	    writeline(outfile,"\n");
			}
			if (keep)
			{
                            ++numwritten;
			    if (!uswitch)
			    {
                                writeline(outfile,keep ? dstr : cdstr);
                                writeline(outfile,"\n");
			    }
			}
			if (vswitch)
			{
			    fprintf(stderr,"\n");
#if LONG_LONG_COUNTERS
                    	    fprintf(stderr,"%3llu : %3llu %3llu",
				numwritten,prevnumread,numread);
#else
                    	    fprintf(stderr,"%3lu : %3lu %3lu",
				numwritten,prevnumread,numread);
#endif
                    	    line = 1;
			}
		    }
		    else
		    {
			if (keep)
			{
                            ++numwritten;
			    if (!uswitch)
			    {
                                writeline(outfile,keep ? dstr : cdstr);
		                writeline(outfile,"\n");
			    }
			}
                	if (vswitch)
			{
                    	    if (line == 15)
                    	    {
                                line = 0;
                                fprintf(stderr,"\n     ");
                            }
#if LONG_LONG_COUNTERS
                            fprintf(stderr," %3llu",numread);
#else
                            fprintf(stderr," %3lu",numread);
#endif
                            ++line;
			}
		    }
                }
		if (prevcdstr) FREES(prevcdstr);
                prevcdstr = stringcopy(cdstr);
		if (prevdstr) FREES(prevdstr);
                if (keep) prevdstr = stringcopy(dstr);
		prevnumread = numread;
            }
            if (vswitch) fprintf(stderr,"\n\n");
        }
        else if (vswitch)
        {
            while (fromsort(sortout,&cdstr,&dstr,&numread))
            {
                if (numwritten == 0 || strcmp(cdstr,prevcdstr) != 0)
                {
                    ++numwritten;
		    if (!uswitch)
		    {
                        writeline(outfile,keep ? dstr : cdstr);
		        writeline(outfile,"\n");
		    }
                    fprintf(stderr,"\n");
#if LONG_LONG_COUNTERS
                    fprintf(stderr,"%3llu : %3llu",numwritten,numread);
#else
                    fprintf(stderr,"%3lu : %3lu",numwritten,numread);
#endif
                    line = 1;
                }
                else
                {
                    if (line == 15)
                    {
                        line = 0;
                        fprintf(stderr,"\n     ");
                    }
#if LONG_LONG_COUNTERS
                    fprintf(stderr," %3llu",numread);
#else
                    fprintf(stderr," %3lu",numread);
#endif
                    ++line;
                }
		if (prevcdstr) FREES(prevcdstr);
                prevcdstr = stringcopy(cdstr);
            }
            fprintf(stderr,"\n\n");
        }
        else
        {
            while (fromsort(sortout,&cdstr,&dstr,&numread))
            {
                ++numwritten;
		if (!uswitch)
		{
                    writeline(outfile,keep ? dstr : cdstr);
		    writeline(outfile,"\n");
		}
            }
        }

        fclose(sortout);
        if (!uswitch) fclose(outfile);

        if (!quiet)
	{
#if LONG_LONG_COUNTERS
	    if (uswitch)
		fprintf(stderr,">Z %6llu graphs produced\n",numwritten);
	    else
                fprintf(stderr,
                      ">Z %6llu graphs written to %s\n",numwritten,outfilename);
#else
	    if (uswitch)
		fprintf(stderr,">Z %6lu graphs produced\n",numwritten);
	    else
                fprintf(stderr,
                      ">Z %6lu graphs written to %s\n",numwritten,outfilename);
#endif
	}

     /* check that the subprocess exitted properly */

        while (wait(&status) != sortpid) {}

#if (defined(WIFSIGNALED) || defined(WTERMSIG)) && defined(WEXITSTATUS)
#ifdef WIFSIGNALED
        if (WIFSIGNALED(status) && WTERMSIG(status) != 0)
#else
        if (WTERMSIG(status) != 0)
#endif
        {
            fprintf(stderr,">E shortg: sort process killed (signal %d)\n",
                          WTERMSIG(status)); 
            gt_abort(NULL);
        }   
        else if (WEXITSTATUS(status) != 0)
        {
            fprintf(stderr,
                    ">E shortg: sort process exited abnormally (code %d)\n",
                    WEXITSTATUS(status));
            gt_abort(NULL);
        }
#endif

        exit(0);
}

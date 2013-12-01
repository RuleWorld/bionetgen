/* gutils.h - procedure declarations for gutil1.c and gutil2.c */

void degstats(graph*,int,int,unsigned long*,int*,int*,int*,int*,boolean*);
void diamstats(graph*,int,int,int*,int*);
void find_dist(graph*,int,int,int,int*);
int girth(graph*,int,int);
boolean isbiconnected1(graph*,int);
boolean isbiconnected(graph*,int,int);
boolean isbipartite(graph*,int,int);
boolean twocolouring(graph*,int*,int,int);
boolean isconnected1(graph*,int);
boolean isconnected(graph*,int,int);
boolean issubconnected(graph*,set*,int,int); 

int loopcount(graph*,int,int);
long pathcount1(graph*,int,setword,setword);
long cyclecount1(graph*,int);
long cyclecount(graph*,int,int);
void commonnbrs(graph*,int*,int*,int*,int*,int,int);
void contract1(graph*,graph*,int,int,int);
int cstarcontent(graph*,int,int);
long numtriangles1(graph*,int);
long numtriangles(graph*,int,int);

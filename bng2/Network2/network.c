/*

$Id: network.c,v 1.5 2007/02/23 01:51:30 faeder Exp $

$Log: network.c,v $
Revision 1.5  2007/02/23 01:51:30  faeder
Fixed bug in get_line which caused reading to terminate after one increase in
the buffer size due to incorrect setting of the buffer length.

Revision 1.4  2007/02/18 03:35:52  faeder
Changed counters in Gillespie code to long int to avoid overflow.

Revision 1.3  2007/01/30 22:40:51  faeder
Working version.

Revision 1.2  2006/05/24 22:03:56  faeder
*** empty log message ***

Revision 1.1  2005/09/19 17:57:58  faeder
Initial revision

Revision 1.1  2005-08-09 13:59:21-06  kozdon
Initial revision


*/

/* include files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "mathutils.h"
#include "network.h"

#define BUFSIZE 10000

char *get_line( FILE *infile){
    char *buf, *retval;
    int lbuf=BUFSIZE;
    buf= (char *) malloc(BUFSIZE*sizeof(char));
    retval=fgets(buf, BUFSIZE, infile);
    if (!retval){
      free(buf);
      return(NULL);
    }

    // Expand bufsize as necessary to read full line
    while ((strlen(buf)==(lbuf-1))&& (buf[lbuf-2]!='\n')){
      buf=realloc( buf, (lbuf+BUFSIZE-1)*sizeof(char));
      retval=fgets(buf+lbuf-1, BUFSIZE, infile);
      lbuf+= BUFSIZE-1;
    }

    return(buf);
}

char **parse_line (char *buf, int *n_tok, char *comment_chars, char *sep_chars){
    int max_tokens=100;
    char *sptr, **tokens;

    /* remove text after comment #*% */
    if (comment_chars){
	if ( (sptr= strpbrk(buf,comment_chars)) != NULL )
	    *sptr = '\0';
    }

    /* parse line using strtok */
    *n_tok=0;
    sptr = buf;
    tokens= (char **) malloc( max_tokens*sizeof(char *));
    while( (tokens[*n_tok]= strtok(sptr, sep_chars)) != NULL){
        sptr= NULL;
        ++(*n_tok);
        if (*n_tok == max_tokens){
	    max_tokens*=2;
	    tokens=realloc( tokens, max_tokens*sizeof(char *));
	}
    }
    if (!(*n_tok)){
	free(tokens);
	tokens=NULL;
    } else {
	tokens= realloc( (void *) tokens, (*n_tok)*sizeof(char *));
    }
    return(tokens);
}

char *chop_suffix( char *string, const char *suff){
    char *suff_start;
    int lstr, lsuff;

    lstr= strlen(string);
    lsuff= strlen(suff);
    if (lstr <= lsuff) return(string);
    suff_start= string + lstr - lsuff;
    /* chop off suffix if present */
    if (!strcmp(suff, suff_start)) *suff_start='\0';
    return(string);
}

Elt *new_Elt( char *name, double val, int index){
    Elt *new;

    new= (Elt *) calloc( 1, sizeof(Elt));
    new->name= strdup(name);
    new->val= val;
    new->index= index;
    return new;
}

void add_Elt( Elt **list, Elt *new){
    Elt *elt;
    if ((*list)==NULL){
	*list=new;
    } else {
	for(elt=*list; elt->next!=NULL; elt=elt->next);
	elt->next=new;
    }
    return;
}

void free_Elt( Elt *elt){
    if (elt){
	if (elt->name) free(elt->name);
	free(elt);
    }
    return;
}

Elt *lookup_Elt( char *name, Elt *list){
    Elt *elt;

    for (elt=list; elt !=NULL; elt=elt->next){
	if (strcmp(name, elt->name)==0) break;
    }
    return (elt);
}

void print_Elt_list( FILE *outfile, Elt *elist){
    Elt *elt;
    char *str;

    for(elt=elist; elt!=NULL; elt=elt->next){
      str= (elt->fixed) ? "$" : "";
      fprintf(outfile, "%5d %s%-20s %22.15e\n",
	      elt->index, str, elt->name, elt->val);
    }
    return;
}

/* Add array elements in erray2 to erray1 */

void append_Elt_array( Elt_array *earray1, Elt_array *earray2){
    Elt **array, *elt;
    int n_elt_old, index;

    if (!earray2) return;
    if (earray2->n_elt<1) return;

    /* Add earray2->list to earray1->list */
    (earray1->elt[earray1->n_elt-1])->next = earray2->list;

    /* New element count for earray1 */
    n_elt_old= earray1->n_elt;
    earray1->n_elt+= earray2->n_elt;

    /* Allocate additional space for earray2 elements */
    earray1->elt= (Elt **) realloc( earray1->elt, earray1->n_elt*sizeof(Elt *));

    /* Set elt pointers to list elements */
    array= earray1->elt + n_elt_old;
    index= earray1->elt[n_elt_old-1]->index + 1;
    for(elt=earray2->list; elt!=NULL; elt=elt->next){
      *array=elt;
      elt->index=index;
      ++array;
      ++index;
    }

    /* Free unused space for earray2 */
    free(earray2->elt);
    free(earray2);

 exit:
    return;
}

Elt_array *new_Elt_array( Elt *list){
    Elt_array *earray=NULL;
    Elt **array, *elt;
    int imin, imax, index;

    if (!list) goto exit;
    earray= (Elt_array *) calloc(1, sizeof(Elt_array));

    /* find maximum and minimum index values and count n_elt */
    imin= imax= list->index;
    for(elt=list; elt!=NULL; elt=elt->next){
	index= elt->index;
	imax= (index>imax)? index : imax;
	imin= (index<imin)? index : imin;
    }

    earray->n_elt= imax-imin+1;
    earray->offset= imin;
    earray->elt= (Elt **) calloc( earray->n_elt, sizeof(Elt *));
    array= earray->elt - earray->offset;
    for(elt=list; elt!=NULL; elt=elt->next){
	array[elt->index]= elt;
/* 	printf(" %d", elt->index); */
    }
/*     printf("\n"); fflush(stdout); */
    earray->list= list;

 exit:
    return(earray);
}

void free_Elt_array( Elt_array *earray){
    Elt *elt, *new;

    if (earray){
	if (earray->elt) free(earray->elt);
	for( elt=earray->list; elt!=NULL; elt=new){
	    new=elt->next;
	    free_Elt( elt);
	}
	free(earray);
    }
    return;
}

Elt_array *read_Elt_array(  FILE *datfile, int *line_number, char *name,
                int *n_read, Elt_array *params){
    Elt_array *earray=NULL;
    Elt *list_start=NULL, *list_end, *elt, *new;
    char *line, **tokens;
    int read_begin=0, read_end=0, error=0;
    int n_tokens, index;
    double val;
    char *elt_name;
    int n_tok;
    int n_fixed;

    /* read data in block */
    *n_read=0;
    n_fixed=0;
    while( line=get_line(datfile)){
        fflush(stdout);
        ++(*line_number);

        tokens= parse_line( line, &n_tokens, "#", " \t\r\n");
        if (n_tokens==0) goto cleanup;


        /* parse commands */

        /* check for beginning of list */
        if (n_tokens==2 && strcmp(tokens[0], "begin") ==0){
            if (strcmp(tokens[1], name)==0){
                read_begin=1;
            }
            goto cleanup;
        }

        /* Goto next line if begin command not yet read */
        if (read_begin==0) goto cleanup;

        /* Check for end of list */
        if (n_tokens==2 && strcmp( tokens[0], "end")==0 &&
            strcmp( tokens[1], name)==0){
            read_end=1;
            goto cleanup;
        }

        /* Process data in list */
        if (n_tokens>1 &&  n_tokens<=4){
            int fixed=0;
            n_tok=0;
            if (sscanf(tokens[n_tok], "%d", &index)==1)
                ++n_tok;
            else
                index= *n_read + 1;

            /* Read elt name */
            elt_name= tokens[n_tok];;

            /* Check for fixed (boundCondition) flag '$'.
                Prior to 2.1.7, the '$' flag was only recognized as the first character.
                This led to problems parsing fixed species with compartments, tags, or other attributes.
                To fix the issue, a '$' character anywhere in the elt_name string will trigger the
                attribute.  --Justin, 30Oct2010.
            */
            if ( strchr(elt_name, '$') != (const char *)NULL )
            {
                /*++elt_name; */
                printf("%s is a fixed (boundaryCondition) variable\n", elt_name);
                fixed=1;
                ++n_fixed;
            }
            ++n_tok;

            /* Read elt value either directly or by looking up parameter value */
            if (n_tok<n_tokens){
                double factor;
                char buf[1000],c;

                /* Look for prefactor */
                if (sscanf(tokens[n_tok], "%lf*%s", &factor, buf)!=2){
                    factor=1;
                    strcpy(buf, tokens[n_tok]);
                }

                /* Try to obtain numerical value for elt, or lookup parameter */
                if (sscanf(buf, "%lf%c", &val, &c) !=1){
                    /* Then try to lookup elt in parameter list (if present) */
                    if (!params){
                        fprintf(stderr, "Invalid value %s at line %d.\n",
                        tokens[n_tok], *line_number);
                        ++error;
                        goto cleanup;
                    } else if (elt=lookup_Elt(buf, params->list)){
                        val= factor*elt->val;
                    } else {
                        fprintf(stderr, "Invalid parameter specification %s at line %d.\n",
                        tokens[n_tok], *line_number);
                        ++error;
                        goto cleanup;
                    }
                } else {
                    val*= factor;
                }
                ++n_tok;
            } else {
                val=0.0;
            }

            /* Read elt normalization */
            if (n_tok<n_tokens){
                fprintf(stderr, "Element normalization no longer supported at line %d\n.",
                    *line_number);
                ++error;
                goto cleanup;
            }

            /* Allocate new list element */
            ++(*n_read);
            new= new_Elt( elt_name, val, index);
            new->fixed=fixed;
            if (list_start){
                list_end->next= new;
                list_end=new;
            } else {
                list_start= list_end= new;
            }

        } else {
            fprintf( stderr,"Invalid list entry at line %d.\n", *line_number);
            ++error;
            goto cleanup;
        }

      cleanup:
        if (tokens) free(tokens);
        free(line);
        if (read_end) break;
    }

    if (read_begin && !read_end){
        fprintf( stderr, "End of file reached before list terminated.\n");
        ++error;
    }
    if (error){
        fprintf( stderr, "%s list not read because of errors.\n", name);
        for( elt=list_start; elt!=NULL; elt=new){
            new=elt->next;
            free_Elt( elt);
        }
    } else {
        earray= new_Elt_array( list_start);
        if (n_fixed){
            int *iarray= IALLOC_VECTOR( n_fixed);
            int i,ifixed=0;
            for(i=0;i<earray->n_elt;++i){
                if (earray->elt[i]->fixed){
                    iarray[ifixed]= i;
                    ++ifixed;
                }
            }
            earray->n_fixed_elts= n_fixed;
            earray->fixed_elts= iarray;
        }
    }

    return(earray);
}

Group *new_Group( char *name, int index){
  Group *new;
  new= (Group *) calloc( 1, sizeof(Group));
  new->name= strdup(name);
  new->index= index;
  return(new);
}

Group *lookup_Group( char *name, Group *list){
  Group *grp;

  for (grp=list; grp !=NULL; grp=grp->next){
    if (strcmp(name, grp->name)==0) break;
  }
  return (grp);
}

Group *add_Group( Group *glist, char *name, int index, int n_elt, int *elt_index, double *elt_factor){
  int i, n_elt_old;
  Group *grp, *gptr, *glist_ret;

/*   printf("Adding group %s\n", name); */
  glist_ret= glist;
  if (!(grp=lookup_Group(name, glist))){
    /* Allocate space for new group */
    grp= new_Group(name, index);
    if (glist){
      /* Add entry to the end of the list */
      for(gptr=glist; gptr->next!=NULL; gptr=gptr->next);
      gptr->next=grp;
    } else {
      glist_ret= grp;
    }
  }

  n_elt_old= grp->n_elt;

  if (n_elt>0){
    grp->n_elt+= n_elt;
    grp->elt_index= (int *) realloc(grp->elt_index, grp->n_elt*sizeof(int));
    grp->elt_factor= (double *) realloc(grp->elt_factor, grp->n_elt*sizeof(double));
    for(i=0; i<n_elt; ++i){
      grp->elt_index[n_elt_old+i]= elt_index[i];
      if (elt_factor)
	grp->elt_factor[n_elt_old+i]= elt_factor[i];
      else
	grp->elt_factor[n_elt_old+i]= 1.0;
    }
  }

  return(glist_ret);
}



void free_Group( Group *group){
    if (group){
	if (group->name) free(group->name);
	if (group->elt_index) free(group->elt_index);
	free(group);
    }
    return;
}

Group *read_Groups(  Group *glist, FILE *datfile, Elt_array *earray, int *line_number,
		     char *name, int *n_read){
  Group *glist_ret;
  char *line, **tokens;
  int read_begin=0, read_end=0, error=0;
  int i, n_tokens;
  int index, n_elt, *elt_index=NULL;
  double *elt_factor=NULL;
  char *group_name;
  int n_tok;

  glist_ret=glist;


/*   printf("Summary of defined observables\n"); */
  /* read data in block */
  *n_read=0;
  while( line=get_line(datfile)){
    ++(*line_number);
    tokens= parse_line( line, &n_tokens, "#", ", \t\r\n");
    if (n_tokens==0) goto cleanup;

    /* parse commands */

    /* check for beginning of list */
    if (n_tokens==2 && strcmp(tokens[0], "begin") ==0){
      if (strcmp(tokens[1], name)==0){
	read_begin=1;
      }
      goto cleanup;
    }

    /* Goto next line if begin command not yet read */
    if (read_begin==0) goto cleanup;

    /* Check for end of list */
    if (n_tokens==2 && strcmp( tokens[0], "end")==0 &&
	strcmp( tokens[1], name)==0){
      read_end=1;
      goto cleanup;
    }

    /* Process data in list */
    if (n_tokens>=2){
      n_tok=0;
      if (sscanf(tokens[n_tok], "%d", &index)==1)
	++n_tok;
      else
	index= *n_read + 1;

      group_name= tokens[n_tok];
      ++n_tok;

      n_elt= n_tokens-n_tok;

      // Check whether group is an expression and ignore if it is
      if (n_elt && strcmp(tokens[n_tok],"expression")==0){
	printf("Ignoring expression %s in group definitions\n", group_name);
	goto cleanup;
      }

      elt_index= (n_elt>0) ? IALLOC_VECTOR( n_elt) : 0x0;
      for(i=0; i<n_elt; ++i,++n_tok){
	Elt *elt;
	double factor;
	char buf[1000];

	if (sscanf(tokens[n_tok], "%lf*%s", &factor, buf)==2){
	  if (!elt_factor){
	    elt_factor= ALLOC_VECTOR(n_elt);
	    INIT_VECTOR( elt_factor, 1.0, n_elt);
	  }
	  elt_factor[i]= factor;
	} else {
	  strcpy(buf, tokens[n_tok]);
	}

	if (sscanf(buf, "%d", elt_index+i)!=1){
	  if (!(elt=lookup_Elt(buf, earray->list))){
	    fprintf(stderr, "Invalid element %s at line %d.\n",
		    tokens[n_tok], *line_number);
	    ++error;
	    goto cleanup;
	  }
	  elt_index[i]= elt->index;
	}
      }

      /* Allocate new list element */
      ++(*n_read);
      glist_ret= add_Group( glist_ret, group_name, index, n_elt, elt_index, elt_factor);
/*       printf("%s has %d elements\n", group_name, n_elt); */
    } else {
      fprintf( stderr,"Invalid list entry at line %d.\n", *line_number);
      ++error;
      goto cleanup;
    }
  cleanup:
    if (tokens) free(tokens);
    free(line);
    if (elt_index) free(elt_index);
    elt_index=NULL;
    if (elt_factor) FREE_VECTOR(elt_factor);
    elt_factor=NULL;
    if (read_end) break;
  }

  if (read_begin && read_end==0){
    fprintf( stderr, "End of file reached before list terminated.\n");
    ++error;
  }
  if (error){
    Group *group, *new;
    fprintf( stderr, "%s list not read because of errors.\n", name);
    for( group=glist_ret; group!=NULL; group=new){
      new= group->next;
      free_Group( group);
    }
    glist_ret=NULL;
  }

  return(glist_ret);
}


void print_Groups( FILE *out, Group *list, Elt_array *earray){
  int i, eindex;
  Group *group;
  double factor;

  fprintf(out, "begin groups\n");
  for(group=list; group!=NULL; group=group->next){
    fprintf(out, "%5d %10s", group->index, group->name);
    for(i=0; i<group->n_elt; ++i){
      eindex= group->elt_index[i];
      factor= (group->elt_factor) ? group->elt_factor[i] : 1.0;
      if (i==0){fprintf(out, " ");} else{fprintf(out, ",");}
      if (factor==1.0)
	fprintf(out,"%d", eindex);
      else
	fprintf(out,"%.8g*%d", factor, eindex);
    }
    fprintf(out, "\n");
  }
  fprintf(out, "end groups\n");

  return;
}

Rxn *new_Rxn(int index, int n_reactants, int n_products,
	     int *r_index, int *p_index, int rateLaw_type,
	     int n_rateLaw_params, int *rateLaw_indices,
	     double stat_factor, Elt_array *rates){
  register int i;
  Rxn *rxn;
  Elt **karray;

  rxn= (Rxn *) calloc( 1, sizeof(Rxn));
  rxn->index= index;
  rxn->n_reactants= n_reactants;
  rxn->n_products= n_products;
  rxn->r_index= (int *) malloc( n_reactants*sizeof(int));
  rxn->p_index= (int *) malloc( n_products*sizeof(int));
  for (i=0; i<n_reactants; ++i){
    rxn->r_index[i]= r_index[i];
  }
  for (i=0; i<n_products; ++i){
    rxn->p_index[i]= p_index[i];
  }
  rxn->rateLaw_type= rateLaw_type;
  rxn->n_rateLaw_params= n_rateLaw_params;
  rxn->stat_factor= stat_factor;
  /* Allocate space for arrays */
  rxn->rateLaw_params= (double *) malloc( n_rateLaw_params*sizeof(double));
  rxn->rateLaw_indices= (int *) malloc( n_rateLaw_params*sizeof(int));
  /* Initialize arrays */
  karray= rates->elt - rates->offset;
  for (i=0; i<n_rateLaw_params; ++i){
    int j= rateLaw_indices[i];
    rxn->rateLaw_indices[i]= j;
    rxn->rateLaw_params[i]= karray[j]->val;
  }

  // printf("Rxn %d stat_factor=%g\n", index, stat_factor);

  return(rxn);
}

Rxn *copy_Rxn ( Rxn *rxn, int index, Elt_array *rates){
    int i;
    Rxn *new;

    if (!rxn){ new=NULL; goto exit;}
    new= new_Rxn( index, rxn->n_reactants, rxn->n_products,
		  rxn->r_index, rxn->p_index, rxn->rateLaw_type,
		  rxn->n_rateLaw_params, rxn->rateLaw_indices,
		  rxn->stat_factor, rates);
 exit:
    return(new);
}

/* Uses previous end of list if list does not change */
void add_Rxn( Rxn **list, Rxn *new){
    Rxn *rxn;
    static Rxn *list_last=NULL, *rxn_last=NULL;
    if ((*list)==NULL){
	*list=new;
    } else if ((*list)==list_last){
	/* Already know end of list */
	rxn_last->next=new;
    } else {
	for(rxn=*list; rxn->next!=NULL; rxn=rxn->next);
	rxn->next=new;
    }
    list_last=*list;
    rxn_last= new;
    return;
}

void free_Rxn( Rxn *rxn){
  if (rxn){
    if (rxn->r_index) free(rxn->r_index);
    if (rxn->p_index) free(rxn->p_index);
    if (rxn->rateLaw_params) free(rxn->rateLaw_params);
    if (rxn->rateLaw_indices) free(rxn->rateLaw_indices);
    free(rxn);
  }
  return;
}

Rxn_array *new_Rxn_array( Rxn *list){
    Rxn_array *rarray;
    Rxn **array, *rxn;
    int imin, imax, index;
    int *index_used, *used;

    if (!list) return(0x0);
    rarray= (Rxn_array *) calloc(1, sizeof(Rxn_array));

    /* find maximum and minimum index values and count n_elt */
    imin= imax= list->index;
    for(rxn=list; rxn!=NULL; rxn=rxn->next){
	index= rxn->index;
	imax= (index>imax)? index : imax;
	imin= (index<imin)? index : imin;
    }


    rarray->n_rxn= imax-imin+1;
    if (rarray->n_rxn<=0){ return(rarray);}
    rarray->offset= imin;
    rarray->rxn= (Rxn **) calloc( rarray->n_rxn, sizeof(Rxn *));
    index_used= IALLOC_VECTOR(rarray->n_rxn);
    IINIT_VECTOR(index_used, 0, rarray->n_rxn);
    used= index_used - rarray->offset;
    array= rarray->rxn - rarray->offset;
    for(rxn=list; rxn!=NULL; rxn=rxn->next){
      if (used[rxn->index]){
	fprintf(stderr, "Each reaction must be assigned a unique index.\n");
	free_Rxn_array(rarray);
	return(0x0);
      }
      array[rxn->index]= rxn;
    }
    rarray->list= list;
    free(index_used);
    return(rarray);
}

void append_Rxn_array( Rxn_array *rarray1, Rxn_array *rarray2){
    Rxn **array, *rxn;
    int n_rxn_old, index;

    if (!rarray2) return;
    if (rarray2->n_rxn<1) return;

    /* Add rarray2->list to rarray1->list */
    (rarray1->rxn[rarray1->n_rxn-1])->next = rarray2->list;

    /* New element count for rarray1 */
    n_rxn_old= rarray1->n_rxn;
    rarray1->n_rxn+= rarray2->n_rxn;

    /* Allocate additional space for rarray2 elements */
    rarray1->rxn= (Rxn **) realloc( rarray1->rxn, rarray1->n_rxn*sizeof(Rxn *));

    /* Set elt pointers to list elements */
    array= rarray1->rxn  + n_rxn_old;
    index= rarray1->rxn[n_rxn_old-1]->index + 1;
    for(rxn=rarray2->list; rxn!=NULL; rxn=rxn->next){
      *array=rxn;
      rxn->index=index;
      ++array;
      ++index;
    }

    /* Free unused space for rarray2 */
    free(rarray2->rxn);
    free(rarray2);

 exit:
    return;
}

void free_Rxn_array( Rxn_array *rarray){
    Rxn *rxn, *new;

    if (rarray){
	if (rarray->rxn) free(rarray->rxn);
	for( rxn=rarray->list; rxn!=NULL; rxn=new){
	    new=rxn->next;
	    free_Rxn( rxn);
	}
	free(rarray);
    }
    return;
}

void remove_zero_rate_rxns( Rxn_array **reactions, Elt_array *rates){
  int n_new=0,n_drop=0;
  Rxn *rxn, *list_new=NULL;
  Rxn_array *rarray_new;

  for(rxn=(*reactions)->list; rxn!=NULL; rxn=rxn->next){
    if (rxn->rateLaw_params[0]!=0.0){
      ++n_new;
      add_Rxn( &list_new, copy_Rxn(rxn, n_new, rates));
    } else {
      ++n_drop;
    }
  }
  if (n_drop>0){
    printf("Removed %d reactions with zero rate\n", n_drop);
  }


  free_Rxn_array(*reactions);
  if (list_new){
    rarray_new= new_Rxn_array(list_new);
    *reactions= rarray_new;
  } else {
    *reactions= NULL;
  }
  return;
}

static int compare_Rxn( const void *addr1, const void *addr2){
    int compare=0;
    Rxn *rxn1, *rxn2;
    int i, n1,n2,nmin;

    rxn1= *((Rxn **) addr1);
    rxn2= *((Rxn **) addr2);

    /* Compare reactants */
    n1= rxn1->n_reactants;
    n2= rxn2->n_reactants;
    nmin= IMIN(n1,n2);
    for(i=0; i<nmin; ++i){
	if (rxn1->r_index[i]< rxn2->r_index[i]){
	    compare=-1;
	    goto exit;
	} else if (rxn1->r_index[i] > rxn2->r_index[i]){
	    compare=1;
	    goto exit;
	}
    }
    /* Rank reactions with fewer reactants lower */
    if (n1<n2){
	compare=-1;
	goto exit;
    } else if (n1>n2){
	compare=1;
	goto exit;
    }

    /* Compare by products */
    n1= rxn1->n_products;
    n2= rxn2->n_products;
    nmin= IMIN(n1,n2);
    for(i=0; i<nmin; ++i){
	if (rxn1->p_index[i]< rxn2->p_index[i]){
	    compare=-1;
	    goto exit;
	} else if (rxn1->p_index[i] > rxn2->p_index[i]){
	    compare=1;
	    goto exit;
	}
    }
    /* Rank reactions with fewer products lower */
    if (n1<n2){
	compare=-1;
	goto exit;
    } else if (n1>n2){
	compare=1;
	goto exit;
    }

    /* Compare reactants */
    n1= rxn1->n_reactants;
    n2= rxn2->n_reactants;
    nmin= IMIN(n1,n2);
    for(i=0; i<nmin; ++i){
	if (rxn1->r_index[i]< rxn2->r_index[i]){
	    compare=-1;
	    goto exit;
	} else if (rxn1->r_index[i] > rxn2->r_index[i]){
	    compare=1;
	    goto exit;
	}
    }
    /* Rank reactions with fewer reactants lower */
    if (n1<n2){
	compare=-1;
	goto exit;
    } else if (n1>n2){
	compare=1;
	goto exit;
    }

    /* Compare by products */
    n1= rxn1->n_products;
    n2= rxn2->n_products;
    nmin= IMIN(n1,n2);
    for(i=0; i<nmin; ++i){
	if (rxn1->p_index[i]< rxn2->p_index[i]){
	    compare=-1;
	    goto exit;
	} else if (rxn1->p_index[i] > rxn2->p_index[i]){
	    compare=1;
	    goto exit;
	}
    }
    /* Rank reactions with fewer products lower */
    if (n1<n2){
	compare=-1;
	goto exit;
    } else if (n1>n2){
	compare=1;
	goto exit;
    }

 exit:
    return(compare);
}

static Elt_array *rates_compare;
static int compare_Rxn_bytype( const void *addr1, const void *addr2){
    int compare=0;
    Rxn *rxn1, *rxn2;
    char *rc1, *rc2;
    int roffset;

    rxn1= *((Rxn **) addr1);
    rxn2= *((Rxn **) addr2);

    if (!rxn1) return (1);
    if (!rxn2) return (-1);

    /* Compare rate constant names */
    roffset=rates_compare->offset;
    rc1= rates_compare->elt[rxn1->rateLaw_indices[0]-roffset]->name;
    rc2= rates_compare->elt[rxn2->rateLaw_indices[0]-roffset]->name;
    if (!(compare=strcmp(rc1,rc2))){
	/* use compare_rxn if type is same */
	compare= compare_Rxn(addr1, addr2);
    }

 exit:
    return(compare);
}

void sort_Rxn_array( Rxn_array *rarray, Elt_array *rates){
    int i;

    if (!rarray->n_rxn) goto exit;
    rates_compare=rates;

    /* Sort array rxn using compare_Rxn */
    qsort((void *) rarray->rxn, rarray->n_rxn, sizeof(Rxn *), compare_Rxn_bytype);

    /* Reset list and indices */
    rarray->offset=1;
    rarray->list= rarray->rxn[0];
    rarray->list->index=1;
    rarray->rxn[0]->next=NULL;
    for(i=1; i<rarray->n_rxn; ++i){
	if (!rarray->rxn[i]) break;
	rarray->rxn[i]->index= i+1;
	rarray->rxn[i]->next=NULL;
	rarray->rxn[i-1]->next= rarray->rxn[i];
    }

 exit:
    return;
}

void connectivity_Rxn_array( Rxn_array *reactions, Elt_array *species,
			     int **as_reactant, int **as_product){
    int i, offset, n_species;
    int *asr, *asp; /* Output arrays */
    Rxn *rxn;

    n_species= species->n_elt;
    asr=IALLOC_VECTOR(n_species);
    IINIT_VECTOR( asr, 0, n_species);
    asp =IALLOC_VECTOR(n_species);
    IINIT_VECTOR( asp, 0, n_species);
    /* Loop over reactions */
    offset= species->offset;
    for(rxn=reactions->list; rxn!=NULL; rxn=rxn->next){
	/* Loop over reactants */
	for(i=0; i<rxn->n_reactants; ++i){
	    ++asr[rxn->r_index[i]-offset];
	}
	/* Loop over products */
	for(i=0; i<rxn->n_products; ++i){
	    ++asp[rxn->p_index[i]-offset];
	}
    }

    *as_reactant= asr;
    *as_product = asp;
 exit:
    return;
}


static int *read_indices_Rxn ( char *string, int *n_indices, Elt_array *species, int line_number )
{
    register int i;
    int *index_list, index;
    int n_tokens, error=0;
    char *line, **tokens;
    int check, offset, n_spec;
    Elt *elt;

    /* track indices that were skipped (null species) */
    int null_count=0;

    line= strdup(string);
    tokens= parse_line( line, &n_tokens, NULL, ",");
    index_list= (int *) malloc( n_tokens*sizeof(int));
    *n_indices= n_tokens;
    offset= species->offset;
    n_spec= species->n_elt;

    for ( i=0; i < n_tokens; ++i )
    {
	    if ( sscanf(tokens[i], "%d", &index)==1)
	    {
	        /* Check that species index is valid */
	        int check = index - offset;
	        if ( check < -1 || check >= n_spec )
	        {
	    	    fprintf(stdout,"Species index %d out of range at line %d.\n",
	    		    index, line_number);
                fflush(stdout);
	    	    ++error;
	    	    goto cleanup;
	        }
	        else if ( check == -1 )
	        {   /* Assume that index=0 corresponds to a Null species --Justin */
	    	    fprintf(stdout,"Found species with index %d at line %d: assuming this is the null species.\n",
	    		    index, line_number);
	    		fflush(stdout);
	    		++null_count;
	    		--(*n_indices);
	    		continue;
	        }

	        index_list[i-null_count] = index;
	    }
	    else if (elt=lookup_Elt(tokens[i], species->list))
	    {   /* Lookup species name */
	        /* Still need to handle this case for the null species?? --Justin */
	        index_list[i-null_count] = elt->index;
	    }
	    else
	    {
	        fprintf(stdout, "Invalid species named %s at line %d.\n", tokens[i], line_number);
	        fflush(stdout);
	        ++error;
	        goto cleanup;
	    }
    }

  cleanup:
    if (error)
    {
	    free(index_list);
	    index_list=NULL;
	    *n_indices=0;
    }

    if (line) free(line);
    if (tokens) free(tokens);

    return (index_list);
}

/* Type of reaction determined by number of rate constants:

    1.   Elementary reaction:  v= k1*r1*...*rn
    2.   Michaelis-Menton catalytic reaction:
           S + E -> P + E, v= k1*[S][E]/([S]+k2)
	   Note: substrate should be listed first.  Remaining reactants are
	   considered enzymes and their concentration is summed to get the
	   total enzyme concentration. [S] is
	   computed self-consistently, i.e. [S]= S_total-[ES], where
           [ES]= E_total [S]/([S]+k2).  If S is substrate in only one
           reaction, then [S] is computed from quadratic formula (see
           implementation below).
    3.   Rate law that saturates with respect to first reactant
           R1 + R2 + ... -> P1 + ...
           v= k1*R1*R2*.../(k2 + [R1]), k1 has units conc.s^-1, k2 has units
           conc.  Effect of complexation on concentration of S is NOT
           considered, since enzyme concentration is not known.
    4.  Hill rate law (k, K, q) : v= k1*(r1)^q*...*rn/(K^n + r1^n) (Generalization of Sat)

*/

// Reaction types
#define ELEMENTARY 0
#define SATURATION 1
#define MICHAELIS_MENTON  2
#define HILL 3

Rxn_array *read_Rxn_array( FILE *datfile, int *line_number, int *n_read,
			   Elt_array *species, Elt_array *rates){
    Rxn_array *rarray;
    Rxn *list_start=NULL, *rxn, *new;
    Elt *elt;
    char *line, **tokens, *format=NULL;
    int read_begin=0, read_end=0, error=0;
    int n_tokens, index;
    int n_reactants, n_products, *r_index=NULL, *p_index=NULL;
    int n_rateLaw_tokens;
    int rateLaw_type;
    int *rateLaw_indices=NULL;
    double stat_factor;
    int perr;
    int n_tok, i;
    char buf[1000];
    enum { FMT_NONE, FMT_DFLT};

    read_begin= FMT_NONE;

    /* read data in block */
    *n_read=0;
    while( line=get_line(datfile)){
    	++(*line_number);
	    tokens= parse_line( line, &n_tokens, "#", " \t\r\n");
	    if (n_tokens==0) continue;

	    /* parse commands */

	    /* check for beginning of list */
	    if (n_tokens==2 && strcmp(tokens[0], "begin") ==0){
 	        if (strcmp(tokens[1], "reactions")==0){
	        	read_begin=FMT_DFLT;
	        	format= strdup(tokens[1]);
	        }
	        goto cleanup;
	    }

        /* Goto next line if begin command not yet read */
	    if (read_begin==0) goto cleanup;

	    /* Check for end of list */
	    if (strcmp( tokens[0], "end")==0){
	        if (n_tokens==2 && strcmp( tokens[1], format)==0){
	        	read_end=1;
	        } else {
	        	fprintf(stderr, "Error in end command at line %d.\n", *line_number);
	        	read_end=-1;
	        }
	        goto cleanup;
	    }

	    /* Parse reaction list input */
	    if (read_begin==FMT_DFLT){

	        n_tok=0;
	        /* Default format */
	        n_rateLaw_tokens= n_tokens - 3;
	        if (n_rateLaw_tokens<1){
	        	fprintf( stderr,"Invalid list entry at line %d.\n", *line_number);
	        	++error;
	        	goto cleanup;
	        }
	        /* Get reaction index */
	        if (sscanf(tokens[n_tok], "%d", &index) !=1){
	        	fprintf(stderr, "Invalid index at line %d.\n", *line_number);
	        	++error;
	        	goto cleanup;
	        }
	        ++n_tok;

	        /* Get indices of reactants */
	        if (!(r_index= read_indices_Rxn( tokens[n_tok], &n_reactants, species, *line_number))){
	        	++error;
	        	goto cleanup;
	        }
	        ++n_tok;

	        /* Get indices of products */
	        if (!(p_index= read_indices_Rxn( tokens[n_tok], &n_products, species, *line_number))){
		        ++error;
		        goto cleanup;
	        }
	        ++n_tok;

	        // Find optional statistical factor for reaction
	        if (sscanf(tokens[n_tok], "%lf*%s", &stat_factor, buf)==2){
	              strcpy(tokens[n_tok], buf);
	        } else {
	            stat_factor=1.0;
	        }

	        // Determine rateLaw type
	        if (n_rateLaw_tokens==1){
	            // Elementary reaction specified with a single rate constant
	            // name
	            rateLaw_type= ELEMENTARY;
	        } else {
	            // Find match on list of types
	            if (strcmp(tokens[n_tok], "Ele")==0){
		            rateLaw_type= ELEMENTARY;
	            }
	            else if (strcmp(tokens[n_tok], "Sat")==0){
		            rateLaw_type= SATURATION;
	            }
	            else if (strcmp(tokens[n_tok], "Hill")==0){
		            rateLaw_type= HILL;
	            }
	            else if (strcmp(tokens[n_tok], "MM")==0){
		            rateLaw_type= MICHAELIS_MENTON;
	            } else {
		            // No match found
		            fprintf(stderr, "Undefined rateLaw type %s at line %d.\n", tokens[n_tok], *line_number);
		            ++error;
		            goto cleanup;
	            }
	            ++n_tok;
	            --n_rateLaw_tokens;
	        }

	        /* Read names of parameters in rate law */
	        rateLaw_indices= (int *) malloc( n_rateLaw_tokens*sizeof(int));

	        // Check that number of rate constants is correct for rateLaw type
	        // TODO: Check that number of reactants and products is also correct
	        perr=0;
	        switch(rateLaw_type)
	        {
	            case ELEMENTARY:
	                perr= (n_rateLaw_tokens!=1);
	                break;
	            case SATURATION:
	                perr= (n_rateLaw_tokens==0) || (n_rateLaw_tokens > (n_reactants+1));
	                break;
	            case HILL:
	                perr= (n_rateLaw_tokens==0) || (n_rateLaw_tokens !=3);
	                break;
	            case MICHAELIS_MENTON:
	                perr= (n_rateLaw_tokens!=2);
	                break;
	        }
	        if (perr){
	            fprintf(stderr, "Incorrect number of rate constants for reaction type at line %d.\n",
		            *line_number);
	            ++error;
	            goto cleanup;
	        }

	        for(i=0; i<n_rateLaw_tokens; ++i){
	            /* Lookup rate name */
	            if (rates){
		            if (elt=lookup_Elt(tokens[n_tok], rates->list)){
		                rateLaw_indices[i]= elt->index;
		            } else {
		                fprintf(stderr, "Undefined rate constant %s at line %d.\n",
			            tokens[n_tok], *line_number);
		                ++error;
		                goto cleanup;
		            }
	            }
	            ++n_tok;
	        }
	    }

	    /* Allocate new reaction */
	    ++(*n_read);
	    new= new_Rxn( index, n_reactants, n_products, r_index, p_index,
		         rateLaw_type, n_rateLaw_tokens, rateLaw_indices,
		         stat_factor, rates);

	    /* Add new reaction to list of reactions */
	    if (list_start){
	        rxn->next= new;
	        rxn=new;
	    } else {
	        list_start= rxn= new;
	    }

      cleanup:
	    if (tokens) free(tokens);
	    free(line);
	    if (r_index) free( r_index);
	    r_index=NULL;
	    if (p_index) free( p_index);
	    p_index=NULL;
	    if (rateLaw_indices) free(rateLaw_indices);
	    rateLaw_indices=NULL;
	    if (read_end) break;
    }

    if (format) free(format);
    if (read_begin==FMT_NONE){
	    fprintf( stderr, "End of file reached before begin command found.\n");
	    ++error;
    } else if (read_end==0){
	    fprintf( stderr, "End of file reached before list terminated.\n");
	    ++error;
    }
    if (error){
	    fprintf( stderr, "Reaction list not read because of errors.\n");
	    for( rxn=list_start; rxn!=NULL; rxn=new){
	        new=rxn->next;
	        free_Rxn(rxn);
	    }
	    rarray=NULL;
    } else {
	    rarray= new_Rxn_array( list_start);
    }

    return(rarray);
}


void print_Rxn_array ( FILE *out, Rxn_array *reactions,
		       Elt_array *species, Elt_array *rates){
    register int i;
    Rxn *rxn;
    Elt **sarr, **rarr;

    sarr= species->elt - species->offset;
    rarr= rates->elt - rates->offset;

    fprintf(out, "begin reactions\n");
    for (rxn=reactions->list; rxn!=NULL; rxn=rxn->next){
	fprintf( out, "%5d ", rxn->index);

	/* print reactant indices */
	fprintf( out, "%d", rxn->r_index[0]);
	for (i=1; i<rxn->n_reactants; ++i){
	    fprintf( out, ",%d", rxn->r_index[i]);
	}

	/* print product indices */
	fprintf( out, " ");
	fprintf( out, "%d", rxn->p_index[0]);
	for (i=1; i<rxn->n_products; ++i){
	    fprintf( out, ",%d", rxn->p_index[i]);
	}

	fprintf(out," ");

	// print optional statistical factor if necessary
	if (rxn->stat_factor!=1.0){
	  fprintf(out, "%.15g*", rxn->stat_factor);
	}

	// print rateLaw_type if not elementary
	if (rxn->rateLaw_type == SATURATION){
	  fprintf(out, "Sat ");
	} else if (rxn->rateLaw_type == MICHAELIS_MENTON){
		  fprintf(out, "MM ");
	} else if (rxn->rateLaw_type == HILL){
		  fprintf(out, "Hill ");
	}

	/* Loop over rate constants */
	for(i=0;i<rxn->n_rateLaw_params;++i){
	  if (i!=0) fprintf(out," ");
	  fprintf(out, "%s", rarr[rxn->rateLaw_indices[i]]->name);
	}
	fprintf( out, "\n");
    }

    fprintf(out, "end reactions\n");
    return;
}


// WARNING: This routine does not give complete information: statistical factor,
// reaction type, and additional rate constants are missing.

void print_Rxn_text( FILE *out, Rxn *rxn, Elt_array *species, Elt_array *rates){
    int i;
    Elt **spec_arr, **rate_arr;

    spec_arr= species->elt-species->offset;
    rate_arr= rates->elt-rates->offset;

    fprintf( out, "%5d", rxn->index);

    /* print rate name */
    fprintf(out, " %s ", rate_arr[rxn->rateLaw_indices[0]]->name);

    /* print reactants */
    fprintf( out, "%s", spec_arr[rxn->r_index[0]]->name);
    for (i=1; i<rxn->n_reactants; ++i){
	fprintf( out, "+%s", spec_arr[rxn->r_index[i]]->name);
    }
    fprintf( out, "->");
    /* print products */
    fprintf( out, "%s", spec_arr[rxn->p_index[0]]->name);
    for (i=1; i<rxn->n_products; ++i){
	fprintf( out, "+%s", spec_arr[rxn->p_index[i]]->name);
    }
    return;
}

/*=========================================================================*/
/* Compute network reaction rates and time derivatives of concentrations   */
/*=========================================================================*/

struct NETWORK network;

void derivs_network( double t, double *conc, double *derivs);

int init_network( Rxn_array *reactions, Elt_array *rates, Elt_array *species,
		  Group *spec_groups, char *name){
    int i;
    Group *group;

    network.name= strdup( name);
    network.reactions= reactions;
    network.rates= rates;
    network.species= species;
    network.spec_groups= spec_groups;
    network.n_groups=0;
    for(group= spec_groups; group!=NULL; group=group->next){
	++network.n_groups;
    }

    /* Set function for calculating time derivatives of species */
    network.derivs= derivs_network;

    /* Initialize function call counters */
    network.n_deriv_calls= network.n_rate_calls= 0;

    return(0);
}

int n_rate_calls_network(){ return(network.n_rate_calls);}

int n_deriv_calls_network(){ return(network.n_deriv_calls);}

int n_rxns_network(){
    return (network.reactions->n_rxn);
}

int n_species_network(){
    return (network.species->n_elt);
}

int n_species_active(){
    register int i;
    int n_act=0, n_species;
    Elt **nconc;

    n_species= n_species_network();
    nconc= network.species->elt;
    for(i=0; i<n_species; ++i){
        if (nconc[i]->val > 0){
	        ++n_act;
        }
    }
    return (n_act);
}

int n_rate_constants_network(){
    return (network.rates->n_elt);
}

int n_groups_network(){
    return (network.n_groups);
}

/* Returns concentrations in network.species to conc array */
int get_conc_network ( double *conc){
    register int i;
    int error=0, n_species;
    Elt **nconc;

    if (!conc){ ++error; goto exit;}

    n_species= n_species_network();
    nconc= network.species->elt;
    for(i=0; i<n_species; ++i){
	conc[i]= nconc[i]->val;
    }

 exit:
    return(error);
}

/* Sets concentrations in network.species to those in conc array */
int set_conc_network ( double *conc){
    register int i, n_species;
    int error=0;
    Elt **nconc;

    n_species= network.species->n_elt;
    nconc= network.species->elt;
    for(i=0; i<n_species; ++i){
	nconc[i]->val= conc[i];
    }

 exit:
    return(error);
}

/* Return the rate of rxn -- for internal use only */
static double rxn_rate( Rxn *rxn, double *X, int discrete){
  double rate;
  int *iarr, *index;
  int ig, i1, i2, n_denom;
  int q;
  double *param,x, xn, kn;
  double St, Et, kcat, Km, S, b;

  /* Don't calculate rate of null reactions */
  if (!rxn) return(0.0);

  ++network.n_rate_calls;

  switch(rxn->rateLaw_type){
  case ELEMENTARY:
    // v= k1*X1...Xn
    rate= rxn->stat_factor*rxn->rateLaw_params[0];
    iarr=rxn->r_index;
    // Handle reactions with discrete molecules with multiple copies of the same reactants.
    // NOTE: Will only apply correct formula if repeated species are grouped together (which is done
    //       automatically by BNG).
    if (discrete){
    	double n=0.0;
    	rate*= X[*iarr];
        for (index=iarr+1; index<iarr+rxn->n_reactants; ++index){
        	if (*index == *(index-1)){
        		n+= 1.0;
        	} else {
        		n=0.0;
        	}
        	rate*= (X[*index]-n);
        }
    } else {
      for (index=iarr; index<iarr+rxn->n_reactants; ++index){
    	  rate*= X[*index];
      }
    }
    break;
  case MICHAELIS_MENTON:
    /* Second rate, if present, is Michaelis-Menton Km */
    St= X[rxn->r_index[0]];
    kcat= rxn->rateLaw_params[0];
    Km= rxn->rateLaw_params[1];
    /* S + E ... -> P + E + .. */
    for(q=1,Et=0; q<rxn->n_reactants;++q){Et+= X[rxn->r_index[q]];}
    b= St-Km-Et;
    S= 0.5*(b + sqrt(b*b+4*St*Km));
    rate= rxn->stat_factor*kcat*Et*S/(Km+S);
    break;
  case SATURATION:
    param= rxn->rateLaw_params;
    /* if dim(param)==1
         rate= stat_factor*param[0], a zeroth order rate law
       else
         rate = stat_factor*param[0]*R1...Rn/((R1+param[1])*(R2+param[2])...
       where terms in denominator are only calculated if param[n] is defined */
    rate= rxn->stat_factor*param[0];
    iarr=rxn->r_index;
    ++param;
    n_denom= rxn->n_rateLaw_params-1;
    if (n_denom>0){
      /* Compute contributions to rate from species appearing in both numerator
	 and denominator */
    	for (ig=0; ig<n_denom; ++ig){
    		x= X[iarr[ig]];
    		rate*= x/(param[ig]+x);
    	}

      /* Compute contributions to rate from species appearing only in numerator */
    	for (ig=n_denom; ig<rxn->n_reactants; ++ig){
    		rate*= X[iarr[ig]];
    	}
    }
    break;
  case HILL:
    param= rxn->rateLaw_params;
    iarr=rxn->r_index;
	x= X[iarr[0]];
	xn= pow(x,param[2]);
	kn= pow(param[1],param[2]);
	rate= rxn->stat_factor*param[0]*xn/(kn+xn);
    /* Compute contributions to rate from species appearing only in numerator */
	for (ig=1; ig<rxn->n_reactants; ++ig){
		rate*= X[iarr[ig]];
    }
    break;
  }

  return(rate);
}

/* Returns the rate of each reaction in the network in #/unit time. */
int rxn_rates_network( double *rxn_rates){
    register int i;
    int error=0, coffset, n_reactions, n_species;
    Rxn **rarray;
    double rate, *X, *conc=NULL;

    n_reactions= n_rxns_network();
    n_species= n_species_network();

    conc= ALLOC_VECTOR(n_species);
    get_conc_network ( conc);
    INIT_VECTOR( rxn_rates, 0.0, n_reactions);

    rarray= network.reactions->rxn;
    X= conc - network.species->offset;
    for(i=0; i<n_reactions;++i){
      rxn_rates[i]= rxn_rate(rarray[i], X, 0);
    }

 exit:
    if (conc) FREE_VECTOR(conc);
    return(error);
}

void derivs_network( double t, double *conc, double *derivs){
  int i;
  int ig;
  int error=0;
  int n_reactions, n_species,*index,*iarr;
  Rxn **rarray, *rxn;
  Elt **rates;
  double x, xn, kn, *X, *dX, rate,rate0, *param;
  int q, n_denom;
  double St, Et, kcat, Km, S, b;

  ++network.n_deriv_calls;

  n_reactions= network.reactions->n_rxn;
  n_species= network.species->n_elt;

  /* Initialize derivatives to zero */
  INIT_VECTOR( derivs, 0.0, n_species);

  /* Compute derivatives of each species by looping over reactions. */
  rarray= network.reactions->rxn;
  rates=  network.rates->elt - network.rates->offset;
  X= conc - network.species->offset;
  dX= derivs - network.species->offset;
  for(i=0; i<n_reactions; ++i){
    /* Compute rate for current reaction */
    if(!(rxn=rarray[i])) continue;
    ++network.n_rate_calls;
    int rateLaw_type= rxn->rateLaw_type;
    switch(rateLaw_type){
    case ELEMENTARY:
      // v= k1*X1...Xn
      rate= rxn->stat_factor*rxn->rateLaw_params[0];
      iarr=rxn->r_index;
      for (index=iarr; index<iarr+rxn->n_reactants; ++index){
	rate*= X[*index];
      }
      // Compute contribution to rate of change of each participant
      for (index=iarr; index<iarr+rxn->n_reactants; ++index){
	dX[*index]-= rate;
      }
      iarr=rxn->p_index;
      for (index=iarr; index<iarr+rxn->n_products; ++index){
	dX[*index]+= rate;
      }
      break;

    case MICHAELIS_MENTON:
      /* Second rate, if present, is Michaelis-Menton Km */
      St= X[rxn->r_index[0]];
      kcat= rxn->rateLaw_params[0];
      Km= rxn->rateLaw_params[1];
      /* S + E ... -> P + E + .. */
      for(q=1,Et=0; q<rxn->n_reactants;++q){Et+= X[rxn->r_index[q]];}
      b= St-Km-Et;
      S= 0.5*(b + sqrt(b*b+4*St*Km));
      rate= rxn->stat_factor*kcat*Et*S/(Km+S);
      // Compute contribution to rate of change of each participant
      iarr=rxn->r_index;
      for (index=iarr; index<iarr+rxn->n_reactants; ++index){
    	  dX[*index]-= rate;
      }
      iarr=rxn->p_index;
      for (index=iarr; index<iarr+rxn->n_products; ++index){
    	  dX[*index]+= rate;
      }
      break;

    case SATURATION:
      param= rxn->rateLaw_params;
      /* if dim(param)==1
         rate= stat_factor*param[0], a zeroth order rate law
	 else
         rate = stat_factor*param[0]*R1...Rn/((R1+param[1])*(R2+param[2])...
	 where terms in denominator are only calculated if param[n] is defined */
      rate= rxn->stat_factor*param[0];
      iarr=rxn->r_index;
      ++param;
      n_denom= rxn->n_rateLaw_params-1;
      if (n_denom>0){
	/* Compute contributions to rate from species appearing in both numerator
	   and denominator */
    	  for (ig=0; ig<n_denom; ++ig){
    		  x= X[iarr[ig]];
    		  rate*= x/(param[ig]+x);
    	  }

    	  /* Compute contributions to rate from species appearing only in numerator */
    	  for (ig=n_denom; ig<rxn->n_reactants; ++ig){
    		  rate*= X[iarr[ig]];
    	  }
      }

      // Compute contribution to rate of change of each participant
      for (index=iarr; index<iarr+rxn->n_reactants; ++index){
    	  dX[*index]-= rate;
      }
      iarr=rxn->p_index;
      for (index=iarr; index<iarr+rxn->n_products; ++index){
    	  dX[*index]+= rate;
      }
      // fprintf(stderr,"Saturation fired\n");
      break;
    case HILL:
      param= rxn->rateLaw_params;
      iarr=rxn->r_index;
      x= X[iarr[0]];
      xn= pow(x,param[2]);
      kn= pow(param[1],param[2]);
      rate= rxn->stat_factor*param[0]*xn/(kn+xn);
      /* Compute contributions to rate from species appearing only in numerator */
      for (ig=1; ig<rxn->n_reactants; ++ig){
    	  rate*= X[iarr[ig]];
      }
      // Compute contribution to rate of change of each participant
       for (index=iarr; index<iarr+rxn->n_reactants; ++index){
     	  dX[*index]-= rate;
       }
       iarr=rxn->p_index;
       for (index=iarr; index<iarr+rxn->n_products; ++index){
     	  dX[*index]+= rate;
       }
       break;
    }
    //rate0= rxn_rate( rxn, X, 0);
    //printf("diff=%.2e\n", rate-rate0);
  }

  // Set derivatives to zero for fixed species
  if (network.species->fixed_elts){
    int *fixed_elts= network.species->fixed_elts;
    int n_fixed= network.species->n_fixed_elts;
    for(i=0;i<n_fixed;++i){
      derivs[fixed_elts[i]]=0.0;
    }
  }

 exit:
  return;
}

int print_derivs_network( FILE *out){
    register int i,j;
    int error=0, n_species;
    double *X=NULL, *dX=NULL;

    n_species= n_species_network();
    X=  ALLOC_VECTOR( n_species);
    dX= ALLOC_VECTOR( n_species);

    /* Compute time derivs of species concentration by reaction */
    get_conc_network( X);
    derivs_network( 0.0, X, dX);

    /* Print reaction rates */
    fprintf( out, "begin derivs\n");
    for(i=0; i<n_species; ++i){
	fprintf(out, "%5d", i+1);
	fprintf(out, " %11.4e", dX[i]);
	fprintf(out, "\n");
    }
    fprintf(out, "end derivs\n");

 exit:
    if (X) FREE_VECTOR(X);
    if (dX) FREE_VECTOR(dX);

    return(error);
}

int print_derivs_species_network( FILE *out){
    register int i,j;
    int error=0, n_species;
    double *X=NULL, *dX=NULL;

    n_species= n_species_network();
    X= ALLOC_VECTOR( n_species);
    dX= ALLOC_VECTOR( n_species);

    /* Compute time derivs of species concentration by reaction */
    get_conc_network( X);
    derivs_network( 0.0, X, dX);

    /* Print reaction rates */
    fprintf( out, "begin derivs_species\n");
    for(i=0; i<n_species; ++i){
	fprintf(out, "%10s", network.species->elt[i]->name);
	fprintf(out, " %11.4e", dX[i]);
	fprintf(out, "\n");
    }
    fprintf(out, "end derivs_species\n");

 exit:
    if (X) FREE_VECTOR(X);
    if (dX) FREE_VECTOR(dX);

    return(error);
}

int print_rates_network( FILE *out){
    register int i;
    int error=0, n_species, n_reactions;
    double *rates_rxn=NULL;
    Rxn **rxns;

    n_reactions= n_rxns_network();
    rates_rxn= ALLOC_VECTOR( n_reactions);

    /* Calculate reaction rates */
    rxn_rates_network( rates_rxn);

    /* Print reaction rates */
    fprintf( out, "begin reaction_rates\n");
    rxns= network.reactions->rxn;
    for(i=0; i<n_reactions; ++i){
	if (rxns[i]){
	    print_Rxn_text(out, rxns[i], network.species, network.rates);
	    fprintf( out, " %15.8e\n", rates_rxn[i]);
	}
    }
    fprintf( out, "end reaction_rates\n");

 exit:
    if (rates_rxn) FREE_VECTOR(rates_rxn);

    return(error);
}

#include "sundialstypes.h"
#include "cvode.h"
#include "nvector_serial.h"

#include "cvdense.h"
#include "dense.h"

#include "cvspgmr.h"
#include "spgmr.h"

typedef struct jacnode *jacnode_ref;

struct jacnode
{
    int    loc;
    double val;
    jacnode_ref next;
};

void jaclist_insert(jacnode_ref *jaclist, int point, int loc, double val)
{
    jacnode_ref back    = NULL;
    jacnode_ref temp    = NULL;
    jacnode_ref current = jaclist[point];

    if(current == NULL)
    {
        current = calloc(1, sizeof(struct jacnode));
        if(current == NULL)
        {
            fprintf(stderr, "Error in allocating jacobian for matlab output\n");
            exit(1);
        }
        current->loc = loc;
        current->val = 0;
        current->next = NULL;
        jaclist[point] = current;
    }

    while(current->loc != loc)
    {
        if(current->loc < loc)
        {
            back = current;
            current = current->next;
            if(current == NULL)
            {
                current = calloc(1, sizeof(struct jacnode));
                if(current == NULL)
                {
                    fprintf(stderr, "Error in allocating jacobian for matlab output\n");
                    exit(1);
                }
                current->loc = loc;
                current->val = 0;
                current->next = NULL;
                back->next = current;
            }
        }
        else
        {
            temp = calloc(1, sizeof(struct jacnode));
            if(temp == NULL)
            {
                fprintf(stderr, "Error in allocating jacobian for matlab output\n");
                exit(1);
            }
            temp->loc = loc;
            temp->val = 0;
            temp->next = current;
            if(back != NULL)
            {
                back->next = temp;
            }
            else
            {
                jaclist[point] = temp;
            }
            current = temp;
        }
    }
    current->val += val;
}

void jacprintlist(jacnode_ref *jacobian, FILE *outfile, char *matname)
{
    int n_species;
    int itor = 0;
    jacnode_ref current = NULL;

    n_species = n_species_network();

    for(itor = 0; itor < n_species; itor++)
    {
        current = jacobian[itor];
        while(current != NULL)
        {
            fprintf(outfile, "%s(%d, %d) = %s(%d, %d) + %1.12e;\n", matname,
                itor + 1, current->loc + 1, matname, itor + 1, current->loc + 1,
                current->val);
            current = current->next;
        }
    }
}

/*
 * Sets up some comments to help the user understand what
 * Matlab is capable of
 */
void init_sparse_matlab_file(FILE *outfile)
{
    fprintf(outfile, "%s", "% Comments about MATLAB capabilities:\n%\n% Information about eigenvalues and vector:\n%\n% The matrix in this file is in sparse form, this means\n% that you can only use sparse MATLAB functions with it\n% to convert it to a full matrix use the command full,\n% i.e. if your sparse matrix was called jac and you\n% wanted to make a full matrix called A then do:\n%            A = full(jac)\n% To compute eigenvalues of a sparse matrix use eigs.\n% The call:\n%            E = eigs(jac)\n%            [V D] = eigs(jac)\n% The first stores the 6 largest in magnitude eigenvalues\n% in the column vector E, whereas the second stores the\n% eigenvectors of the 6 largest in magnitude eigenvalues\n% in the columns of V and the corresponding values along\n% along the diagonal of the matrix D.\n% The call:\n%            E = eigs(jac, k)\n%            [V D] = eigs(jac, k)\n% Does the same except for the kth largest in magnitude\n% eigenvalues. NOTE: that for nonsymmetric problems k < n-1\n% where n corresponds to the size of the matrix, n-by-n.\n%\n% For full matrices use the call:\n%            E = eig(A)\n%            [V, D] = eig(A)\n% which will return all the eigenvalues and vectors of the\n% matrix A.\n%\n% For more options on both these commands use the call:\n%            help eigs\n%            help eig\n%\n% NOTE: to suppress the value of the variable after assignment\n% a semicolon:\n%            E = eig(A)\n%            E = eig(A);\n% the first will list the value of E after assignment and\n% the second will not.\n%\n% Useful graphing functions:\n%\n% a convient way to view these eigenvalues are the functions\n% plot and semilogx. Suppose that the eigenvalues are stored in\n% a vector E. To view the real and imaginary parts the command:\n%            plot(E, '*')\n% will plot the real part and imaginary parts of each component\n% of E on the x and y axis, respectively, each marked with an *.\n% The command:\n%            semilogx(abs(E), 0, 'b*')\n% will produce a semilog plot of the magnitude of E where each\n% component of E is denoted by a blue *.\n%\n% Use the help function for more info on these functions.\n\n\n");
}


/*
 * Outputs a sparse matlab matrix in an m file.
 */
void sparse_jac_matlab(FILE *outfile)
{
    register int i, j, r, n;
    int n_reactions;
    Rxn **rarray, *rxn;
    int *irxn, *ipxn, *index;
    int n_reactants, n_products;
    int n_species;
    Elt **rates;
    double **J_ptr;
    double rate, RATE;
    realtype *X;
    int deriv_index;
    char matname[100];
    jacnode_ref *jacobian;


    n_species = n_species_network();
    n_reactions = n_rxns_network();

    X = calloc(n_species, sizeof(double));
    get_conc_network(X);

    jacobian = (jacnode_ref *)calloc(n_species, sizeof(jacnode_ref));
    if(jacobian == NULL)
    {
        fprintf(stderr, "Error in allocating jacobian for matlab output\n");
        exit(1);
    }

    sprintf(matname, "jac");
    fprintf(outfile, "%s = sparse([%d], [%d], [0]);\n", matname, n_species, n_species);

    /* Loop over all reactions */
    rarray = network.reactions->rxn;
    rates =  network.rates->elt - network.rates->offset;
    for(r = 0; r < n_reactions; ++r)
    {
        rxn = rarray[r];
        /* Calculate the rate of this reaction */
        /* First rate constant is elementary reaction rate constant
           v = k1*X1...Xn */
        RATE = rxn->stat_factor * rxn->rateLaw_params[0]; //i.e. k1

        irxn = rxn->r_index;
        ipxn = rxn->p_index;

        n_reactants = rxn->n_reactants;
        n_products  = rxn->n_products;

        for(n = 0; n < n_reactants; n++)
        {
            deriv_index = irxn[n];
            rate = RATE;
            for(i = 0; i < n_reactants; i++)
            {
                if(i != n){
                    j = irxn[i];
                    rate *= X[j - 1];
                }
            }

            for(index = irxn; index < irxn + n_reactants; index++)
            {
                jaclist_insert(jacobian, deriv_index - 1, *index - 1, -rate);
            }

            for(index = ipxn; index < ipxn + n_products; index++)
            {
                jaclist_insert(jacobian, deriv_index - 1, *index - 1, rate);
            }
        }
    }
    jacprintlist(jacobian, outfile, matname);
}

/*
 * Dense Jacobian function for cvode
 */
static void CVjac_network(long int N, DenseMat J, realtype t,
    N_Vector y, N_Vector fy, void *jac_data,
    N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    register int i, j, r, n;
    int n_reactions;
    Rxn **rarray, *rxn;
    int *irxn, *ipxn, *index;
    int n_reactants, n_products;
    Elt **rates;
    double **J_ptr;
    double rate, Km, RATE;
    realtype *X;
    int deriv_index;

    n_reactions = n_rxns_network();

    /* Initialize f and J */
    DenseZero(J);
    J_ptr = J->data;

    /* Loop over all reactions */
    rarray = network.reactions->rxn;
    rates =  network.rates->elt - network.rates->offset;
    X = (double *)NV_DATA_S(y);
    for(r = 0; r < n_reactions; ++r)
    {
        rxn = rarray[r];
        /* Calculate the rate of this reaction */
        /* First rate constant is elementary reaction rate constant
           v = k1*X1...Xn */
        RATE = rxn->stat_factor * rxn->rateLaw_params[0]; //i.e. k1

        irxn = rxn->r_index;
        ipxn = rxn->p_index;

        n_reactants = rxn->n_reactants;
        n_products  = rxn->n_products;

        for(n = 0; n < n_reactants; n++)
        {
            deriv_index = irxn[n] - 1;
            rate = RATE;
            for(i = 0; i < n_reactants; i++)
            {
                if(i != n){
                    j = irxn[i];
                    rate *= X[j - 1];
                }
            }

            for(index = irxn; index < irxn + n_reactants; index++)
            {
                J_ptr[deriv_index][*index - 1] += -rate;
            }

            for(index = ipxn; index < ipxn + n_products; index++)
            {
                J_ptr[deriv_index][*index - 1] += rate;
            }
        }
    }
}

/*
 * Jacobian vector multiply for the CVODE GMRES solver
 */
static int CVjtimes(N_Vector v, N_Vector Jv, realtype t, N_Vector y,
    N_Vector fy, void *jac_data, N_Vector tmp)
{
    register int i, j, r, n;
    int n_reactions;
    Rxn **rarray, *rxn;
    int *irxn, *ipxn, *index;
    int n_reactants, n_products;
    Elt **rates;
    double **J_ptr;
    double rate, Km, RATE;
    realtype *X, *J, *V;
    static N_Vector START_V;
    static int init = (0 == 0);
    static double* Jacobian;
    //printf("%1.5e\n", t);

    n_reactions = n_rxns_network();

    /* Initialize f and J */
    N_VConst_Serial(0.0, Jv);
    J = (double *)NV_DATA_S(Jv);
    V = (double *)NV_DATA_S(v);

    /* Loop over all reactions */
    rarray = network.reactions->rxn;
    rates =  network.rates->elt - network.rates->offset;
    X = (double *)NV_DATA_S(y);
    for(r = 0; r < n_reactions; ++r)
    {
        rxn = rarray[r];
        /* Calculate the rate of this reaction */
        /* First rate constant is elementary reaction rate constant
           v = k1*X1...Xn */
        RATE = rxn->stat_factor * rxn->rateLaw_params[0]; //i.e. k1

        irxn = rxn->r_index;
        ipxn = rxn->p_index;

        n_reactants = rxn->n_reactants;
        n_products  = rxn->n_products;

        for(n = 0; n < n_reactants; n++)
        {
            rate = RATE;
            for(i = 0; i < n_reactants; i++)
            {
                if(i != n){
                    j = irxn[i];
                    rate *= X[j - 1];
                }
            }

            for(index = irxn; index < irxn + n_reactants; index++)
            {
                J[*index - 1] += -rate * V[irxn[n] - 1];
            }

            for(index = ipxn; index < ipxn + n_products; index++)
            {
                J[*index - 1] += rate * V[irxn[n] - 1];
            }
        }
    }
    return(0);
}

static void cvode_derivs(realtype t, N_Vector y, N_Vector ydot, void *f_data)
{

    /*     printf("t=%.15e\n", t); */
    (*network.derivs)( (double) t, (double *) NV_DATA_S(y), (double *) NV_DATA_S(ydot));
    return;
}

int propagate_cvode_network( double *t, double delta_t, long int *n_steps,
    double *rtol, double *atol, int SOLVER){
    int error=0;
    int i;
    double t_end;
    static int n_species;
    static N_Vector y;
    static void *cvode_mem;
    static int initflag=0;

    /* Initializations at the beginning of new propagation */
    if (initflag==0){

        /* Free previously allocated space */
        if (y) N_VDestroy_Serial(y);
        if (cvode_mem) CVodeFree(cvode_mem);

        n_species= n_species_network();

        /* Allocate space */
        y= N_VNew_Serial( n_species );
        get_conc_network( NV_DATA_S(y));

        /* Call to initialize CVODE */
        cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
        CVodeMalloc(cvode_mem, cvode_derivs, *t, y, CV_SS, *rtol, atol);
        CVodeSetErrFile(cvode_mem, stdout);
        if (cvode_mem==NULL){
            fprintf( stderr, "CVodeMalloc failed.\n");
            return(1);
        }

        /*
         * Specific intitialization for CVODE:
         * GMRES vs Dense
         * Jacobian vs finite difference
         */
        if(SOLVER == GMRES || SOLVER == GMRES_J){
            CVSpgmr(cvode_mem, PREC_NONE, 0);
            if(SOLVER == GMRES_J)
                CVSpgmrSetJacTimesVecFn(cvode_mem, CVjtimes, NULL);
        } else if (SOLVER == DENSE || SOLVER == DENSE_J) {
            CVDense( cvode_mem, n_species);
            if(SOLVER == DENSE_J)
                CVDenseSetJacFn(cvode_mem, CVjac_network, NULL);
        } else {
            fprintf(stderr, "Invalid CVODE solver\n");
            return(1);
        }

        /* Done with initialization */
        initflag=1;
    } else {
        get_conc_network( NV_DATA_S(y));
    }

    /* Propagation */
    t_end= (*t) + delta_t;
    while (1){
        error= CVode( cvode_mem, t_end, y, t, CV_NORMAL);
        if (!error) break;
        else if (error==CV_TOO_MUCH_WORK) continue;
        else exit(1);
    }
    CVodeGetNumSteps(cvode_mem, n_steps);

    /* Set network concentrations to values returned in X */
    set_conc_network( NV_DATA_S(y));

exit:
    return(error);
}

#define TINY 1e-8

int propagate_euler_network( double *t, double delta_t, long int *n_steps,
			     double h){
    int error=0;
    int i, n;
    int n_species;
    double htry, t_end, t_left, dt_inv;
    double *y, *dy;

    n_species= n_species_network();
    y= ALLOC_VECTOR(n_species);
    dy= ALLOC_VECTOR(n_species);

    /* Initialize concentrations */
    get_conc_network( y);

    /* Propagation */
    t_left= delta_t;
    dt_inv= 1/delta_t;
    while(1){
      htry= (h<=t_left) ? h : t_left;

      /* Compute derivatives */
      (*(network.derivs))( *t, y, dy);
      /* Update concentrations */
      for (n=0; n<n_species; ++n){
	y[n]+= htry*dy[n];
      }
      /* Update time */
      t_left-= htry;
      (*t)+= htry;
      ++(*n_steps);
      /* Avoid underflow */
      if (fabs(t_left*dt_inv)<TINY) break;
    }

    /* Check for overflow */
    for (n=0; n<n_species; ++n){
      if(isnan(y[n])){
	fprintf( stderr, "Dependent variable overflow at t=%f\n", *t);
	return(1);
      }
    }

    /* Set final concentrations */
    set_conc_network(y);

    FREE_VECTOR(y);
    FREE_VECTOR(dy);

    return(0);
}

int propagate_rkcs_network( double *t, double delta_t, long int *n_steps, double tol){
    int error=0, n_species;
    double *X=NULL, *dX=NULL;
    double t_end, t_left, htry, hdid, dt_inv;
    static double hnext;

    n_species= n_species_network();
    X= ALLOC_VECTOR( n_species);
    dX= ALLOC_VECTOR( n_species);

    /* Set size of first time step to suggested value of last propagation
       step, unless no previous steps. */
    hnext= (hnext) ? hnext : 0.01*delta_t;

    /* Initialize concentrations */
    get_conc_network( X);

    /* Propagation */
    t_end= *t + delta_t;
    dt_inv= 1/delta_t;
    while (1){
      t_left= t_end - (*t);
      htry= (hnext<=t_left) ? hnext : t_left;
      /* avoid underflow */
      if (dt_inv*fabs(htry)<TINY) break;
      (*(network.derivs))( *t, X, dX);
      RKQS( X, dX, n_species, t, htry, tol, NULL, &hdid, &hnext, network.derivs);
/*       printf("hdid=%e\n", hdid); */
      ++(*n_steps);
      if (hdid==t_left)	break;
    }

    /* Set network concentrations to values returned in X */
    set_conc_network( X);

 exit:
    if (X) FREE_VECTOR(X);
    if (dX) FREE_VECTOR(dX);

    return(error);
}

FILE *init_print_concentrations_network( char *prefix, int append){
    FILE *out;
    int i, error=0;
    char buf[1000];
    Elt **nconc;
    char *fmt="%23s", *specname;
    char *mode= (append) ? "a" : "w";

    sprintf(buf, "%s.cdat", prefix);
    if (!(out=fopen(buf, mode))){
	++error;
	fprintf(stderr, "Couldn't open file %s.\n", buf);
	goto exit;
    }

    /* Skip header if this trajectory is a continuation */
    if (append) goto exit;

    /* Write header  */
    fprintf(out,"#");
    fprintf(out, "%22s", "time");
    for(i=0; i<n_species_network(); ++i){
      fprintf(out," %23d", i+1);
    }
    fprintf(out,"\n");

 exit:
    return(out);
}

int print_concentrations_network( FILE *out, double t){
    int i, error=0, n_species;
    double conc;
    Elt **nconc;

    if (!out){ ++error; goto exit;}
    n_species= n_species_network();

    fprintf( out, "%23.12e", t);
    nconc= network.species->elt;
    for(i=0; i<n_species; ++i,++nconc){
      conc= (*nconc)->val;
      fprintf( out, " %23.12e", conc);
    }
    fprintf( out, "\n");
    fflush(out);

 exit:
    return(error);
}

int finish_print_concentrations_network( FILE *out){
    int error=0;

    if (!out){++error; goto exit;}

/*     fprintf(out, "end conc(time)\n"); */
    fclose(out);

 exit:
    return(error);
}

FILE *init_print_group_concentrations_network( char *prefix, int append){
    FILE *out;
    Group *group;
    int i, error=0;
    char buf[1000];
    char *fmt="%15s";
    char *mode= (append) ? "a" : "w";

    if (!n_groups_network()){
	out=NULL;
	goto exit;
    }

    sprintf(buf, "%s.gdat", prefix);
    if (!(out=fopen(buf, mode))){
	++error;
	fprintf(stderr, "Couldn't open file %s.\n", buf);
	goto exit;
    }

    /* Skip header if this file is a continuation */
    if (append) goto exit;

    /* Write group header  */
    fprintf(out, fmt, "#      time    ");
    for(group= network.spec_groups; group!=NULL; group=group->next){
	fprintf(out, " ");
	fprintf(out, fmt, group->name);
    }
    fprintf(out,"\n");

 exit:
    return(out);
}

int print_group_concentrations_network( FILE *out, double t){
    int i, error=0, n_species, offset, index;
    Group *group;
    double *X=NULL, conc, factor;
    const char *fmt="%15.8e";

    if (!out){ ++error; goto exit;}
    if (!n_groups_network()) goto exit;

    n_species= n_species_network();
    X=ALLOC_VECTOR(n_species);
    get_conc_network(X);

    fprintf( out, fmt, t);
    offset= network.species->offset;
    for(group= network.spec_groups; group!=NULL; group=group->next){
	conc=0.0;
	for (i=0; i<group->n_elt; ++i){
	    factor= (group->elt_factor) ? group->elt_factor[i] : 1.0;
	    index= group->elt_index[i]-offset;
	    conc+= factor*X[index];
	}
	fprintf( out, " ");
	fprintf( out, fmt, conc);
    }
    fprintf( out, "\n");
    fflush(out);

 exit:
    if (X) FREE_VECTOR(X);

    return(error);
}

int finish_print_group_concentrations_network( FILE *out){
    int error=0;

    if (!out){++error; goto exit;}
    fclose(out);

 exit:
    return(error);
}

double *get_group_concentrations_network(){
    int i, error=0, n_species, n_groups, offset, index;
    Group *group;
    double *X=NULL, *gconc=NULL, *gc, conc, factor;

    if (!(n_groups=n_groups_network())) goto exit;

    gconc= ALLOC_VECTOR(n_groups);

    n_species= n_species_network();
    X=ALLOC_VECTOR(n_species);
    get_conc_network(X);

    offset= network.species->offset;
    gc= gconc;
    for(group= network.spec_groups; group!=NULL; group=group->next, ++gc){
	conc=0.0;
	for (i=0; i<group->n_elt; ++i){
	    factor= (group->elt_factor) ? group->elt_factor[i] : 1.0;
	    index= group->elt_index[i]-offset;
	    conc+= factor*X[index];
	}
	*gc= conc;
    }

 exit:
    if (X) FREE_VECTOR(X);

    return(gconc);
}


FILE *init_print_species_stats( char *prefix, int append){
	FILE *out;
    int error = 0;
    char buf[1000];
    //char *mode = (append) ? "a" : "w";
    char *mode; 
    if (append) 
    	mode = "a";
    else 
    	mode = "w";

    sprintf(buf, "%s.jdat", prefix);
    if (!(out=fopen(buf, mode))){
    	++error;
    	fprintf(stderr, "Couldn't open file %s.\n", buf);
    	goto exit;
    }

    /* Skip header if this trajectory is a continuation */
    if (append) goto exit;

    /* Write header  */
    fprintf(out,"#");
    fprintf(out, "%18s", "time");
    fprintf(out, "%19s", "curr_pop_species");
    fprintf(out, "%19s", "ever_pop_species");
    fprintf(out, "%19s", "tot_species_net");
    fprintf(out,"\n");

    exit:
    return(out);
}

int print_species_stats( FILE *out, double t){
	int i, error=0;

	if (!out){ ++error; goto exit;}
    fprintf( out, "%19.12e", t );
    fprintf( out, "%19d", n_species_active()  );
    fprintf( out, "%19d", n_species_ever_populated()  );
    fprintf( out, "%19d", n_species_network() );
    fprintf( out, "\n");
    fflush(out);

    exit:
    return(error);
}

int finish_print_species_stats( FILE *out){
	int error=0;

	if (!out){++error; goto exit;}
	fclose(out);

	exit:
	return(error);
}


FILE *init_print_flux_network( char *prefix){
    FILE *out;
    Group *group;
    int i, error=0;
    char buf[1000];

    sprintf(buf, "%s.fdat", prefix);
    printf("Writing fluxes to file %s.\n", buf);
    if (!(out=fopen(buf, "w"))){
	++error;
	fprintf(stderr, "Couldn't open file %s.\n", buf);
	goto exit;
    }

  exit:
    return(out);
}

int print_flux_network(FILE *out, double t){
    register int i;
    int error=0, n_reactions;
    double *rates_rxn=NULL;
    const char *fmt="%15.8e";

    n_reactions= n_rxns_network();
    rates_rxn= ALLOC_VECTOR( n_reactions);

    /* Calculate reaction rates from current concentrations */
    rxn_rates_network( rates_rxn);

    /* Print reaction fluxes */
    fprintf(out, fmt, t);
    for(i=0; i<n_reactions; ++i){
	fprintf( out," ");
	fprintf( out, fmt, rates_rxn[i]);
    }
    fprintf(out,"\n");
    fflush(out);

 exit:
    if (rates_rxn) FREE_VECTOR(rates_rxn);

    return(error);
}

int print_network( FILE *out){
    int error=0;

    if (network.rates){
	fprintf(out, "begin parameters\n");
	print_Elt_list( out, network.rates->list);
	fprintf(out, "end parameters\n");
    }

    if (network.species){
	fprintf(out, "begin species\n");
	print_Elt_list( out, network.species->list);
	fprintf(out, "end species\n");
    }

    if (network.reactions){
/* 	print_derivs_network(out); */
	print_Rxn_array( out, network.reactions, network.species, network.rates);
/*	print_rates_network( out); */
    }

    if (network.spec_groups){
	print_Groups( out, network.spec_groups, network.species);
    }

 exit:
    return(error);
}

void print_species_network( FILE *out){
    fprintf(out, "begin species\n");
    print_Elt_list( out, network.species->list);
    fprintf(out, "end species\n");
    return;
}

/* Array of integer arrays with unequal lengths
   add elements by using iarray_add_elt */
typedef struct {
    int n_arr;  /* number of arrays */
    int increment; /* Amount of space to add each time array is
		      reallocated as it grows. Controls frequency of
		      realloc calls */
    int *l_arr; /* length of each array */
    int *maxl_arr; /* maximum length each array */
    int **arr;  /* pointer to arrays */
} iarray;

iarray *new_iarray( int n_arr, int increment){
    int i;
    iarray *iarr=0x0;

    iarr= (iarray *) calloc( 1, sizeof(iarray));

    iarr->n_arr= n_arr;
    iarr->increment= increment;
    iarr->l_arr= (int *) calloc( n_arr, sizeof(int));
    iarr->maxl_arr= (int *) calloc( n_arr, sizeof(int));
    iarr->arr= (int **) calloc( n_arr, sizeof(int*));
    for(i=0; i<n_arr; ++i){
	iarr->maxl_arr[i]= increment;
	iarr->arr[i]= (int *) calloc( increment, sizeof(int));
    }
    return(iarr);
}

void free_iarray( iarray *iarr){
  int i;

  if (!iarr) return;

  if (iarr->n_arr){
    free(iarr->l_arr);
    free(iarr->maxl_arr);
    for(i=0; i<iarr->n_arr; ++i){
      free(iarr->arr[i]);
    }
    free(iarr->arr);
  }
  free(iarr);
  return;
}

void print_iarray( FILE *out, iarray *iarr){
    int row, elt, *arr;

    for(row=0; row<iarr->n_arr; ++row){
	fprintf( out, "%5d  ", row);
	arr= iarr->arr[row];
	for(elt=0; elt<iarr->l_arr[row]; ++elt){
	    fprintf( out, " %d", arr[elt]);
	}
	fprintf( out, "\n");
    }
    return;
}

void print_iarray_lengths( FILE *out, iarray *iarr){
    int row, elt, *arr;

    for(row=0; row<iarr->n_arr; ++row){
	fprintf( out, "%5d  %d\n", row, iarr->l_arr[row]);
    }
    return;
}

/* Add new rows to iarray up to index row */
int iarray_add_rows( iarray *iarr, int row){
  int err=0;
  int i, n_arr_old;

  if (row<iarr->n_arr) goto exit;

  n_arr_old= iarr->n_arr;

  iarr->n_arr= row+1;
  iarr->l_arr= (int *) realloc( iarr->l_arr, iarr->n_arr*sizeof(int));
  iarr->maxl_arr= (int *) realloc( iarr->maxl_arr, iarr->n_arr*sizeof(int));
  iarr->arr= (int **) realloc( iarr->arr, iarr->n_arr*sizeof(int*));
  for(i=n_arr_old; i<=row; ++i){
    iarr->l_arr[i]=0;
    iarr->maxl_arr[i]= iarr->increment;
    iarr->arr[i]= (int *) calloc( iarr->increment, sizeof(int));
  }
  iarr->n_arr= row+1;

 exit:
  return(err);
}

/* Add a new element to array row of the iarray- fancier versions could check
   element for redundancy with prexisting elements, or add to sorted list */
int iarray_add_elt( iarray *iarr, int row, int elt){
    int err=0;


    if(row>=iarr->n_arr){
      iarray_add_rows(iarr,row);
    }

    /* Check if space for new element */
    if(iarr->l_arr[row]>=iarr->maxl_arr[row]){
	/* Add increment of space */
	iarr->maxl_arr[row]+= iarr->increment;
	iarr->arr[row]= (int *) realloc(iarr->arr[row], iarr->maxl_arr[row]*sizeof(int));
    }

    /* Add new element */
    iarr->arr[row][iarr->l_arr[row]]= elt;
    ++iarr->l_arr[row];

 exit:
    return(err);
}

static struct {
  double *c; /* concentrations- could be integer */
  int    *ever_populated; /* array of length nc: element is 1 if corresponding species was ever populated, 0 otherwise --justin */
  double *c_offset; /* Offset pointer for passing to routines that use
		       spcecies element indices */
  double *a; /* reaction rates */
  double a_tot; /* total reaction rate */
  double t_next; /* time to next reaction */
  int nc;  /* number of species */
  int na;  /* number of reactions */
  iarray *as_reactant_list; /* gives the array of reactions in which
			       a species appears as a reactant */
  iarray *as_product_list; /* gives the array of reactions in which
			      a species appears as a product*/
  iarray *rxn_update_list; /* array of reactions that must be updated after
			      reaction of given index fires */
  long int n_steps;
  int rxn_update_size;
  int rxn_rate_update_interval;

  long int recalc_a_tot_every_n_steps;
} GSP;

int n_species_ever_populated(){
    int ii;
    int n_species = 0;
    for ( ii=0; ii < GSP.nc; ++ii)
    {
        if (GSP.ever_populated[ii] == 1) ++n_species;
    }
    return n_species;
}

void print_rxn_update_list(FILE *out){
    iarray *iarr;
    int irxn, jrxn, rindex;
    Rxn **rarray, *rxn;
    int n_update_avg=0;

    iarr= GSP.rxn_update_list;
    rarray= network.reactions->rxn;

    for(irxn=0; irxn<iarr->n_arr; ++irxn){
	rxn= rarray[irxn];
/* 	fprintf(out, "Rxn "); */
/* 	print_Rxn_text(out, rxn, network.species, network.rates); */
/* 	fprintf(out, "  %d\n", iarr->l_arr[irxn]); */
	n_update_avg+= iarr->l_arr[irxn];
/* 	for(jrxn=0; jrxn<iarr->l_arr[irxn]; ++jrxn){ */
/* 	    rindex= iarr->arr[irxn][jrxn]; */
/* 	    if (rindex>=iarr->n_arr){ */
/* 	      fprintf(out, "%d %d\n", irxn, rindex); */
/* 	    } */
/* 	    fprintf(out, "   "); */
/* 	    print_Rxn_text(out, rarray[rindex], network.species, network.rates); */
/* 	    fprintf(out, "\n"); */
/* 	} */
    }
    fprintf(out, "Average number of update rxns is %.1f\n", (double) n_update_avg/GSP.na);
    return;
}

void print_as_reactant_list(FILE *out){
    iarray *iarr;
    int ispec, irxn, rindex;
    int offset;
    Elt **earray, *elt;
    Rxn **rarray, *rxn;

    iarr= GSP.as_reactant_list;
    offset= network.species->offset;
    earray= network.species->elt;
    rarray= network.reactions->rxn;

    for(ispec=0; ispec<iarr->n_arr; ++ispec){
	elt= earray[ispec];
	fprintf( out, "%s\n", elt->name);
	for(irxn=0; irxn<iarr->l_arr[ispec]; ++irxn){
	    rindex= iarr->arr[ispec][irxn];
	    print_Rxn_text(out, rarray[rindex], network.species, network.rates);
	    fprintf(out, "\n");
	}
    }
    return;
}

/* Determine list of reactions whose rates depend on each species */
int create_update_lists(){
    int i, ip, ir, is,irxn;
    int err=0;
    int *rxn_dep; /* a boolean array to keep track of dependent reaction in
		     creating rxn update list */
    iarray *arl, *apl, *rul; /* species and reaction update lists */
    int offset, r_index, p_index;
    Rxn **rarray, *rxn;

    arl= new_iarray( GSP.nc, 10);
    apl= new_iarray( GSP.nc, 10);
    rul= new_iarray( GSP.na, 10);

    offset= network.species->offset;
    rarray= network.reactions->rxn;

    /* Create species update list by looping over reactions adding rxn to each
       species that appears as reactant */
    for(i=0; i<GSP.na; ++i){
      if (!(rxn=rarray[i])) continue;
      /* loop over reactant indices for reaction */
      for(ir=0; ir<rxn->n_reactants;++ir){
	r_index= rxn->r_index[ir]-offset;
	iarray_add_elt(arl, r_index, i);
      }
      for(ip=0; ip<rxn->n_products;++ip){
	p_index= rxn->p_index[ip]-offset;
	iarray_add_elt(apl, p_index, i);
      }
    }

    /* Create reaction update list by looping over reactants for each reaction
       and adding all reactions that appear on species update lists */
    rxn_dep= IALLOC_VECTOR(GSP.na);
    GSP.rxn_update_size=0;
    for(i=0; i<GSP.na; ++i){
	if (!(rxn=rarray[i])) continue;
	IINIT_VECTOR(rxn_dep, 0, GSP.na);

	/* loop over reactant indices for reaction */
	for(ir=0; ir<rxn->n_reactants;++ir){
	    r_index= rxn->r_index[ir]-offset;
	    /* loop over reactions in species update list for this species */
	    for(is=0; is<arl->l_arr[r_index]; ++is){
		++rxn_dep[arl->arr[r_index][is]];
	    }
	}

	/* loop over product indices for reaction */
	for(ip=0; ip<rxn->n_products;++ip){
	    p_index= rxn->p_index[ip]-offset;
	    /* loop over reactions in species update list for this species */
	    for(is=0; is<arl->l_arr[p_index]; ++is){
		++rxn_dep[arl->arr[p_index][is]];
	    }
	}

	/* loop over elements of rxn_dep adding nonzero elts to rxn update
	   list for this reaction */
	for(irxn=0; irxn<GSP.na; ++irxn){
	  if (rxn_dep[irxn]){
	    iarray_add_elt(rul, i, irxn);
	    ++GSP.rxn_update_size;
	  }
	}
    }
    free(rxn_dep);

    GSP.as_reactant_list= arl;
    GSP.as_product_list=  apl;
    GSP.rxn_update_list= rul;

/*     print_as_reactant_list( stdout); */
    print_rxn_update_list( stdout);

/*     printf("RUL_size: %d\n", GSP.rxn_update_size); */
    return(err);
}

int init_gillespie_direct_network(int update_interval, int seed){
    int i;

    Rxn **rarray;

    // Initialize random number generator
    if (seed>=0){
      SEED_RANDOM(seed);
    }

    GSP.n_steps=0;

    /* set some algorithm parameters */
    GSP.recalc_a_tot_every_n_steps = 1000000;

    /* Initialize reaction number and species number */
    GSP.nc= n_species_network();
    GSP.na= n_rxns_network();

    /* Initialize concentration array */
    GSP.c= ALLOC_VECTOR(GSP.nc);
    get_conc_network(GSP.c);
    GSP.c_offset= GSP.c - network.species->offset;

    /* Round all concentrations to nearest integer */
    for(i=0; i<GSP.nc; ++i){
      GSP.c[i]= rint(GSP.c[i]);
    }
    set_conc_network( GSP.c);

    /* initialize ever_populated array */
    GSP.ever_populated = IALLOC_VECTOR(GSP.nc);
    for( i=0; i<GSP.nc; ++i ){
      GSP.ever_populated[i] = ( GSP.c[i] > 0.0 ? 1 : 0 );
    }

    /* Initialize reaction rate array */
    GSP.a= ALLOC_VECTOR( GSP.na);
    /* rxn_rates_network( GSP.a); */
    rarray= network.reactions->rxn;
    for(i=0; i<GSP.na; ++i){
      GSP.a[i]= rxn_rate( rarray[i], GSP.c_offset, 1);
      GSP.a_tot+= GSP.a[i];
    }

    if (update_interval>0){
      GSP.rxn_rate_update_interval=update_interval;
      printf("rxn_rate_update_interval=%d\n", update_interval);
    }
    else
      GSP.rxn_rate_update_interval=1;

    create_update_lists();

    return(0);
}

int update_gillespie_direct_network(int n_spec_new, int n_rxns_new){
  int i;
  int nc_old, na_old;
  int ip, ir, is,irxn;
  int err=0;
  int *rxn_dep, *rxn_dep2, *rxn_dep3; /* boolean arrays to keep track of dependent reactions in
		   creating rxn update list */

  iarray *arl, *apl, *rul; /* species and reaction update lists */
  int offset, r_index, p_index;
  Rxn **rarray, *rxn;

  /* Add space for new species and reaction rates and initialize*/
  nc_old=GSP.nc;
  if (n_spec_new){
    GSP.nc+=  n_spec_new;
    GSP.c= REALLOC_VECTOR( GSP.c, GSP.nc);
    GSP.ever_populated = REIALLOC_VECTOR( GSP.ever_populated, GSP.nc);
    GSP.c_offset= GSP.c - network.species->offset;
    for(i=nc_old; i<GSP.nc; ++i)
    {   
        GSP.c[i] = 0.0;
        GSP.ever_populated[i] = ( GSP.c[i] > 0.0 ? 1 : 0 );
    }
  }

  na_old=GSP.na;
  if (n_rxns_new){
    GSP.na+=  n_rxns_new;
    GSP.a= REALLOC_VECTOR( GSP.a,GSP.na);
    for(i=na_old; i<GSP.na; ++i){GSP.a[i]=0.0;}
  }

  /*   create_update_lists(); */
  /* Append update lists */
  iarray_add_rows(arl=GSP.as_reactant_list, GSP.nc-1);
  iarray_add_rows(apl=GSP.as_product_list , GSP.nc-1);
  iarray_add_rows(rul=GSP.rxn_update_list , GSP.na-1);

  offset= network.species->offset;
  rarray= network.reactions->rxn;

  /* Create species update list by looping over reactions adding rxn to each
     species that appears as reactant */
  for(i=na_old; i<GSP.na; ++i){
    if (!(rxn=rarray[i])) continue;
    /* loop over reactant indices for reaction */
    for(ir=0; ir<rxn->n_reactants;++ir){
      r_index= rxn->r_index[ir]-offset;
      iarray_add_elt(arl, r_index, i);
    }
    for(ip=0; ip<rxn->n_products;++ip){
      p_index= rxn->p_index[ip]-offset;
      iarray_add_elt(apl, p_index, i);
    }
  }

  /* This method is far more straightforward than the (in principle)
     more optimized method that is commented out below. */
  rxn_dep= IALLOC_VECTOR(GSP.na);
  for(i=0; i<GSP.na; ++i){
    int ilower;
    if (!(rxn=rarray[i])) continue;
    IINIT_VECTOR(rxn_dep, 0, GSP.na);

    /* loop over reactant indices for reaction */
    for(ir=0; ir<rxn->n_reactants;++ir){
      r_index= rxn->r_index[ir]-offset;
      /* loop over reactions in species update list for this species */
      for(is=0; is<arl->l_arr[r_index]; ++is){
	++rxn_dep[arl->arr[r_index][is]];
      }
    }

    /* loop over product indices for reaction */
    for(ip=0; ip<rxn->n_products;++ip){
      p_index= rxn->p_index[ip]-offset;
      /* loop over reactions in species update list for this species */
      for(is=0; is<arl->l_arr[p_index]; ++is){
	++rxn_dep[arl->arr[p_index][is]];
      }
    }

    /* loop over elements of rxn_dep adding nonzero elts to rxn update
       list for this reaction */
    ilower= (i>=na_old) ? 0 : na_old;
    for(irxn=ilower; irxn<GSP.na; ++irxn){
      if (rxn_dep[irxn]){
	iarray_add_elt(rul, i, irxn);
	++GSP.rxn_update_size;
      }
    }
  }
  free(rxn_dep);

#ifdef COMMENT
  /* Create reaction update list by looping over reactants for each reaction
     and adding all reactions that appear on species update lists */
  rxn_dep= IALLOC_VECTOR(GSP.na);
  rxn_dep2= IALLOC_VECTOR(GSP.na);
  rxn_dep3= IALLOC_VECTOR(GSP.na);
  for(i=na_old; i<GSP.na; ++i){
    if (!(rxn=rarray[i])) continue;
    IINIT_VECTOR(rxn_dep, 0, GSP.na);
    IINIT_VECTOR(rxn_dep2, 0, GSP.na);
    IINIT_VECTOR(rxn_dep3, 0, GSP.na);

    /* loop over reactant indices for reaction */
    for(ir=0; ir<rxn->n_reactants;++ir){
      r_index= rxn->r_index[ir]-offset;
      /* loop over reactions in which reactant species appears as reactant */
      for(is=0; is<arl->l_arr[r_index]; ++is){
	++rxn_dep[arl->arr[r_index][is]];
     }
      /* loop over reactions in which reactant species appears as
	 product */
      for(is=0; is<apl->l_arr[r_index]; ++is){
	++rxn_dep2[apl->arr[r_index][is]];
      }
    }

    /* loop over product indices for reaction */
    for(ip=0; ip<rxn->n_products;++ip){
      p_index= rxn->p_index[ip]-offset;
      /* loop over reactions in which product species appears as
	 reactant */
      for(is=0; is<arl->l_arr[p_index]; ++is){
	++rxn_dep3[arl->arr[p_index][is]];
      }
    }

    /* Add new dependencies for old reactions and dependencies
       for new reactions involving old reactions */
    for(irxn=0; irxn<na_old; ++irxn){
      if (rxn_dep[irxn]){
	iarray_add_elt(rul, i, irxn);
	++GSP.rxn_update_size;
	iarray_add_elt(rul, irxn, i);
	++GSP.rxn_update_size;
      } else {
	if (rxn_dep2[irxn]){
	  iarray_add_elt(rul, irxn, i);
	  ++GSP.rxn_update_size;
	}
	if (rxn_dep3[irxn]){
	  iarray_add_elt(rul, i, irxn);
	  ++GSP.rxn_update_size;
	}
      }
    }
    /* Add dependencies for new reactions involving new reactions */
    for(irxn=na_old; irxn<GSP.na; ++irxn){
      if (rxn_dep[irxn]||rxn_dep3[irxn]){
	iarray_add_elt(rul, i, irxn);
	++GSP.rxn_update_size;
      }
    }
  }
  free(rxn_dep);
  free(rxn_dep2);
  free(rxn_dep3);
#endif

/*   printf("RUL_size: %d\n", GSP.rxn_update_size); */

/*   print_rxn_update_list( stdout); */

  return(0);
}


/* Returns index of next reaction determined randomly */
/* NOTE: Value of a_tot must be greater than zero, or this produces incorrect results. */
int select_next_rxn(){
    int irxn;
    double f, a_sum;

	/* find next reaction */
    while (1) {

        /* generate random number between 0 and a_tot */
        while ( (f = RANDOM(0.0, GSP.a_tot)) == 0 );

        /* find rxn corresponding to random sample */
        a_sum = 0.0;
        for ( irxn=0; irxn < GSP.na; ++irxn ) {
	        a_sum += GSP.a[irxn];
	        if (f<=a_sum) break;
        }

        if ( irxn==GSP.na ) {
            /* we picked a reaction that doesn't exist! */
            GSP.a_tot = a_sum;
            if (a_sum == 0.0) {
                /* no reactions have positive propensity! */
                return( irxn );
            } 
        }
        else {
            return(irxn);
        }
    }
}

int update_concentrations(int irxn) {
	int i;
	int offset;
	int force_update = 0;
	Rxn *rxn;
	Elt **spec;
	iarray *spec_newpop = 0x0;
	const int thresh_occ = 10;

	rxn = network.reactions->rxn[irxn];
	offset = network.species->offset;

	/* loop over reactants */
	spec = network.species->elt;
	for (i = 0; i < rxn->n_reactants; ++i) {
		int ri = rxn->r_index[i] - offset;
		if (!spec[ri]->fixed) {
			double newpop = --GSP.c[ri];
			if (newpop < 1.0)
				GSP.c[ri] = 0.0; /* This must be done to avoid negative concentrations! */
			if (newpop < thresh_occ)
				force_update = 1;
		}
	}

	/* loop over products */
	for (i = 0; i < rxn->n_products; ++i)
    {
   		int pi = rxn->p_index[i] - offset;
		if (!spec[pi]->fixed)
        {
			double newpop = ++GSP.c[pi];
			if (newpop <= thresh_occ) force_update = 1;
			/* Handle species populated for first time */
            if ( GSP.ever_populated[pi] == 0 )
            {
                if ( GSP.as_reactant_list->l_arr[pi] == 0)
                {
				    if (!spec_newpop) spec_newpop = new_iarray(1, 10);
				    /* Check if reactions have already been generated for this
				        species and if not, call network generator to generate all
				        reacions arising either from this species or at this level,
				        as determined by network generator */
				    iarray_add_elt(spec_newpop, 0, pi + offset);
                }
                GSP.ever_populated[pi] = 1;
            }
		}
	}

	/* Create new reactions and species arising from newly populated species */
	if (spec_newpop) {
		char *line;
		int ispec;
		int line_number = 0, n_spec_new = 0, n_rxns_new = 0, n_groups_updated;
		Elt **elt;
		Rxn * *rxn;
		Elt_array *spec_new;
		Rxn_array *rxns_new;
		int nspec_newpop = spec_newpop->l_arr[0];
		int *ispec_newpop = spec_newpop->arr[0];

		/* Call network generator with index and name of newly
		 populated species */
		elt = network.species->elt - network.species->offset;
		printf("edgepop:");
		for (i = 0; i < nspec_newpop; ++i) {
			ispec = ispec_newpop[i];
			printf(" %s", elt[ispec]->name);
		}
		printf("\n");
		fflush(stdout);
		/* Process return data */
		line = get_line(stdin);
		/* Only read data preceeded by read command, otherwise continue */
		if (strncmp("read", line, 4) == 0) {
			/* Read new species */
			spec_new = read_Elt_array(stdin, &line_number, "species",
					&n_spec_new, 0x0);
			append_Elt_array(network.species, spec_new);

			/* Read new reactions */
			rxns_new = read_Rxn_array(stdin, &line_number, &n_rxns_new,
					network.species, network.rates);
			append_Rxn_array(network.reactions, rxns_new);

			/* Update species and reaction dependency lists */
			if (n_rxns_new)
				update_gillespie_direct_network(n_spec_new, n_rxns_new);

			/* Read group updates */
			read_Groups(network.spec_groups, stdin, network.species,
					&line_number, "groups", &n_groups_updated);

			printf(
					"At step %ld added %d new species (%d total) %d new reactions (%d total)\n",
					GSP.n_steps, n_spec_new, GSP.nc, n_rxns_new, GSP.na);
			/* 	if (n_groups_updated) */
			/* 	  printf("  and updated %d groups.", n_groups_updated); */
			/* 	printf("\n"); */
		} else {
			printf("Population of species");
			for (i = 0; i < nspec_newpop; ++i) {
				ispec = ispec_newpop[i];
				printf("  %s", elt[ispec]->name);
			}
			printf(" did not produce new reactions or species.\n");
		}
		fflush(stdout);
		if (line)
			free(line);
		free_iarray(spec_newpop);
	}

	return (force_update);
}

void update_rxn_rates(int irxn){
    Rxn **rarray;
    int j, jrxn, n_rxns, *rxns;
    double anew;

    rarray = network.reactions->rxn;
    n_rxns = GSP.rxn_update_list->l_arr[irxn];
    rxns   = GSP.rxn_update_list->arr[irxn];

    /* Loop over reactions in rxn update list updating both reaction and total
        rate */
   
    /* periodically re-calculate a_tot from scratch */
    if ( GSP.n_steps % GSP.recalc_a_tot_every_n_steps == 0 ){  
        /* first update rxn propensities influenced by this rxn */
        for (j=0; j<n_rxns; ++j){
            jrxn = rxns[j];
            GSP.a[jrxn] = rxn_rate( rarray[jrxn], GSP.c_offset, 1);
        }

        /* now calculate a_tot from scratch */
        GSP.a_tot = 0.0;
        for (jrxn=0; jrxn<GSP.na; ++jrxn ){
            GSP.a_tot += GSP.a[jrxn];
        }
    }
    /* standard a_tot update: delta rxn rate */
    else{  
        /* update rxn propensities influenced by this rxn and simultaneously make delta updates to a_tot */
        for (j=0; j<n_rxns; ++j){
            jrxn = rxns[j];
            anew = rxn_rate( rarray[jrxn], GSP.c_offset, 1);
            GSP.a_tot += (anew - GSP.a[jrxn]);
            GSP.a[jrxn] = anew;
        }
    }
    return;
}

int gillespie_direct_network( double *t, double delta_t,
			      double *C_avg, double *C_sig){
    int i;
    double t_remain;
    double rnd;
    int irxn;
    int error=0;
    int rxn_rate_update;

    /* Initialize times */
    t_remain= delta_t;

    if (!GSP.c){
        fprintf(stderr, "gillespie_direct_network called without initialization.\n");
        exit(1);
    }

    while (1){
        /* Determine time to next reaction */
        if ( GSP.a_tot <= 0.0 ) break;
        while( (rnd = RANDOM(0.0, 1.0)) == 0.0 ); /* avoid taking log of zero */
        t_remain -= -log(rnd)/GSP.a_tot;

        /* Don't fire the next reaction if it occurs past the current integration endpoint */
        if ( t_remain < 0.0 ) break;

        /* Select next reaction to fire */
        irxn = select_next_rxn();
        if (irxn == GSP.na) break;

        /* fire rule by updating concentrations */
        rxn_rate_update = update_concentrations(irxn);
        ++GSP.n_steps;

        /* update rxn rates */
        if (rxn_rate_update || (GSP.n_steps%GSP.rxn_rate_update_interval==0))
            update_rxn_rates(irxn);
    }
    /* printf("conc "); for(i=0; i<GSP.nc; ++i) printf(" %.1e", GSP.c[i]); printf("\n"); */
    /* printf("rates"); for(i=0; i<GSP.na; ++i) printf(" %.1e", GSP.a[i]); printf("\n"); */

    /* Back up to return time*/
    *t += delta_t;

    /* Set final network concentrations */
    set_conc_network( GSP.c);

 exit:
    return(error);
}

double gillespie_frac_species_active(){
  int i;
  int n_act=0;
  double *conc;

  conc= GSP.c;
  for(i=0; i<GSP.nc; ++i){
    if (conc[i]>0.0) ++n_act;
  }
  return((double)n_act/GSP.nc);
}

double gillespie_frac_rxns_active(){
  int i;
  int n_act=0;
  double *rxn_rates;

  rxn_rates= GSP.a;
  for(i=0; i<GSP.na; ++i){
    if (rxn_rates[i]>0.0) ++n_act;
  }
  return((double)n_act/GSP.na);
}


long int gillespie_n_steps(){
    return(GSP.n_steps);
}

#include	"link.h"
#include	<string.h> // For strcpy, strcat, strlen
#include	<stdlib.h> // For calloc, exit
#include	<unistd.h> // For unlink
/* linker support functions */

// External variables (Nerrors, Mapp, Outname, etc.) should be declared as extern in link.h
// and defined in link.c (or their appropriate defining .c file).
// Do not add extern declarations for them here in sup.c.

char returnchar(unsigned short k); // Forward declaration for returnchar
void derad50(unsigned short x, char *s); // Forward declaration for derad50

// char *strcpy(), *strcat(); // These are provided by <string.h>

void uerror(char *mess)
{
	Nerrors++;
	fprintf(stderr, "%s\n", mess);
	if (Mapp != NULL) // Check if map file is open
		fprintf(Mapp, "%s\n", mess);
}

void bail_out(void)
{
	// Minimal bail_out for sup.c, specific file unlinking might be better in pass2.c's version
	// or pass file pointers to a more generic bail_out.
	// This version just exits. The one in pass2.c unlinked specific temp files.
	// If linker errors persist for Outname etc., they need to be declared extern here too.
	if (Outname != NULL && Outp != NULL) unlink(Outname); // Example cleanup
	if (Mapname != NULL && Mapp != NULL) unlink(Mapname);
	if (Symname != NULL && Symp != NULL) unlink(Symname);
	if (Bitname != NULL && Bitp != NULL) unlink(Bitname);
	exit(1);
}

/************************  lerror  ****************************************/


void lerror(mess)	/* linker program error, print message and exit */
register char	*mess;
{
	fprintf(stderr, "linker program error: %s\n", mess);
	bail_out();
}


/**************************  dc_symbol  **************************************/


void dc_symbol(s)	/* decode rad50 symbol in input stream and place in */
		/* the buffer s. */
register char	*s;

{
	// WORD getword(); // Declaration is in link.h
	
	derad50(getword(), s);
	derad50(getword(), s + 3);
	*(s + 6) = '\0';
}


/******************************  derad50  ************************************/


void derad50(x,s)		/* decode a word in which 3 characters are coded by */
			/* the RAD50 scheme. */
register unsigned short	x;
register char 	*s;

{
	s[2] = returnchar(x % 40);
	x /= 40; 
	s[1] = returnchar(x % 40);
	x /= 40;
	s[0] = returnchar(x % 40); 
}


/******************************  returnchar  *******************************/


char returnchar(k)   	/* return a character according to RAD50 coding */ // Added char
			/* scheme, called by derad50 */
register unsigned short	k;
{
	if (k >= 1 && k <= 26)
		/* k represents a letter */
		return('a' + k - 1);

	else if (k >= 30 && k <= 39)
		/* k represents a digit */
		return('0' + k - 30);

	else switch (k)
	{
		case 0:
			return(' ');
		
		case 27:
			return('$');

		case 28:
			return('.');

		case 29:
			return ('_');

		default:
			lerror("RAD50 non-character");
			/* NOTREACHED */
	}

}


/*****************************  lalloc  **************************************/


char	*lalloc(amount)		/* storage allocator, calls calloc and if */ // Already char*
				/* null is returned calls error */
register int	amount;		/* number of bytes of storage needed */
{
	// char *calloc(); // Provided by <stdlib.h>
	register char	*temp;

	if ((temp = calloc(1, (unsigned)amount)) == NULL)
	{
		fprintf(stderr, "Error: core size exceeded.\n");
		bail_out();
		/* NOTREACHED */
	}
	else
		return (temp);
}


/**********************************  tack  ***********************************/


char	*tack(s, t)	/* catenate s with t if s does not already end with t */ // Already char*

register char	*s;
register char	*t;
{
	register char	*new;
	int 		s_len;
	int		t_len;

	s_len = strlen(s);
	t_len = strlen(t);
	if (s_len < t_len || strcmp(s + s_len - t_len, t))
	{
		new = lalloc(s_len + t_len + 1);
		strcpy(new, s);
		strcat(new, t);
		return(new);
	}
	else
		return (s);
}


/******************************  strip  ************************************/


void strip(s, t)	/* strip t off the end of s if it is there */ // Added void

register char	*s;
register char	*t;
{
	register int	s_len;
	int		t_len;

	s_len = strlen(s);
	t_len = strlen(t);
	if (s_len >= t_len && !strcmp(s + s_len - t_len, t))
		*(s + s_len - t_len) = '\0';
}

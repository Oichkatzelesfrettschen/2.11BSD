# include	<stdio.h>
#ifdef vax
#define WORD	unsigned short
#elif defined(pdp11)
#define WORD	unsigned
#else
#define WORD unsigned short
#endif /* vax, pdp11 */

/********************* structure declarations *************************/

struct psect		/* program section structure
			** Since program sections have to be referenced in
			** different ways,  there are multiple link-lists
			** running through each structure.
			** Program sections are referenced 1) by ins-dat-bss
			** type to facilitate the computation of relocation
			** constants (*next), 2) by object file so that relocation
			** constants may be found when interpreting the code
			** section within a object file (*obsame).
			** In addition global program sections of the same 
			** name and type must be grouped together (*pssame).
			** Also, a list of global relocatable symbols defined
			** in the psect in used to relocate these 
			** symbols (*slist). */
{
	char		name[7];
	char		type;		/* attribute byte from M11 */
	int		nbytes;		/* number of bytes in psect */
	WORD		rc;		/* relocation constant */
	struct psect	*next;		/* next psect of same ins-dat-bss type */
	struct psect	*pssame;	/* same psect from different object file */
	struct psect	*obsame;	/* next psect in same object file */
	struct symbol	*slist;		/* list of symbols defined in this psect */
};


struct symbol			/* global symbol structure, the symbol
				** table is a binary tree of these structures .
				** There are also link-lists from each psect
				** running through the symbol table to
				** facilitate  the relocation of symbols. */
{
	char		name[7];
	char		type;		/* attribute byte from M11 */
	int		value;
	int		t_numb;		/* number of the symbol in the out
					** file symbol table */
	struct symbol	*symlist;	/* continuation of list of symbols
					** defined in this symbol's psect */
	struct psect	*prsect;	/* program section of the symbol */
	struct symbol	*left;		/* left child */
	struct symbol	*right;		/* right child */
};


struct objfile		/* object file structure */
{
	char		*fname;
	struct objfile  *nextfile;	/* next file in link-list of files */
	struct psect	*psect_list;	/* root to link-list of psects 
					** in this object file */
	char		pname[7];	/* program name */
	char		*ver_id;	/* version identification */
};


struct outword		/* structure for linking a word to be written in
			** the out file with its relocation bits */
{
	WORD	val;
	WORD	rbits;
};


struct g_sect		/* structure for global program section tree */
{
	char		name[7];	/* psect name */
	char		type;		/* M11 coded type */
	struct psect	*last_sect;	/* pointer to the last psect in the 
					** link-list of global same psects */
	struct g_sect	*leftt;		/* left child */
	struct g_sect	*rightt;	/* right child */
};


/***********************  macros  ****************************************/


	/* macros for requesting input from the different parts of the
	** object files. */

# define	HEADER		001
# define	CODE		017
# define	SYMBOLS 	022

	/* bit flags for attributes of psects and symbols */

# define	SHR		001
# define	INS		002
# define	BSS		004
# define	DEF		010
# define	OVR		020
# define	REL		040
# define	GBL		0100

# define	isabs(x)	(((x)->type & REL) == 0)
# define	isrel(x)	(((x)->type & REL) != 0)
# define	islcl(x)	(((x)->type & GBL) == 0)
# define	isgbl(x)	(((x)->type & GBL) != 0)
# define	isprv(x)	(((x)->type & SHR) == 0)
# define	isshr(x)	(((x)->type & SHR) != 0)
# define	isins(x)	(((x)->type & INS) != 0)
# define	isbss(x)	(((x)->type & BSS) != 0)
# define	isovr(x)	(((x)->type & OVR) != 0)
# define	iscon(x)	(((x)->type & OVR) == 0)
# define	isdef(x)	(((x)->type & DEF) != 0)
# define	isudf(x)	(((x)->type & DEF) == 0)

// Forward declarations for l11 module

// From in.c
WORD getword(); // To match definition WORD getword() in in.c
void dc_symbol(char *sname); // Defined in sup.c, used by in.c, pass2.c, obint.c
void ch_input(char *newfile, int newmod);
int morebytes();
int getbyte();
void inerror(char *mess);
int read_mod(void); // Added (void)
int getb();

// From link.c (many of these are called by main in link.c)
void scanargs(int argc, char *argv[]);
void pass1(void);
void relocate(void);
void relsyms(struct symbol *sym);
void post_bail(void);
void printmap(void);
void warmup(void); // Calls functions in pass2.c
void pass2(void);  // Calls functions in pass2.c
void loose_ends(void); // Calls functions in pass2.c
void outnames(void);
void strip(char *name, char *suffix); // Changed to void to match sup.c definition
void place_global(struct psect *ps);
void place_local(struct psect *ps);
void table(struct symbol **root, struct symbol *new);
void asgn_rcs(struct psect *p);
void get_rc(struct outword *wbuff, struct objfile *obj, char *psname); // Used in pass2.c from link.c context
void brag(char *s, long start, long size, char *tag);
void prattr(int x);
void dump_symlist(struct symbol *sym);
int dump_undefs(struct symbol *sym, int n);
void attr(int x, char *s, char *t);
void sigx(int n);
struct objfile *newfile(void);
struct psect *newpsect(void);
struct symbol *newsymbol(void);
struct g_sect *new_gsect(void);

// From sup.c (already listed some like dc_symbol, strip)
void lerror(char *mess);
void uerror(char *mess); // Used in link.c, pass2.c
void bail_out(void);
char *lalloc(int nbytes);
char *tack(char *s1, char *s2);

// From pass2.c (if any are called externally, e.g. by link.c's warmup/pass2/loose_ends)
// Most functions in pass2.c seem to be called internally or via the main pass2() call.
// Functions like Putw, werror, etc. if they were not static and used by other files would go here.
// For now, assuming they are effectively module-local to pass2.c or handled by its own internal declarations.

// Global variables defined in link.c (or elsewhere) and used across modules
extern int Nerrors;
extern FILE *Mapp;
extern char *Outname, *Mapname; // Symname, Bitname are char arrays defined in pass2.c
extern char Symname[]; // Defined as char Symname[20] in pass2.c
extern char Bitname[];// Defined as char Bitname[20] in pass2.c
extern FILE *Outp, *Symp, *Bitp;                   // For bail_out cleanup in sup.c

extern struct objfile	*File_root;
extern struct symbol	*Sym_root;
extern struct g_sect	*Gsect_root;
extern WORD	Maxabs;
extern WORD	R_counter;
extern long	Seekoff;
extern char	Do_410;
extern char	Do_411;
extern char	Do_map;
extern char	Do_lpr_map;
extern char	Do_bits;
extern char	Do_kludge;
extern char	Do_silent;
extern char	Do_table;
extern char	No_locals;
extern WORD	Transadd;
extern char	Erstring[80];
// extern char	*Outname; // Already covered by Outname, Mapname etc. line
extern WORD	Tex_size;
extern WORD	Dat_size;
extern WORD	Bss_size;
// extern FILE	*Outp; // Already covered
// extern char	*Mapname; // Already covered
// extern FILE	*Mapp; // Already covered
extern char	No_out;

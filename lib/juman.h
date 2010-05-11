/*
==============================================================================
	juman.h
		1990/12/06/Thu	Yutaka MYOKI(Nagao Lab., KUEE)
		1990/01/09/Wed	Last Modified
                                          >>> 94/02 changed by T.Nakamura <<< 
==============================================================================
*/

/*
------------------------------------------------------------------------------
	inclusion of header files
------------------------------------------------------------------------------
*/

#ifndef JUMAN_H_DEF
#define JUMAN_H_DEF

#include	<stdio.h>
#include	<ctype.h>

#ifdef HAVE_STRING_H
#include	<string.h>
#endif

#include	<stdarg.h>

#ifdef HAVE_LIMITS_H
#include	<limits.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include	<sys/types.h>
#endif

#ifdef HAVE_SYS_FILE_H
#include	<sys/file.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include	<sys/stat.h>
#endif

#include	<time.h>

/* added by T.Nakamura */
#ifdef HAVE_STDLIB_H
#include        <stdlib.h>
#endif

#ifdef HAVE_FCNTL_H
#include 	<fcntl.h>
#endif

#include	"juman_pat.h"

#ifndef JEXPORT
#if ( defined(_WIN32) && !defined(__GNUC__) && !defined(__CYGWIN__) )
#ifdef JUMAN_LIB
#define JEXPORT __declspec(dllexport)
#else
#define JEXPORT __declspec(dllimport)
#endif
#else
#define JEXPORT
#endif
#endif

/*
 * MS Windows の場合は SJIS入出力になるよう変更
 * Added by Taku Kudoh (taku@pine.kuee.kyoto-u.ac.jp)
 * Thu Oct 29 03:42:45 JST 1998
 */
#ifdef _WIN32
extern char *toStringEUC(char *str);
extern char *toStringSJIS(char *str);

#include        <stdarg.h>

#ifdef HAVE_WINDOWS_H
#include        <windows.h>

#if ! defined __CYGWIN__
typedef char *	caddr_t;
#endif
#endif

/* あとで 競合するらしいので undef (tricky?) */
#undef          TRUE  
#undef          FALSE
#else
#define my_fprintf fprintf
#endif

/* assert handling */
#define my_assert(ctx, exp) ( (exp) || (my_assert_(ctx, #exp, __FILE__, __LINE__), 0) )

#define EOf		0x0b	/* クライアントサーバ間通信でのEOF */

#define NDBM_KEY_MAX 	256
#define NDBM_CON_MAX 	1024

/*
------------------------------------------------------------------------------
	global definition of macros
------------------------------------------------------------------------------
*/

#define 	FALSE		((int)(0))
#define 	TRUE		(!(FALSE))
#define		BOOL		int

#define		U_CHAR		unsigned char
#define		U_INT		unsigned int

/* #define		SEEK_SET	0		* use in "fseek()"         */
/* #define		SEEK_CUR	1		* use in "fseek()"         */
/* #define		SEEK_END	2		* use in "fseek()"         */

#define		EOA		((char *)(-1))	/* end of arguments         */

#ifndef 	FILENAME_MAX			/* maximum of filename      */
#define		FILENAME_MAX	1024		
#endif
#ifndef		BUFSIZE				/* (char) size of buffer    */
#define 	BUFSIZE		1025
#endif

#define 	NAME_MAX_	1024		/* maximum of various names */
#define 	MIDASI_MAX	129		/* maximum length of MIDASI */
#define 	YOMI_MAX	129		/* maximum length of YOMI   */
#define 	IMI_MAX		1024		/* maximum length of IMI    */


#define		ENV_JUMANPATH	"JUMANPATH"

#define		GRAMMARFILE	"JUMAN.grammar"
#define 	CLASSIFY_NO	128

#define		KATUYOUFILE	"JUMAN.katuyou"
#define		TYPE_NO		128
#define		FORM_NO		128

#define		CONNECTFILE	"JUMAN.connect"
#define		KANKEIFILE	"JUMAN.kankei"

/* added by kuro */
#define		OPTIONFILE	"JUMAN.option"


#define 	S_POSTFIX	".dic"
				/* file postfix for s-expression dic-file */
#define 	I_POSTFIX	".int"
				/* file postfix for intermediate dic-file */
#define		I_FILE_ID	"this is INT-file for makehd.\n"

#define 	CONS		0
#define 	ATOM		1
#define 	NIL		((CELL *)(NULL))

#define 	COMMENTCHAR	';'
#define 	BPARENTHESIS	'('
#define 	BPARENTHESIS2	'<'
#define 	BPARENTHESIS3	'['
#define 	EPARENTHESIS	')'
#define 	EPARENTHESIS2	'>'
#define 	EPARENTHESIS3	']'
#define 	SCANATOM	"%[^(;) \n\t]"
#define 	NILSYMBOL	"NIL"

#define		CELLALLOCSTEP	1024
#define		BLOCKSIZE	16384

#define		Consp(x)	(!Null(x) && (_Tag(x) == CONS))
#define		Atomp(x)	(!Null(x) && (_Tag(x) == ATOM))
#define		_Tag(cell)	(((CELL *)(cell))->tag)
#define		_Car(cell)	(((CELL *)(cell))->value.cons.car)
#define		_Cdr(cell)	(((CELL *)(cell))->value.cons.cdr)
#define		Null(cell)	((cell) == NIL)
#define		new_cell()	(cons(NIL, NIL))
#define		Eq(x, y)	(x == y)
#define		_Atom(cell)	(((CELL *)(cell))->value.atom)


#define		EUC(c)		((c) | '\x80')
#define 	endchar(str)	(str[strlen(str)-1])

#define		M		30		/* order of B-tree         */
#define		MM		(M*2) 		/* order*2	           */

#define		PTRNIL		(-1L)		/* instead of NULL pointer */

#define		KANJI_CODE	128*128

#define 	BASIC_FORM 	"基本形"

#define		TABLEFILE	"jumandic.tab"
#define		MATRIXFILE	"jumandic.mat"
#define		DICFILE		"jumandic.dat"
#define		PATFILE		"jumandic.pat"

#define		calc_index(x)	((((int)(*(x)&0x7f))<<7)|((int)(*((x)+1))&0x7f))

enum		_ExitCode 	{ NormalExit, 
				       SystemError, OpenError, AllocateError, 
				       GramError, DicError, ConnError,  
				       ConfigError, ProgramError,
				       SyntaxError, UnknownId, OtherError };

/* added by T.Utsuro for weight of rensetu matrix */
#define         DEFAULT_C_WEIGHT  10

/* added by S.Kurohashi for mrph weight default values */
#define		MRPH_DEFAULT_WEIGHT	10

/* .jumanrc default values */

#define         CLASS_COST_DEFAULT   10            
#define         RENSETSU_WEIGHT_DEFAULT  100
#define         KEITAISO_WEIGHT_DEFAULT  1
#define         COST_WIDTH_DEFAULT      20
#define         UNDEF_WORD_DEFAULT  10000

#define         RENGO_ID   "999"    /* yamaji */
#define         RENGO_DEFAULT_WEIGHT 0.5

/* for juman_lib.c */
#define         BUFFER_BLOCK_SIZE       1000

#define         Op_B		     	0
#define         Op_M		     	1
#define         Op_P		     	2
#define         Op_BB		     	3
#define         Op_PP		     	4
#define         Op_F		     	0
#define         Op_E		     	1
#define         Op_C		     	2
#define         Op_EE		     	3
#define         Op_E2		     	4
#define         Op_EF		     	10
#define         Op_NODEBUG	     	0
#define         Op_DEBUG	     	1
#define         Op_DEBUG2	     	2

#define		MAX_PATHES		500
#define		MAX_PATHES_WK		5000

#define		KUUHAKU            	0x20
#define		HANKAKU            	0x80
#define		PRIOD            	0xa1a5
#define		CHOON            	0xa1bc
#define		KIGOU            	0xa3b0
#define		SUJI           	        0xa3c0
#define		ALPH            	0xa4a0
#define		HIRAGANA                0xa5a0
#define		KATAKANA                0xa6a0
#define		GR                      0xb0a0
#define		KANJI                   0xffff

/*
------------------------------------------------------------------------------
	global type definition of structures
------------------------------------------------------------------------------
*/

/* <car> 部と <cdr> 部へのポインタで表現されたセル */
typedef		struct		_BIN {
     void		*car;			/* address of <car> */
     void		*cdr;			/* address of <cdr> */
} BIN;

/* <BIN> または 文字列 を表現する完全な構造 */
typedef		struct		_CELL {
     int		tag;			/* tag of <cell> */
                                                /*   0: cons     */
                                                /*   1: atom     */
     union {
	  BIN		cons;
	  U_CHAR	*atom;
     } value;
} CELL;

/* "malloc" の回数を減少させるため，一定のメモリ領域を確保するテーブル */
typedef		struct		_CELLTABLE {
     void		*pre;
     void		*next;
     int		max;
     int		n;
     CELL		*cell;
} CELLTABLE;

/* changed by T.Nakamura and S.Kurohashi 
	構造体 MRPH がすべての情報を持ち， 
	構造体 MORPHEME はなくなった */
typedef         struct          _MRPH {
     U_CHAR             midasi[MIDASI_MAX];
     U_CHAR             midasi2[MIDASI_MAX];
     U_CHAR             yomi[YOMI_MAX];
     U_CHAR             imis[IMI_MAX];
     CELL		*imi;

     char               hinsi;
     char               bunrui;
     char               katuyou1;
     char               katuyou2;

     U_CHAR             weight;
     int                con_tbl;
     int                length;
} MRPH;

/* 形態品詞の分類・細分類 */
typedef		struct		_CLASS {
     U_CHAR	*id;
     int        cost;     /*品詞コスト by k.n*/
     int	kt;
} CLASS;

/* 活用型 */
typedef		struct		_TYPE {
     U_CHAR	*name;
} TYPE;

/* 活用形 */
typedef		struct		_FORM {
     U_CHAR	*name;
     U_CHAR	*gobi;
     U_CHAR	*gobi_yomi;	/* カ変動詞来 などの読みのため */
} FORM;

/* 辞書登録オプション */
typedef		struct		_DICOPT {
     int	toroku;
} DICOPT;

/* stat() ライブラリ関数で使用 */
typedef		struct stat	 STAT;

/* 連接表 */
typedef         struct          _RENSETU_PAIR {
     int   i_pos;
     int   j_pos;
     int   hinsi;
     int   bunrui;
     int   type;
     int   form;
     U_CHAR  *goi;
} RENSETU_PAIR;

typedef struct _process_buffer {
    int mrph_p;
    int start;
    int end;
    int score;
    int path[MAX_PATHES];     /* 前の PROCESS_BUFFER の情報 */
    int connect;	      /* FALSE なら接続禁止(連語の途中への割込禁止) */
} PROCESS_BUFFER;

typedef struct _chk_connect_wk {
  int pre_p;     /* PROCESS_BUFFER のインデックス */
  int score;     /* それまでのスコア */
} CHK_CONNECT_WK;

typedef struct _connect_cost {
    short p_no;     /* PROCESS_BUFFER のインデックス */
    short pos;
    int cost;     /* コスト */
    int dakuon_flag;
} CONNECT_COST;

/*
------------------------------------------------------------------------------
 additional type definition  written by K. Yanagi  >>>changed by T.Nakamura<<<
------------------------------------------------------------------------------
*/

typedef struct _DIC_FILES {
  int number;
  /*int now;*/
  /*FILE *dic[MAX_DIC_NUMBER];*/
  DIC_FILE_ENTRY* dic[MAX_DIC_NUMBER];
  pat_node tree_top[MAX_DIC_NUMBER];
} DIC_FILES;

typedef struct _cost_omomi  {
    int rensetsu;
    int keitaiso;
    int cost_haba;
  } COST_OMOMI;            /*k.n*/


/*
------------------------------------------------------------------------------
 additional structure definition for thread safe context  written by X. Roche
------------------------------------------------------------------------------
*/

typedef         struct          _JCONTEXT {
  /* juman_lib.c */
  /* Input */
  int String_max;                      /* suggested LENMAX: 50000 */
  U_CHAR *String;
  int NormalizedString_max;            /* >= String_max ; suggested LENMAX: 50000 */
  U_CHAR *NormalizedString;
  int error;
  char *error_string;
  int max_level;
  
  /* Output */
  char **OutputAV;
  int OutputAVnum;
  int OutputAVmax;
  void (*OutputAVfun)(struct _JCONTEXT*, const char*);
                                       /* optional callback
                                          for process_path_mrph() */
  char *path_mrph_buff;
  int path_mrph_max;
  int path_mrph_offs;
  int path_mrph_last;

  /* Pattern buffer */
  int pat_buffer_max;                   /* default: 50000 */
  char *pat_buffer;
  char *pat_search_buffer;              /* 汎用バッファ - size: 50000 */

  /* juman_lib.c local */
  int              connect_cache_max;    /* default CONNECT_MATRIX_MAX: 1000 */
  CONNECT_COST	  *connect_cache;
  CHK_CONNECT_WK  *chk_connect;          /* size: MAX_PATHES_WK */

  /* MRPH_BUFFER_MAX の制限を撤廃，動的にメモリ確保 */
  int		        mrph_buffer_max;
  MRPH *                m_buffer;
  int *                 m_check_buffer;
  int                   m_buffer_num;
  
  /* PROCESS_BUFFER_MAX の制限を撤廃，動的にメモリ確保 */
  int		        process_buffer_max;
  PROCESS_BUFFER        *p_buffer;
  int *                 path_buffer;
  int *		        match_pbuf;
  int                   p_buffer_num;

  U_CHAR	       *kigou;   /* 先頭の記号(@) - size: MIDASI_MAX */
  U_CHAR	       *midasi1; /* 活用  - size: MIDASI_MAX */
  U_CHAR	       *midasi2; /* 原形  - size: MIDASI_MAX */
  U_CHAR	       *yomi;    /* 活用の読み  - size: MIDASI_MAX */

  /* pat.c */

  int dicFile_now;
  
} JCONTEXT;

#define FMTOPT_MAX 256
typedef struct _JOPTIONS {
  int		Show_Opt1;
  int		Show_Opt2;
  char		Show_Opt_tag[MIDASI_MAX];
  int		Show_Opt_jumanrc;
  int		Show_Opt_debug;
  char		Show_Opt_fmt[FMTOPT_MAX];
  int		Vocalize_Opt;
  int		Repetition_Opt;
  int		Normalized_Opt;
  
  int		undef_hinsi;
  int		undef_kata_bunrui, undef_alph_bunrui, undef_etc_bunrui;
  int		undef_kata_con_tbl, undef_alph_con_tbl,
   		undef_etc_con_tbl;
  int		suusi_hinsi, suusi_bunrui;
  int		kakko_hinsi, kakko_bunrui1, kakko_bunrui2;
  int		kuuhaku_hinsi, kuuhaku_bunrui, kuuhaku_con_tbl;
  
  COST_OMOMI	cost_omomi;     /*k.n*/
} JOPTIONS;

/*
------------------------------------------------------------------------------
	prototype definition of functions
------------------------------------------------------------------------------
*/


/* iotool.c */
JEXPORT JCONTEXT *juman_create_context(void);
JEXPORT void juman_destroy_context(JCONTEXT *ctx);
JEXPORT void juman_clear_context(JCONTEXT *ctx);
JEXPORT void juman_seterrlog(void (*fun)(const char* format, ...));
JEXPORT size_t juman_context_size(JCONTEXT *ctx);
#ifdef _WIN32
JEXPORT size_t juman_context_size_internal(void);
JEXPORT size_t juman_context_size_internal_options(void);
#endif

JEXPORT FILE	*my_fopen(const char *filename, const char *mode);
JEXPORT FILE	*my_fopen_silent(const char *filename, const char *mode);
JEXPORT DIC_FILE_ENTRY *my_open_entry(const char *filename);
JEXPORT void my_close_entry(DIC_FILE_ENTRY *ent);
JEXPORT void my_set_default_path(const char *path);
JEXPORT void build_full_filename(const char *path, const char *filename,
                                 char *dest);
JEXPORT const char* my_get_default_path(void);
JEXPORT FILE	*pathfopen(const char *filename, const char *mode, 
		   const char *path, char *filename_path);
JEXPORT FILE	*my_pathfopen(const char *filename, const char *mode, 
		      const char *path, char *filename_path);
JEXPORT int	my_feof(FILE *fp);
JEXPORT void	append_postfix(char *filename, char *affix);
JEXPORT void	change_postfix(char *filename, char *affix1, char *affix2);
JEXPORT void	getpath(char *cur_path, char *juman_path);
JEXPORT void	*my_alloc(int n);
JEXPORT void	*my_realloc(void *ptr, int n);
JEXPORT char    *my_strdup(const char *s);
JEXPORT void    my_free(void *ptr);
JEXPORT void    set_allocators(void* (*fun_alloc)(int n),
                               void* (*fun_realloc)(void *ptr, int n),
                               char* (*fun_strdup)(const char *s),
                               void (*fun_free)(void *ptr));
JEXPORT void	my_exit(int exit_code);
JEXPORT void	error(int errno, char *msg, ...);
JEXPORT void    my_assert_(JCONTEXT *ctx, const char *exp, const char *file, int line);
JEXPORT char	lower(char c);
JEXPORT char	upper(char c);
JEXPORT int 	my_strlen(U_CHAR *s);
JEXPORT void	my_strcpy(U_CHAR *s1, U_CHAR *s2);
JEXPORT int	my_strcmp(U_CHAR *s1, U_CHAR *s2);
JEXPORT int 	compare_top_str(U_CHAR *s1, U_CHAR *s2);
JEXPORT int 	compare_top_str1(U_CHAR *s1, U_CHAR *s2);
JEXPORT int 	compare_top_str2(U_CHAR *s1, U_CHAR *s2);
JEXPORT int 	compare_end_str(U_CHAR *s1, U_CHAR *s2);

JEXPORT void	ls(FILE *fp, char *p, char *f);
JEXPORT void	print_current_time(FILE *fp);
JEXPORT void	print_execute_time(FILE *fp, int dt, float dp);

JEXPORT void (*juman_log_err)(const char* format, ...);

/* lisp.c */
JEXPORT int	s_feof(FILE *fp);
JEXPORT int	s_feof_comment(FILE *fp);
JEXPORT CELL	*make_cell(void);
/* CELLTABLE	*make_cell_table(CELLTABLE *pre, int size);*/
JEXPORT CELL	*tmp_atom(U_CHAR *atom);
JEXPORT CELL	*cons(void *car, void *cdr);
JEXPORT CELL	*car(CELL *cell);
JEXPORT CELL	*cdr(CELL *cell);
JEXPORT int	equal(void *x, void *y);
JEXPORT int	length(CELL *list);
JEXPORT int	ifnextchar(FILE *fp, int c);
JEXPORT int	comment(FILE *fp);
JEXPORT CELL	*s_read(FILE *fp);
JEXPORT CELL	*s_read_atom(FILE *fp);
JEXPORT CELL	*s_read_car(FILE *fp);
JEXPORT CELL	*s_read_cdr(FILE *fp);
JEXPORT CELL	*assoc(CELL *item, CELL *alist);
JEXPORT CELL	*s_print(FILE *fp, CELL *cell);
JEXPORT CELL	*_s_print_(FILE *fp, CELL *cell);
JEXPORT CELL	*_s_print_cdr(FILE *fp, CELL *cell);
JEXPORT void	*lisp_alloc(int n);
JEXPORT void 	lisp_alloc_push(void);
JEXPORT void 	lisp_alloc_pop(void);

/* grammar.c */
JEXPORT void	error_in_grammar(int n, int line_no);
JEXPORT void	initialize_class(void);
#define	print_class(fp) 	print_class_(fp, 0, 8, "*")
JEXPORT void 	print_class_(FILE *fp, int tab1, int tab2, char *flag);
JEXPORT void	read_class(FILE *fp);
JEXPORT void	grammar(FILE *fp_out);

/* katuyou.c */
static void	initialize_type_form(void);
JEXPORT void 	print_type_form(FILE *fp);
JEXPORT void	read_type_form(FILE *fp);
JEXPORT void	katuyou(FILE *fp);

/* connect.c */
JEXPORT void connect_table(FILE *fp_out);
JEXPORT void read_table(FILE *fp);
JEXPORT void check_edrtable(MRPH *mrph_p, CELL *x);
JEXPORT void check_table(MRPH *morpheme);
JEXPORT void check_table_for_rengo(MRPH *mrph_p);
JEXPORT int check_table_for_undef(int hinsi, int bunrui);
JEXPORT void connect_matrix(FILE *fp_out);
JEXPORT void read_matrix(FILE *fp);
JEXPORT int check_matrix(int postcon, int precon);
JEXPORT int check_matrix_left(int precon);
JEXPORT int check_matrix_right(int postcon);

/* zentohan.c */
JEXPORT unsigned char	*zentohan(unsigned char *str1);
JEXPORT unsigned char	*hantozen(unsigned char *str1);

/* for edr-dic */
JEXPORT void check_edrtable(MRPH *mrph_p, CELL *x);

#endif

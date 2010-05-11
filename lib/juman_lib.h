/*
==============================================================================
	juman_lib.h
==============================================================================
*/

#ifndef JUMAN_LIB_H_DEF
#define JUMAN_LIB_H_DEF

#include "juman.h"


/*
------------------------------------------------------------------------------
	GLOBAL:
	definition of global variables          >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

JEXPORT char		*ProgName;
/*JEXPORT FILE	       	*db;*/
JEXPORT CLASS		Class[CLASSIFY_NO + 1][CLASSIFY_NO + 1];
JEXPORT TYPE		Type[TYPE_NO];
JEXPORT FORM		Form[TYPE_NO][FORM_NO];
JEXPORT RENSETU_PAIR     *rensetu_tbl;
JEXPORT U_CHAR           *rensetu_mtr;

JEXPORT DIC_FILES        DicFile;

JEXPORT FILE		*Jumanrc_Fileptr;
JEXPORT int              LineNo;
JEXPORT int     	 LineNoForError;       /* k.n */
JEXPORT char             *Dic_DefaultDir;

/*
------------------------------------------------------------------------------
	LOCAL:
	definition of global variables          >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

JEXPORT JOPTIONS JOpt;

/*JEXPORT int		jiritsu_num;
  JEXPORT int		Jiritsu_buffer[CLASSIFY_NO + 1];*/
JEXPORT char		Jumangram_Dirname[FILENAME_MAX];  /*k.n*/

/*
------------------------------------------------------------------------------
	prototype definition of functions       >>> changed by T.Nakamura <<<
------------------------------------------------------------------------------
*/

JEXPORT BOOL	juman_init_rc(FILE *fp);
JEXPORT BOOL    juman_init_rc_file(const char *filename);
JEXPORT int	juman_close(void);
JEXPORT JOPTIONS *juman_get_options(void);
JEXPORT void	realloc_mrph_buffer(JCONTEXT* ctx);
JEXPORT void	realloc_process_buffer(JCONTEXT* ctx);
JEXPORT void     read_class_cost(CELL *cell); /* k.n */
JEXPORT int      search_all(JCONTEXT* ctx, int position);
JEXPORT int      take_data(JCONTEXT* ctx, int pos, char **pbuf, int dakuon_flag, int normal_flag);
JEXPORT char *	_take_data(JCONTEXT* ctx, char *s, MRPH *mrph, int dakuon_flag, int normal_flag);
JEXPORT int 	numeral_decode(char **str);
JEXPORT int 	numeral_decode2(char **str);
JEXPORT void 	hiragana_decode(char **str, char *yomi);
JEXPORT void    check_rc(void);
JEXPORT void     changeDictionary(JCONTEXT* ctx, int number);

JEXPORT int      trim_space(JCONTEXT* ctx, int pos);
JEXPORT int	undef_word(JCONTEXT* ctx, int pos);
JEXPORT int	check_code(U_CHAR *cp, int position);
JEXPORT void 	juman_init_etc(void);
JEXPORT int 	suusi_word(JCONTEXT* ctx, int pos , int m_num);
JEXPORT int	through_word(JCONTEXT* ctx, int pos , int m_num);
JEXPORT int 	is_through(MRPH *mrph_p);

/*
  結果の出力関数については、サーバーモード対応のため、出力先を引数として取る
  ように変更
  NACSIS 吉岡
*/   

JEXPORT void print_path_mrph(JCONTEXT* ctx, FILE* output, int path_num , int para_flag);
JEXPORT char **print_best_path(JCONTEXT* ctx, FILE* output);
JEXPORT char **print_all_mrph(JCONTEXT* ctx, FILE* output);
JEXPORT void _print_all_mrph(JCONTEXT* ctx, FILE* output, int path_num);
JEXPORT char **print_all_path(JCONTEXT* ctx, FILE* output);
JEXPORT void _print_all_path(JCONTEXT* ctx, FILE* output, int path_num, int pathes);
JEXPORT char **print_homograph_path(JCONTEXT* ctx, FILE* output);
JEXPORT int _print_homograph_path(JCONTEXT* ctx, FILE* output,
                                  const int level, int pbuf_start, int new_p);

JEXPORT int	pos_match_process(JCONTEXT* ctx, int pos, int p_start);
JEXPORT int	pos_right_process(JCONTEXT* ctx, int position);
JEXPORT int	check_connect(JCONTEXT* ctx, int pos_start, int m_num, int dakuon_flag);
JEXPORT int	juman_sent(JCONTEXT* ctx);

#endif

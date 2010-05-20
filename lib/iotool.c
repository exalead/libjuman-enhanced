/*
==============================================================================
	iotool.c
		1990/12/14/Fri	Yutaka MYOKI(Nagao Lab., KUEE)
		1990/12/25/Tue	Last Modified
==============================================================================
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MMAP
#undef USE_HASH
#include <sys/mman.h>

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#endif

#define JUMAN_LIB
#include	<juman.h>
#include	<juman_pat.h>
#define         DEF_GRAM_FILE           "文法ファイル"
/*
------------------------------------------------------------------------------
	GLOBAL:
	definition of global variable
------------------------------------------------------------------------------
*/

extern char	*ProgName;
FILE		*Jumanrc_Fileptr;
extern char            Jumangram_Dirname[FILENAME_MAX];  /*k.n*/
extern int             LineNoForError, LineNo;

int Cha_errno = 0;
FILE 		*Cha_stderr = NULL;

char             *Dic_DefaultDir = NULL;

/*
------------------------------------------------------------------------------
	PROCEDURE: <juman_create_context>, <juman_destroy_context>,
                   <juman_clear_context>
------------------------------------------------------------------------------
*/

JCONTEXT *juman_create_context() {
  JCONTEXT* ctx = (JCONTEXT*) my_alloc(sizeof(JCONTEXT));
  
  ctx->String_max = 0;
  ctx->String = NULL;
  ctx->NormalizedString_max = 0;
  ctx->NormalizedString = NULL;
  ctx->error = 0;
  ctx->error_string = NULL;
  ctx->max_level = 0;
  
  ctx->OutputAVmax = 0;
  ctx->OutputAV = NULL;
  ctx->OutputAVfun = NULL;
  
  ctx->pat_buffer_max = 0;
  ctx->pat_buffer = NULL;

  ctx->connect_cache_max = 0;
  ctx->connect_cache = NULL;
  
  ctx->mrph_buffer_max = 0;
  ctx->m_buffer = NULL;
  ctx->m_check_buffer = NULL;
  
  ctx->process_buffer_max = 0;
  ctx->p_buffer = NULL;
  ctx->path_buffer = NULL;
  ctx->match_pbuf = NULL;
  ctx->path_mrph_buff = NULL;
  ctx->path_mrph_max = 0;

  ctx->pat_search_buffer = NULL;
  ctx->chk_connect = NULL;

  ctx->kigou = NULL;
  ctx->midasi1 = NULL;
  ctx->midasi2 = NULL;
  ctx->yomi = NULL;
  
  juman_clear_context(ctx);
  return ctx;
}

void juman_destroy_context(JCONTEXT *ctx) {
  if (ctx != NULL) {
    if (ctx->String != NULL) {
      my_free(ctx->String);
      my_free(ctx->NormalizedString);
      ctx->String = NULL;
      ctx->NormalizedString = NULL;
      ctx->String_max = 0;
      ctx->NormalizedString_max = 0;
    }
    if (ctx->OutputAV != NULL) {
/*       int i; */
/*       for(i = 0 ; i < ctx->OutputAVnum ; i++) { */
/*         if ((ctx->OutputAV)[i] != NULL) { */
/*           my_free((ctx->OutputAV)[i]); */
/*           (ctx->OutputAV)[i] = NULL; */
/*         } */
/*       } */
      ctx->OutputAVmax = 0;
      my_free(ctx->OutputAV);
      ctx->OutputAV = NULL;
    }
    if (ctx->pat_buffer != NULL) {
      ctx->pat_buffer_max = 0;
      my_free(ctx->pat_buffer);
      ctx->pat_buffer = NULL;
    }
    if (ctx->connect_cache != NULL) {
      my_free(ctx->connect_cache);
      ctx->connect_cache = NULL;
      ctx->connect_cache_max = 0;
    }
    if (ctx->m_buffer != NULL) {
      ctx->mrph_buffer_max = 0;
      my_free(ctx->m_buffer);
      my_free(ctx->m_check_buffer);
      ctx->m_buffer = NULL;
      ctx->m_check_buffer = NULL;
    }
    if (ctx->p_buffer != NULL) {
      ctx->process_buffer_max = 0;
      my_free(ctx->p_buffer);
      my_free(ctx->path_buffer);
      my_free(ctx->match_pbuf);
      ctx->p_buffer = NULL;
      ctx->path_buffer = NULL;
      ctx->match_pbuf = NULL;
    }
    if (ctx->path_mrph_buff != NULL) {
      ctx->path_mrph_max = 0;
      my_free(ctx->path_mrph_buff);
      ctx->path_mrph_offs = 0;
      ctx->path_mrph_last = 0;
    }
    if (ctx->pat_search_buffer != NULL) {
      my_free(ctx->pat_search_buffer);
      ctx->pat_search_buffer = NULL;
    }
    if (ctx->chk_connect != NULL) {
      my_free(ctx->chk_connect);
      ctx->chk_connect = NULL;
    }
    if (ctx->kigou != NULL) {
      my_free(ctx->kigou);
      ctx->kigou = NULL;
    }
    if (ctx->midasi1 != NULL) {
      my_free(ctx->midasi1);
      ctx->midasi1 = NULL;
    }
    if (ctx->midasi2 != NULL) {
      my_free(ctx->midasi2);
      ctx->midasi2 = NULL;
    }
    if (ctx->yomi != NULL) {
      my_free(ctx->yomi);
      ctx->yomi = NULL;
    }
    
    my_free(ctx);
  }
}

/* get the context size of allocated buffers (estimation) */
size_t juman_context_size(JCONTEXT *ctx) {
#define ARR_SIZE(BUFF, SIZE) ( (SIZE)*sizeof(BUFF[0]) )
  return ARR_SIZE(ctx->String, ctx->String_max)
    + ARR_SIZE(ctx->NormalizedString, ctx->NormalizedString_max)
    + ARR_SIZE(ctx->OutputAV, ctx->OutputAVmax)
    + ARR_SIZE(ctx->pat_buffer, ctx->pat_buffer_max)
    + ARR_SIZE(ctx->connect_cache, ctx->connect_cache_max)
    + ARR_SIZE(ctx->m_buffer, ctx->mrph_buffer_max)
    + ARR_SIZE(ctx->m_check_buffer, ctx->mrph_buffer_max)
    + ARR_SIZE(ctx->p_buffer, ctx->process_buffer_max)
    + ARR_SIZE(ctx->path_buffer, ctx->process_buffer_max)
    + ARR_SIZE(ctx->match_pbuf, ctx->process_buffer_max)
    + ARR_SIZE(ctx->path_mrph_buff, ctx->path_mrph_max)
    ;
#undef ARR_SIZE
}

size_t juman_context_size_internal() {
  return sizeof(JCONTEXT);
}

size_t juman_context_size_internal_options() {
  return sizeof(JOPTIONS);
}

void juman_clear_context(JCONTEXT *ctx) {
  if (ctx->String != NULL)
    ctx->String[0] = '\0';
  if (ctx->NormalizedString != NULL)
    ctx->NormalizedString[0] = '\0';
/*   if (ctx->OutputAV != NULL && ctx->OutputAVnum > 0) { */
/*     int i; */
/*     for(i = 0 ; i < ctx->OutputAVnum ; i++) { */
/*       if ((ctx->OutputAV)[i] != NULL) { */
/*         my_free((ctx->OutputAV)[i]); */
/*         (ctx->OutputAV)[i] = NULL; */
/*       } */
/*     } */
/*   } */
  ctx->OutputAVnum = 0;
  ctx->error = 0;
  if (ctx->error_string != NULL) {
    my_free(ctx->error_string);
    ctx->error_string = NULL;
  }
  ctx->max_level = 0;
  if (ctx->pat_buffer != NULL)
    ctx->pat_buffer[0] = '\0';
  ctx->m_buffer_num = 0;
  ctx->p_buffer_num = 0;
  if (ctx->kigou != NULL)
    ctx->kigou[0] = '\0';
  if (ctx->midasi1 != NULL)
    ctx->midasi1[0] = '\0';
  if (ctx->midasi2 != NULL)
    ctx->midasi2[0] = '\0';
  if (ctx->yomi != NULL)
    ctx->yomi[0] = '\0';
  ctx->dicFile_now = 0;
  ctx->path_mrph_offs = 0;
  ctx->path_mrph_last = 0;
}

static void juman_log_err_default(const char* format, ...) {
  FILE* logFile = stderr;
  va_list args;
  
  va_start(args, format);
  vfprintf(logFile, format, args);
  fflush(logFile);
}

static void juman_log_err_null(const char* format, ...) {
}

void (*juman_log_err)(const char* format, ...) = juman_log_err_default;

void juman_seterrlog(void (*fun)(const char* format, ...)) {
  if (fun != NULL) {
    juman_log_err = fun;
  } else {
    juman_log_err = juman_log_err_null;
  }
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_fopen>: do "fopen"/<filename> and error processing
------------------------------------------------------------------------------
*/

static int is_abs(const char *path) {
#ifdef _WIN32
  return (
          ( path[0] == '\\' && path[1] == '\\' )
          || ( path[0] == '/' && path[1] == '/' )
          || ( isalpha(path[0]) && path[1] == ':' )
          );
#else
  return ( path[0] == '/' );
#endif
}

void build_full_filename(const char *path, const char *filename,
                         char *dest) {
  if (Dic_DefaultDir != NULL && path[0] != '\0' && !is_abs(path)) {
#ifdef _WIN32
    char sep = '\\';
#else
    char sep = '/';
#endif
    strcpy(dest, Dic_DefaultDir);
    if (path[0] == '.' && ( path[1] == '/' || path[1] == '\\' ) ) {
      sep = path[1];
      path += 2;
    }
    sprintf(dest, "%s%c%s%s", Dic_DefaultDir, sep, path, filename);
  } else {
    sprintf(dest, "%s%s", path, filename);
  }
}

FILE *my_fopen_silent(const char *filename, const char *mode)
{
  if (Dic_DefaultDir != NULL && !is_abs(filename)) {
    FILE	*fp;
    int psize = strlen(filename) + strlen(Dic_DefaultDir) + 1 + 1;
    char *fpath = my_alloc(psize);
    build_full_filename(filename, "", fpath);
    fp = fopen(fpath, mode);
    my_free(fpath);
    return fp;
  } else {
    return fopen(filename, mode);
  }
}

FILE *my_fopen(const char *filename, const char *mode)
{
     FILE	*fp;

     if ( ( fp = my_fopen_silent(filename, mode) ) == NULL)
	  error(OpenError, "can't open", filename, ".", EOA);

     return fp;
}

DIC_FILE_ENTRY *my_open_entry(const char *filename)
{
  DIC_FILE_ENTRY ent;
  FILE *fp = my_fopen_silent(filename, "rb");
  ent.data.ptr = NULL;
  if (fp != NULL) {
    struct stat st;
    int fd = fileno(fp);
    if (fstat(fd, &st) == 0) {
      ent.size = (size_t) st.st_size;
#ifndef HAVE_MMAP
      ent.data.fp = fp;
#else
      ent.data.mm = (char*) mmap(NULL, st.st_size,
                                 PROT_READ, MAP_PRIVATE, fd, 0);
#endif
    }
#ifdef HAVE_MMAP
    fclose(fp);
#endif
  }
  if (ent.data.ptr != NULL) {
    DIC_FILE_ENTRY *pent = (DIC_FILE_ENTRY*) my_alloc(sizeof(DIC_FILE_ENTRY));
    memcpy(pent, &ent, sizeof(DIC_FILE_ENTRY));
    return pent;
  }
  return NULL;
}

void my_close_entry(DIC_FILE_ENTRY *ent) {
  if (ent != NULL) {
    if (ent->data.ptr != NULL) {
#ifndef HAVE_MMAP
      fclose(ent->data.fp);
#else
      munmap(ent->data.mm, ent->size);
#endif
      ent->data.ptr = NULL;
    }
    my_free(ent);
  }
}

void my_set_default_path(const char *path)
{
  if (Dic_DefaultDir != NULL) {
    my_free(Dic_DefaultDir);
    Dic_DefaultDir = NULL;
  }
  if (path != NULL && path[0] != '\0') {
    int size;
    Dic_DefaultDir = my_strdup(path);
    size = strlen(Dic_DefaultDir);
    if (Dic_DefaultDir[size - 1] == '/'
        || Dic_DefaultDir[size - 1] == '\\') {
      Dic_DefaultDir[size - 1] = '\0';
    }
  }
}

const char* my_get_default_path()
{
  return Dic_DefaultDir;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<pathfopen>: do "fopen"/<filename_path> ( <path> + <filename> )
------------------------------------------------------------------------------
*/

FILE *pathfopen(const char *filename, const char *mode, const char *path,
                char *filename_path)
{
     FILE	*fp;

     strcpy(filename_path, path);
     strcat(filename_path, filename);

     return (fp = my_fopen_silent(filename_path, mode));
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_pathfopen>: do <pathfopen> and error processing
------------------------------------------------------------------------------
*/

FILE *my_pathfopen(const char *filename, const char *mode, const char *path,
                   char *filename_path)
{
     FILE	*fp;
     
     if ((fp = pathfopen(filename, mode, path, filename_path)) == NULL)
	  error(OpenError, "Can't open", filename_path, EOA);

     return fp;
}
	  
/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_feof>: if <fp> points to "EOF" return <TRUE> else return <FALSE>
------------------------------------------------------------------------------
*/

int my_feof(FILE *fp)
{
     int	c;

     if ((c = fgetc(fp)) == EOF) {
	  return TRUE;
     } else {
	  ungetc(c, fp);
	  return FALSE;
     }
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<append_postfix>: append <affix>  to <filename>
	<change_postfix>: change <affix1> of <filename> to <affix2>
------------------------------------------------------------------------------
*/

void append_postfix(char *filename, char *affix)
{
     if ((strcmp(&filename[strlen(filename) - strlen(affix)], affix)) &&
	 (endchar(filename) != '.'))
	  strcat(filename, affix);
}

void change_postfix(char *filename, char *affix1, char *affix2)
{
     if (!strcmp(&filename[strlen(filename) - strlen(affix1)], affix1))
	  filename[strlen(filename) - strlen(affix1)] = '\0';
     strcat(filename, affix2);
}

/*	
------------------------------------------------------------------------------
	PROCEDURE:
	<getpath>: get <cur_path> and <juman_path>
------------------------------------------------------------------------------
*/

void getpath(char *cur_path, char *juman_path)
{     
     char	*env, *getenv();

#ifdef _WIN32
     GetCurrentDirectory(FILENAME_MAX,cur_path);
#else
     getcwd(cur_path, FILENAME_MAX);
#endif
     strcpy(juman_path, Jumangram_Dirname);

#ifdef _WIN32
     if ((endchar(cur_path)) != '\\') strcat(cur_path, "\\");
     if ((endchar(juman_path)) != '\\') strcat(juman_path, "\\");
#else
     if ((endchar(cur_path)) != '/') strcat(cur_path, "/");
     if ((endchar(juman_path)) != '/') strcat(juman_path, "/");
#endif
}

static void* default_malloc(int n) {
  return malloc(n);  /* libc malloc() */
}
static void* default_realloc(void *ptr, int n) {
  return realloc(ptr, n);  /* libc realloc() */
}
static char* default_strdup(const char *s) {
  return strdup(s);  /* libc strdup() */
}
static void default_free(void *ptr) {
  free(ptr);  /* libc free() */
}

static void* (*f_malloc)(int n) = default_malloc;
static void* (*f_realloc)(void *ptr, int n) = default_realloc;
static char* (*f_strdup)(const char *s) = default_strdup;
static void (*f_free)(void *ptr) = default_free;

void set_allocators(void* (*fun_alloc)(int n),
                    void* (*fun_realloc)(void *ptr, int n),
                    char* (*fun_strdup)(const char *s),
                    void (*fun_free)(void *ptr)
                    ) {
  f_malloc = fun_alloc;
  f_realloc = fun_realloc;
  f_strdup = fun_strdup;
  f_free = fun_free;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_alloc>: do "malloc" (library function) and error processing
------------------------------------------------------------------------------
*/

void *my_alloc(int n)
{
     void *p;

     if ((p = (void *)f_malloc(n)) == NULL)
	  error(AllocateError, "Not enough memory. Can't allocate.", EOA);

     return p;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_realloc>: do "realloc" (library function) and error processing
------------------------------------------------------------------------------
*/

void *my_realloc(void *ptr, int n)
{
     void *p;

     if ((p = (void *)f_realloc(ptr, n)) == NULL)
	  error(AllocateError, "Not enough memory. Can't allocate.", EOA);

     return p;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_strdup>: do "strdup" (library function) and error processing
------------------------------------------------------------------------------
*/

char *my_strdup(const char *s)
{
     char *p;

     if ((p = f_strdup(s)) == NULL)
	  error(AllocateError, "Not enough memory. Can't allocate.", EOA);

     return p;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_free>: do "free" (library function) and error processing
------------------------------------------------------------------------------
*/

void my_free(void *ptr)
{
  f_free(ptr);
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<my_exit>: print error-number on "stderr", and "exit"
------------------------------------------------------------------------------
*/

void my_exit(int exit_code)
{
     juman_log_err( "exit(%d)\n", exit_code);
     exit(exit_code);
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<error>: print error-message(s) on "stderr", and "exit"
------------------------------------------------------------------------------
*/

void error(int errno, char *msg, ...)
{
     char *str;
     va_list ap;

     if (Cha_stderr == NULL)
	 Cha_stderr = stderr;

     fputs( (Cha_stderr != stderr)? "600 " : "\n", Cha_stderr );
     fprintf(Cha_stderr, "%s: %s ", ProgName, msg);
     va_start(ap, msg);

     while ((str = va_arg(ap, char *)) != EOA)
	 fprintf(Cha_stderr, "%s ", str);
     fputc('\n', Cha_stderr); 

     va_end(ap);

#ifndef JUMAN_NO_EXIT
     my_exit(errno);
#endif
}

void my_assert_(JCONTEXT *ctx, const char *exp, const char *file, int line)
{
  const char *fmt = "%s:%d: Assertion `%s' failed.";
  const size_t fmtsz = strlen(fmt) + strlen(file) + sizeof("4294967296") + strlen(exp) + 8;
  ctx->error = 1;
  if (ctx->error_string != NULL) {
    my_free(ctx->error_string);
    ctx->error_string = NULL;
  }
  ctx->error_string = my_alloc(fmtsz);
  sprintf(ctx->error_string, fmt, file, line, exp);
  juman_log_err( "* warning: %s\n", ctx->error_string);
}

/*
------------------------------------------------------------------------------
	PROCEDURE
	<warning>: print warning-message(s) on "stderr"
------------------------------------------------------------------------------
*/

void warning(int errno, char *msg, ...)
{
     char *str;
     va_list ap;

     juman_log_err( "\n%s: %s ", ProgName, msg);
     va_start(ap, msg);

     while ((str = va_arg(ap, char *)) != EOA)
	  juman_log_err( "%s ", str);
     fputc('\n', stderr); 

     va_end(ap);
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<lower>: if <char:c> is a large character, lower <c>
------------------------------------------------------------------------------
*/

char lower(char c)
{
     if (isupper(c))
	  return (char)tolower(c);
     else
	  return c;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<upper>: if <char:c> is a small character, upper <c>
------------------------------------------------------------------------------
*/

char upper(char c)
{
     if (islower(c))
	  return (char)toupper(c);
     else
	  return c;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_strlen>: return length of the string which is pointed to by <s>.
	             if <s> == NULL return 0
------------------------------------------------------------------------------
*/

int my_strlen(U_CHAR *s)
{
     int	n = 0;

     if (s != NULL) 
	  while (s[n])	n++;

     return n;
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<my_strcpy>:
------------------------------------------------------------------------------
*/

void my_strcpy(U_CHAR *s1, U_CHAR *s2)
{
     if (s2 == NULL) {
	  s1 = NULL; return;
     }

     strcpy(s1, s2);
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<my_strcmp>: 
------------------------------------------------------------------------------
*/

int my_strcmp(U_CHAR *s1, U_CHAR *s2)
{
     if (s1 == NULL && s2 == NULL) return 0;

     if (s1 == NULL) return -1;
     if (s2 == NULL) return  1;

     return strcmp(s1, s2);
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str>: if <s1> = <s2...> or <s2> = <s1...> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str(U_CHAR *s1, U_CHAR *s2)
{
     int	i = 0;

     while (s1[i] && s2[i]) {
	  if (s1[i] != s2[i]) return FALSE;
	  i++;
     }
     return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str1>: if <s1> = <s2...> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str1(U_CHAR *s1, U_CHAR *s2)
{
     int	l1, l2;

     l1 = strlen(s1);
     l2 = strlen(s2);

     if (l1 > l2) return FALSE;

     while (l1--)
	  if (s1[l1] != s2[l1]) return FALSE;

     return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_top_str2>: if <s1...> = <s2> return TRUE
------------------------------------------------------------------------------
*/

int compare_top_str2(U_CHAR *s1, U_CHAR *s2)
{
     int	l1, l2;

     l1 = strlen(s1);
     l2 = strlen(s2);

     if (l1 < l2) return FALSE;

     while (l2--)
	  if (s1[l2] != s2[l2]) return FALSE;

     return TRUE;
}

/*
------------------------------------------------------------------------------
	FUNCTION:
	<compare_end_str>: if <s1> = <...s2> or <s2> = <...s1> return TRUE
------------------------------------------------------------------------------
*/

int compare_end_str(U_CHAR *s1, U_CHAR *s2)
{
     int	l1, l2;

     l1 = strlen(s1);
     l2 = strlen(s2);

     if (l1 >= l2) {
	  if (strcmp(s1 + l1 - l2, s2) == 0)
	       return TRUE;
	  else
	       return FALSE;
     } else {
	  if (strcmp(s2 + l2 - l1, s1) == 0)
	       return TRUE;
	  else
	       return FALSE;
     }
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<ls>: print <char *:p> file-information on <FILE *:fp>
------------------------------------------------------------------------------
*/

void ls(FILE *fp, char *p, char *f)
{
     char	path[FILENAME_MAX];
     STAT	stbuf;
     
     strcpy(path, p);
     strcat(path, f);
     stat(path, &stbuf);

     fprintf(fp, "%8ld bytes: %s\n", stbuf.st_size, path);
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<print_current_time>: print current local-time on <FILE *:fp>
------------------------------------------------------------------------------
*/

#ifdef _WIN32
#define localtime_r(A, B) localtime(A)
#define asctime_r(A, B) asctime(A)
#endif

void print_current_time(FILE *fp)
{
     time_t	t;
     struct tm	tp_;
     struct tm	*tp;
     char buf[42];  /* at least 26 */

     time(&t);
     tp = localtime_r(&t, &tp_);
     buf[0] = '\0';
     fprintf(fp, asctime_r(tp, buf));
}

/*
------------------------------------------------------------------------------
	PROCEDURE:
	<print_execute_time>: print two kinds of execution time on <FILE *:fp>
------------------------------------------------------------------------------
*/

void print_execute_time(FILE *fp, int dt, float dp)
{
     dp = dp/1000000.0;
     fprintf(fp, "execution time: %8.3fs\n", (float)dt);
     fprintf(fp, "processor time: %8.3fs\n", dp);
}

/*
------------------------------------------------------------------------------
        PROCEDURE:
        <set_jumanrc_fileptr>: set Jumanrc_Fileptr

	WIN32 用に juman.ini を見に行くように変更
	RC_DEFAULTがない場合にexitするかどうかをflagで制御するように変更  (2002/11/08)
------------------------------------------------------------------------------
*/
void set_jumanrc_fileptr(char *option_rcfile, int look_rcdefault_p, int exit_rc_notfound_p)
{
    /*
      rcfileをさがす順

      <makeint, makemat>
      	$HOME/.jumanrc
	→ rcfileがなくてもよい

      <juman server, standalone> 
       	-r オプション
	$HOME/.jumanrc                _WIN32 の場合は探す必要はない (Changed by Taku Kudoh)
        c:\(winnt|windows)\juman.ini  _WIN32 の場合juman.ini を探す (Changed by Taku Kudoh)
	RC_DEFAULT (Makefile)         
	→ rcfileがなければエラー

      <juman client>
       	-r オプション
	$HOME/.jumanrc
	→ rcfileがなくてもよい
    */

    char *user_home_ptr, *getenv(), filename[FILENAME_MAX];

    if (option_rcfile) {
	if ((Jumanrc_Fileptr = fopen(option_rcfile, "r")) == NULL) {
	    juman_log_err( "not found <%s>.\n", option_rcfile);
	    exit(0);
	}
    } else {
#ifdef _WIN32
	GetPrivateProfileString("juman","dicfile","C:\\juman\\dic",filename,sizeof(filename),"juman.ini");
	if ((endchar(filename)) != '\\') strcat(filename, "\\");
	strcat(filename,"jumanrc");
#else
	if((user_home_ptr = (char *)getenv("HOME")) == NULL)
	    /* error(ConfigError, "please set <environment variable> HOME.", EOA); */
	    filename[0] = '\0';
	else
	    sprintf(filename, "%s/.jumanrc" , user_home_ptr);
#endif
	if (filename[0] == '\0' || (Jumanrc_Fileptr = fopen(filename, "r")) == NULL) {
	    if (look_rcdefault_p) {
#ifdef RC_DEFAULT
		if ((Jumanrc_Fileptr = fopen(RC_DEFAULT ,"r")) == NULL) {
		    if (exit_rc_notfound_p) {
			juman_log_err( 
				"not found <.jumanrc> and <RC_DEFAULT> file.\n");
			exit(0);
		    }
		    else {
			Jumanrc_Fileptr = NULL;
		    }
		}
#else
		juman_log_err(
			"not found <.jumanrc> file in your home directory.\n");
		exit(0);
#endif          
	    } else {
		Jumanrc_Fileptr = NULL;
	    }
	}
     }
}

/*
------------------------------------------------------------------------------
        PROCEDURE:
        <set_jumangram_dirname>: read Jumanrc_File 
------------------------------------------------------------------------------
*/

void set_jumangram_dirname()
{
    CELL *cell1,*cell2;
    Jumangram_Dirname[0]='\0';

#ifdef  _WIN32
    /* MS Windows のばあいは,juman.ini を見に行くように変更 
     dicfile == gramfile */
    GetPrivateProfileString("juman","dicfile","",Jumangram_Dirname,sizeof(Jumangram_Dirname),"juman.ini");
    if (Jumangram_Dirname[0]) {
	return;
	/* juman.iniが利用できなければ、jumanrcから読む */
    }
#endif

    LineNo = 0 ;

    while (!s_feof(Jumanrc_Fileptr))  {
	LineNoForError = LineNo ;
	cell1 = s_read(Jumanrc_Fileptr);

	if (!strcmp(DEF_GRAM_FILE, _Atom(car(cell1)))) { 
	    if (!Atomp(cell2 = car(cdr(cell1)))) {
		juman_log_err( "error in .jumanrc");
		exit(0);
	    } else 
		strcpy(Jumangram_Dirname , _Atom(cell2));
	}
    }
    /* fclose(Jumanrc_Fileptr); */
}
      
/*
==============================================================================
		Oct. 1996       A.Kitauchi <akira-k@is.aist-nara.ac.jp>
==============================================================================
*/

#define CHA_FILENAME_MAX 1024

static char progpath[CHA_FILENAME_MAX] = "juman";
static char filepath[CHA_FILENAME_MAX];
static char grammar_dir[CHA_FILENAME_MAX];
static char chasenrc_path[CHA_FILENAME_MAX];

/*
------------------------------------------------------------------------------
        PROCEDURE
        <cha_exit>: print error messages on stderr and exit
------------------------------------------------------------------------------
*/

void cha_exit(status, format, a, b, c, d, e, f, g, h)
    int status;
    char *format, *a, *b, *c, *d, *e, *f, *g, *h;
{
    if (Cha_errno)
      return;

    if (Cha_stderr != stderr)
      fputs("500 ", Cha_stderr);

    if (progpath)
      fprintf(Cha_stderr, "%s: ", progpath);
    fprintf(Cha_stderr, format, a, b, c, d, e, f, g, h);

    if (status >= 0) {
	fputc('\n', Cha_stderr);
	if (Cha_stderr == stderr)
	  exit(status);
	Cha_errno = 1;
    }
}

void cha_exit_file(status, format, a, b, c, d, e, f, g, h)
    int status;
    char *format, *a, *b, *c, *d, *e, *f, *g, *h;
{
    if (Cha_errno)
      return;

    if (Cha_stderr != stderr)
      fputs("500 ", Cha_stderr);

    if (progpath)
      fprintf(Cha_stderr, "%s: ", progpath);

    if (LineNo == 0)
      ; /* do nothing */
    else if (LineNo == LineNoForError)
      fprintf(Cha_stderr, "%s:%d: ", filepath, LineNo);
    else
      fprintf(Cha_stderr, "%s:%d-%d: ", filepath, LineNoForError, LineNo);

    fprintf(Cha_stderr, format, a, b, c, d, e, f, g, h);

    if (status >= 0) {
	fputc('\n', Cha_stderr); 
	if (Cha_stderr == stderr)
	  exit(status);
	Cha_errno = 1;
    }
}

void cha_perror(s)
    char *s;
{
    cha_exit(-1, "");
    perror(s);
}

void cha_exit_perror(s)
    char *s;
{
    cha_perror(s);
    exit(1);
}


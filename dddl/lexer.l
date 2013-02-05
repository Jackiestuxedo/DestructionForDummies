%c

%{
#include <stdio.h>
#include <string.h>
#include "builtin.h"
#include "parser.h"
#include "strings.h"

#define LEXDEBUG 0

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif    

static int find_id(char *string);
static void process_lineno(void);

/*
"<<"[a-zA-Z_"(), ]*">>"	return T_BLIZZ_API;
*/
%}

%%

"/*"([^*]*|("*"+[^*/]))*"*"+"/"	{
    // no longer really needed, as C Preprocessor is run before us
    char *c;

    for (c = yytext ; *c != '\0' ; c++) {
	if (*c == '\n')
	    lineno++;
    }
}
"//".*			;

^#.*\n			process_lineno();

"&&"			return T_AND;
"||"			return T_OR;
"<="			return T_LEQ;
">="			return T_GEQ;
"!="			return T_NEQ;
"=="			return T_EQ;

[0-9]+ 			{ yylval.intg = atoi(yytext); return T_INTEGER; }
[0-9]*"."[0-9]+		{ yylval.fl = atof(yytext); return T_FLOAT; }

\"([^"]|[^"])*\"	{ return isSpell(yytext) ? T_SPELL : T_STRING; }

[a-zA-Z_.]+		return find_id(yytext);

[ \t]			;
\n			lineno++;
.			{
			    return *yytext;

			    /*
 			     * put in a reference to find_rule so the
			     * so the compiler shuts up
			     */
			}


%%

struct kw {
    struct kw *next;
    char *word;
    int token;
    int caseSensitive;
    YYSTYPE yylval;
} *first_kw = NULL;

static void
keyword_add (char *word, int token, YYSTYPE lval, int caseSens)
{
    struct kw *new;

    new = amalloc(sizeof(*new));
    new->word = word;
    new->token = token;
    new->yylval = lval;
    new->caseSensitive = caseSens;
    new->next = first_kw;
    first_kw = new;
}

void
lex_init (FILE *fp)
{
    static struct {
        char *word;
	int token;
	enum built_in val;
    } init_keywords[] = {
	{ "while",	T_WHILE },
	{ "takedamage",	T_TAKE_DAMAGE },
	{ "takeheal",	T_TAKE_HEAL },
	{ "absval",	T_ABSVAL },
	{ "alternate",	T_ALTERNATE },
	{ "build", 	T_BUILD },
	{ "call",	T_CALL },
	{ "case", 	T_CASE },
	{ "class", 	T_CLASS },
	{ "clip", 	T_CLIP },
	{ "default",	T_DEFAULT },
	{ "else", 	T_ELSE },
	{ "elseif", 	T_ELSEIF },
	{ "extern", 	T_EXTERN },
	{ "fail", 	T_FAIL },
	{ "false", 	T_FALSE },
	{ "free",	T_FREE },
	{ "function",	T_FUNCTION },
	{ "if", 	T_IF },
	{ "incombat", 	T_COMBAT },
	{ "mutex",	T_MUTEX },
	{ "nil", 	T_NIL },
	{ "outofcombat",T_OUTOFCOMBAT },
	{ "postspell",	T_POSTSPELL },
	{ "precombat", 	T_PRECOMBAT },
	{ "print", 	T_PRINT },
	{ "printf", 	T_PRINT },
	{ "spellsent",	T_SPELLSENT },
	{ "strict", 	T_STRICT },
	{ "switch",	T_SWITCH },
	{ "true", 	T_TRUE },
	{ "var",	T_VAR },
 
	// built-in functions
	{ "runecount",		T_BUILTIN_4,	BI_RUNE_COUNT },
	{ "totemtype",		T_BUILTIN_3,	BI_TOTEMTYPE },
	{ "floor",		T_BUILTIN_3,	BI_FLOOR },
	{ "haverunes",		T_BUILTIN_5,	BI_HAVE_RUNES },
	{ "cooldown",		T_BUILTIN_2,	BI_COOLDOWN },
	{ "talent",		T_BUILTIN_2M,	BI_TALENT },
	{ "boss",		T_BUILTIN_1,	BI_BOSS },
	{ "uptime",		T_BUILTIN_1,	BI_UPTIME },
	{ "ontank",		T_BUILTIN_1,	BI_ONTANK },
	{ "tapped",		T_BUILTIN_1,	BI_TAPPED },
	{ "distance",		T_BUILTIN_1,	BI_DISTANCE },
	{ "dualwielding",	T_BUILTIN_1,	BI_DUAL_WIELDING },
	{ "buff",		T_BUILTIN_2,	BI_BUFF },
	{ "buffbyme",		T_BUILTIN_2,	BI_BUFF_BY_ME },
	{ "playerdebuff",	T_BUILTIN_2,	BI_PLAYER_DEBUFF },
	{ "playerdebuffinfo",	T_BUILTIN_2,	BI_PLAYER_DEBUFF_INFO },
	{ "debuffbymeinfo",	T_BUILTIN_2,	BI_DEBUFF_BY_ME_INFO },
	{ "buffInfo",		T_BUILTIN_2,	BI_BUFF_INFO },
	{ "petBuffInfo",	T_BUILTIN_2,	BI_PETBUFF_INFO },
	{ "buffTime",		T_BUILTIN_2,	BI_BUFF_TIME },
	{ "debuffTime",		T_BUILTIN_2,	BI_DEBUFF_TIME },
	{ "castable",		T_BUILTIN_2, 	BI_CASTABLE },
	{ "combopoints",	T_BUILTIN_1,	BI_COMBOPOINTS },
	{ "creaturetype", 	T_BUILTIN_2M,	BI_CREATURETYPE },
	{ "debuff",		T_BUILTIN_2,	BI_DEBUFF_BY_ANY },
	{ "debuffByMe",		T_BUILTIN_2,	BI_DEBUFF_BY_ME },
	{ "debuffInfo",		T_BUILTIN_2,	BI_DEBUFF_INFO },
	{ "requiredpower",	T_BUILTIN_2,	BI_REQUIREDPOWER },
	{ "ferralmode",		T_BUILTIN_1,	BI_FERRALMODE },
	{ "flaggedPVP",		T_BUILTIN_1,	BI_FLAGGEDPVP },
	{ "flighttime",		T_BUILTIN_1,	BI_FLIGHTTIME },
	{ "gcd",		T_BUILTIN_2,	BI_GCD },
	{ "hasglyph",		T_BUILTIN_2M, 	BI_HASGLYPH },
	{ "haspet",		T_BUILTIN_1, 	BI_HASPET },
	{ "holypower",		T_BUILTIN_1,	BI_HOLY_POWER },
	{ "lunarpower",		T_BUILTIN_1,	BI_LUNAR_POWER },
	{ "known",		T_BUILTIN_2,	BI_KNOWSSPELL },
	{ "lastcast",		T_BUILTIN_2,	BI_LASTCAST },
	{ "mounted",		T_BUILTIN_1, 	BI_MOUNTED },
	{ "mxbuff",		T_BUILTIN_MX,	BI_MXBUFF },
	{ "mxbuffbyme",		T_BUILTIN_MX,	BI_MXBUFF_BY_ME },
	{ "mxdebuff",		T_BUILTIN_MX,	BI_MXDEBUFF },
	{ "mxdebuffbyme",	T_BUILTIN_MX,	BI_MXDEBUFF_BY_ME },
	{ "mxneed",		T_BUILTIN_MX2,	BI_MXNEED },
	{ "happypet",		T_BUILTIN_1,	BI_PETHAPPINESS },
	{ "petbuff", 		T_BUILTIN_2,	BI_PETBUFF },
	{ "petexists",		T_BUILTIN_1, 	BI_PETEXISTS },
	{ "pethealth", 		T_BUILTIN_1,	BI_PETHEALTH },
	{ "power", 		T_BUILTIN_1,	BI_POWER },
	{ "rawpower", 		T_BUILTIN_1,	BI_RAWPOWER },
	{ "inraid", 		T_BUILTIN_1,	BI_IN_RAID },
	{ "inparty", 		T_BUILTIN_1,	BI_IN_PARTY },
	{ "solo", 		T_BUILTIN_1,	BI_SOLO },
	{ "soulshardcount",	T_BUILTIN_1,	BI_SOULSHARDCOUNT },
	{ "stance", 		T_BUILTIN_3,	BI_STANCE },
	{ "threat",		T_BUILTIN_1,	BI_THREAT },
	{ "totem",		T_BUILTIN_2,	BI_TOTEM },
	{ "rangedcombat",	T_BUILTIN_2M, 	BI_RANGED },
	{ "weaponenhancement",	T_BUILTIN_2,	BI_WEAPONENHANCE },
    	{ "playerhealth",	T_BUILTIN_1,	BI_PLAYERHEALTH, },
    	{ "rawplayerhealth",	T_BUILTIN_1,	BI_RAWPLAYERHEALTH, },
    	{ "rawplayermaxhealth",	T_BUILTIN_1,	BI_RAWPLAYERMAXHEALTH, },
    	{ "targethealth",	T_BUILTIN_1,	BI_TARGETHEALTH, },
    	{ "timetodie",		T_BUILTIN_1,	BI_TIMETODIE, },
    };
    int i;

    yyin = fp;
    for (i = 0 ; i < sizeof(init_keywords) / sizeof(*init_keywords) ; i++) {
	YYSTYPE lval;

	lval.bi = init_keywords[i].val;
	keyword_add(init_keywords[i].word, init_keywords[i].token, lval, FALSE);
    }
}

void
lex_register_variable (char *name, int token)
{
    YYSTYPE lval;

    lval.str = astrdup(name);
    keyword_add(lval.str, token, lval, TRUE);
}

void
lex_register_mutex (char *name, int mindex)
{
    YYSTYPE lval;

    lval.intg = mindex;
    keyword_add(astrdup(name), T_MUTEXVAR, lval, TRUE);
}

static int
find_id (char *string)
{
    struct kw *cur;

    // linear search: efficient enough for these few entries
    for (cur = first_kw ; cur != NULL ; cur = cur->next) {
	if (!(cur->caseSensitive ? strcmp : strcasecmp)(string, cur->word)) {
	   yylval = cur->yylval;
#if LEXDEBUG
	   fflush(stdout);
	   fprintf(stderr, "<find_id(%s) returns %d>\n", string, cur->token);
#endif	   
	   return cur->token;
	}
    }
#if LEXDEBUG
    fflush(stdout);
    fprintf(stderr, "<find_id(%s) returns T_ID>\n", string);
#endif	   
    return T_ID;
}

// make the compiler shut up about unused references by referencing them all
void
unused_func (void)
{
    //void *vs = NULL;
    //yy_size_t s = 0;
    char c = 2;
    
    //yy_flex_realloc(vs, s);
    input();
    yyunput(c, &c);
}

// we have a # line -- pull out the new filename and line number
static void
process_lineno (void)
{
    static char buf[1024];

    sscanf(yytext, "# %d \"%s\"", &lineno, buf);
}


/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser.y"

/*
 *  Destruction for Dummies Definition Language
 *
 *  Language for defining a class's actions for Destruction for Dummies.
 *
 *  This takes as input a .dddl file, and produces a .lua file as output
 *
 *  Format:
 *
 *  ---------------
 *
 *  Class: <class>;
 *  Build: <talent build>;
 *
 *  <variables>
 *
 *  InCombat [ (resource : spell) ]
 *		// the resource: spell is the resource to watch for running
 *		// out of, and the spell to cast if you do (like Life Tap for
 *		// mana, or Tiger's Fury for energy)
 *
 *	clip {			// define the clip times
 *	    spell: <num>;	// time in seconds
 *		...
 *	};
 *
 *	strict {
 *	    // strict order rotation.  These are things that will cause an
 *	    // absolute selection, irrespective of cooldown, mana, etc.  For
 *	    // example, kitty form for a druid.  Should only be used if other
 *	    // choices depend on it.
 *
 *	    <strict rotation>
 *	};
 *
 *	alternative {
 *	    // this for the alternative recommendation
 *	};
 *
 *      <soonest rotation>
 *  };
 *
 *  OutOfCombat [ (resource : spell) ] {
 *	<soonest rotation>
 *  };
 *
 *  The rotation can make use of the following functions:
 *	Buff("buff")		-- true if player has the buff
 *	BuffInfo("buff")	-- return time left and stack count
 *	PetBuff("buff")		-- true if your pet has the buff
 *	PlayerDebuff("debuff")	-- true if the player has the debuff
 *	Debuff("debuff")	-- true if target has debuff up
 *	DebuffByMe("debuff")	-- true if target has the debuff from player
 *	DebuffInfo("debuff")	-- return time left and stack count
 *	DebuffByMeInfo("debuff")-- return time left and stack count
 *	BuffTime("buff")	-- time left on buff (0 if perm, nil if down)
 *	DebuffTime("buff")	-- time left on debuff (0 if perm, nil if down)
 *	Known("spell")		-- True if the user knows the spell
 *	LastCast("spell")	-- the passed spell was the last one we cast
 *	GCD("spell")		-- GCD triggered by this spell
 *	Cooldown("spell")	-- Remaining cooldown time on this spell
 *
 *	Solo()			-- true if the player is solo questing (not
 *				   in a party or raid)
 *	InParty()		-- true if in a party (but not a raid)
 *	InRaid()		-- true if in a raid
 *
 *	Stance(exp)		-- true if we are in the given stance
 *				   Stance numbers are the same as in the
 *				   GetShapeshiftFormInfo() API from Bliz.
 *
 *	DualWielding()		-- true if player has two weapons equipped
 *
 *	Totem("spell")		-- Amount of time that totem has remaining,
 *				   or nil if not down
 *
 *	TotemType(number)	-- Return info about the passed in totem
 *		-- totem numbers: Fire = 1, Earth = 2, Water = 3, Air = 4
 *		-- returns haveTotem, totemName, remainingTime
 *
 *	(Note: the following two routines differ in the way they treat death
 *	runes.  RuneCount returns the number of runes of that type that are
 *	fully cooled (so a blood rune that is now a death rune only counts for
 *	tests on blood runes).  This is primarily useful for tests to see
 *	whether both of a rune are up (which will cause us to want a cast that
 *	will start one cooling).  HaveRunes will treat death runes as wild
 *	cards, and is mainly useful for tests to see whether you can cast
 *	a spell.  As of WoW 4.0.6 there is a weird inconsistency in clients;
 *	my client will show a spell on cooldown while its runes are cooling,
 *	at least one of my users' does not.)
 *	RuneCount(runetype)	-- returns the number of that type of rune
 *	HaveRunes(b,f,u)	-- returns true if we have at least the
 *				-- specified number of blood, frost, and death
 *				-- runes
 *
 *      (Note: the following two functions differ in that HasPet() returns
 *	true if the pet is available.  PetExists() returns true if the pet
 *	is available or will become available when you dismount or get out
 *	of the vehicle.  Except this doesn't seem exactly true.  Sigh)
 *	HasPet()		-- True if a pet is out
 *	PetExists()		-- True if a pet has been called
 *
 *	PetHealth()		-- Pet's health as a percent of max health
 *	TargetHealth()		-- Target's health as a percent of max health
 *	PlayerHealth()		-- Player's health as a percent of max health
 *	RawPlayerHealth()	-- Players current and max health
 *	Power()			-- Player's current power, as a percentage
 *	RawPower()		-- Player's power at end of cur cast, raw number
 *
 *	RequiredPower("spell")	-- Return amount of power required to cast spell
 *
 *	Distance()		-- Yards to target (best guess)
 *	RangedCombat("ranged spell")
 *				-- True if we are at ranged or greater combat
 *				   distance.  You must give a ranged spell 
 *				   that we can test on.  Best to give one with
 *				   no mana requirements
 *
 *	FlaggedPVP()		-- True if you are currently flagged for PVP
 *	HasGlyph("glyph")	-- True if they have the glyph
 *
 *	Threat()		-- UnitDetailedThreatStatus(), with nils
 *				   changed to 0's
 *
 *	CreatureType("type")	-- true if the target is the passed type
 *	Boss()			-- true if the target is a boss (elite,
 *				   three or more levels above you)
 *	TimeToDie()		-- Est. time until target dies (in seconds)
 *
 *
 *	FerralMode()		-- true if player does not want help with
 *				   regular rotation spells
 *
 *	Talent()		-- returns number of points in that talent
 *
 *	Tapped()		-- returns true if the target is tapped by
 *				   another player
 *
 *	Uptime()		-- current system uptime in seconds
 *
 *	OnTank()		-- true if the current mob is targeting a tank
 *
 *  There are three functions that take Mutex'es.  They just look to see if
 *  one of the things are there--no clipping is considered.  If the buff/debuff
 *  is up, it returns true:
 *	MxBuff(mutex_number)
 *	MxBuffByMe(mutex_number)
 *	MxDebuff(mutex_number)
 *	MxDebuffByMe(mutex_number)
 *
 *
 *  The builtin funcs come in the following flavors:
 *	BUILTIN_1	-- takes no parameters
 *
 *	BUILTIN_2	-- takes a spell as a parameter.  if none is given,
 *			   we will insert the current spell name
 *
 *	BUILTIN_2M	-- Takes a spell as a parameter.  Must be given
 *
 *	BUILTIN_3	-- Takes a number as a parameter
 *
 *	BUILTIN_4	-- Takes a rune type as a parameter
 *
 *	BUILTIN_5	-- Takes 3 numbers as parameters
 *
 *	BUILTIN_MX	-- Takes a mutex
 *
 *	BUILTIN_MX2	-- Takes a mutex and a spell
 *
 *
 *  Program accepts C and C++ style comments
 *
 *  Spells *must* be entered inside double quotes
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "builtin.h"
#include "parser.h"
#include "strings.h"

#define YYDEBUG 1

#define MAX_SWITCH_COUNT	1024	// max # of switches in dddl file
#define BIG_NUMBER		 512	// size of arrays that need to be big

#define NELEM(n)	(sizeof(n) / sizeof(*(n)))    

#define	CMD_CPP		"/lib/cpp"

char *classname, *buildname;
static char *curSpell = NULL;
static char *curvar;
static int curvar_array;

static enum { RM_NORMAL, RM_STRICT, RM_ALTERNATE } rotMode = RM_NORMAL;
static int switchcount = 0, cur_switchnum;
static short seen_case[MAX_SWITCH_COUNT];
static short seen_a_case = 0;

extern char *yytext;
extern int yylex();
extern int yyerror(char *); 

static char *spell2clip(char *);
static char *proc_builtin1(enum built_in);
static char *proc_builtin2(enum built_in, char *);
static char *proc_builtin3(enum built_in, char *);
static char *proc_builtin4(enum built_in, char *);
static char *proc_builtin5(enum built_in, char *, char *, char *);
static char *proc_builtin_mx(enum built_in, int);
static char *proc_builtin_mx2(enum built_in, int, char *);
static void have_clip(char *, char *); 
static char *find_clip(char *);
static void indent(int);
static char *normalize(char *);
static char *upstring(char *);

static struct cliplist {
    struct cliplist *next;
    char *spell;
    char *clip;
} *firstclip = NULL;

int lineno; 
char *filename = NULL;

static int indent_level = 0; 
static int mIndex = 0;		// which mutex number we are on
static int cur_mutexno = 0;	// which entry in our mutex we are on
static int utilOK = 0;
static char *free_spell = NULL, *fail_spell = NULL;
static int altnum = 0;
static int debugging = 0;


/* Line 189 of yacc.c  */
#line 318 "parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_CLASS = 258,
     T_BUILD = 259,
     T_COMBAT = 260,
     T_OUTOFCOMBAT = 261,
     T_CLIP = 262,
     T_STRICT = 263,
     T_FLOAT = 264,
     T_INTEGER = 265,
     T_PRECOMBAT = 266,
     T_POSTSPELL = 267,
     T_SPELLSENT = 268,
     T_NIL = 269,
     T_ID = 270,
     T_SPELL = 271,
     T_ABSVAL = 272,
     T_STRING = 273,
     T_TAKE_DAMAGE = 274,
     T_TAKE_HEAL = 275,
     T_TRUE = 276,
     T_FALSE = 277,
     T_BUILTIN_1 = 278,
     T_BUILTIN_2 = 279,
     T_BUILTIN_2M = 280,
     T_BUILTIN_3 = 281,
     T_BUILTIN_4 = 282,
     T_BUILTIN_5 = 283,
     T_BUILTIN_MX = 284,
     T_BUILTIN_MX2 = 285,
     T_SWITCH = 286,
     T_CASE = 287,
     T_DEFAULT = 288,
     T_IF = 289,
     T_ELSEIF = 290,
     T_ELSE = 291,
     T_EXTERN = 292,
     T_FUNCTION = 293,
     T_WHILE = 294,
     T_ALTERNATE = 295,
     T_MUTEX = 296,
     T_MUTEXVAR = 297,
     T_FREE = 298,
     T_FAIL = 299,
     T_PRINT = 300,
     T_CALL = 301,
     T_VAR = 302,
     T_VARIABLE = 303,
     T_EXTERNVARIABLE = 304,
     T_OR = 305,
     T_AND = 306,
     T_EQ = 307,
     T_LEQ = 308,
     T_NEQ = 309,
     T_GEQ = 310,
     UNIMINUS = 311
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 416 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1511

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  101
/* YYNRULES -- Number of rules.  */
#define YYNRULES  193
/* YYNRULES -- Number of states.  */
#define YYNSTATES  400

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    62,     2,     2,     2,     2,     2,     2,
      68,    70,    60,    58,    71,    59,     2,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    65,
      52,    69,    53,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    72,     2,    73,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    66,     2,    67,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    54,    55,    56,
      57,    63
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     9,    10,    11,    20,    24,    25,
      34,    35,    40,    41,    47,    55,    58,    62,    64,    66,
      70,    71,    72,    78,    80,    82,    84,    86,    87,    90,
      91,    93,    95,    98,   103,   105,   108,   114,   115,   117,
     119,   122,   124,   126,   129,   131,   133,   134,   140,   141,
     152,   153,   162,   163,   172,   173,   182,   183,   184,   195,
     196,   198,   200,   204,   206,   208,   209,   216,   217,   221,
     225,   227,   229,   231,   235,   237,   239,   243,   247,   251,
     255,   256,   263,   264,   267,   268,   276,   277,   281,   287,
     289,   292,   293,   299,   300,   301,   309,   311,   314,   315,
     318,   324,   326,   330,   332,   334,   336,   337,   340,   342,
     344,   346,   348,   349,   355,   360,   361,   371,   372,   381,
     382,   393,   395,   397,   402,   403,   411,   413,   417,   419,
     421,   424,   427,   428,   430,   432,   435,   436,   437,   444,
     446,   449,   453,   454,   455,   462,   463,   465,   467,   470,
     471,   480,   481,   482,   488,   492,   495,   498,   502,   506,
     510,   514,   518,   522,   526,   530,   534,   538,   542,   546,
     551,   553,   555,   557,   559,   561,   563,   565,   566,   570,
     572,   574,   576,   578,   582,   587,   592,   597,   602,   611,
     616,   623,   624,   626
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      75,     0,    -1,    -1,    77,    98,    76,   101,    -1,    -1,
      -1,     3,    64,    15,    78,     4,    64,    15,    79,    -1,
      47,    86,    65,    -1,    -1,    41,    90,    81,    83,    66,
      84,    67,    65,    -1,    -1,    37,    15,    82,    65,    -1,
      -1,    68,     7,    69,   167,    70,    -1,    68,     7,    69,
     167,    71,   167,    70,    -1,    85,    71,    -1,    84,    85,
      71,    -1,    16,    -1,    87,    -1,    86,    71,    87,    -1,
      -1,    -1,    91,    88,    92,    89,    93,    -1,    15,    -1,
      42,    -1,    15,    -1,    48,    -1,    -1,    72,    73,    -1,
      -1,    94,    -1,    95,    -1,    69,   167,    -1,    69,    66,
      96,    67,    -1,    97,    -1,    96,    97,    -1,    72,   167,
      73,   167,    71,    -1,    -1,    99,    -1,   100,    -1,    99,
     100,    -1,    80,    -1,   102,    -1,   101,   102,    -1,   113,
      -1,   121,    -1,    -1,    11,    66,   103,   135,    67,    -1,
      -1,    12,    68,   112,    71,   112,    70,    66,   104,   135,
      67,    -1,    -1,    19,    68,   112,    70,    66,   105,   135,
      67,    -1,    -1,    20,    68,   112,    70,    66,   106,   135,
      67,    -1,    -1,    13,    68,   112,    70,    66,   107,   135,
      67,    -1,    -1,    -1,    38,    15,   108,    68,   110,    70,
      66,   109,   135,    67,    -1,    -1,   111,    -1,   112,    -1,
     111,    71,   112,    -1,    15,    -1,   167,    -1,    -1,     5,
     115,   114,    66,   123,    67,    -1,    -1,    68,   116,    70,
      -1,    68,   117,    70,    -1,    16,    -1,   151,    -1,   118,
      -1,   117,    71,   118,    -1,   119,    -1,   120,    -1,    44,
      69,    16,    -1,    44,    69,   151,    -1,    43,    69,    16,
      -1,    43,    69,   151,    -1,    -1,     6,   122,   115,    66,
     123,    67,    -1,    -1,   124,   135,    -1,    -1,    40,   127,
      66,   126,   135,    67,    65,    -1,    -1,    68,    10,    70,
      -1,     7,    66,   129,    67,    65,    -1,   130,    -1,   129,
     130,    -1,    -1,    16,   131,    64,   167,    65,    -1,    -1,
      -1,     8,    66,   133,   135,    67,   134,    65,    -1,   142,
      -1,   135,   142,    -1,    -1,    64,   167,    -1,    45,    68,
     138,    70,    65,    -1,   139,    -1,   138,    71,   139,    -1,
     167,    -1,   140,    -1,    18,    -1,    -1,    61,   174,    -1,
      65,    -1,   137,    -1,   132,    -1,   128,    -1,    -1,   174,
     143,   141,   136,    65,    -1,    14,    64,   167,    65,    -1,
      -1,    31,    68,   167,    70,    66,   144,   152,   159,    67,
      -1,    -1,    39,    68,   167,    70,    66,   145,   135,    67,
      -1,    -1,    34,    68,   167,    70,    66,   146,   135,    67,
     161,   165,    -1,    80,    -1,   125,    -1,   148,    69,   167,
      65,    -1,    -1,    46,    15,   147,    68,   110,    70,    65,
      -1,   151,    -1,   148,    71,   151,    -1,    48,    -1,    49,
      -1,   149,   168,    -1,   150,   168,    -1,    -1,   153,    -1,
     154,    -1,   153,   154,    -1,    -1,    -1,   155,   157,   156,
      66,   135,    67,    -1,   158,    -1,   157,   158,    -1,    32,
     167,    64,    -1,    -1,    -1,    33,    64,    66,   160,   135,
      67,    -1,    -1,   162,    -1,   163,    -1,   162,   163,    -1,
      -1,    35,    68,   167,    70,    66,   164,   135,    67,    -1,
      -1,    -1,    36,    66,   166,   135,    67,    -1,    68,   167,
      70,    -1,    59,   167,    -1,    62,   167,    -1,   167,    51,
     167,    -1,   167,    50,   167,    -1,   167,    58,   167,    -1,
     167,    59,   167,    -1,   167,    60,   167,    -1,   167,    61,
     167,    -1,   167,    53,   167,    -1,   167,    52,   167,    -1,
     167,    57,   167,    -1,   167,    55,   167,    -1,   167,    54,
     167,    -1,   167,    56,   167,    -1,    17,    68,   167,    70,
      -1,   169,    -1,    14,    -1,    22,    -1,    21,    -1,   174,
      -1,   172,    -1,   151,    -1,    -1,    72,   167,    73,    -1,
     170,    -1,   171,    -1,    10,    -1,     9,    -1,    23,    68,
      70,    -1,    24,    68,   173,    70,    -1,    25,    68,   174,
      70,    -1,    26,    68,   167,    70,    -1,    27,    68,   140,
      70,    -1,    28,    68,   167,    71,   167,    71,   167,    70,
      -1,    29,    68,    42,    70,    -1,    30,    68,    42,    71,
     174,    70,    -1,    -1,   167,    -1,    16,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   268,   268,   268,   277,   278,   277,   297,   299,   298,
     314,   314,   319,   324,   330,   339,   340,   343,   353,   354,
     359,   369,   358,   395,   396,   399,   400,   403,   404,   407,
     408,   409,   413,   424,   428,   429,   433,   446,   448,   451,
     452,   456,   460,   461,   465,   466,   468,   467,   479,   478,
     495,   494,   508,   507,   521,   520,   533,   535,   532,   554,
     555,   559,   560,   570,   576,   580,   579,   602,   604,   612,
     616,   617,   621,   622,   625,   633,   649,   650,   654,   655,
     659,   658,   685,   685,   691,   690,   697,   698,   701,   704,
     705,   709,   708,   725,   727,   724,   731,   732,   736,   737,
     741,   750,   751,   761,   762,   765,   776,   777,   781,   782,
     783,   784,   786,   785,   850,   866,   865,   897,   896,   911,
     910,   923,   924,   925,   933,   932,   943,   944,   954,   962,
     965,   975,   987,   989,   993,   994,   998,  1000,   998,  1006,
    1007,  1010,  1025,  1028,  1027,  1037,  1038,  1041,  1042,  1046,
    1045,  1053,  1055,  1054,  1062,  1071,  1080,  1089,  1099,  1109,
    1119,  1129,  1139,  1149,  1159,  1169,  1179,  1189,  1199,  1209,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1227,  1228,  1231,
    1232,  1235,  1245,  1256,  1258,  1263,  1268,  1273,  1278,  1285,
    1287,  1295,  1314,  1317
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_CLASS", "T_BUILD", "T_COMBAT",
  "T_OUTOFCOMBAT", "T_CLIP", "T_STRICT", "T_FLOAT", "T_INTEGER",
  "T_PRECOMBAT", "T_POSTSPELL", "T_SPELLSENT", "T_NIL", "T_ID", "T_SPELL",
  "T_ABSVAL", "T_STRING", "T_TAKE_DAMAGE", "T_TAKE_HEAL", "T_TRUE",
  "T_FALSE", "T_BUILTIN_1", "T_BUILTIN_2", "T_BUILTIN_2M", "T_BUILTIN_3",
  "T_BUILTIN_4", "T_BUILTIN_5", "T_BUILTIN_MX", "T_BUILTIN_MX2",
  "T_SWITCH", "T_CASE", "T_DEFAULT", "T_IF", "T_ELSEIF", "T_ELSE",
  "T_EXTERN", "T_FUNCTION", "T_WHILE", "T_ALTERNATE", "T_MUTEX",
  "T_MUTEXVAR", "T_FREE", "T_FAIL", "T_PRINT", "T_CALL", "T_VAR",
  "T_VARIABLE", "T_EXTERNVARIABLE", "T_OR", "T_AND", "'<'", "'>'", "T_EQ",
  "T_LEQ", "T_NEQ", "T_GEQ", "'+'", "'-'", "'*'", "'/'", "'!'", "UNIMINUS",
  "':'", "';'", "'{'", "'}'", "'('", "'='", "')'", "','", "'['", "']'",
  "$accept", "file", "$@1", "header", "$@2", "$@3", "vardecl", "$@4",
  "$@5", ".mutexclip", "mx_spellist", "mx_spell", "vardecl_line",
  "vardeclent", "$@6", "$@7", "mutexname", "varname", ".array", ".init",
  "single_init", "array_init", "array_init_lines", "array_init_line",
  ".optstuff", "optstuff", "optthang", "calledRoutines", "calledRoutine",
  "$@8", "$@9", "$@10", "$@11", "$@12", "@13", "@14", ".paramlist",
  "paramlist", "parameter", "incombat", "$@15", ".pick_opts",
  "just_one_opt", "pick_opts", "pick_opt", "failspell", "freespell",
  "oocombat", "$@16", "combatstuff", "$@17", "alternate", "$@18",
  ".alt_number", "clips", "clipents", "clip", "$@19", "strict", "$@20",
  "$@21", "rotation", ".spell_predicate", "print_statement", "stuff2print",
  "printThang", "string", ".spell_mintime", "rotation_entry", "$@22",
  "@23", "$@24", "$@25", "@26", "varlist", "t_variable",
  "t_externvariable", "variable", ".case_statements", "case_statements",
  "case_statement", "$@27", "$@28", "caselines", "one_case",
  ".default_statement", "$@29", ".elseifs", "elseifs", "elseif", "$@30",
  ".else", "$@31", "exp", ".arrayindex", "number", "integer", "float",
  "builtin", ".spell", "spell", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    60,    62,   307,   308,   309,   310,    43,    45,
      42,    47,    33,   311,    58,    59,   123,   125,    40,    61,
      41,    44,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    76,    75,    78,    79,    77,    80,    81,    80,
      82,    80,    83,    83,    83,    84,    84,    85,    86,    86,
      88,    89,    87,    90,    90,    91,    91,    92,    92,    93,
      93,    93,    94,    95,    96,    96,    97,    98,    98,    99,
      99,   100,   101,   101,   102,   102,   103,   102,   104,   102,
     105,   102,   106,   102,   107,   102,   108,   109,   102,   110,
     110,   111,   111,   112,   112,   114,   113,   115,   115,   115,
     116,   116,   117,   117,   118,   118,   119,   119,   120,   120,
     122,   121,   124,   123,   126,   125,   127,   127,   128,   129,
     129,   131,   130,   133,   134,   132,   135,   135,   136,   136,
     137,   138,   138,   139,   139,   140,   141,   141,   142,   142,
     142,   142,   143,   142,   142,   144,   142,   145,   142,   146,
     142,   142,   142,   142,   147,   142,   148,   148,   149,   150,
     151,   151,   152,   152,   153,   153,   155,   156,   154,   157,
     157,   158,   159,   160,   159,   161,   161,   162,   162,   164,
     163,   165,   166,   165,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   168,   168,   169,
     169,   170,   171,   172,   172,   172,   172,   172,   172,   172,
     172,   173,   173,   174
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     4,     0,     0,     8,     3,     0,     8,
       0,     4,     0,     5,     7,     2,     3,     1,     1,     3,
       0,     0,     5,     1,     1,     1,     1,     0,     2,     0,
       1,     1,     2,     4,     1,     2,     5,     0,     1,     1,
       2,     1,     1,     2,     1,     1,     0,     5,     0,    10,
       0,     8,     0,     8,     0,     8,     0,     0,    10,     0,
       1,     1,     3,     1,     1,     0,     6,     0,     3,     3,
       1,     1,     1,     3,     1,     1,     3,     3,     3,     3,
       0,     6,     0,     2,     0,     7,     0,     3,     5,     1,
       2,     0,     5,     0,     0,     7,     1,     2,     0,     2,
       5,     1,     3,     1,     1,     1,     0,     2,     1,     1,
       1,     1,     0,     5,     4,     0,     9,     0,     8,     0,
      10,     1,     1,     4,     0,     7,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     2,     0,     0,     6,     1,
       2,     3,     0,     0,     6,     0,     1,     1,     2,     0,
       8,     0,     0,     5,     3,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     1,
       1,     1,     1,     3,     4,     4,     4,     4,     8,     4,
       6,     0,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    37,     0,     1,     0,     0,     0,    41,
       2,    38,    39,     4,    10,    23,    24,     8,    25,    26,
       0,    18,    20,     0,    40,     0,     0,    12,     7,     0,
      27,    67,    80,     0,     0,     0,     0,     0,     0,     3,
      42,    44,    45,     0,    11,     0,     0,    19,     0,    21,
       0,    65,    67,    46,     0,     0,     0,     0,    56,    43,
       0,     0,     0,    28,    29,    70,     0,     0,   128,   129,
       0,     0,    72,    74,    75,   177,   177,    71,     0,     0,
       0,   182,   181,   171,    63,   193,     0,   173,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,    64,   170,   179,   180,   175,   174,     0,     0,
       0,     0,     5,     0,    17,     0,     0,     0,    22,    30,
      31,     0,     0,    68,    69,     0,     0,   130,   131,    82,
      82,     0,     0,     0,     0,     0,     0,    86,     0,     0,
     108,   121,   122,   111,   110,     0,   109,    96,     0,   126,
     112,     0,     0,   191,     0,     0,     0,     0,     0,     0,
     155,   156,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       6,     0,     0,     0,    15,     0,    32,    78,    79,    76,
      77,    73,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,   124,    47,    97,     0,     0,
     106,     0,   183,   192,     0,     0,     0,   105,     0,     0,
       0,     0,   154,     0,   158,   157,   164,   163,   167,   166,
     168,   165,   159,   160,   161,   162,    54,    50,    52,     0,
      60,    61,    13,     0,     9,    16,     0,     0,    34,   178,
      66,    83,    81,    91,     0,    89,     0,     0,     0,     0,
       0,     0,    84,     0,   101,   104,   103,     0,     0,   127,
       0,    98,   169,   184,   185,   186,   187,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    35,
       0,     0,    90,     0,   114,     0,     0,     0,    87,     0,
       0,     0,    59,   123,   107,     0,     0,     0,     0,    48,
       0,     0,     0,    57,    62,    14,     0,     0,    88,    94,
     115,   119,   117,     0,   100,   102,     0,    99,   113,     0,
     190,     0,    55,    51,    53,     0,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,    36,    92,    95,
     142,   133,   134,     0,     0,     0,    85,   125,   188,    49,
      58,     0,     0,   135,     0,   137,   139,   145,   118,     0,
     116,     0,     0,   140,     0,   151,   146,   147,   143,   141,
       0,     0,     0,   120,   148,     0,     0,     0,   152,     0,
     138,     0,     0,   144,   149,     0,     0,   153,     0,   150
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    23,     3,    25,   180,   141,    27,    26,    46,
     115,   116,    20,    21,    30,    64,    17,    22,    49,   118,
     119,   120,   247,   248,    10,    11,    12,    39,    40,    80,
     331,   282,   283,   281,   111,   335,   239,   240,   241,    41,
      78,    51,    70,    71,    72,    73,    74,    42,    52,   193,
     194,   142,   299,   203,   143,   254,   255,   290,   144,   256,
     338,   145,   306,   146,   263,   264,   265,   271,   147,   210,
     339,   341,   340,   267,   148,    75,    76,   101,   350,   351,
     352,   353,   372,   365,   366,   362,   385,   375,   376,   377,
     396,   383,   392,   102,   127,   103,   104,   105,   106,   214,
     107
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -193
static const yytype_int16 yypact[] =
{
      18,   -33,    58,    55,    46,  -193,    59,     4,   -11,  -193,
    -193,    55,  -193,  -193,  -193,  -193,  -193,  -193,  -193,  -193,
      32,  -193,  -193,    21,  -193,    66,    23,    47,  -193,   -11,
      34,    51,  -193,    60,    72,    73,    74,    77,   112,    21,
    -193,  -193,  -193,    64,  -193,   139,    81,  -193,    88,  -193,
      50,  -193,    51,  -193,   108,   108,   108,   108,  -193,  -193,
     147,    85,   148,  -193,    94,  -193,    96,    97,  -193,  -193,
      98,   -57,  -193,  -193,  -193,    99,    99,  -193,   103,   106,
    1098,  -193,  -193,  -193,  -193,  -193,   105,  -193,  -193,   107,
     109,   110,   111,   113,   114,   115,   116,  1074,  1074,  1074,
     119,  -193,  1439,  -193,  -193,  -193,  -193,  -193,   104,   117,
     121,   124,  -193,  1074,  -193,   -15,   123,   964,  -193,  -193,
    -193,   -13,     2,  -193,  -193,     0,  1074,  -193,  -193,  -193,
    -193,   130,   131,   134,   135,   137,   140,   141,   142,   165,
    -193,  -193,  -193,  -193,  -193,     8,  -193,  -193,    -9,  -193,
    -193,  1074,   129,  1074,   169,  1074,   184,  1074,   170,   172,
    -193,  -193,  1187,   108,  1074,  1074,  1074,  1074,  1074,  1074,
    1074,  1074,  1074,  1074,  1074,  1074,   145,   149,   150,   108,
    -193,  1099,   173,   136,  -193,   158,  1439,  -193,  -193,  -193,
    -193,  -193,   945,   167,  1098,   175,   221,  -193,  1074,  1074,
    1074,  1074,   229,   174,  1019,  -193,  -193,  -193,  1074,    52,
     183,  1208,  -193,  1439,   176,   177,  1229,  -193,   178,  1121,
     180,   181,  -193,   185,  1450,   166,    92,    92,    92,    92,
      92,    92,    44,    44,  -193,  -193,  -193,  -193,  -193,   186,
     182,  -193,  -193,  1074,  -193,  -193,  1074,   -44,  -193,  -193,
    -193,  1098,  -193,  -193,     1,  -193,  1098,  1376,  1250,  1271,
    1292,   187,  -193,    38,  -193,  -193,  1439,   190,  1392,  -193,
     169,   195,  -193,  -193,  -193,  -193,  -193,  1074,  -193,   169,
     179,  1098,  1098,  1098,   188,   108,  1313,  1000,  -193,  -193,
     199,   200,  -193,   401,  -193,   198,   201,   202,  -193,  1098,
     204,  1019,   108,  -193,  -193,  1074,   205,  1143,   207,  -193,
     444,   487,   530,  -193,  -193,  -193,  1074,  1074,  -193,  -193,
    -193,  -193,  -193,   573,  -193,  -193,   208,  1439,  -193,  1074,
    -193,  1098,  -193,  -193,  -193,  1098,  1165,  1408,   206,   219,
    1098,  1098,   209,   210,  1334,   616,   659,  -193,  -193,  -193,
     239,   219,  -193,   247,   702,   745,  -193,  -193,  -193,  -193,
    -193,   216,   215,  -193,  1074,   247,  -193,   248,  -193,   218,
    -193,  1424,   220,  -193,   226,   251,   248,  -193,  -193,  -193,
    1098,  1074,   222,  -193,  -193,  1098,   788,  1355,  -193,   831,
    -193,   223,  1098,  -193,  -193,   874,  1098,  -193,   917,  -193
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -193,  -193,  -193,  -193,  -193,  -193,     9,  -193,  -193,  -193,
    -193,   191,  -193,   269,  -193,  -193,  -193,  -193,  -193,  -193,
    -193,  -193,  -193,    53,  -193,  -193,   288,  -193,   262,  -193,
    -193,  -193,  -193,  -193,  -193,  -193,     5,  -193,   -49,  -193,
    -193,   250,  -193,  -193,   189,  -193,  -193,  -193,  -193,   203,
    -193,  -193,  -193,  -193,  -193,  -193,    49,  -193,  -193,  -193,
    -193,  -192,  -193,  -193,  -193,    11,   153,  -193,    15,  -193,
    -193,  -193,  -193,  -193,  -193,  -193,  -193,   -50,  -193,  -193,
     -41,  -193,  -193,  -193,   -52,  -193,  -193,  -193,  -193,   -61,
    -193,  -193,  -193,   -88,   240,  -193,  -193,  -193,  -193,  -193,
      41
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -137
static const yytype_int16 yytable[] =
{
      77,   114,   251,   187,    18,   100,   108,   109,   110,   160,
     161,   162,     9,   124,   125,   131,   132,   253,   189,    15,
       9,     1,   133,   288,    85,   181,    31,    32,   246,   186,
     149,     4,    33,    34,    35,    68,    69,    19,   192,   134,
      36,    37,   135,    66,    67,     6,    16,   136,   137,     7,
      68,    69,   182,   138,   139,     8,    68,    69,     5,    38,
     208,    13,   209,   211,   293,   213,    65,   216,   291,   219,
      43,   188,   190,   140,    14,   206,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,    44,   310,
     311,   312,     6,    66,    67,   149,     7,    28,    68,    69,
      68,    69,     8,    29,   174,   175,    48,   323,   300,   301,
     257,   258,   259,   260,   223,    45,   266,    81,    82,    50,
     268,   150,    83,    84,    85,    86,    53,    58,    60,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,   345,
      54,    55,    56,   346,   149,    57,    61,    62,   354,   355,
     172,   173,   174,   175,   113,   286,    68,    69,   287,   269,
     207,    63,   112,   117,   114,   121,   122,    97,   123,   129,
      98,   126,   130,   151,   176,   152,    99,   153,   154,   155,
     205,   156,   157,   158,   159,    85,   150,   177,   386,   307,
     163,   178,   179,   389,   184,   215,   196,   197,   198,   212,
     395,   149,   217,   199,   398,   200,   149,   245,   201,   202,
     204,   236,   220,   266,   221,   237,   238,   327,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   336,   337,
     246,   149,   149,   149,   250,   150,   314,   253,   244,   261,
     262,   344,   252,   149,   270,   309,   273,   274,   276,   149,
     278,  -136,   279,   285,   313,   280,   284,   298,   302,   305,
     149,   149,   149,   317,   320,   318,   207,   321,   322,   324,
     328,   349,   361,   149,   356,   357,   371,   330,   343,   364,
     369,   149,   370,   374,   378,   149,   380,   382,   388,   394,
     149,   149,   150,   387,   381,   149,   149,   150,    47,    24,
     289,    59,    79,   292,   149,   149,   183,   326,   207,   218,
     363,   304,   325,   373,   191,   384,   128,     0,     0,     0,
     308,     0,   150,   150,   150,   207,   207,   207,     0,     0,
     149,     0,     0,   195,   150,   149,   149,     0,   207,   149,
     150,     0,   149,     0,     0,   149,   149,     0,   149,     0,
       0,   150,   150,   150,     0,     0,     0,     0,     0,     0,
     207,   207,     0,     0,   150,     0,     0,     0,     0,   207,
     207,     0,   150,     0,     0,     0,   150,     0,     0,     0,
       0,   150,   150,     0,     0,     0,   150,   150,     0,     0,
       0,     0,     0,     0,     0,   150,   150,     0,     0,     0,
       0,   207,     0,     0,   207,     0,     0,     0,   131,   132,
     207,     0,     0,   207,     0,   133,     0,    85,     0,     0,
       0,   150,     0,     0,     0,     0,   150,   150,     0,     0,
     150,     0,   134,   150,     0,   135,   150,   150,     6,   150,
     136,   137,     7,     0,     0,     0,   138,   139,     8,    68,
      69,   131,   132,     0,     0,     0,     0,     0,   133,     0,
      85,     0,     0,     0,     0,     0,   140,     0,   319,     0,
       0,     0,     0,     0,     0,   134,     0,     0,   135,     0,
       0,     6,     0,   136,   137,     7,     0,     0,     0,   138,
     139,     8,    68,    69,   131,   132,     0,     0,     0,     0,
       0,   133,     0,    85,     0,     0,     0,     0,     0,   140,
       0,   332,     0,     0,     0,     0,     0,     0,   134,     0,
       0,   135,     0,     0,     6,     0,   136,   137,     7,     0,
       0,     0,   138,   139,     8,    68,    69,   131,   132,     0,
       0,     0,     0,     0,   133,     0,    85,     0,     0,     0,
       0,     0,   140,     0,   333,     0,     0,     0,     0,     0,
       0,   134,     0,     0,   135,     0,     0,     6,     0,   136,
     137,     7,     0,     0,     0,   138,   139,     8,    68,    69,
     131,   132,     0,     0,     0,     0,     0,   133,     0,    85,
       0,     0,     0,     0,     0,   140,     0,   334,     0,     0,
       0,     0,     0,     0,   134,     0,     0,   135,     0,     0,
       6,     0,   136,   137,     7,     0,     0,     0,   138,   139,
       8,    68,    69,   131,   132,     0,     0,     0,     0,     0,
     133,     0,    85,     0,     0,     0,     0,     0,   140,     0,
     342,     0,     0,     0,     0,     0,     0,   134,     0,     0,
     135,     0,     0,     6,     0,   136,   137,     7,     0,     0,
       0,   138,   139,     8,    68,    69,   131,   132,     0,     0,
       0,     0,     0,   133,     0,    85,     0,     0,     0,     0,
       0,   140,     0,   359,     0,     0,     0,     0,     0,     0,
     134,     0,     0,   135,     0,     0,     6,     0,   136,   137,
       7,     0,     0,     0,   138,   139,     8,    68,    69,   131,
     132,     0,     0,     0,     0,     0,   133,     0,    85,     0,
       0,     0,     0,     0,   140,     0,   360,     0,     0,     0,
       0,     0,     0,   134,     0,     0,   135,     0,     0,     6,
       0,   136,   137,     7,     0,     0,     0,   138,   139,     8,
      68,    69,   131,   132,     0,     0,     0,     0,     0,   133,
       0,    85,     0,     0,     0,     0,     0,   140,     0,   367,
       0,     0,     0,     0,     0,     0,   134,     0,     0,   135,
       0,     0,     6,     0,   136,   137,     7,     0,     0,     0,
     138,   139,     8,    68,    69,   131,   132,     0,     0,     0,
       0,     0,   133,     0,    85,     0,     0,     0,     0,     0,
     140,     0,   368,     0,     0,     0,     0,     0,     0,   134,
       0,     0,   135,     0,     0,     6,     0,   136,   137,     7,
       0,     0,     0,   138,   139,     8,    68,    69,   131,   132,
       0,     0,     0,     0,     0,   133,     0,    85,     0,     0,
       0,     0,     0,   140,     0,   390,     0,     0,     0,     0,
       0,     0,   134,     0,     0,   135,     0,     0,     6,     0,
     136,   137,     7,     0,     0,     0,   138,   139,     8,    68,
      69,   131,   132,     0,     0,     0,     0,     0,   133,     0,
      85,     0,     0,     0,     0,     0,   140,     0,   393,     0,
       0,     0,     0,     0,     0,   134,     0,     0,   135,     0,
       0,     6,     0,   136,   137,     7,     0,     0,     0,   138,
     139,     8,    68,    69,   131,   132,     0,     0,     0,     0,
       0,   133,     0,    85,     0,     0,     0,     0,     0,   140,
       0,   397,     0,     0,     0,     0,     0,     0,   134,     0,
       0,   135,     0,     0,     6,     0,   136,   137,     7,     0,
       0,     0,   138,   139,     8,    68,    69,     0,     0,     0,
       0,     0,     0,    81,    82,     0,     0,     0,    83,     0,
      85,    86,   140,     0,   399,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,    68,    69,     0,     0,     0,     0,   249,     0,
       0,     0,     0,    97,     0,     0,    98,     0,    81,    82,
     185,     0,    99,    83,     0,    85,    86,   217,     0,     0,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,     0,     0,     0,    68,    69,     0,
       0,     0,     0,   316,     0,     0,     0,     0,    97,     0,
       0,    98,     0,    81,    82,     0,     0,    99,    83,     0,
      85,    86,     0,     0,     0,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,   131,   132,     0,     0,     0,
       0,     0,   133,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    68,    69,     0,     0,     0,     0,     0,   134,
       0,     0,   135,    97,     0,     6,    98,   136,   137,     7,
       0,     0,    99,   138,   139,     8,    68,    69,     0,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,   140,     0,     0,     0,     0,     0,   242,
     243,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   329,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
       0,     0,     0,     0,     0,     0,     0,   222,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,     0,     0,     0,     0,     0,     0,   275,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     295,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,   297,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,     0,     0,     0,     0,
       0,     0,     0,   315,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,     0,     0,     0,
       0,     0,     0,     0,   358,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,   391,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,     0,
       0,   294,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,   303,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,   348,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,     0,   379,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175
};

static const yytype_int16 yycheck[] =
{
      50,    16,   194,    16,    15,    54,    55,    56,    57,    97,
      98,    99,     3,    70,    71,     7,     8,    16,    16,    15,
      11,     3,    14,    67,    16,   113,     5,     6,    72,   117,
      80,    64,    11,    12,    13,    48,    49,    48,   126,    31,
      19,    20,    34,    43,    44,    37,    42,    39,    40,    41,
      48,    49,    67,    45,    46,    47,    48,    49,     0,    38,
      69,    15,    71,   151,   256,   153,    16,   155,    67,   157,
       4,   121,   122,    65,    15,    67,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    65,   281,
     282,   283,    37,    43,    44,   145,    41,    65,    48,    49,
      48,    49,    47,    71,    60,    61,    72,   299,    70,    71,
     198,   199,   200,   201,   163,    68,   204,     9,    10,    68,
     208,    80,    14,    15,    16,    17,    66,    15,    64,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   331,
      68,    68,    68,   335,   194,    68,     7,    66,   340,   341,
      58,    59,    60,    61,    69,   243,    48,    49,   246,   209,
     145,    73,    15,    69,    16,    69,    69,    59,    70,    66,
      62,    72,    66,    68,    70,    68,    68,    68,    68,    68,
      15,    68,    68,    68,    68,    16,   145,    70,   380,   277,
      71,    70,    68,   385,    71,   154,    66,    66,    64,    70,
     392,   251,    18,    68,   396,    68,   256,    71,    68,    68,
      68,    66,    42,   301,    42,    66,    66,   305,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,   316,   317,
      72,   281,   282,   283,    67,   194,   285,    16,    65,    10,
      66,   329,    67,   293,    61,    66,    70,    70,    70,   299,
      70,    32,    71,    71,    66,    70,    70,    70,    68,    64,
     310,   311,   312,    64,    66,    65,   251,    66,    66,    65,
      65,    65,    33,   323,    65,    65,   364,    70,    70,    32,
      64,   331,    67,    35,    66,   335,    66,    36,    66,    66,
     340,   341,   251,   381,    68,   345,   346,   256,    29,    11,
     247,    39,    52,   254,   354,   355,   115,   302,   293,   156,
     351,   270,   301,   365,   125,   376,    76,    -1,    -1,    -1,
     279,    -1,   281,   282,   283,   310,   311,   312,    -1,    -1,
     380,    -1,    -1,   130,   293,   385,   386,    -1,   323,   389,
     299,    -1,   392,    -1,    -1,   395,   396,    -1,   398,    -1,
      -1,   310,   311,   312,    -1,    -1,    -1,    -1,    -1,    -1,
     345,   346,    -1,    -1,   323,    -1,    -1,    -1,    -1,   354,
     355,    -1,   331,    -1,    -1,    -1,   335,    -1,    -1,    -1,
      -1,   340,   341,    -1,    -1,    -1,   345,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,    -1,    -1,    -1,
      -1,   386,    -1,    -1,   389,    -1,    -1,    -1,     7,     8,
     395,    -1,    -1,   398,    -1,    14,    -1,    16,    -1,    -1,
      -1,   380,    -1,    -1,    -1,    -1,   385,   386,    -1,    -1,
     389,    -1,    31,   392,    -1,    34,   395,   396,    37,   398,
      39,    40,    41,    -1,    -1,    -1,    45,    46,    47,    48,
      49,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    37,    -1,    39,    40,    41,    -1,    -1,    -1,    45,
      46,    47,    48,    49,     7,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,    37,    -1,    39,    40,    41,    -1,
      -1,    -1,    45,    46,    47,    48,    49,     7,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    37,    -1,    39,
      40,    41,    -1,    -1,    -1,    45,    46,    47,    48,    49,
       7,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      37,    -1,    39,    40,    41,    -1,    -1,    -1,    45,    46,
      47,    48,    49,     7,     8,    -1,    -1,    -1,    -1,    -1,
      14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    37,    -1,    39,    40,    41,    -1,    -1,
      -1,    45,    46,    47,    48,    49,     7,     8,    -1,    -1,
      -1,    -1,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    37,    -1,    39,    40,
      41,    -1,    -1,    -1,    45,    46,    47,    48,    49,     7,
       8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    37,
      -1,    39,    40,    41,    -1,    -1,    -1,    45,    46,    47,
      48,    49,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    37,    -1,    39,    40,    41,    -1,    -1,    -1,
      45,    46,    47,    48,    49,     7,     8,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    -1,    37,    -1,    39,    40,    41,
      -1,    -1,    -1,    45,    46,    47,    48,    49,     7,     8,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    37,    -1,
      39,    40,    41,    -1,    -1,    -1,    45,    46,    47,    48,
      49,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    37,    -1,    39,    40,    41,    -1,    -1,    -1,    45,
      46,    47,    48,    49,     7,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,    37,    -1,    39,    40,    41,    -1,
      -1,    -1,    45,    46,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    14,    -1,
      16,    17,    65,    -1,    67,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    62,    -1,     9,    10,
      66,    -1,    68,    14,    -1,    16,    17,    18,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    62,    -1,     9,    10,    -1,    -1,    68,    14,    -1,
      16,    17,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     7,     8,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    59,    -1,    37,    62,    39,    40,    41,
      -1,    -1,    68,    45,    46,    47,    48,    49,    -1,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    70,
      71,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,    65,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    -1,    -1,    65,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    65,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    64,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    75,    77,    64,     0,    37,    41,    47,    80,
      98,    99,   100,    15,    15,    15,    42,    90,    15,    48,
      86,    87,    91,    76,   100,    78,    82,    81,    65,    71,
      88,     5,     6,    11,    12,    13,    19,    20,    38,   101,
     102,   113,   121,     4,    65,    68,    83,    87,    72,    92,
      68,   115,   122,    66,    68,    68,    68,    68,    15,   102,
      64,     7,    66,    73,    89,    16,    43,    44,    48,    49,
     116,   117,   118,   119,   120,   149,   150,   151,   114,   115,
     103,     9,    10,    14,    15,    16,    17,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    59,    62,    68,
     112,   151,   167,   169,   170,   171,   172,   174,   112,   112,
     112,   108,    15,    69,    16,    84,    85,    69,    93,    94,
      95,    69,    69,    70,    70,    71,    72,   168,   168,    66,
      66,     7,     8,    14,    31,    34,    39,    40,    45,    46,
      65,    80,   125,   128,   132,   135,   137,   142,   148,   151,
     174,    68,    68,    68,    68,    68,    68,    68,    68,    68,
     167,   167,   167,    71,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    70,    70,    70,    68,
      79,   167,    67,    85,    71,    66,   167,    16,   151,    16,
     151,   118,   167,   123,   124,   123,    66,    66,    64,    68,
      68,    68,    68,   127,    68,    15,    67,   142,    69,    71,
     143,   167,    70,   167,   173,   174,   167,    18,   140,   167,
      42,    42,    70,   112,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,    66,    66,    66,   110,
     111,   112,    70,    71,    65,    71,    72,    96,    97,    73,
      67,   135,    67,    16,   129,   130,   133,   167,   167,   167,
     167,    10,    66,   138,   139,   140,   167,   147,   167,   151,
      61,   141,    70,    70,    70,    70,    70,    71,    70,    71,
      70,   107,   105,   106,    70,    71,   167,   167,    67,    97,
     131,    67,   130,   135,    65,    70,    70,    70,    70,   126,
      70,    71,    68,    65,   174,    64,   136,   167,   174,    66,
     135,   135,   135,    66,   112,    70,    73,    64,    65,    67,
      66,    66,    66,   135,    65,   139,   110,   167,    65,    71,
      70,   104,    67,    67,    67,   109,   167,   167,   134,   144,
     146,   145,    67,    70,   167,   135,   135,    71,    65,    65,
     152,   153,   154,   155,   135,   135,    65,    65,    70,    67,
      67,    33,   159,   154,    32,   157,   158,    67,    67,    64,
      67,   167,   156,   158,    35,   161,   162,   163,    66,    64,
      66,    68,    36,   165,   163,   160,   135,   167,    66,   135,
      67,    70,   166,    67,    66,   135,   164,    67,   135,    67
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 268 "parser.y"
    { utilOK = 1; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 269 "parser.y"
    {
	      printf("-- populate our registries\n");
	      printf("DestroDummies.registry.decideCombat[DestroDummies.class%s%s] = PickCombat;\n", classname, buildname);
	      printf("DestroDummies.registry.decideNoCombat[DestroDummies.class%s%s] = PickNoCombat;\n", classname, buildname);
	      printf("DestroDummies.lastMutexIndex = mIndexBase + %d;\n", mIndex);
	  ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 277 "parser.y"
    { classname = normalize(astrdup(yytext)); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 278 "parser.y"
    { buildname = normalize(astrdup(yytext)); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 279 "parser.y"
    {
	      printf("-- Autogenerated file.  Do not edit.  All changes "
		     					"will be lost\n");
	      printf("--\n");
	      printf("-- Definition of %s %s\n", buildname, classname);
	      printf("\n");

	      // set up the class variable
	      printf("DestroDummies.class%s%s = DestroDummies.lastClassVal + 1;\n",
		     classname, buildname);
	      printf("DestroDummies.lastClassVal = "
		                         "DestroDummies.lastClassVal + 1;\n");
	      printf("\n");
	      printf("local mIndexBase = DestroDummies.lastMutexIndex;\n");
	      printf("\n");
	  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 299 "parser.y"
    {
	      curvar = amalloc(200);
	      sprintf(curvar, 
	      	      "DestroDummies.mutex[mIndexBase + %d]",
		      ++mIndex);
	      lex_register_mutex(yytext, mIndex);
	      indent(0);
	      printf("%s = {};\n", curvar);
	      cur_mutexno = 0;
	  ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 310 "parser.y"
    {
	      indent(0);
	      printf("%s.count = %d;\n", curvar, cur_mutexno);
	  ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 314 "parser.y"
    { lex_register_variable(yytext, T_EXTERNVARIABLE); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 319 "parser.y"
    {
	    	indent(0);
	    	printf("%s.clipCombat = 0;\n", curvar);
	    	printf("%s.clipOOCombat = 0;\n", curvar);
	    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 325 "parser.y"
    {
	    	indent(0);
	    	printf("%s.clipCombat = %s;\n", curvar, (yyvsp[(4) - (5)]).str);
	    	printf("%s.clipOOCombat = %s;\n", curvar, (yyvsp[(4) - (5)]).str);
	    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 331 "parser.y"
    {
	    	indent(0);
	    	printf("%s.clipOOCombat = %s;\n", curvar, (yyvsp[(4) - (7)]).str);
	    	printf("%s.clipCombat = %s;\n", curvar, (yyvsp[(6) - (7)]).str);
	    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 344 "parser.y"
    {
	      indent(0);
	      printf("%s[%d] = DestroDummies:xlateString(%s)\n",
		      curvar, ++cur_mutexno, yytext);
	  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 359 "parser.y"
    {
	      // note that something very similar is done at the 
	      // parameter production.  these ought to be unified, but
	      // for now, just note that any changes here probably have
	      // to be made there too
	      curvar = amalloc(strlen(yytext) + 4);
	      sprintf(curvar, "var%s", yytext);
	      lex_register_variable(yytext, T_VARIABLE);
	  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 369 "parser.y"
    {
	      curvar_array = (yyvsp[(3) - (3)]).intg;
	      if (curvar_array) {
		  indent(0);
	          printf("local %s = {};\n", curvar);
	      }
	  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 377 "parser.y"
    {
	      if (!curvar_array) {
		  indent(0);
	          printf("local %s", curvar);
		  if ((yyvsp[(5) - (5)]).str != NULL) {
		      printf(" = %s", (yyvsp[(5) - (5)]).str);
		      free((yyvsp[(5) - (5)]).str);
		  }
		  printf(";\n");
	      }

	      
	      free(curvar);
	      curvar = NULL;
	  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 403 "parser.y"
    { (yyval).intg = 0; ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 404 "parser.y"
    { (yyval).intg = 1; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 407 "parser.y"
    { (yyval).str = NULL; ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 408 "parser.y"
    { (yyval).str = (yyvsp[(1) - (1)]).str; ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 409 "parser.y"
    { (yyval).str = NULL; ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 414 "parser.y"
    {
	      if (curvar_array) {
		  fprintf(stderr, "[%d]: Error: %s is an array, cannot single init it.\n", lineno, curvar);
		  exit(1);
	      }
	      (yyval) = (yyvsp[(2) - (2)]);
	  ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 434 "parser.y"
    {
	      if (!curvar_array) {
		  fprintf(stderr, "[%d]: Error: %s is not an array.  Cannot aggregate init it.\n", lineno, curvar);
		  exit(1);
	      }
	      printf("%s[%s] = %s;\n", curvar, (yyvsp[(2) - (5)]).str, (yyvsp[(4) - (5)]).str);
	      free((yyvsp[(2) - (5)]).str);
	      free((yyvsp[(4) - (5)]).str);
	  ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 468 "parser.y"
    {
	    	printf("local function preCombat()\n");
		indent_level++;
	    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 473 "parser.y"
    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.preCombat[DestroDummies.class%s%s] = preCombat;\n\n", classname, buildname);
	    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 479 "parser.y"
    {
		// called with parameters:
		//     first:  spell name
		//     second: true if spell succeeded
		printf("local function postSpell(%s, %s)\n", (yyvsp[(3) - (7)]).str, (yyvsp[(5) - (7)]).str);
		free((yyvsp[(3) - (7)]).str);
		free((yyvsp[(5) - (7)]).str);
		indent_level++;
	    ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 489 "parser.y"
    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.postSpell[DestroDummies.class%s%s] = postSpell;\n\n", classname, buildname);
	    ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 495 "parser.y"
    {
	    	printf("local function takeDamage(%s)\n", (yyvsp[(3) - (5)]).str);
		free((yyvsp[(3) - (5)]).str);
		indent_level++;
	    ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 501 "parser.y"
    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.takeDamage[DestroDummies.class%s%s] = takeDamage;\n\n", classname, buildname);
	    ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 508 "parser.y"
    {
	    	printf("local function takeHeal(%s)\n", (yyvsp[(3) - (5)]).str);
		free((yyvsp[(3) - (5)]).str);
		indent_level++;
	    ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 514 "parser.y"
    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.takeHeal[DestroDummies.class%s%s] = takeHeal;\n\n", classname, buildname);
	    ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 521 "parser.y"
    {
	    	printf("local function spellSent(%s)\n", (yyvsp[(3) - (5)]).str);
		free((yyvsp[(3) - (5)]).str);
		indent_level++;
	    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 527 "parser.y"
    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.spellSent[DestroDummies.class%s%s] = spellSent;\n\n", classname, buildname);
	    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 533 "parser.y"
    { (yyval).str = strdup(yytext); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 535 "parser.y"
    {
	    	printf("function DestroDummies:genfunc_%s(%s)\n",
		       (yyvsp[(3) - (7)]).str, (yyvsp[(5) - (7)]).str);
		free((yyvsp[(3) - (7)]).str);
		free((yyvsp[(5) - (7)]).str);
		indent_level++;

		(yyval).intg = utilOK;
		utilOK = 1;	// this will be called from where it's ok
	    ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 546 "parser.y"
    {
		utilOK = (yyvsp[(8) - (10)]).intg;
	    	indent_level--;
		printf("end\n\n");
	    ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 554 "parser.y"
    { (yyval).str = strdup(""); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 561 "parser.y"
    {
		(yyval).str = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 3);
		sprintf((yyval).str, "%s, %s", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
		free((yyvsp[(1) - (3)]).str);
		free((yyvsp[(3) - (3)]).str);
	    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 571 "parser.y"
    {
	      (yyval).str = amalloc(strlen(yytext) + 4);
	      sprintf((yyval).str, "var%s", yytext);
	      lex_register_variable(yytext, T_VARIABLE);
	  ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 580 "parser.y"
    {
	      printf("local function PickCombat()\n");
	      indent_level++;
	  ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 585 "parser.y"
    {
	      indent(0);
	      printf("return DestroDummies:analysisPick(%s, %s);\n",
		     fail_spell == NULL ? "nil" : fail_spell,
		     free_spell == NULL ? "nil" : free_spell);
	      if (fail_spell != NULL)
		  free(fail_spell);
	      fail_spell = NULL;
	      if (free_spell != NULL)
		  free(free_spell);
	      free_spell = NULL;
	      indent_level--;
	      indent(0);
	      printf("end;\n\n");
	  ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 605 "parser.y"
    {
		if (fail_spell != NULL) {
		    fprintf(stderr, "[%d]: too many fail cases\n", lineno);
		    exit(1);
		}
		fail_spell = (yyvsp[(2) - (3)]).str;
	    ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 616 "parser.y"
    { (yyval).str = astrdup(yytext); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 626 "parser.y"
    {
		if (fail_spell != NULL) {
		    fprintf(stderr, "[%d]: too many fail cases\n", lineno);
		    exit(1);
		}
		fail_spell = (yyvsp[(1) - (1)]).str;
	    ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 634 "parser.y"
    {
		if (free_spell != NULL) {
		    /*
		     *  There's no reason not to allow more than one and
		     *  rotate through, but I don't think there will ever
		     *  be a desire, so I'm just implementing it easy now
		     */
		    fprintf(stderr, "[%d]: too many free cases\n", lineno);
		    exit(1);
		}
		free_spell = (yyvsp[(1) - (1)]).str;
	    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 649 "parser.y"
    { (yyval).str = astrdup(yytext); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 650 "parser.y"
    { (yyval) = (yyvsp[(3) - (3)]); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 654 "parser.y"
    { (yyval).str = astrdup(yytext); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 655 "parser.y"
    { (yyval) = (yyvsp[(3) - (3)]); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 659 "parser.y"
    {
	      printf("local function PickNoCombat()\n");
	      indent_level++;
	  ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 665 "parser.y"
    {
	      indent(0);
	      printf("return DestroDummies:analysisPick(%s, %s);\n",
		     fail_spell == NULL ? "nil" : fail_spell,
		     free_spell == NULL ? "nil" : free_spell);
	      if (fail_spell != NULL)
		  free(fail_spell);
	      fail_spell = NULL;
	      if (free_spell != NULL)
		  free(free_spell);
	      free_spell = NULL;

	      indent_level--;
	      indent(0);
	      printf("end;\n\n");
	  ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 685 "parser.y"
    { indent(0); printf("DestroDummies:analysisInit();\n"); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 691 "parser.y"
    { rotMode = RM_ALTERNATE; ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 693 "parser.y"
    { rotMode = RM_NORMAL; ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 697 "parser.y"
    { altnum = 0; ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 698 "parser.y"
    { altnum = (yyvsp[(2) - (3)]).intg; ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 709 "parser.y"
    {
	      char *clip;

	      clip = spell2clip(yytext);
	      have_clip(yytext, clip);
	      indent(0);
	      printf("local %s = ", clip);
	  ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 718 "parser.y"
    { 
	      printf("%s;\n", (yyvsp[(4) - (5)]).str);
	      free((yyvsp[(4) - (5)]).str);
	  ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 725 "parser.y"
    { rotMode = RM_STRICT; ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 727 "parser.y"
    { rotMode = RM_NORMAL; ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 736 "parser.y"
    { (yyval).str = NULL; ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 737 "parser.y"
    { (yyval) = (yyvsp[(2) - (2)]); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 742 "parser.y"
    { 
	    	indent(0);
		printf("print(%s);\n", (yyvsp[(3) - (5)]).str);
		free((yyvsp[(3) - (5)]).str);
	    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 752 "parser.y"
    {
	    	(yyval).str = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 3);
		sprintf((yyval).str, "%s, %s", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
		free((yyvsp[(1) - (3)]).str);
		free((yyvsp[(3) - (3)]).str);
	    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 766 "parser.y"
    { (yyval).str = astrdup(yytext); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 776 "parser.y"
    { (yyval).str = NULL; ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 777 "parser.y"
    { (yyval) = (yyvsp[(2) - (2)]); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 786 "parser.y"
    { curSpell = (yyvsp[(1) - (1)]).str; ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 788 "parser.y"
    {
		if ((yyvsp[(4) - (5)]).str != NULL) {
		    indent(0);
		    printf("if (%s) then\n", (yyvsp[(4) - (5)]).str); 
		    indent_level++;
		}
		switch (rotMode) {
		case RM_NORMAL:
		    if (debugging) {
			indent(0);
			printf("if DestroDummies.debug then\n");
			indent(1);
			printf("print(\"Analysis Add \", %s", curSpell);
			if ((yyvsp[(3) - (5)]).str != NULL)
			    printf(", %s", (yyvsp[(3) - (5)]).str);
			printf(");\n");
			indent(0);
			printf("end\n");
		    }
		    indent(0);
		    printf("DestroDummies:analysisAdd(%s", curSpell);
		    if ((yyvsp[(3) - (5)]).str != NULL)
			printf(", %s", (yyvsp[(3) - (5)]).str);
		    printf(");\n");
		    break;
	      
		case RM_STRICT:
		    if ((yyvsp[(3) - (5)]).str != NULL) {
		        fprintf(stderr, 
			  	"Cannot have a spell mintime with strict\n");
			exit(1);
		    }
		    indent(0);
		    printf("return %s;\n", curSpell);
		    break;

		case RM_ALTERNATE:
		    if ((yyvsp[(3) - (5)]).str != NULL) {
		        fprintf(stderr, 
			  	"Cannot have a spell mintime with strict\n");
			exit(1);
		    }
		    indent(0);
		    printf("DestroDummies:altRecommend(%s, %d);\n", 
		    	   curSpell, altnum);
		    break;
	
		default:
		    fprintf(stderr, "Unknown rotMode: %d\n", rotMode);
		    exit(1);
		}
		if ((yyvsp[(3) - (5)]).str != NULL)
		    free((yyvsp[(3) - (5)]).str);
		if ((yyvsp[(4) - (5)]).str != NULL) {
		    indent_level--;
		    indent(0);
		    printf("end;\n");
		    free((yyvsp[(4) - (5)]).str);
		}
		free(curSpell);
		curSpell = NULL;
	    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 851 "parser.y"
    {
		if (rotMode != RM_STRICT) {
		    fprintf(stderr, "%d: nil only makes sense in a strict\n",
			    lineno);
		    exit(1);
		}
	        indent(0);
		printf("if (%s) then\n", (yyvsp[(3) - (4)]).str);
		indent(1);
		printf("return nil;\n");
		indent(0);
		printf("end\n");
		free((yyvsp[(3) - (4)]).str);
	    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 866 "parser.y"
    {
	    	/*
		 *  This is pretty ugly.  Because of nested switch statements
		 *  we save off the one we are currently processing and
		 *  replace with our own (so our cases refer to the right
		 *  variable), and then restore when we are done with our
		 *  switch.  This is cleaner than using $-2.intg or some
		 *  such YACC trick.
		 */
	        int count = switchcount++;

		if (count > MAX_SWITCH_COUNT) {
		    fprintf(stderr, "Too many switches (%d max)\n", 
		    	    MAX_SWITCH_COUNT);
		    exit(1);
		}
		(yyval).intg = cur_switchnum;
		cur_switchnum = count;
		seen_case[count] = 0;

		indent(0);
		printf("local switchvar%d = %s;\n", count, (yyvsp[(3) - (5)]).str);
		free((yyvsp[(3) - (5)]).str);
	    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 891 "parser.y"
    {
	    	cur_switchnum = (yyvsp[(6) - (9)]).intg;
		indent(0);
		printf("end\n");
	    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 897 "parser.y"
    {
	      indent(0);
	      printf("while %s do\n", (yyvsp[(3) - (5)]).str);
	      free((yyvsp[(3) - (5)]).str);
	      indent_level++;
	  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 904 "parser.y"
    {
	      indent_level--;
	      indent(0);
	      printf("end;\n");
	  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 911 "parser.y"
    {
	      indent(0);
	      printf("if %s then\n", (yyvsp[(3) - (5)]).str);
	      free((yyvsp[(3) - (5)]).str);
	      indent_level++;
	  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 918 "parser.y"
    {
	      indent_level--;
	      indent(0);
	      printf("end;\n");
	  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 926 "parser.y"
    {
	      indent(0);
	      printf("%s = %s\n", (yyvsp[(1) - (4)]).str, (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(1) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 933 "parser.y"
    { (yyval).str = strdup(yytext); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 935 "parser.y"
    {
	      indent(0);
	      printf("DestroDummies:genfunc_%s(%s);\n", (yyvsp[(3) - (7)]).str, (yyvsp[(5) - (7)]).str);
	      free((yyvsp[(3) - (7)]).str);
	      free((yyvsp[(5) - (7)]).str);
	  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 943 "parser.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 945 "parser.y"
    {
	    	(yyval).str = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 3);
		sprintf((yyval).str, "%s, %s", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
		free((yyvsp[(1) - (3)]).str);
		free((yyvsp[(3) - (3)]).str);
	    ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 955 "parser.y"
    {
	    	(yyval).str = amalloc(strlen(yytext) + 4);
		sprintf((yyval).str, "var%s", yytext);
	    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 962 "parser.y"
    { (yyval).str = astrdup(yytext); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 966 "parser.y"
    {
	      if ((yyvsp[(2) - (2)]).str != NULL) {
		  (yyval).str = amalloc(strlen((yyvsp[(1) - (2)]).str) + strlen((yyvsp[(2) - (2)]).str) + 3);
		  sprintf((yyval).str, "%s[%s]", (yyvsp[(1) - (2)]).str, (yyvsp[(2) - (2)]).str);
		  free((yyvsp[(1) - (2)]).str);
		  free((yyvsp[(2) - (2)]).str);
	      } else
		  (yyval) = (yyvsp[(1) - (2)]);
	  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 976 "parser.y"
    {
	      if ((yyvsp[(2) - (2)]).str != NULL) {
		  (yyval).str = amalloc(strlen((yyvsp[(1) - (2)]).str) + strlen((yyvsp[(2) - (2)]).str) + 3);
		  sprintf((yyval).str, "%s[%s]", (yyvsp[(1) - (2)]).str, (yyvsp[(2) - (2)]).str);
		  free((yyvsp[(1) - (2)]).str);
		  free((yyvsp[(2) - (2)]).str);
	      } else
		  (yyval) = (yyvsp[(1) - (2)]);
	  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 998 "parser.y"
    { seen_a_case = 0; ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1000 "parser.y"
    { printf(") then\n"); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1002 "parser.y"
    { indent_level--; ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1011 "parser.y"
    {
	      if (!seen_a_case) {
		  indent(0);
		  printf("%s ((switchvar%d == (%s)) ", 
			   seen_case[cur_switchnum] ? "elseif" : "if",
			   cur_switchnum, (yyvsp[(2) - (3)]).str);
		  seen_case[cur_switchnum] = 1;
		  indent_level++;
		  seen_a_case = 1;
	      }	else
	      	  printf("or (switchvar%d == (%s))", cur_switchnum, (yyvsp[(2) - (3)]).str);
	  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1028 "parser.y"
    {
	      indent(0);
	      printf("else\n");
	      indent_level++;
	  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1034 "parser.y"
    { indent_level--; ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1046 "parser.y"
    {
	      indent(-1);
	      printf("elseif (%s) then\n", (yyvsp[(3) - (5)]).str);
	      free((yyvsp[(3) - (5)]).str);
	  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1055 "parser.y"
    {
	      indent(-1);
	      printf("else\n");
	  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1063 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(2) - (3)]).str) + 3);
	      sprintf(newstr, "(%s)", (yyvsp[(2) - (3)]).str);
	      free((yyvsp[(2) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1072 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(2) - (2)]).str) + 250);
	      sprintf(newstr, "- (%s)", (yyvsp[(2) - (2)]).str);
	      free((yyvsp[(2) - (2)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1081 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(2) - (2)]).str) + 250);
	      sprintf(newstr, "not (%s)", (yyvsp[(2) - (2)]).str);
	      free((yyvsp[(2) - (2)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1090 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) and (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1100 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) or (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1110 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) + (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1120 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) - (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1130 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) * (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1140 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) / (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1150 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) > (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1160 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) < (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1170 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) >= (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1180 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) <= (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1190 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) == (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1200 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(strlen((yyvsp[(1) - (3)]).str) + strlen((yyvsp[(3) - (3)]).str) + 250);
	      sprintf(newstr, "(%s) ~= (%s)", (yyvsp[(1) - (3)]).str, (yyvsp[(3) - (3)]).str);
	      free((yyvsp[(1) - (3)]).str);
	      free((yyvsp[(3) - (3)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1210 "parser.y"
    {
	      char *newstr;
	      newstr = amalloc(strlen((yyvsp[(3) - (4)]).str) + 250);
	      sprintf(newstr, "math.abs(%s)", (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	      (yyval).str = newstr;
	  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1218 "parser.y"
    { (yyval).str = astrdup("nil"); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1219 "parser.y"
    { (yyval).str = astrdup("nil"); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1220 "parser.y"
    { (yyval).str = astrdup("true"); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1227 "parser.y"
    { (yyval).str = NULL; ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1228 "parser.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1236 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(250);
	      sprintf(newstr, "%d", (yyvsp[(1) - (1)]).intg);
	      (yyval).str = newstr;
	  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1246 "parser.y"
    {
	      char *newstr;

	      newstr = amalloc(250);
	      sprintf(newstr, "%0.2lf", (yyvsp[(1) - (1)]).fl);
	      (yyval).str = newstr;
	  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1257 "parser.y"
    { (yyval).str = proc_builtin1((yyvsp[(1) - (3)]).bi); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1259 "parser.y"
    {
	      (yyval).str = proc_builtin2((yyvsp[(1) - (4)]).bi, (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1264 "parser.y"
    {
	      (yyval).str = proc_builtin2((yyvsp[(1) - (4)]).bi, (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1269 "parser.y"
    {
	      (yyval).str = proc_builtin3((yyvsp[(1) - (4)]).bi, (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1274 "parser.y"
    {
	      (yyval).str = proc_builtin4((yyvsp[(1) - (4)]).bi, (yyvsp[(3) - (4)]).str);
	      free((yyvsp[(3) - (4)]).str);
	  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1279 "parser.y"
    {
	      (yyval).str = proc_builtin5((yyvsp[(1) - (8)]).bi, (yyvsp[(3) - (8)]).str, (yyvsp[(5) - (8)]).str, (yyvsp[(7) - (8)]).str);
	      free((yyvsp[(3) - (8)]).str);
	      free((yyvsp[(5) - (8)]).str);
	      free((yyvsp[(7) - (8)]).str);
	  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1286 "parser.y"
    { (yyval).str = proc_builtin_mx((yyvsp[(1) - (4)]).bi, (yyvsp[(3) - (4)]).intg); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1288 "parser.y"
    {
	      (yyval).str = proc_builtin_mx2((yyvsp[(1) - (6)]).bi, (yyvsp[(3) - (6)]).intg, (yyvsp[(5) - (6)]).str);
	      free((yyvsp[(5) - (6)]).str);
	  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1295 "parser.y"
    {
	      if (curSpell == NULL) {
		  /*
		   *  Better than a SEGV, though it should never happen
		   *
		   *  Actually, this can happen (and has) in a bizarre 
		   *  circumstance which is a pain in the ass to figure out.
		   *  If you are referring to a spell outside the normal
		   *  combat/nocombat routines (say, in the postspell
		   *  handler), and the spell doesn't exist in strings.c,
		   *  you will end up here (because the spell is returned
		   *  as T_STRING instead of T_SPELL).
		   */
		  fprintf(stderr, "%d: curSpell set to NULL???\n", lineno);
		  fprintf(stderr, "Perhaps the spell is not in strings.c?\n");
		  exit(1);
	      }
	      (yyval).str = astrdup(curSpell);
	  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1318 "parser.y"
    {
	      (yyval).str = amalloc(strlen(yytext) + 100);
	      sprintf((yyval).str, "DestroDummies:xlateString(%s)", yytext);
	  ;}
    break;



/* Line 1455 of yacc.c  */
#line 3669 "parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1323 "parser.y"


static void
indent (int extra)
{
    int i;

    for (i = 0 ; i < 4 * (indent_level + extra) ; i++)
	putchar(' ');
}

int
main (int argc, char *argv[])
{
    int c;
    int lflg = 0;
    char *inFN = NULL, cmd[1024];
    FILE *pp;

    while ((c = getopt(argc, argv, "Ddli:")) != EOF) {
	switch (c) {
	case 'D':	// parser debugging
#if YYDEBUG
	    yydebug = 1;
#endif
	    break;

	case 'd':
	    debugging++;
	    break;

	case 'i':
	    inFN = optarg;
	    break;

	case 'l':
	    lflg++;
	    break;

	default:
	    exit(1);
	}
    }

    verifyStrings();

    // just generate Locale.lua?
    if (lflg) {
	genLocale();
	exit(0);
    }

    if (inFN == NULL) {
	fprintf(stderr, "Error: no filename given\n");
	exit(1);
    }

    filename = inFN;
    lineno = 1;

    // run the C preprocessor on our input
    if (snprintf(cmd, sizeof(cmd), "%s %s", CMD_CPP, filename) >= sizeof(cmd)){
	fprintf(stderr, "cmd overflow\n");
	exit(1);
    }
    if ((pp = popen(cmd, "r")) == NULL) {
	fprintf(stderr, "popen(\"%s\"): %s\n", cmd, strerror(errno));
	exit(1);
    }

    lex_init(pp);
    while(yyparse())
	;
    exit(0);
}

/*
 *  convert a spell name to a clip variable for that spell.  Return a pointer
 *  to it in static storage (will be overwritten on the next call)
 */
static char *
spell2clip (char *orig_spell)
{
    static char *clipstring = NULL;
    char *c;
    int len, upnext;
    char buffer[100], *spell;

    strcpy(buffer, orig_spell);
    spell = buffer;
    
    len = strlen(spell);
    if (spell[0] != '"' || spell[len - 1] != '"') {
	fprintf(stderr, "spell name: %s badly formed!\n", spell);
	exit(1);
    }
    // clip the quotes
    spell[len - 1] = '\0';
    spell++;
    len -= 2;

    clipstring = realloc(clipstring, len + strlen("clip") + 1);
    strcpy(clipstring, "clip");
    for (upnext = 1, c = &clipstring[strlen(clipstring)] ; *spell ; spell++) {
	if (!isalpha(*spell)) {
	    upnext = 1;
	    continue;
	}
	if (upnext) {
	    *c++ = toupper(*spell);
	    upnext = 0;
	} else
	    *c++ = tolower(*spell);
    }
    *c = '\0';
    return clipstring;
}

/*
 *  We cannot use calls on Utility functions until we are inside the combat or
 *  nocombat routines (DestroDummies.lua will not have been loaded before we
 *  try to use them otherwise).  Fail if we are unable to use them
 */
static void
utilCheck (void)
{
    if (!utilOK) {
	fprintf(stderr, "Line %d: Attempt to use a utility function outside of called routine\n", lineno);
	exit(1);
    }
}

static char *
proc_builtin_mx (enum built_in proc, int mIndex)
{
    char *retval;

    switch (proc) {
    case BI_MXBUFF:
	utilCheck();
    	retval = amalloc(200);
	sprintf(retval, "DestroDummies:utilMxBuff(mIndexBase + %d)", mIndex);
	return retval;

    case BI_MXBUFF_BY_ME:
	utilCheck();
    	retval = amalloc(200);
	sprintf(retval, "DestroDummies:utilMxBuffByMe(mIndexBase + %d)",
		mIndex);
	return retval;

    case BI_MXDEBUFF:
	utilCheck();
    	retval = amalloc(200);
	sprintf(retval, "DestroDummies:utilMxDebuff(mIndexBase + %d)",
		mIndex);
	return retval;

    case BI_MXDEBUFF_BY_ME:
	utilCheck();
    	retval = amalloc(200);
	sprintf(retval, "DestroDummies:utilMxDebuffByMe(mIndexBase + %d)",
		mIndex);
	return retval;

    default:
        fprintf(stderr, "Unknown proc (%d) in proc_builtin_mx.  Var %d\n",
		proc, mIndex);
	exit(1);
    }
}

static char *
proc_builtin_mx2 (enum built_in proc, int mIndex, char *spell)
{
    char *retval;

    switch (proc) {
    case BI_MXNEED:
	utilCheck();
    	retval = amalloc(200);
	sprintf(retval, "DestroDummies:utilMxNeedDebuff(mIndexBase + %d, %s)",
		mIndex, spell);
	return retval;

    default:
        fprintf(stderr, "Unknown proc (%d) in proc_builtin_mx2.  Var %d\n",
		proc, mIndex);
	exit(1);
    }
}

static char *
proc_builtin1 (enum built_in proc)
{
    char *retval;
    
    switch (proc) {
    case BI_DUAL_WIELDING:
	return astrdup("(GetInventoryItemID(\"player\", DestroDummies.offhandSlotNum) ~= nil)");
	
    case BI_DISTANCE:
	return astrdup("DestroDummies:distanceToTarget()");

    case BI_FERRALMODE:
	utilCheck();
    	retval = astrdup("DestroDummiesPCdb.ferralMode");
	return retval;

    case BI_TAPPED:
	retval = astrdup("(UnitIsTapped(\"target\") and not UnitIsTappedByPlayer(\"target\"))");
	return retval;		 

    case BI_BOSS:
	utilCheck();
    	retval = astrdup("DestroDummies:utilBoss()");
	return retval;

    case BI_ONTANK:
	utilCheck();
    	retval = astrdup("DestroDummies:utilOnTank()");
	return retval;
	
    case BI_UPTIME:
	return astrdup("GetTime()");
	
    case BI_COMBOPOINTS:
	return astrdup("GetComboPoints(\"player\", \"target\")");
	
    case BI_POWER:
	utilCheck();
	return astrdup("((DestroDummies:unitPowerAfterCast() * 100) / UnitPowerMax(\"player\"))");

    case BI_RAWPOWER:
	utilCheck();
	return astrdup("DestroDummies:unitPowerAfterCast()");
	
    case BI_MOUNTED:
    	return astrdup("IsMounted()");

    case BI_FLAGGEDPVP:
	return astrdup("UnitIsPVP(\"player\")");

    case BI_FLIGHTTIME:
	utilCheck();
    	retval = astrdup("DestroDummies:utilFlightTime()");
	return retval;
	
    case BI_THREAT:
	utilCheck();
    	retval = astrdup("DestroDummies:utilThreat()");
	return retval;

    case BI_HASPET:
    	// Note that UnitExists("pet") won't work because it will return
	// true when you are on a vehicle even though the pet isn't out
    	retval = astrdup("HasPetUI()");
	return retval;

    case BI_PETEXISTS:
    	retval = astrdup("UnitExists(\"pet\")");
	return retval;

    case BI_PETHEALTH:
	utilCheck();
    	retval = astrdup("DestroDummies:utilHealth(\"pet\")");
	return retval;

    case BI_PLAYERHEALTH:
	utilCheck();
    	retval = astrdup("DestroDummies:utilHealth(\"player\")");
	return retval;

    case BI_RAWPLAYERHEALTH:
    	retval = astrdup("UnitHealth(\"player\")");
	return retval;

    case BI_RAWPLAYERMAXHEALTH:
    	retval = astrdup("UnitHealthMax(\"player\")");
	return retval;

    case BI_TARGETHEALTH:
	utilCheck();
    	retval = astrdup("DestroDummies:utilHealth(\"target\")");
	return retval;

    case BI_TIMETODIE:
	utilCheck();
    	retval = astrdup("DestroDummies:utilTimeToDie()");
	return retval;

    case BI_IN_PARTY:
	retval = astrdup("IsInGroup()");
	return retval;
	
    case BI_IN_RAID:
	retval = astrdup("IsInRaid()");
	return retval;
	
    case BI_SOLO:
	retval = astrdup("(not IsInGroup())");
	return retval;
	
    case BI_SOULSHARDCOUNT:
	retval = astrdup("UnitPower(\"player\", SPELL_POWER_SOUL_SHARDS)");
	return retval;
	
    case BI_HOLY_POWER:
	retval = astrdup("UnitPower(\"player\", SPELL_POWER_HOLY_POWER)");
	return retval;

    case BI_LUNAR_POWER:
	retval = astrdup("UnitPower(\"player\", SPELL_POWER_ECLIPSE)");
	return retval;

	
    default:
	fprintf(stderr,
		"Error: call to proc_builtin1 with unknown proc (%d)\n", proc);
	exit(1);
    }
}

static char *
proc_builtin2 (enum built_in proc, char *spell)
{
    char *retval;
    
    switch (proc) {
    case BI_TOTEM: {
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilTotem(%s)", spell);
	return retval;
    }

    case BI_BUFF: {
	char *clip;

	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval,
		"DestroDummies:utilBuff(%s, %s, \"player\", \"HELPFUL\", nil)",
	 	spell, clip);
	return retval;
    }

    case BI_BUFF_BY_ME: {
	char *clip;

	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval,
		"DestroDummies:utilBuff(%s, %s, \"player\", \"HELPFUL\", \"player\")",
	 	spell, clip);
	return retval;
    }

    case BI_PLAYER_DEBUFF: {
	char *clip;

	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval,
		"DestroDummies:utilBuff(%s, %s, \"player\", \"HARMFUL\", nil)",
	 	spell, clip);
	return retval;
    }

    case BI_PETBUFF: {
	char *clip;

	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval,
		"DestroDummies:utilBuff(%s, %s, \"pet\", \"HELPFUL\", nil)",
		spell, clip);
	return retval;
    }

    case BI_DEBUFF_BY_ME: {
	char *clip;
	
	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval, "DestroDummies:utilDebuffByMe(%s, %s)", spell, clip);
	return retval;
    }

    case BI_DEBUFF_BY_ANY: {
	char *clip;
	
	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(spell) + strlen(clip));
	sprintf(retval, "DestroDummies:utilDebuff(%s, %s)", spell, clip);
	return retval;
    }

    case BI_BUFF_TIME:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilBuffTime(%s)", spell);
	return retval;

    case BI_DEBUFF_TIME:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilDebuffTime(%s)", spell);
	return retval;

    case BI_BUFF_INFO:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilBuffInfo(%s, false, \"player\")",
		spell);
	return retval;

    case BI_DEBUFF_BY_ME_INFO:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilDebuffByMeInfo(%s)", spell);
	return retval;

    case BI_PETBUFF_INFO:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilBuffInfo(%s, false, \"pet\")",
		spell);
	return retval;
	
    case BI_DEBUFF_INFO:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilBuffInfo(%s, true, \"target\")",
		spell);
	return retval;

    case BI_PLAYER_DEBUFF_INFO:
	utilCheck();
	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilBuffInfo(%s, true, \"player\")",
		spell);
	return retval;

    case BI_CREATURETYPE:
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "(UnitCreatureType(\"target\") == %s)", spell);
	return retval;
	
    case BI_LASTCAST: 
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "(DestroDummies.lastSpell == %s)", spell);
	return retval;

    case BI_KNOWSSPELL:
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "(GetSpellInfo(%s) ~= nil)", spell);
	return retval;

    case BI_CASTABLE:
	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:spellReady(%s)", spell);
	return retval;

    case BI_REQUIREDPOWER:
	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilRequiredPower(%s)", spell);
	return retval;

    case BI_INRANGE:
    	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilSpellInRange(%s)", spell);
	return retval;

    case BI_RANGED: 
    	// Any distance > 9.9 yards is always ranged, or check spell
    	retval = amalloc(250 + strlen(spell));
    	sprintf(retval, "((CheckInteractDistance(\"target\", 3) == nil) or (IsSpellInRange(%s) == 1))", spell);
	return retval;

    case BI_HASGLYPH:
	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilHasGlyph(%s)", spell);
	return retval;

    case BI_WEAPONENHANCE: {
	char *clip;
	
	utilCheck();
	clip = find_clip(spell);
	retval = amalloc(250 + strlen(clip));
	sprintf(retval, "DestroDummies:utilWeaponEnhance(%s)", clip);
	return retval;
    }

    case BI_GCD: {
        char *upname;

	utilCheck();
	upname = upstring(classname);
    	retval = amalloc(250 + strlen(spell) + strlen(upname));
    	sprintf(retval, "(DestroDummies:computeGCD(\"%s\", %s))",upname,spell);
	free(upname);
	return retval;
    }

    case BI_COOLDOWN:
	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:cooldownTime(%s)", spell);
	return retval;

    case BI_TALENT:
	utilCheck();
    	retval = amalloc(250 + strlen(spell));
	sprintf(retval, "DestroDummies:utilTalentCount(%s)", spell);
	return retval;
	
    default:
	fprintf(stderr,
		"Error: call to proc_builtin2 with unknown proc (%d)\n", proc);
	fprintf(stderr, "Spell was %s\n", spell);
	exit(1);
    }
}

static char *
proc_builtin3 (enum built_in proc, char *expr)
{
    char *retval;
    
    switch (proc) {
    case BI_STANCE:
	utilCheck();
	retval = amalloc(250 + strlen(expr));
	sprintf(retval, "DestroDummies:utilStance(%s)", expr);
	return retval;

    case BI_FLOOR:
	retval = amalloc(250 + strlen(expr));
	sprintf(retval, "math.floor(%s)", expr);
	return retval;

    case BI_TOTEMTYPE:
	utilCheck();
	retval = amalloc(250 + strlen(expr));
	sprintf(retval, "DestroDummies:utilTotemType(%s)", expr);
	return retval;

    default:
	fprintf(stderr,
		"Error: call to proc_builtin3 with unknown proc (%d)\n", proc);
	fprintf(stderr, "Expr was %s\n", expr);
	exit(1);
    }
}

void
have_clip (char *spell, char *clip)
{
    struct cliplist *new;
    int len;

    new = amalloc(sizeof(*new));
    new->next = firstclip;
    firstclip = new;

    // special-case: spells ending in () don't get those put in the clip, 
    // because they won't show up in the buff/debuff we are looking at
    new->spell = astrdup(spell);
    len = strlen(new->spell);
    if (!strcmp(&new->spell[len - 3], "()\"")) {
	new->spell[len - 3] = '"';
	new->spell[len - 2] = '\0';
    }
    
    new->clip = astrdup(clip);

}

static char *
proc_builtin4 (enum built_in proc, char *runeOrig)
{
    // for mapping between useful strings and WoW rune numbers
    static struct {
	char *runestr;
	int runeid;
    } runemap[] = {
	{ "\"blood\"", 	1 },
	{ "\"unholy\"",	2 },
	{ "\"frost\"",	3 },
    };
    char *retval;
    int rNum, rune;

    for (rNum = 0 ; rNum < NELEM(runemap) ; rNum++)
	if (!strcasecmp(runemap[rNum].runestr, runeOrig))
	    break;
    if (rNum >= NELEM(runemap)) {
	fprintf(stderr, "%d: Unknown rune type %s\n", lineno, runeOrig);
	exit(1);
    }
    rune = runemap[rNum].runeid;

    switch (proc) {
    case BI_RUNE_COUNT:
	utilCheck();
	retval = amalloc(250);
	sprintf(retval, "DestroDummies:utilRuneCount(%d)", rune);
	return retval;

    default:
	fprintf(stderr,
		"Error: call to proc_builtin4 with unknown proc (%d)\n", proc);
	fprintf(stderr, "Rune was %s\n", runeOrig);
	exit(1);
    }
}

// built-ins that take 3 parameters
static char *
proc_builtin5 (enum built_in proc, char *p1, char *p2, char *p3)
{
    char *retval;
    
    switch (proc) {
    case BI_HAVE_RUNES:
	utilCheck();
	retval = amalloc(BIG_NUMBER);
	snprintf(retval, BIG_NUMBER, "DestroDummies:utilHaveRunes(%s, %s, %s)",
		 p1, p2, p3);
	return retval;

    default:
	fprintf(stderr,
		"Error: call to %s with unknown proc (%d)\n",__FUNCTION__,proc);
	exit(1);
    }
}
	

char *
find_clip (char *spell_full)
{
    struct cliplist *cur;
    char spellbuf[1024], *spell, *end;
    int len;

    // make a working copy
    strcpy(spellbuf, spell_full);

    // it's probably in DestroDummies:xlateString(%s) format.  Unlocalize it
    if ((spell = strchr(spellbuf, '(')) == NULL) {
	// hmm, it wasn't.  That shouldn't happen
	fprintf(stderr, "We seem to be missing localization--call to %s(%s)\n",
		__FUNCTION__, spell_full);
	exit(1);
    }
    spell++;
    if ((end = strrchr(spell, ')')) == NULL) {
	fprintf(stderr, "missing ')' in %s(%s)\n", __FUNCTION__, spell_full);
	exit(1);
    }
    *end = '\0';
 
    // special-case the spells ending in ()--were removed when stored, so
    // remove now
    len = strlen(spell);
    if (!strcmp(&spell[len - 3], "()\"")) {
	spell[len - 3] = '"';
	spell[len - 2] = '\0';
    }
    
    for (cur = firstclip ; cur != NULL ; cur = cur->next) {
	if (!strcmp(cur->spell, spell))
	    return cur->clip;
    }
    return "0";
}

// convert str to start with upper case, the rest lower case
char *
normalize (char *string)
{
    char *str;

    *string = toupper(*string);
    for (str = &string[1] ; *str != '\0' ; str++)
    	*str = tolower(*str);

    return string;
}

// Allocate memory to copy the string to, making it all upper case
static char *
upstring (char *string)
{
    char *upstr, *cur;

    upstr = astrdup(string);
    for (cur = upstr ; *cur != '\0' ; cur++)
    	*cur = toupper(*cur);
    return upstr;
}

char *
astrdup (char *from)
{
    char *retval;

    if ((retval = strdup(from)) == NULL) {
	perror("strdup");
	exit(1);
    }
    return retval;
}

void *
amalloc (int n)
{
    void *retval;

    if ((retval = malloc(n)) == NULL) {
	perror("malloc");
	exit(1);
    }
    return retval;
}

int
yyerror (char *str)
{
    fprintf(stderr, "%s[%d]: %s at %s\n", filename, lineno, str, yytext);
    exit(1);
}


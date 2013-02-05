%{
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
%}

%token T_CLASS T_BUILD T_COMBAT T_OUTOFCOMBAT T_CLIP T_STRICT T_FLOAT T_INTEGER
%token T_PRECOMBAT T_POSTSPELL T_SPELLSENT T_NIL T_ID T_SPELL T_ABSVAL
%token T_STRING T_TAKE_DAMAGE T_TAKE_HEAL
%token T_TRUE T_FALSE
%token T_BUILTIN_1 T_BUILTIN_2 T_BUILTIN_2M T_BUILTIN_3 
%token T_BUILTIN_4 T_BUILTIN_5 T_BUILTIN_MX T_BUILTIN_MX2
%token T_SWITCH T_CASE T_DEFAULT T_IF T_ELSEIF T_ELSE T_EXTERN T_FUNCTION
%token T_WHILE
%token T_ALTERNATE T_MUTEX T_MUTEXVAR T_FREE T_FAIL T_PRINT T_CALL
%token T_VAR		// the keyword "var"
%token T_VARIABLE 	// a variable declared with a T_VAR
%token T_EXTERNVARIABLE	// a variable that we don't put "var" in front of

%left T_OR 			    /* lowest precedence */
%left T_AND
%left T_GEQ T_NEQ T_LEQ T_EQ '<' '>'
%left '+' '-'
%left '*' '/'
%left '!'
%left UNIMINUS                      /* highest precedence */

%%

file	: header .optstuff { utilOK = 1; } calledRoutines
	  {
	      printf("-- populate our registries\n");
	      printf("DestroDummies.registry.decideCombat[DestroDummies.class%s%s] = PickCombat;\n", classname, buildname);
	      printf("DestroDummies.registry.decideNoCombat[DestroDummies.class%s%s] = PickNoCombat;\n", classname, buildname);
	      printf("DestroDummies.lastMutexIndex = mIndexBase + %d;\n", mIndex);
	  }
	;

header	: T_CLASS ':' T_ID { classname = normalize(astrdup(yytext)); }
	  T_BUILD ':' T_ID { buildname = normalize(astrdup(yytext)); }
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
	  }
	;

vardecl : T_VAR vardecl_line ';'
	| T_MUTEX mutexname
	  {
	      curvar = amalloc(200);
	      sprintf(curvar, 
	      	      "DestroDummies.mutex[mIndexBase + %d]",
		      ++mIndex);
	      lex_register_mutex(yytext, mIndex);
	      indent(0);
	      printf("%s = {};\n", curvar);
	      cur_mutexno = 0;
	  }
	  .mutexclip '{' mx_spellist '}' ';'
	  {
	      indent(0);
	      printf("%s.count = %d;\n", curvar, cur_mutexno);
	  }
	| T_EXTERN T_ID { lex_register_variable(yytext, T_EXTERNVARIABLE); } ';'
	;

.mutexclip
	: /* epsilon */
	    {
	    	indent(0);
	    	printf("%s.clipCombat = 0;\n", curvar);
	    	printf("%s.clipOOCombat = 0;\n", curvar);
	    }
	| '(' T_CLIP '=' exp ')'
	    {
	    	indent(0);
	    	printf("%s.clipCombat = %s;\n", curvar, $4.str);
	    	printf("%s.clipOOCombat = %s;\n", curvar, $4.str);
	    }
	| '(' T_CLIP '=' exp ',' exp ')'
	    {
	    	indent(0);
	    	printf("%s.clipOOCombat = %s;\n", curvar, $4.str);
	    	printf("%s.clipCombat = %s;\n", curvar, $6.str);
	    }
	;

mx_spellist
	: mx_spell ','
	| mx_spellist mx_spell ','
	;

mx_spell: T_SPELL 
	  {
	      indent(0);
	      printf("%s[%d] = DestroDummies:xlateString(%s)\n",
		      curvar, ++cur_mutexno, yytext);
	  }
	;


vardecl_line
	: vardeclent
	| vardecl_line ',' vardeclent
	;

vardeclent
	: varname
	  {
	      // note that something very similar is done at the 
	      // parameter production.  these ought to be unified, but
	      // for now, just note that any changes here probably have
	      // to be made there too
	      curvar = amalloc(strlen(yytext) + 4);
	      sprintf(curvar, "var%s", yytext);
	      lex_register_variable(yytext, T_VARIABLE);
	  }
	  .array
	  {
	      curvar_array = $3.intg;
	      if (curvar_array) {
		  indent(0);
	          printf("local %s = {};\n", curvar);
	      }
	  }
	  .init 
	  {
	      if (!curvar_array) {
		  indent(0);
	          printf("local %s", curvar);
		  if ($5.str != NULL) {
		      printf(" = %s", $5.str);
		      free($5.str);
		  }
		  printf(";\n");
	      }

	      
	      free(curvar);
	      curvar = NULL;
	  }
	;

mutexname
	: T_ID
	| T_MUTEXVAR	// in case it's already been declared before
	;

varname : T_ID
	| T_VARIABLE	// in case it's already been declared before
	;

.array	: /* epsilon */		{ $$.intg = 0; }
	| '[' ']'		{ $$.intg = 1; }
	;

.init	: /* epsilon */ 	{ $$.str = NULL; }
	| single_init 		{ $$.str = $1.str; }
	| array_init		{ $$.str = NULL; }
	;

single_init
	: '=' exp
	  {
	      if (curvar_array) {
		  fprintf(stderr, "[%d]: Error: %s is an array, cannot single init it.\n", lineno, curvar);
		  exit(1);
	      }
	      $$ = $2;
	  }
	;

array_init
	: '=' '{' array_init_lines '}' 
	;

array_init_lines
	: array_init_line
	| array_init_lines array_init_line
	;

array_init_line
	: '[' exp ']' exp ','
	  {
	      if (!curvar_array) {
		  fprintf(stderr, "[%d]: Error: %s is not an array.  Cannot aggregate init it.\n", lineno, curvar);
		  exit(1);
	      }
	      printf("%s[%s] = %s;\n", curvar, $2.str, $4.str);
	      free($2.str);
	      free($4.str);
	  }
	;


.optstuff
	: // epsilon
	| optstuff
	;

optstuff: optthang
	| optstuff optthang
	;


optthang: vardecl
        ;

calledRoutines
	: calledRoutine
	| calledRoutines calledRoutine
	;

calledRoutine
	: incombat 
	| oocombat
	| T_PRECOMBAT '{' 
	    {
	    	printf("local function preCombat()\n");
		indent_level++;
	    }
	  rotation '}'
	    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.preCombat[DestroDummies.class%s%s] = preCombat;\n\n", classname, buildname);
	    }
	| T_POSTSPELL '(' parameter ',' parameter ')' '{'
	    {
		// called with parameters:
		//     first:  spell name
		//     second: true if spell succeeded
		printf("local function postSpell(%s, %s)\n", $3.str, $5.str);
		free($3.str);
		free($5.str);
		indent_level++;
	    }
	  rotation '}'
	    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.postSpell[DestroDummies.class%s%s] = postSpell;\n\n", classname, buildname);
	    }
	| T_TAKE_DAMAGE '(' parameter ')' '{'
	    {
	    	printf("local function takeDamage(%s)\n", $3.str);
		free($3.str);
		indent_level++;
	    }
	  rotation '}'
	    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.takeDamage[DestroDummies.class%s%s] = takeDamage;\n\n", classname, buildname);
	    }

	| T_TAKE_HEAL '(' parameter ')' '{'
	    {
	    	printf("local function takeHeal(%s)\n", $3.str);
		free($3.str);
		indent_level++;
	    }
	  rotation '}'
	    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.takeHeal[DestroDummies.class%s%s] = takeHeal;\n\n", classname, buildname);
	    }
	
	| T_SPELLSENT '(' parameter ')' '{' 
	    {
	    	printf("local function spellSent(%s)\n", $3.str);
		free($3.str);
		indent_level++;
	    }
	  rotation '}'
	    {
	    	indent_level--;
		printf("end\n");
		printf("DestroDummies.registry.spellSent[DestroDummies.class%s%s] = spellSent;\n\n", classname, buildname);
	    }
	| T_FUNCTION T_ID
	    { $$.str = strdup(yytext); }
	'(' .paramlist ')' '{'
	    {
	    	printf("function DestroDummies:genfunc_%s(%s)\n",
		       $3.str, $5.str);
		free($3.str);
		free($5.str);
		indent_level++;

		$$.intg = utilOK;
		utilOK = 1;	// this will be called from where it's ok
	    }
	rotation '}'
	    {
		utilOK = $8.intg;
	    	indent_level--;
		printf("end\n\n");
	    }
	;

.paramlist
	: /* epsilon */		{ $$.str = strdup(""); }
	| paramlist
	;

paramlist
	: parameter
	| paramlist ',' parameter
	    {
		$$.str = amalloc(strlen($1.str) + strlen($3.str) + 3);
		sprintf($$.str, "%s, %s", $1.str, $3.str);
		free($1.str);
		free($3.str);
	    }
	;

parameter
	: T_ID
	  {
	      $$.str = amalloc(strlen(yytext) + 4);
	      sprintf($$.str, "var%s", yytext);
	      lex_register_variable(yytext, T_VARIABLE);
	  }
	| exp
	;

incombat: T_COMBAT .pick_opts
	  {
	      printf("local function PickCombat()\n");
	      indent_level++;
	  }
	  '{' combatstuff '}' 
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
	  }
	;

.pick_opts
	: // epsilon
	| '(' just_one_opt ')'
	    {
		if (fail_spell != NULL) {
		    fprintf(stderr, "[%d]: too many fail cases\n", lineno);
		    exit(1);
		}
		fail_spell = $2.str;
	    }
	| '(' pick_opts ')'
	;

just_one_opt
	: T_SPELL { $$.str = astrdup(yytext); }
	| variable 
	;

pick_opts
	: pick_opt
	| pick_opts ',' pick_opt
	;

pick_opt: failspell
	    {
		if (fail_spell != NULL) {
		    fprintf(stderr, "[%d]: too many fail cases\n", lineno);
		    exit(1);
		}
		fail_spell = $1.str;
	    }
	| freespell
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
		free_spell = $1.str;
	    }
	;

failspell
	: T_FAIL '=' T_SPELL  	{ $$.str = astrdup(yytext); }
	| T_FAIL '=' variable 	{ $$ = $3; }
	;

freespell
	: T_FREE '=' T_SPELL 	{ $$.str = astrdup(yytext); }
	| T_FREE '=' variable	{ $$ = $3; }
	;

oocombat: T_OUTOFCOMBAT
	  {
	      printf("local function PickNoCombat()\n");
	      indent_level++;
	  }
	  .pick_opts
	  '{' combatstuff '}' 
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
	  }
	;

combatstuff
	: 
	  { indent(0); printf("DestroDummies:analysisInit();\n"); }
	  rotation
	;

alternate
	: T_ALTERNATE .alt_number '{' 
	  { rotMode = RM_ALTERNATE; }
	  rotation '}' ';'
	  { rotMode = RM_NORMAL; }
	;

.alt_number
	: /* epsilon */			{ altnum = 0; }
	| '(' T_INTEGER ')' 		{ altnum = $2.intg; }
	;

clips	: T_CLIP '{' clipents '}' ';'
	;

clipents: clip
	| clipents clip
	;

clip	: T_SPELL 
	  {
	      char *clip;

	      clip = spell2clip(yytext);
	      have_clip(yytext, clip);
	      indent(0);
	      printf("local %s = ", clip);
	  }
	  ':' exp ';'
	  { 
	      printf("%s;\n", $4.str);
	      free($4.str);
	  }
	;

strict	: T_STRICT '{' 
	  { rotMode = RM_STRICT; }
	  rotation '}'
	  { rotMode = RM_NORMAL; }
	  ';'
	;

rotation: rotation_entry
	| rotation rotation_entry
	;

.spell_predicate
	: /* epsilon */			{ $$.str = NULL; }
	| ':' exp 			{ $$ = $2; }
	;

print_statement
	: T_PRINT '(' stuff2print ')' ';'
	    { 
	    	indent(0);
		printf("print(%s);\n", $3.str);
		free($3.str);
	    }
	;

stuff2print
	: printThang
	| stuff2print ',' printThang
	    {
	    	$$.str = amalloc(strlen($1.str) + strlen($3.str) + 3);
		sprintf($$.str, "%s, %s", $1.str, $3.str);
		free($1.str);
		free($3.str);
	    }
	;

printThang
	: exp
	| string
	;

string  : T_STRING
	      { $$.str = astrdup(yytext); }
	;

/*
 *  Specify a spell who's ready time will serve as a minimum for this spell.
 *  This is typically used for a spell off the GCD that shouldn't be cast
 *  unless some other spell would otherwise be cast.  (For example, the only
 *  time a warlock should cast Soulburn is before Soul Fire)
 */
.spell_mintime
	: /* epsilon */		{ $$.str = NULL; }
	| '/' spell		{ $$ = $2; }
	;

rotation_entry
	: ';'		// useful inside if()'s
	| print_statement
	| strict
	| clips
	| spell
	    { curSpell = $1.str; }	// $1.str will be free()'d as curSpell
	  .spell_mintime .spell_predicate ';'
	    {
		if ($4.str != NULL) {
		    indent(0);
		    printf("if (%s) then\n", $4.str); 
		    indent_level++;
		}
		switch (rotMode) {
		case RM_NORMAL:
		    if (debugging) {
			indent(0);
			printf("if DestroDummies.debug then\n");
			indent(1);
			printf("print(\"Analysis Add \", %s", curSpell);
			if ($3.str != NULL)
			    printf(", %s", $3.str);
			printf(");\n");
			indent(0);
			printf("end\n");
		    }
		    indent(0);
		    printf("DestroDummies:analysisAdd(%s", curSpell);
		    if ($3.str != NULL)
			printf(", %s", $3.str);
		    printf(");\n");
		    break;
	      
		case RM_STRICT:
		    if ($3.str != NULL) {
		        fprintf(stderr, 
			  	"Cannot have a spell mintime with strict\n");
			exit(1);
		    }
		    indent(0);
		    printf("return %s;\n", curSpell);
		    break;

		case RM_ALTERNATE:
		    if ($3.str != NULL) {
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
		if ($3.str != NULL)
		    free($3.str);
		if ($4.str != NULL) {
		    indent_level--;
		    indent(0);
		    printf("end;\n");
		    free($4.str);
		}
		free(curSpell);
		curSpell = NULL;
	    }
	| T_NIL ':' exp ';'
	    {
		if (rotMode != RM_STRICT) {
		    fprintf(stderr, "%d: nil only makes sense in a strict\n",
			    lineno);
		    exit(1);
		}
	        indent(0);
		printf("if (%s) then\n", $3.str);
		indent(1);
		printf("return nil;\n");
		indent(0);
		printf("end\n");
		free($3.str);
	    }
	| T_SWITCH '(' exp ')' '{'
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
		$$.intg = cur_switchnum;
		cur_switchnum = count;
		seen_case[count] = 0;

		indent(0);
		printf("local switchvar%d = %s;\n", count, $3.str);
		free($3.str);
	    }
	  .case_statements .default_statement '}'
	    {
	    	cur_switchnum = $6.intg;
		indent(0);
		printf("end\n");
	    }
	| T_WHILE '(' exp ')' '{'
	  {
	      indent(0);
	      printf("while %s do\n", $3.str);
	      free($3.str);
	      indent_level++;
	  }
	  rotation '}'
	  {
	      indent_level--;
	      indent(0);
	      printf("end;\n");
	  }

        | T_IF '(' exp ')' '{'
	  {
	      indent(0);
	      printf("if %s then\n", $3.str);
	      free($3.str);
	      indent_level++;
	  }
	  rotation '}' .elseifs .else
	  {
	      indent_level--;
	      indent(0);
	      printf("end;\n");
	  }
	| vardecl
	| alternate
	| varlist '=' exp ';'
	  {
	      indent(0);
	      printf("%s = %s\n", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	  }
	| T_CALL T_ID
	  { $$.str = strdup(yytext); }
	'(' .paramlist ')' ';'
	  {
	      indent(0);
	      printf("DestroDummies:genfunc_%s(%s);\n", $3.str, $5.str);
	      free($3.str);
	      free($5.str);
	  }
	;

varlist : variable			{ $$ = $1; }
	| varlist ',' variable
	    {
	    	$$.str = amalloc(strlen($1.str) + strlen($3.str) + 3);
		sprintf($$.str, "%s, %s", $1.str, $3.str);
		free($1.str);
		free($3.str);
	    }
	;

t_variable
	: T_VARIABLE
	    {
	    	$$.str = amalloc(strlen(yytext) + 4);
		sprintf($$.str, "var%s", yytext);
	    }
	;

t_externvariable
	: T_EXTERNVARIABLE	 { $$.str = astrdup(yytext); }
	;

variable: t_variable .arrayindex
	  {
	      if ($2.str != NULL) {
		  $$.str = amalloc(strlen($1.str) + strlen($2.str) + 3);
		  sprintf($$.str, "%s[%s]", $1.str, $2.str);
		  free($1.str);
		  free($2.str);
	      } else
		  $$ = $1;
	  }
	| t_externvariable .arrayindex
	  {
	      if ($2.str != NULL) {
		  $$.str = amalloc(strlen($1.str) + strlen($2.str) + 3);
		  sprintf($$.str, "%s[%s]", $1.str, $2.str);
		  free($1.str);
		  free($2.str);
	      } else
		  $$ = $1;
	  }
	;

.case_statements
	: // epsilon
	| case_statements
	;

case_statements
	: case_statement
	| case_statements case_statement
	;

case_statement
	: { seen_a_case = 0; }
	  caselines 
	  { printf(") then\n"); }
	  '{' rotation '}'
	  { indent_level--; }
	;

caselines
	: one_case
	| caselines one_case
	;

one_case: T_CASE exp ':' 
	  {
	      if (!seen_a_case) {
		  indent(0);
		  printf("%s ((switchvar%d == (%s)) ", 
			   seen_case[cur_switchnum] ? "elseif" : "if",
			   cur_switchnum, $2.str);
		  seen_case[cur_switchnum] = 1;
		  indent_level++;
		  seen_a_case = 1;
	      }	else
	      	  printf("or (switchvar%d == (%s))", cur_switchnum, $2.str);
	  }
	;

.default_statement
	: // epsilon
	| T_DEFAULT ':' '{'
	  {
	      indent(0);
	      printf("else\n");
	      indent_level++;
	  }
	  rotation '}'
	  { indent_level--; }
	;

.elseifs: // epsilon
	| elseifs
	;
	
elseifs	: elseif
	| elseifs elseif
	;

elseif	: T_ELSEIF '(' exp ')' '{' 
	  {
	      indent(-1);
	      printf("elseif (%s) then\n", $3.str);
	      free($3.str);
	  }
	  rotation '}' 

.else   : // epsilon
        | T_ELSE '{'
	  {
	      indent(-1);
	      printf("else\n");
	  }
          rotation '}'
	;

exp	: '(' exp ')'
	  {
	      char *newstr;

	      newstr = amalloc(strlen($2.str) + 3);
	      sprintf(newstr, "(%s)", $2.str);
	      free($2.str);
	      $$.str = newstr;
	  }
	| '-' exp		%prec UNIMINUS
	  {
	      char *newstr;

	      newstr = amalloc(strlen($2.str) + 250);
	      sprintf(newstr, "- (%s)", $2.str);
	      free($2.str);
	      $$.str = newstr;
	  }
	| '!' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($2.str) + 250);
	      sprintf(newstr, "not (%s)", $2.str);
	      free($2.str);
	      $$.str = newstr;
	  }
	| exp T_AND exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) and (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp T_OR exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) or (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '+' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) + (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '-' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) - (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '*' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) * (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '/' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) / (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '>' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) > (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp '<' exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) < (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp T_GEQ exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) >= (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp T_LEQ exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) <= (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp T_EQ exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) == (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| exp T_NEQ exp
	  {
	      char *newstr;

	      newstr = amalloc(strlen($1.str) + strlen($3.str) + 250);
	      sprintf(newstr, "(%s) ~= (%s)", $1.str, $3.str);
	      free($1.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| T_ABSVAL '(' exp ')'
	  {
	      char *newstr;
	      newstr = amalloc(strlen($3.str) + 250);
	      sprintf(newstr, "math.abs(%s)", $3.str);
	      free($3.str);
	      $$.str = newstr;
	  }
	| number
	| T_NIL		{ $$.str = astrdup("nil"); }
	| T_FALSE	{ $$.str = astrdup("nil"); }
	| T_TRUE	{ $$.str = astrdup("true"); }
	| spell
	| builtin
	| variable
	;

.arrayindex
	: /* epsilon */		{ $$.str = NULL; }
	| '[' exp ']' 		{ $$ = $2; }
	;

number	: integer
	| float
	;

integer	: T_INTEGER
	  {
	      char *newstr;

	      newstr = amalloc(250);
	      sprintf(newstr, "%d", $1.intg);
	      $$.str = newstr;
	  }
	;

float	: T_FLOAT
	  {
	      char *newstr;

	      newstr = amalloc(250);
	      sprintf(newstr, "%0.2lf", $1.fl);
	      $$.str = newstr;
	  }
	;


builtin : T_BUILTIN_1 '(' ')'
	  { $$.str = proc_builtin1($1.bi); }
	| T_BUILTIN_2 '(' .spell ')'
	  {
	      $$.str = proc_builtin2($1.bi, $3.str);
	      free($3.str);
	  }
	| T_BUILTIN_2M '(' spell ')'
	  {
	      $$.str = proc_builtin2($1.bi, $3.str);
	      free($3.str);
	  }
	| T_BUILTIN_3 '(' exp ')'
	  {
	      $$.str = proc_builtin3($1.bi, $3.str);
	      free($3.str);
	  }
	| T_BUILTIN_4 '(' string ')'
	  {
	      $$.str = proc_builtin4($1.bi, $3.str);
	      free($3.str);
	  }
	| T_BUILTIN_5 '(' exp ',' exp ',' exp ')'
	  {
	      $$.str = proc_builtin5($1.bi, $3.str, $5.str, $7.str);
	      free($3.str);
	      free($5.str);
	      free($7.str);
	  }
	| T_BUILTIN_MX '(' T_MUTEXVAR ')'
	  { $$.str = proc_builtin_mx($1.bi, $3.intg); }
	| T_BUILTIN_MX2 '(' T_MUTEXVAR ',' spell ')'
	  {
	      $$.str = proc_builtin_mx2($1.bi, $3.intg, $5.str);
	      free($5.str);
	  }
	;

.spell 	: // epsilon -- return the current one
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
	      $$.str = astrdup(curSpell);
	  }
	| exp
	;

spell	: T_SPELL
	  {
	      $$.str = amalloc(strlen(yytext) + 100);
	      sprintf($$.str, "DestroDummies:xlateString(%s)", yytext);
	  }
	;
%%

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

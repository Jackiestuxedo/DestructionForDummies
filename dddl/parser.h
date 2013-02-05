
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

extern YYSTYPE yylval;



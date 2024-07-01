/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* First part of user prologue.  */
#line 1 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"

#include "yacc.tab.h"
#include <iostream>
#include <memory>
#include "ast.h"

int yylex(YYSTYPE* yylval, YYLTYPE* yylloc);

void yyerror(YYLTYPE* locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column "
              << locp->first_column << ": " << s << std::endl;
}

using namespace ast;

#line 86 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"

#ifndef YY_CAST
#ifdef __cplusplus
#define YY_CAST(Type, Val) static_cast<Type>(Val)
#define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type>(Val)
#else
#define YY_CAST(Type, Val) ((Type)(Val))
#define YY_REINTERPRET_CAST(Type, Val) ((Type)(Val))
#endif
#endif
#ifndef YY_NULLPTR
#if defined __cplusplus
#if 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#else
#define YY_NULLPTR ((void*)0)
#endif
#endif

#include "yacc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t {
    YYSYMBOL_YYEMPTY = -2,
    YYSYMBOL_YYEOF = 0,             /* "end of file"  */
    YYSYMBOL_YYerror = 1,           /* error  */
    YYSYMBOL_YYUNDEF = 2,           /* "invalid token"  */
    YYSYMBOL_SHOW = 3,              /* SHOW  */
    YYSYMBOL_TABLES = 4,            /* TABLES  */
    YYSYMBOL_CREATE = 5,            /* CREATE  */
    YYSYMBOL_TABLE = 6,             /* TABLE  */
    YYSYMBOL_DROP = 7,              /* DROP  */
    YYSYMBOL_DESC = 8,              /* DESC  */
    YYSYMBOL_INSERT = 9,            /* INSERT  */
    YYSYMBOL_INTO = 10,             /* INTO  */
    YYSYMBOL_VALUES = 11,           /* VALUES  */
    YYSYMBOL_DELETE = 12,           /* DELETE  */
    YYSYMBOL_FROM = 13,             /* FROM  */
    YYSYMBOL_ASC = 14,              /* ASC  */
    YYSYMBOL_ORDER = 15,            /* ORDER  */
    YYSYMBOL_GROUP = 16,            /* GROUP  */
    YYSYMBOL_BY = 17,               /* BY  */
    YYSYMBOL_HAVING = 18,           /* HAVING  */
    YYSYMBOL_SUM = 19,              /* SUM  */
    YYSYMBOL_COUNT = 20,            /* COUNT  */
    YYSYMBOL_MAX = 21,              /* MAX  */
    YYSYMBOL_MIN = 22,              /* MIN  */
    YYSYMBOL_AS = 23,               /* AS  */
    YYSYMBOL_WHERE = 24,            /* WHERE  */
    YYSYMBOL_UPDATE = 25,           /* UPDATE  */
    YYSYMBOL_SET = 26,              /* SET  */
    YYSYMBOL_SELECT = 27,           /* SELECT  */
    YYSYMBOL_INT = 28,              /* INT  */
    YYSYMBOL_CHAR = 29,             /* CHAR  */
    YYSYMBOL_FLOAT = 30,            /* FLOAT  */
    YYSYMBOL_INDEX = 31,            /* INDEX  */
    YYSYMBOL_AND = 32,              /* AND  */
    YYSYMBOL_JOIN = 33,             /* JOIN  */
    YYSYMBOL_EXIT = 34,             /* EXIT  */
    YYSYMBOL_HELP = 35,             /* HELP  */
    YYSYMBOL_TXN_BEGIN = 36,        /* TXN_BEGIN  */
    YYSYMBOL_TXN_COMMIT = 37,       /* TXN_COMMIT  */
    YYSYMBOL_TXN_ABORT = 38,        /* TXN_ABORT  */
    YYSYMBOL_TXN_ROLLBACK = 39,     /* TXN_ROLLBACK  */
    YYSYMBOL_ORDER_BY = 40,         /* ORDER_BY  */
    YYSYMBOL_GROUP_BY = 41,         /* GROUP_BY  */
    YYSYMBOL_ENABLE_NESTLOOP = 42,  /* ENABLE_NESTLOOP  */
    YYSYMBOL_ENABLE_SORTMERGE = 43, /* ENABLE_SORTMERGE  */
    YYSYMBOL_LEQ = 44,              /* LEQ  */
    YYSYMBOL_NEQ = 45,              /* NEQ  */
    YYSYMBOL_GEQ = 46,              /* GEQ  */
    YYSYMBOL_IN = 47,               /* IN  */
    YYSYMBOL_T_EOF = 48,            /* T_EOF  */
    YYSYMBOL_IDENTIFIER = 49,       /* IDENTIFIER  */
    YYSYMBOL_VALUE_STRING = 50,     /* VALUE_STRING  */
    YYSYMBOL_VALUE_INT = 51,        /* VALUE_INT  */
    YYSYMBOL_VALUE_FLOAT = 52,      /* VALUE_FLOAT  */
    YYSYMBOL_VALUE_BOOL = 53,       /* VALUE_BOOL  */
    YYSYMBOL_54_ = 54,              /* ';'  */
    YYSYMBOL_55_ = 55,              /* '='  */
    YYSYMBOL_56_ = 56,              /* '('  */
    YYSYMBOL_57_ = 57,              /* ')'  */
    YYSYMBOL_58_ = 58,              /* ','  */
    YYSYMBOL_59_ = 59,              /* '.'  */
    YYSYMBOL_60_ = 60,              /* '*'  */
    YYSYMBOL_61_ = 61,              /* '<'  */
    YYSYMBOL_62_ = 62,              /* '>'  */
    YYSYMBOL_63_ = 63,              /* '+'  */
    YYSYMBOL_64_ = 64,              /* '-'  */
    YYSYMBOL_YYACCEPT = 65,         /* $accept  */
    YYSYMBOL_start = 66,            /* start  */
    YYSYMBOL_stmt = 67,             /* stmt  */
    YYSYMBOL_txnStmt = 68,          /* txnStmt  */
    YYSYMBOL_dbStmt = 69,           /* dbStmt  */
    YYSYMBOL_setStmt = 70,          /* setStmt  */
    YYSYMBOL_ddl = 71,              /* ddl  */
    YYSYMBOL_dml = 72,              /* dml  */
    YYSYMBOL_fieldList = 73,        /* fieldList  */
    YYSYMBOL_colNameList = 74,      /* colNameList  */
    YYSYMBOL_field = 75,            /* field  */
    YYSYMBOL_type = 76,             /* type  */
    YYSYMBOL_valueList = 77,        /* valueList  */
    YYSYMBOL_value = 78,            /* value  */
    YYSYMBOL_condition = 79,        /* condition  */
    YYSYMBOL_optWhereClause = 80,   /* optWhereClause  */
    YYSYMBOL_whereClause = 81,      /* whereClause  */
    YYSYMBOL_col = 82,              /* col  */
    YYSYMBOL_colList = 83,          /* colList  */
    YYSYMBOL_op = 84,               /* op  */
    YYSYMBOL_expr = 85,             /* expr  */
    YYSYMBOL_setClauses = 86,       /* setClauses  */
    YYSYMBOL_setClause = 87,        /* setClause  */
    YYSYMBOL_selector = 88,         /* selector  */
    YYSYMBOL_tableList = 89,        /* tableList  */
    YYSYMBOL_opt_order_clause = 90, /* opt_order_clause  */
    YYSYMBOL_order_clause = 91,     /* order_clause  */
    YYSYMBOL_order_clauses = 92,    /* order_clauses  */
    YYSYMBOL_opt_asc_desc = 93,     /* opt_asc_desc  */
    YYSYMBOL_opt_group_clause = 94, /* opt_group_clause  */
    YYSYMBOL_group_clause = 95,     /* group_clause  */
    YYSYMBOL_group_clauses = 96,    /* group_clauses  */
    YYSYMBOL_opt_having_conds = 97, /* opt_having_conds  */
    YYSYMBOL_having_conds = 98,     /* having_conds  */
    YYSYMBOL_set_knob_type = 99,    /* set_knob_type  */
    YYSYMBOL_tbName = 100,          /* tbName  */
    YYSYMBOL_colName = 101          /* colName  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;

#ifdef short
#undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
#include <limits.h> /* INFRINGES ON USER NAME SPACE */
#if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#define YY_STDINT_H
#endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
#undef UINT_LEAST8_MAX
#undef UINT_LEAST16_MAX
#define UINT_LEAST8_MAX 255
#define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H && \
       UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H && \
       UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
#if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#define YYPTRDIFF_T __PTRDIFF_TYPE__
#define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
#elif defined PTRDIFF_MAX
#ifndef ptrdiff_t
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#endif
#define YYPTRDIFF_T ptrdiff_t
#define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
#else
#define YYPTRDIFF_T long
#define YYPTRDIFF_MAXIMUM LONG_MAX
#endif
#endif

#ifndef YYSIZE_T
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned
#endif
#endif

#define YYSIZE_MAXIMUM                                              \
    YY_CAST(YYPTRDIFF_T, (YYPTRDIFF_MAXIMUM < YY_CAST(YYSIZE_T, -1) \
                              ? YYPTRDIFF_MAXIMUM                   \
                              : YY_CAST(YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST(YYPTRDIFF_T, sizeof(X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#define YY_ATTRIBUTE_PURE
#endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define YY_ATTRIBUTE_UNUSED
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YY_USE(E) ((void)(E))
#else
#define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && !defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
#if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma("GCC diagnostic push")          \
        _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")
#else
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                   \
    _Pragma("GCC diagnostic push")                            \
        _Pragma("GCC diagnostic ignored \"-Wuninitialized\"") \
            _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
#endif
#define YY_IGNORE_MAYBE_UNINITIALIZED_END _Pragma("GCC diagnostic pop")
#else
#define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && !defined __ICC && 6 <= __GNUC__
#define YY_IGNORE_USELESS_CAST_BEGIN \
    _Pragma("GCC diagnostic push")   \
        _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"")
#define YY_IGNORE_USELESS_CAST_END _Pragma("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_END
#endif

#define YY_ASSERT(E) ((void)(0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#ifdef YYSTACK_USE_ALLOCA
#if YYSTACK_USE_ALLOCA
#ifdef __GNUC__
#define YYSTACK_ALLOC __builtin_alloca
#elif defined __BUILTIN_VA_ARG_INCR
#include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#elif defined _AIX
#define YYSTACK_ALLOC __alloca
#elif defined _MSC_VER
#include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#define alloca _alloca
#else
#define YYSTACK_ALLOC alloca
#if !defined _ALLOCA_H && !defined EXIT_SUCCESS
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#endif
#endif
#endif

#ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#define YYSTACK_FREE(Ptr) \
    do { /* empty */      \
        ;                 \
    } while (0)
#ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#endif
#else
#define YYSTACK_ALLOC YYMALLOC
#define YYSTACK_FREE YYFREE
#ifndef YYSTACK_ALLOC_MAXIMUM
#define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#endif
#if (defined __cplusplus && !defined EXIT_SUCCESS && \
     !((defined YYMALLOC || defined malloc) &&       \
       (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined EXIT_SUCCESS
void* malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined EXIT_SUCCESS
void free(void*); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#endif
#endif /* 1 */

#if (!defined yyoverflow &&                                \
     (!defined __cplusplus ||                              \
      (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL && \
       defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
    yy_state_t yyss_alloc;
    YYSTYPE yyvs_alloc;
    YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (YYSIZEOF(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N)                                                    \
    ((N) * (YYSIZEOF(yy_state_t) + YYSIZEOF(YYSTYPE) + YYSIZEOF(YYLTYPE)) + \
     2 * YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                               \
    do {                                                                   \
        YYPTRDIFF_T yynewbytes;                                            \
        YYCOPY(&yyptr->Stack_alloc, Stack, yysize);                        \
        Stack = &yyptr->Stack_alloc;                                       \
        yynewbytes = yystacksize * YYSIZEOF(*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF(*yyptr);                            \
    } while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count) \
    __builtin_memcpy(Dst, Src, YY_CAST(YYSIZE_T, (Count)) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)             \
    do {                                    \
        YYPTRDIFF_T yyi;                    \
        for (yyi = 0; yyi < (Count); yyi++) \
            (Dst)[yyi] = (Src)[yyi];        \
    } while (0)
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 37
/* YYNRULES -- Number of rules.  */
#define YYNRULES 112
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 221

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK 308

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                              \
    (0 <= (YYX) && (YYX) <= YYMAXUTOK                 \
         ? YY_CAST(yysymbol_kind_t, yytranslate[YYX]) \
         : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  56, 57, 60, 63, 58, 64, 59, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  54, 61, 55, 62, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] = {
    0,   62,  62,  67,  72,  77,  85,  86,  87,  88,  89,  93,  97,  101, 105,
    112, 119, 126, 130, 134, 138, 142, 146, 153, 157, 161, 165, 169, 176, 180,
    187, 191, 198, 205, 209, 213, 220, 224, 231, 235, 239, 243, 250, 254, 261,
    262, 269, 273, 280, 284, 288, 292, 296, 300, 304, 308, 312, 316, 320, 324,
    328, 332, 336, 340, 344, 348, 352, 356, 360, 364, 371, 375, 382, 386, 390,
    394, 398, 402, 406, 413, 417, 424, 428, 435, 439, 443, 447, 454, 458, 462,
    466, 470, 477, 481, 485, 491, 495, 502, 503, 504, 508, 512, 515, 520, 524,
    530, 531, 537, 541, 548, 549, 552, 554};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST(yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char* yysymbol_name(yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] = {"\"end of file\"",
                                      "error",
                                      "\"invalid token\"",
                                      "SHOW",
                                      "TABLES",
                                      "CREATE",
                                      "TABLE",
                                      "DROP",
                                      "DESC",
                                      "INSERT",
                                      "INTO",
                                      "VALUES",
                                      "DELETE",
                                      "FROM",
                                      "ASC",
                                      "ORDER",
                                      "GROUP",
                                      "BY",
                                      "HAVING",
                                      "SUM",
                                      "COUNT",
                                      "MAX",
                                      "MIN",
                                      "AS",
                                      "WHERE",
                                      "UPDATE",
                                      "SET",
                                      "SELECT",
                                      "INT",
                                      "CHAR",
                                      "FLOAT",
                                      "INDEX",
                                      "AND",
                                      "JOIN",
                                      "EXIT",
                                      "HELP",
                                      "TXN_BEGIN",
                                      "TXN_COMMIT",
                                      "TXN_ABORT",
                                      "TXN_ROLLBACK",
                                      "ORDER_BY",
                                      "GROUP_BY",
                                      "ENABLE_NESTLOOP",
                                      "ENABLE_SORTMERGE",
                                      "LEQ",
                                      "NEQ",
                                      "GEQ",
                                      "IN",
                                      "T_EOF",
                                      "IDENTIFIER",
                                      "VALUE_STRING",
                                      "VALUE_INT",
                                      "VALUE_FLOAT",
                                      "VALUE_BOOL",
                                      "';'",
                                      "'='",
                                      "'('",
                                      "')'",
                                      "','",
                                      "'.'",
                                      "'*'",
                                      "'<'",
                                      "'>'",
                                      "'+'",
                                      "'-'",
                                      "$accept",
                                      "start",
                                      "stmt",
                                      "txnStmt",
                                      "dbStmt",
                                      "setStmt",
                                      "ddl",
                                      "dml",
                                      "fieldList",
                                      "colNameList",
                                      "field",
                                      "type",
                                      "valueList",
                                      "value",
                                      "condition",
                                      "optWhereClause",
                                      "whereClause",
                                      "col",
                                      "colList",
                                      "op",
                                      "expr",
                                      "setClauses",
                                      "setClause",
                                      "selector",
                                      "tableList",
                                      "opt_order_clause",
                                      "order_clause",
                                      "order_clauses",
                                      "opt_asc_desc",
                                      "opt_group_clause",
                                      "group_clause",
                                      "group_clauses",
                                      "opt_having_conds",
                                      "having_conds",
                                      "set_knob_type",
                                      "tbName",
                                      "colName",
                                      YY_NULLPTR};

static const char* yysymbol_name(yysymbol_kind_t yysymbol) {
    return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-133)

#define yypact_value_is_default(Yyn) ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-112)

#define yytable_value_is_error(Yyn) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    77,   29,   6,    30,   -41,  11,   18,   -41,  48,   8,    -133, -133,
    -133, -133, -133, -133, -133, 44,   16,   -133, -133, -133, -133, -133,
    -133, 34,   -41,  -41,  -41,  -41,  -133, -133, -41,  -41,  41,   -133,
    -133, 24,   38,   40,   42,   54,   94,   -133, -133, 70,   142,  101,
    133,  -133, -133, -41,  105,  110,  -133, 111,  146,  144,  120,  117,
    122,  -11,  122,  122,  4,    -41,  120,  120,  -133, 120,  120,  120,
    116,  4,    -133, -133, -23,  -133, 118,  -133, 119,  123,  124,  125,
    126,  127,  128,  129,  130,  -133, -15,  -133, 154,  -133, 43,   -133,
    103,  69,   -133, 82,   71,   -133, 147,  97,   120,  -133, 56,   120,
    159,  166,  120,  167,  120,  168,  120,  169,  -41,  -41,  177,  120,
    -133, 120,  -133, 138,  -133, -133, -133, 120,  -133, -133, -133, -133,
    -133, 88,   -133, 4,    -133, -133, -133, -133, -133, -133, -133, 98,
    -133, -133, 112,  139,  120,  120,  140,  120,  141,  120,  143,  120,
    -133, -133, 178,  184,  -133, -133, 152,  -133, -133, 71,   -133, 7,
    -133, -133, -133, 71,   71,   -133, 181,  -133, -133, 182,  -133, 183,
    -133, 185,  -133, 4,    4,    192,  155,  -133, 156,  -133, -133, 120,
    120,  120,  120,  -133, -133, 151,  -133, 179,  197,  -133, -133, -133,
    -133, -133, -133, -133, 4,    4,    4,    -133, -133, 58,   -133, 157,
    -133, -133, -133, 4,    -133};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] = {
    0,  0,  0,  0,   0,   0,   0,   0,   0,  0,  4,   3,   11, 12, 13,  14,
    5,  0,  0,  9,   6,   10,  7,   8,   15, 0,  0,   0,   0,  0,  111, 19,
    0,  0,  0,  109, 110, 0,   0,   0,   0,  0,  112, 87,  70, 88, 0,   0,
    51, 1,  2,  0,   0,   0,   18,  0,   0,  44, 0,   0,   0,  0,  0,   0,
    0,  0,  0,  0,   22,  0,   0,   0,   0,  0,  24,  112, 44, 81, 0,   16,
    0,  0,  0,  0,   0,   0,   0,   0,   0,  71, 44,  89,  50, 49, 0,   28,
    0,  0,  30, 0,   0,   46,  45,  0,   0,  25, 0,   0,   63, 53, 0,   69,
    0,  65, 0,  67,  0,   0,   101, 0,   17, 0,  33,  0,   35, 32, 20,  0,
    21, 40, 38, 39,  41,  0,   36,  0,   76, 75, 77,  78,  72, 73, 74,  0,
    82, 86, 0,  0,   0,   0,   0,   0,   0,  0,  0,   0,   91, 90, 0,   105,
    48, 29, 0,  31,  23,  0,   47,  0,   79, 80, 42,  0,   0,  85, 55,  62,
    52, 61, 68, 57,  64,  59,  66,  0,   0,  26, 0,   37,  0,  83, 84,  0,
    0,  0,  0,  102, 103, 100, 107, 106, 0,  27, 34,  43,  54, 60, 56,  58,
    0,  0,  0,  104, 108, 99,  95,  92,  98, 97, 94,  0,   96};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -133, -133, -133, -133, -133, -133, -133, 49,   -133, 148, 96,   -133, -133,
    -84,  -132, -7,   -133, -9,   -133, -133, -133, -133, 114, -133, -133, -133,
    1,    -133, -133, -133, 14,   -133, -133, -133, -133, 13,  -56};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] = {
    0,   17,  18,  19,  20,  21,  22,  23,  94, 97, 95, 125, 133,
    134, 101, 74,  102, 103, 45,  143, 170, 76, 77, 46, 90,  201,
    214, 215, 218, 159, 196, 197, 185, 199, 37, 47, 48};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    44,  73,  78,  166,  81,  84,  86,  88,  30,  73,  92,  93,  26,  96,  98,
    98,  5,   31,  116,  6,   34,  32,  145, 38,  39,  40,  41,  38,  39,  40,
    41,  33,  7,   24,   9,   104, 28,  27,  42,  52,  53,  54,  55,  117, 49,
    56,  57,  51,  78,   82,  146, 147, 198, 42,  150, 89,  152, 42,  154, 168,
    25,  29,  173, 160,  68,  96,  216, 58,  43,  105, 50,  163, 217, 80,  83,
    85,  87,  212, 91,   59,  1,   187, 2,   118, 3,   4,   5,   189, 190, 6,
    35,  36,  175, 176,  60,  178, 61,  180, 62,  182, 120, 121, 7,   8,   9,
    75,  129, 130, 131,  132, 63,  10,  11,  12,  13,  14,  15,  38,  39,  40,
    41,  129, 130, 131,  132, 16,  126, 127, 64,  156, 157, 122, 123, 124, 169,
    204, 205, 206, 207,  128, 127, 136, 137, 138, 139, 164, 165, 42,  129, 130,
    131, 132, 140, -111, 167, 65,  67,  72,  141, 142, 66,  69,  129, 130, 131,
    132, 70,  71,  73,   75,  79,  42,  100, 106, 195, 171, 172, 119, 107, 135,
    108, 109, 148, 111,  110, 113, 112, 115, 114, 149, 151, 153, 155, 158, 162,
    183, 174, 177, 179,  195, 181, 213, 184, 186, 191, 192, 193, 200, 194, 208,
    213, 209, 202, 203,  210, 219, 188, 161, 144, 99,  220, 0,   211};

static const yytype_int16 yycheck[] = {
    9,   24,  58,  135, 60,  61,  62,  63,  49,  24,  66,  67,  6,   69,  70,
    71,  9,   4,   33,  12,  7,   10,  106, 19,  20,  21,  22,  19,  20,  21,
    22,  13,  25,  4,   27,  58,  6,   31,  49,  26,  27,  28,  29,  58,  0,
    32,  33,  13,  104, 60,  106, 107, 184, 49,  110, 64,  112, 49,  114, 143,
    31,  31,  146, 119, 51,  121, 8,   26,  60,  76,  54,  127, 14,  60,  61,
    62,  63,  209, 65,  55,  3,   165, 5,   90,  7,   8,   9,   171, 172, 12,
    42,  43,  148, 149, 56,  151, 56,  153, 56,  155, 57,  58,  25,  26,  27,
    49,  50,  51,  52,  53,  56,  34,  35,  36,  37,  38,  39,  19,  20,  21,
    22,  50,  51,  52,  53,  48,  57,  58,  58,  116, 117, 28,  29,  30,  143,
    191, 192, 193, 194, 57,  58,  44,  45,  46,  47,  57,  58,  49,  50,  51,
    52,  53,  55,  59,  56,  13,  23,  11,  61,  62,  59,  56,  50,  51,  52,
    53,  56,  56,  24,  49,  53,  49,  56,  55,  183, 63,  64,  23,  59,  32,
    57,  57,  23,  57,  59,  57,  59,  57,  59,  23,  23,  23,  23,  16,  56,
    17,  57,  57,  57,  208, 57,  210, 18,  51,  23,  23,  23,  15,  23,  58,
    219, 32,  57,  57,  17,  58,  167, 121, 104, 71,  219, -1,  208};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] = {
    0,   3,   5,   7,   8,   9,   12,  25,  26,  27,  34,  35,  36,  37,  38,
    39,  48,  66,  67,  68,  69,  70,  71,  72,  4,   31,  6,   31,  6,   31,
    49,  100, 10,  13,  100, 42,  43,  99,  19,  20,  21,  22,  49,  60,  82,
    83,  88,  100, 101, 0,   54,  13,  100, 100, 100, 100, 100, 100, 26,  55,
    56,  56,  56,  56,  58,  13,  59,  23,  100, 56,  56,  56,  11,  24,  80,
    49,  86,  87,  101, 53,  100, 101, 60,  100, 101, 100, 101, 100, 101, 82,
    89,  100, 101, 101, 73,  75,  101, 74,  101, 74,  56,  79,  81,  82,  58,
    80,  55,  59,  57,  57,  59,  57,  59,  57,  59,  57,  33,  58,  80,  23,
    57,  58,  28,  29,  30,  76,  57,  58,  57,  50,  51,  52,  53,  77,  78,
    32,  44,  45,  46,  47,  55,  61,  62,  84,  87,  78,  101, 101, 23,  23,
    101, 23,  101, 23,  101, 23,  100, 100, 16,  94,  101, 75,  56,  101, 57,
    58,  79,  56,  78,  82,  85,  63,  64,  78,  57,  101, 101, 57,  101, 57,
    101, 57,  101, 17,  18,  97,  51,  78,  72,  78,  78,  23,  23,  23,  23,
    82,  95,  96,  79,  98,  15,  90,  57,  57,  101, 101, 101, 101, 58,  32,
    17,  95,  79,  82,  91,  92,  8,   14,  93,  58,  91};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] = {
    0,  65, 66, 66, 66, 66, 67, 67, 67, 67, 67, 68, 68, 68, 68, 69, 70,  71, 71,
    71, 71, 71, 71, 72, 72, 72, 72, 72, 73, 73, 74, 74, 75, 76, 76, 76,  77, 77,
    78, 78, 78, 78, 79, 79, 80, 80, 81, 81, 82, 82, 82, 82, 82, 82, 82,  82, 82,
    82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 83, 84, 84,  84, 84,
    84, 84, 84, 85, 85, 86, 86, 87, 87, 87, 87, 88, 88, 89, 89, 89, 90,  90, 91,
    92, 92, 93, 93, 93, 94, 94, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 101};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] = {
    0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 6, 3, 2, 6, 6, 4,
    7, 4, 5, 7, 8, 1, 3, 1, 3, 2, 1, 4, 1, 1, 3, 1, 1, 1, 1, 3, 5, 0, 2,
    1, 3, 5, 3, 3, 1, 6, 4, 8, 6, 8, 6, 8, 6, 8, 6, 6, 4, 6, 4, 6, 4, 6,
    4, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 5, 4, 3, 1, 1, 1, 3, 3,
    3, 0, 2, 1, 3, 1, 1, 0, 3, 0, 1, 1, 3, 0, 2, 1, 3, 1, 1, 1, 1};

enum { YYENOMEM = -2 };

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab
#define YYNOMEM goto yyexhaustedlab

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                     \
    do                                                             \
        if (yychar == YYEMPTY) {                                   \
            yychar = (Token);                                      \
            yylval = (Value);                                      \
            YYPOPSTACK(yylen);                                     \
            yystate = *yyssp;                                      \
            goto yybackup;                                         \
        } else {                                                   \
            yyerror(&yylloc, YY_("syntax error: cannot back up")); \
            YYERROR;                                               \
        }                                                          \
    while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                             \
    do                                                              \
        if (N) {                                                    \
            (Current).first_line = YYRHSLOC(Rhs, 1).first_line;     \
            (Current).first_column = YYRHSLOC(Rhs, 1).first_column; \
            (Current).last_line = YYRHSLOC(Rhs, N).last_line;       \
            (Current).last_column = YYRHSLOC(Rhs, N).last_column;   \
        } else {                                                    \
            (Current).first_line = (Current).last_line =            \
                YYRHSLOC(Rhs, 0).last_line;                         \
            (Current).first_column = (Current).last_column =        \
                YYRHSLOC(Rhs, 0).last_column;                       \
        }                                                           \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])

/* Enable debugging if requested.  */
#if YYDEBUG

#ifndef YYFPRINTF
#include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)     \
    do {                    \
        if (yydebug)        \
            YYFPRINTF Args; \
    } while (0)

/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YYLOCATION_PRINT

#if defined YY_LOCATION_PRINT

/* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#define YYLOCATION_PRINT(File, Loc) YY_LOCATION_PRINT(File, *(Loc))

#elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int yy_location_print_(FILE* yyo, YYLTYPE const* const yylocp) {
    int res = 0;
    int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
    if (0 <= yylocp->first_line) {
        res += YYFPRINTF(yyo, "%d", yylocp->first_line);
        if (0 <= yylocp->first_column)
            res += YYFPRINTF(yyo, ".%d", yylocp->first_column);
    }
    if (0 <= yylocp->last_line) {
        if (yylocp->first_line < yylocp->last_line) {
            res += YYFPRINTF(yyo, "-%d", yylocp->last_line);
            if (0 <= end_col)
                res += YYFPRINTF(yyo, ".%d", end_col);
        } else if (0 <= end_col && yylocp->first_column < end_col)
            res += YYFPRINTF(yyo, "-%d", end_col);
    }
    return res;
}

#define YYLOCATION_PRINT yy_location_print_

/* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#define YY_LOCATION_PRINT(File, Loc) YYLOCATION_PRINT(File, &(Loc))

#else

#define YYLOCATION_PRINT(File, Loc) ((void)0)
/* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#define YY_LOCATION_PRINT YYLOCATION_PRINT

#endif
#endif /* !defined YYLOCATION_PRINT */

#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)       \
    do {                                                    \
        if (yydebug) {                                      \
            YYFPRINTF(stderr, "%s ", Title);                \
            yy_symbol_print(stderr, Kind, Value, Location); \
            YYFPRINTF(stderr, "\n");                        \
        }                                                   \
    } while (0)

/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void yy_symbol_value_print(FILE* yyo, yysymbol_kind_t yykind,
                                  YYSTYPE const* const yyvaluep,
                                  YYLTYPE const* const yylocationp) {
    FILE* yyoutput = yyo;
    YY_USE(yyoutput);
    YY_USE(yylocationp);
    if (!yyvaluep)
        return;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void yy_symbol_print(FILE* yyo, yysymbol_kind_t yykind,
                            YYSTYPE const* const yyvaluep,
                            YYLTYPE const* const yylocationp) {
    YYFPRINTF(yyo, "%s %s (", yykind < YYNTOKENS ? "token" : "nterm",
              yysymbol_name(yykind));

    YYLOCATION_PRINT(yyo, yylocationp);
    YYFPRINTF(yyo, ": ");
    yy_symbol_value_print(yyo, yykind, yyvaluep, yylocationp);
    YYFPRINTF(yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void yy_stack_print(yy_state_t* yybottom, yy_state_t* yytop) {
    YYFPRINTF(stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++) {
        int yybot = *yybottom;
        YYFPRINTF(stderr, " %d", yybot);
    }
    YYFPRINTF(stderr, "\n");
}

#define YY_STACK_PRINT(Bottom, Top)          \
    do {                                     \
        if (yydebug)                         \
            yy_stack_print((Bottom), (Top)); \
    } while (0)

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void yy_reduce_print(yy_state_t* yyssp, YYSTYPE* yyvsp, YYLTYPE* yylsp,
                            int yyrule) {
    int yylno = yyrline[yyrule];
    int yynrhs = yyr2[yyrule];
    int yyi;
    YYFPRINTF(stderr, "Reducing stack by rule %d (line %d):\n", yyrule - 1,
              yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++) {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr, YY_ACCESSING_SYMBOL(+yyssp[yyi + 1 - yynrhs]),
                        &yyvsp[(yyi + 1) - (yynrhs)],
                        &(yylsp[(yyi + 1) - (yynrhs)]));
        YYFPRINTF(stderr, "\n");
    }
}

#define YY_REDUCE_PRINT(Rule)                           \
    do {                                                \
        if (yydebug)                                    \
            yy_reduce_print(yyssp, yyvsp, yylsp, Rule); \
    } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#define YYDPRINTF(Args) ((void)0)
#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
#define YY_STACK_PRINT(Bottom, Top)
#define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Context of a parse error.  */
typedef struct {
    yy_state_t* yyssp;
    yysymbol_kind_t yytoken;
    YYLTYPE* yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int yypcontext_expected_tokens(const yypcontext_t* yyctx,
                                      yysymbol_kind_t yyarg[], int yyargn) {
    /* Actual size of YYARG. */
    int yycount = 0;
    int yyn = yypact[+*yyctx->yyssp];
    if (!yypact_value_is_default(yyn)) {
        /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        /* Stay within bounds of both yycheck and yytname.  */
        int yychecklim = YYLAST - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        int yyx;
        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror &&
                !yytable_value_is_error(yytable[yyx + yyn])) {
                if (!yyarg)
                    ++yycount;
                else if (yycount == yyargn)
                    return 0;
                else
                    yyarg[yycount++] = YY_CAST(yysymbol_kind_t, yyx);
            }
    }
    if (yyarg && yycount == 0 && 0 < yyargn)
        yyarg[0] = YYSYMBOL_YYEMPTY;
    return yycount;
}

#ifndef yystrlen
#if defined __GLIBC__ && defined _STRING_H
#define yystrlen(S) (YY_CAST(YYPTRDIFF_T, strlen(S)))
#else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T yystrlen(const char* yystr) {
    YYPTRDIFF_T yylen;
    for (yylen = 0; yystr[yylen]; yylen++)
        continue;
    return yylen;
}
#endif
#endif

#ifndef yystpcpy
#if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#define yystpcpy stpcpy
#else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char* yystpcpy(char* yydest, const char* yysrc) {
    char* yyd = yydest;
    const char* yys = yysrc;

    while ((*yyd++ = *yys++) != '\0')
        continue;

    return yyd - 1;
}
#endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T yytnamerr(char* yyres, const char* yystr) {
    if (*yystr == '"') {
        YYPTRDIFF_T yyn = 0;
        char const* yyp = yystr;
        for (;;)
            switch (*++yyp) {
                case '\'':
                case ',':
                    goto do_not_strip_quotes;

                case '\\':
                    if (*++yyp != '\\')
                        goto do_not_strip_quotes;
                    else
                        goto append;

                append:
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
    do_not_strip_quotes:;
    }

    if (yyres)
        return yystpcpy(yyres, yystr) - yyres;
    else
        return yystrlen(yystr);
}
#endif

static int yy_syntax_error_arguments(const yypcontext_t* yyctx,
                                     yysymbol_kind_t yyarg[], int yyargn) {
    /* Actual size of YYARG. */
    int yycount = 0;
    /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
    if (yyctx->yytoken != YYSYMBOL_YYEMPTY) {
        int yyn;
        if (yyarg)
            yyarg[yycount] = yyctx->yytoken;
        ++yycount;
        yyn = yypcontext_expected_tokens(yyctx, yyarg ? yyarg + 1 : yyarg,
                                         yyargn - 1);
        if (yyn == YYENOMEM)
            return YYENOMEM;
        else
            yycount += yyn;
    }
    return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int yysyntax_error(YYPTRDIFF_T* yymsg_alloc, char** yymsg,
                          const yypcontext_t* yyctx) {
    enum { YYARGS_MAX = 5 };
    /* Internationalized format string. */
    const char* yyformat = YY_NULLPTR;
    /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
    yysymbol_kind_t yyarg[YYARGS_MAX];
    /* Cumulated lengths of YYARG.  */
    YYPTRDIFF_T yysize = 0;

    /* Actual size of YYARG. */
    int yycount = yy_syntax_error_arguments(yyctx, yyarg, YYARGS_MAX);
    if (yycount == YYENOMEM)
        return YYENOMEM;

    switch (yycount) {
#define YYCASE_(N, S) \
    case N:           \
        yyformat = S; \
        break
        default: /* Avoid compiler warnings. */
            YYCASE_(0, YY_("syntax error"));
            YYCASE_(1, YY_("syntax error, unexpected %s"));
            YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
            YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
            YYCASE_(
                4,
                YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
            YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or "
                           "%s or %s"));
#undef YYCASE_
    }

    /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
    yysize = yystrlen(yyformat) - 2 * yycount + 1;
    {
        int yyi;
        for (yyi = 0; yyi < yycount; ++yyi) {
            YYPTRDIFF_T yysize1 =
                yysize + yytnamerr(YY_NULLPTR, yytname[yyarg[yyi]]);
            if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                yysize = yysize1;
            else
                return YYENOMEM;
        }
    }

    if (*yymsg_alloc < yysize) {
        *yymsg_alloc = 2 * yysize;
        if (!(yysize <= *yymsg_alloc && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
            *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
        return -1;
    }

    /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
    {
        char* yyp = *yymsg;
        int yyi = 0;
        while ((*yyp = *yyformat) != '\0')
            if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount) {
                yyp += yytnamerr(yyp, yytname[yyarg[yyi++]]);
                yyformat += 2;
            } else {
                ++yyp;
                ++yyformat;
            }
    }
    return 0;
}

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void yydestruct(const char* yymsg, yysymbol_kind_t yykind,
                       YYSTYPE* yyvaluep, YYLTYPE* yylocationp) {
    YY_USE(yyvaluep);
    YY_USE(yylocationp);
    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yykind, yyvaluep, yylocationp);

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*----------.
| yyparse.  |
`----------*/

int yyparse(void) {
    /* Lookahead token kind.  */
    int yychar;

    /* The semantic value of the lookahead symbol.  */
    /* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
    YY_INITIAL_VALUE(static YYSTYPE yyval_default;)
    YYSTYPE yylval YY_INITIAL_VALUE(= yyval_default);

    /* Location data for the lookahead symbol.  */
    static YYLTYPE yyloc_default
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
        = {1, 1, 1, 1}
#endif
    ;
    YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t* yyss = yyssa;
    yy_state_t* yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE* yyvs = yyvsa;
    YYSTYPE* yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE* yyls = yylsa;
    YYLTYPE* yylsp = yyls;

    int yyn;
    /* The return value of yyparse.  */
    int yyresult;
    /* Lookahead symbol kind.  */
    yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
    /* The variables used to return semantic value and location from the
     action routines.  */
    YYSTYPE yyval;
    YYLTYPE yyloc;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    /* Buffer for error messages, and its allocated size.  */
    char yymsgbuf[128];
    char* yymsg = yymsgbuf;
    YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    YYDPRINTF((stderr, "Starting parse\n"));

    yychar = YYEMPTY; /* Cause a token to be read.  */

    yylsp[0] = yylloc;
    goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
    /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
    yyssp++;

/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
    YYDPRINTF((stderr, "Entering state %d\n", yystate));
    YY_ASSERT(0 <= yystate && yystate < YYNSTATES);
    YY_IGNORE_USELESS_CAST_BEGIN
    *yyssp = YY_CAST(yy_state_t, yystate);
    YY_IGNORE_USELESS_CAST_END
    YY_STACK_PRINT(yyss, yyssp);

    if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
        YYNOMEM;
#else
    {
        /* Get the current used size of the three stacks, in elements.  */
        YYPTRDIFF_T yysize = yyssp - yyss + 1;

#if defined yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
            yy_state_t* yyss1 = yyss;
            YYSTYPE* yyvs1 = yyvs;
            YYLTYPE* yyls1 = yyls;

            /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
            yyoverflow(YY_("memory exhausted"), &yyss1,
                       yysize * YYSIZEOF(*yyssp), &yyvs1,
                       yysize * YYSIZEOF(*yyvsp), &yyls1,
                       yysize * YYSIZEOF(*yylsp), &yystacksize);
            yyss = yyss1;
            yyvs = yyvs1;
            yyls = yyls1;
        }
#else /* defined YYSTACK_RELOCATE */
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
            YYNOMEM;
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
            yystacksize = YYMAXDEPTH;

        {
            yy_state_t* yyss1 = yyss;
            union yyalloc* yyptr = YY_CAST(
                union yyalloc*,
                YYSTACK_ALLOC(YY_CAST(YYSIZE_T, YYSTACK_BYTES(yystacksize))));
            if (!yyptr)
                YYNOMEM;
            YYSTACK_RELOCATE(yyss_alloc, yyss);
            YYSTACK_RELOCATE(yyvs_alloc, yyvs);
            YYSTACK_RELOCATE(yyls_alloc, yyls);
#undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
                YYSTACK_FREE(yyss1);
        }
#endif

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;
        yylsp = yyls + yysize - 1;

        YY_IGNORE_USELESS_CAST_BEGIN
        YYDPRINTF((stderr, "Stack size increased to %ld\n",
                   YY_CAST(long, yystacksize)));
        YY_IGNORE_USELESS_CAST_END

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
    if (yypact_value_is_default(yyn))
        goto yydefault;

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
    if (yychar == YYEMPTY) {
        YYDPRINTF((stderr, "Reading a token\n"));
        yychar = yylex(&yylval, &yylloc);
    }

    if (yychar <= YYEOF) {
        yychar = YYEOF;
        yytoken = YYSYMBOL_YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
    } else if (yychar == YYerror) {
        /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
        yychar = YYUNDEF;
        yytoken = YYSYMBOL_YYerror;
        yyerror_range[1] = yylloc;
        goto yyerrlab1;
    } else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
        goto yydefault;
    yyn = yytable[yyn];
    if (yyn <= 0) {
        if (yytable_value_is_error(yyn))
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
     status.  */
    if (yyerrstatus)
        yyerrstatus--;

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
    yystate = yyn;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END
    *++yylsp = yylloc;

    /* Discard the shifted token.  */
    yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
    /* yyn is the number of a rule to reduce with.  */
    yylen = yyr2[yyn];

    /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
    yyval = yyvsp[1 - yylen];

    /* Default location. */
    YYLLOC_DEFAULT(yyloc, (yylsp - yylen), yylen);
    yyerror_range[1] = yyloc;
    YY_REDUCE_PRINT(yyn);
    switch (yyn) {
        case 2: /* start: stmt ';'  */
#line 63 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = (yyvsp[-1].sv_node);
            YYACCEPT;
        }
#line 1726 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 3: /* start: HELP  */
#line 68 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = std::make_shared<Help>();
            YYACCEPT;
        }
#line 1735 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 4: /* start: EXIT  */
#line 73 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = nullptr;
            YYACCEPT;
        }
#line 1744 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 5: /* start: T_EOF  */
#line 78 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = nullptr;
            YYACCEPT;
        }
#line 1753 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 11: /* txnStmt: TXN_BEGIN  */
#line 94 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnBegin>();
        }
#line 1761 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 12: /* txnStmt: TXN_COMMIT  */
#line 98 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnCommit>();
        }
#line 1769 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 13: /* txnStmt: TXN_ABORT  */
#line 102 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnAbort>();
        }
#line 1777 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 14: /* txnStmt: TXN_ROLLBACK  */
#line 106 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnRollback>();
        }
#line 1785 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 15: /* dbStmt: SHOW TABLES  */
#line 113 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<ShowTables>();
        }
#line 1793 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 16: /* setStmt: SET set_knob_type '=' VALUE_BOOL  */
#line 120 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SetStmt>(
                (yyvsp[-2].sv_setKnobType), (yyvsp[0].sv_bool));
        }
#line 1801 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 17: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 127 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<CreateTable>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
        }
#line 1809 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 18: /* ddl: DROP TABLE tbName  */
#line 131 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
        }
#line 1817 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 19: /* ddl: DESC tbName  */
#line 135 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
        }
#line 1825 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 20: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 139 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<CreateIndex>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
        }
#line 1833 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 21: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 143 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str),
                                                          (yyvsp[-1].sv_strs));
        }
#line 1841 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 22: /* ddl: SHOW INDEX FROM tbName  */
#line 147 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
        }
#line 1849 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 23: /* dml: INSERT INTO tbName VALUES '(' valueList ')'  */
#line 154 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-4].sv_str),
                                                           (yyvsp[-1].sv_vals));
        }
#line 1857 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 24: /* dml: DELETE FROM tbName optWhereClause  */
#line 158 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str),
                                                           (yyvsp[0].sv_conds));
        }
#line 1865 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 25: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 162 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<UpdateStmt>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses),
                (yyvsp[0].sv_conds));
        }
#line 1873 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 26: /* dml: SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds  */
#line 166 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SelectStmt>(
                (yyvsp[-5].sv_cols), (yyvsp[-3].sv_strs), (yyvsp[-2].sv_conds),
                (yyvsp[-1].sv_groupbys), (yyvsp[0].sv_having_conds));
        }
#line 1881 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 27: /* dml: SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds opt_order_clause  */
#line 170 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SelectStmt>(
                (yyvsp[-6].sv_cols), (yyvsp[-4].sv_strs), (yyvsp[-3].sv_conds),
                (yyvsp[-2].sv_groupbys), (yyvsp[-1].sv_having_conds),
                (yyvsp[0].sv_orderbys));
        }
#line 1889 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 28: /* fieldList: field  */
#line 177 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_fields) =
                std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
        }
#line 1897 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 29: /* fieldList: fieldList ',' field  */
#line 181 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_fields).push_back((yyvsp[0].sv_field));
        }
#line 1905 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 30: /* colNameList: colName  */
#line 188 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
        }
#line 1913 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 31: /* colNameList: colNameList ',' colName  */
#line 192 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 1921 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 32: /* field: colName type  */
#line 199 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str),
                                                        (yyvsp[0].sv_type_len));
        }
#line 1929 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 33: /* type: INT  */
#line 206 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
        }
#line 1937 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 34: /* type: CHAR '(' VALUE_INT ')'  */
#line 210 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
        }
#line 1945 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 35: /* type: FLOAT  */
#line 214 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(double));
        }
#line 1953 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 36: /* valueList: value  */
#line 221 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_vals) =
                std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
        }
#line 1961 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 37: /* valueList: valueList ',' value  */
#line 225 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_vals).push_back((yyvsp[0].sv_val));
        }
#line 1969 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 38: /* value: VALUE_INT  */
#line 232 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
        }
#line 1977 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 39: /* value: VALUE_FLOAT  */
#line 236 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
        }
#line 1985 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 40: /* value: VALUE_STRING  */
#line 240 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
        }
#line 1993 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 41: /* value: VALUE_BOOL  */
#line 244 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<BoolLit>((yyvsp[0].sv_bool));
        }
#line 2001 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 42: /* condition: col op expr  */
#line 251 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cond) = std::make_shared<BinaryExpr>(
                (yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
        }
#line 2009 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 43: /* condition: col op '(' dml ')'  */
#line 255 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cond) = std::make_shared<BinaryExpr>(
                (yyvsp[-4].sv_col), (yyvsp[-3].sv_comp_op),
                (yyvsp[-1].sv_node));
        }
#line 2017 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 44: /* optWhereClause: %empty  */
#line 261 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2023 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 45: /* optWhereClause: WHERE whereClause  */
#line 263 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) = (yyvsp[0].sv_conds);
        }
#line 2031 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 46: /* whereClause: condition  */
#line 270 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) =
                std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
        }
#line 2039 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 47: /* whereClause: whereClause AND condition  */
#line 274 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
        }
#line 2047 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 48: /* col: tbName '.' colName AS colName  */
#line 281 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                (yyvsp[-4].sv_str), (yyvsp[-2].sv_str), (yyvsp[0].sv_str));
        }
#line 2055 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 49: /* col: colName AS colName  */
#line 285 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-2].sv_str),
                                                   (yyvsp[0].sv_str));
        }
#line 2063 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 50: /* col: tbName '.' colName  */
#line 289 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
        }
#line 2071 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 51: /* col: colName  */
#line 293 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
        }
#line 2079 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 52: /* col: COUNT '(' '*' ')' AS colName  */
#line 297 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>("", "*", (yyvsp[0].sv_str), "COUNT");
        }
#line 2087 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 53: /* col: COUNT '(' '*' ')'  */
#line 301 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>("", "*", "count_row", "COUNT");
        }
#line 2095 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 54: /* col: SUM '(' tbName '.' colName ')' AS colName  */
#line 305 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "SUM");
        }
#line 2103 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 55: /* col: SUM '(' tbName '.' colName ')'  */
#line 309 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "sum_" + (yyvsp[-1].sv_str), "SUM");
        }
#line 2111 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 56: /* col: MAX '(' tbName '.' colName ')' AS colName  */
#line 313 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "MAX");
        }
#line 2119 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 57: /* col: MAX '(' tbName '.' colName ')'  */
#line 317 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "max_" + (yyvsp[-1].sv_str), "MAX");
        }
#line 2127 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 58: /* col: MIN '(' tbName '.' colName ')' AS colName  */
#line 321 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "MIN");
        }
#line 2135 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 59: /* col: MIN '(' tbName '.' colName ')'  */
#line 325 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "min_" + (yyvsp[-1].sv_str), "MIN");
        }
#line 2143 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 60: /* col: COUNT '(' tbName '.' colName ')' AS colName  */
#line 329 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "COUNT");
        }
#line 2151 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 61: /* col: COUNT '(' tbName '.' colName ')'  */
#line 333 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "count_" + (yyvsp[-1].sv_str), "COUNT");
        }
#line 2159 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 62: /* col: SUM '(' colName ')' AS colName  */
#line 337 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "SUM");
        }
#line 2167 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 63: /* col: SUM '(' colName ')'  */
#line 341 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "sum_" + (yyvsp[-1].sv_str), "SUM");
        }
#line 2175 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 64: /* col: MAX '(' colName ')' AS colName  */
#line 345 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "MAX");
        }
#line 2183 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 65: /* col: MAX '(' colName ')'  */
#line 349 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "max_" + (yyvsp[-1].sv_str), "MAX");
        }
#line 2191 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 66: /* col: MIN '(' colName ')' AS colName  */
#line 353 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "MIN");
        }
#line 2199 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 67: /* col: MIN '(' colName ')'  */
#line 357 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "min_" + (yyvsp[-1].sv_str), "MIN");
        }
#line 2207 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 68: /* col: COUNT '(' colName ')' AS colName  */
#line 361 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "COUNT");
        }
#line 2215 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 69: /* col: COUNT '(' colName ')'  */
#line 365 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "count" + (yyvsp[-1].sv_str), "COUNT");
        }
#line 2223 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 70: /* colList: col  */
#line 372 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols) =
                std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
        }
#line 2231 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 71: /* colList: colList ',' col  */
#line 376 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols).push_back((yyvsp[0].sv_col));
        }
#line 2239 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 72: /* op: '='  */
#line 383 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_EQ;
        }
#line 2247 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 73: /* op: '<'  */
#line 387 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_LT;
        }
#line 2255 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 74: /* op: '>'  */
#line 391 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_GT;
        }
#line 2263 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 75: /* op: NEQ  */
#line 395 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_NE;
        }
#line 2271 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 76: /* op: LEQ  */
#line 399 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_LE;
        }
#line 2279 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 77: /* op: GEQ  */
#line 403 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_GE;
        }
#line 2287 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 78: /* op: IN  */
#line 407 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_IN;
        }
#line 2295 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 79: /* expr: value  */
#line 414 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
        }
#line 2303 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 80: /* expr: col  */
#line 418 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
        }
#line 2311 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 81: /* setClauses: setClause  */
#line 425 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{
                (yyvsp[0].sv_set_clause)};
        }
#line 2319 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 82: /* setClauses: setClauses ',' setClause  */
#line 429 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
        }
#line 2327 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 83: /* setClause: colName '=' colName '+' value  */
#line 436 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-4].sv_str), (yyvsp[0].sv_val), SV_OP_ADD);
        }
#line 2335 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 84: /* setClause: colName '=' colName '-' value  */
#line 440 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-4].sv_str), (yyvsp[0].sv_val), SV_OP_SUB);
        }
#line 2343 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 85: /* setClause: colName '=' colName value  */
#line 444 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-3].sv_str), (yyvsp[0].sv_val), SV_OP_ADD);
        }
#line 2351 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 86: /* setClause: colName '=' value  */
#line 448 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-2].sv_str), (yyvsp[0].sv_val));
        }
#line 2359 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 87: /* selector: '*'  */
#line 455 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols) = {};
        }
#line 2367 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 89: /* tableList: tbName  */
#line 463 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
        }
#line 2375 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 90: /* tableList: tableList ',' tbName  */
#line 467 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 2383 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 91: /* tableList: tableList JOIN tbName  */
#line 471 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 2391 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 92: /* opt_order_clause: ORDER BY order_clauses  */
#line 478 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys) = (yyvsp[0].sv_orderbys);
        }
#line 2399 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 93: /* opt_order_clause: %empty  */
#line 481 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2405 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 94: /* order_clause: col opt_asc_desc  */
#line 486 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby) = std::make_shared<OrderBy>(
                (yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
        }
#line 2413 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 95: /* order_clauses: order_clause  */
#line 492 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys) =
                std::vector<std::shared_ptr<OrderBy>>{(yyvsp[0].sv_orderby)};
        }
#line 2421 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 96: /* order_clauses: order_clauses ',' order_clause  */
#line 496 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys).push_back((yyvsp[0].sv_orderby));
        }
#line 2429 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 97: /* opt_asc_desc: ASC  */
#line 502 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_ASC;
        }
#line 2435 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 98: /* opt_asc_desc: DESC  */
#line 503 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_DESC;
        }
#line 2441 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 99: /* opt_asc_desc: %empty  */
#line 504 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_DEFAULT;
        }
#line 2447 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 100: /* opt_group_clause: GROUP BY group_clauses  */
#line 509 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys) = (yyvsp[0].sv_groupbys);
        }
#line 2455 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 101: /* opt_group_clause: %empty  */
#line 512 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2461 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 102: /* group_clause: col  */
#line 516 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupby) = std::make_shared<GroupBy>((yyvsp[0].sv_col));
        }
#line 2469 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 103: /* group_clauses: group_clause  */
#line 521 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys) =
                std::vector<std::shared_ptr<GroupBy>>{(yyvsp[0].sv_groupby)};
        }
#line 2477 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 104: /* group_clauses: group_clauses ',' group_clause  */
#line 525 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys).push_back((yyvsp[0].sv_groupby));
        }
#line 2485 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 105: /* opt_having_conds: %empty  */
#line 530 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2491 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 106: /* opt_having_conds: HAVING having_conds  */
#line 532 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_having_conds) = (yyvsp[0].sv_having_conds);
        }
#line 2499 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 107: /* having_conds: condition  */
#line 538 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_having_conds) =
                std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
        }
#line 2507 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 108: /* having_conds: having_conds AND condition  */
#line 542 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_having_conds).push_back((yyvsp[0].sv_cond));
        }
#line 2515 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 109: /* set_knob_type: ENABLE_NESTLOOP  */
#line 548 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_setKnobType) = EnableNestLoop;
        }
#line 2521 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 110: /* set_knob_type: ENABLE_SORTMERGE  */
#line 549 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_setKnobType) = EnableSortMerge;
        }
#line 2527 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

#line 2531 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"

        default:
            break;
    }
    /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
    YY_SYMBOL_PRINT("-> $$ =", YY_CAST(yysymbol_kind_t, yyr1[yyn]), &yyval,
                    &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;

    *++yyvsp = yyval;
    *++yylsp = yyloc;

    /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
    {
        const int yylhs = yyr1[yyn] - YYNTOKENS;
        const int yyi = yypgoto[yylhs] + *yyssp;
        yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
                       ? yytable[yyi]
                       : yydefgoto[yylhs]);
    }

    goto yynewstate;

/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
    yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE(yychar);
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus) {
        ++yynerrs;
        {
            yypcontext_t yyctx = {yyssp, yytoken, &yylloc};
            char const* yymsgp = YY_("syntax error");
            int yysyntax_error_status;
            yysyntax_error_status =
                yysyntax_error(&yymsg_alloc, &yymsg, &yyctx);
            if (yysyntax_error_status == 0)
                yymsgp = yymsg;
            else if (yysyntax_error_status == -1) {
                if (yymsg != yymsgbuf)
                    YYSTACK_FREE(yymsg);
                yymsg = YY_CAST(char*,
                                YYSTACK_ALLOC(YY_CAST(YYSIZE_T, yymsg_alloc)));
                if (yymsg) {
                    yysyntax_error_status =
                        yysyntax_error(&yymsg_alloc, &yymsg, &yyctx);
                    yymsgp = yymsg;
                } else {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                    yysyntax_error_status = YYENOMEM;
                }
            }
            yyerror(&yylloc, yymsgp);
            if (yysyntax_error_status == YYENOMEM)
                YYNOMEM;
        }
    }

    yyerror_range[1] = yylloc;
    if (yyerrstatus == 3) {
        /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

        if (yychar <= YYEOF) {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
                YYABORT;
        } else {
            yydestruct("Error: discarding", yytoken, &yylval, &yylloc);
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
    /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
    if (0)
        YYERROR;
    ++yynerrs;

    /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3; /* Each real token shifted decrements this.  */

    /* Pop stack until we find a state that shifts the error token.  */
    for (;;) {
        yyn = yypact[yystate];
        if (!yypact_value_is_default(yyn)) {
            yyn += YYSYMBOL_YYerror;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror) {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;

        yyerror_range[1] = *yylsp;
        yydestruct("Error: popping", YY_ACCESSING_SYMBOL(yystate), yyvsp,
                   yylsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    yyerror_range[2] = yylloc;
    ++yylsp;
    YYLLOC_DEFAULT(*yylsp, yyerror_range, 2);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", YY_ACCESSING_SYMBOL(yyn), yyvsp, yylsp);

    yystate = yyn;
    goto yynewstate;

/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
    yyresult = 0;
    goto yyreturnlab;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
    yyresult = 1;
    goto yyreturnlab;

/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
    yyerror(&yylloc, YY_("memory exhausted"));
    yyresult = 2;
    goto yyreturnlab;

/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
    if (yychar != YYEMPTY) {
        /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
        yytoken = YYTRANSLATE(yychar);
        yydestruct("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);
    }
    /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
    YYPOPSTACK(yylen);
    YY_STACK_PRINT(yyss, yyssp);
    while (yyssp != yyss) {
        yydestruct("Cleanup: popping", YY_ACCESSING_SYMBOL(+*yyssp), yyvsp,
                   yylsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif
    if (yymsg != yymsgbuf)
        YYSTACK_FREE(yymsg);
    return yyresult;
}

#line 555 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"

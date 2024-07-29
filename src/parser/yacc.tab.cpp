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
    YYSYMBOL_YYEOF = 0,                /* "end of file"  */
    YYSYMBOL_YYerror = 1,              /* error  */
    YYSYMBOL_YYUNDEF = 2,              /* "invalid token"  */
    YYSYMBOL_SHOW = 3,                 /* SHOW  */
    YYSYMBOL_TABLES = 4,               /* TABLES  */
    YYSYMBOL_CREATE = 5,               /* CREATE  */
    YYSYMBOL_TABLE = 6,                /* TABLE  */
    YYSYMBOL_DROP = 7,                 /* DROP  */
    YYSYMBOL_DESC = 8,                 /* DESC  */
    YYSYMBOL_INSERT = 9,               /* INSERT  */
    YYSYMBOL_INTO = 10,                /* INTO  */
    YYSYMBOL_VALUES = 11,              /* VALUES  */
    YYSYMBOL_DELETE = 12,              /* DELETE  */
    YYSYMBOL_FROM = 13,                /* FROM  */
    YYSYMBOL_ASC = 14,                 /* ASC  */
    YYSYMBOL_ORDER = 15,               /* ORDER  */
    YYSYMBOL_GROUP = 16,               /* GROUP  */
    YYSYMBOL_BY = 17,                  /* BY  */
    YYSYMBOL_HAVING = 18,              /* HAVING  */
    YYSYMBOL_SUM = 19,                 /* SUM  */
    YYSYMBOL_COUNT = 20,               /* COUNT  */
    YYSYMBOL_MAX = 21,                 /* MAX  */
    YYSYMBOL_MIN = 22,                 /* MIN  */
    YYSYMBOL_AS = 23,                  /* AS  */
    YYSYMBOL_LOAD = 24,                /* LOAD  */
    YYSYMBOL_WHERE = 25,               /* WHERE  */
    YYSYMBOL_UPDATE = 26,              /* UPDATE  */
    YYSYMBOL_SET = 27,                 /* SET  */
    YYSYMBOL_SELECT = 28,              /* SELECT  */
    YYSYMBOL_INT = 29,                 /* INT  */
    YYSYMBOL_CHAR = 30,                /* CHAR  */
    YYSYMBOL_FLOAT = 31,               /* FLOAT  */
    YYSYMBOL_DATETIME = 32,            /* DATETIME  */
    YYSYMBOL_INDEX = 33,               /* INDEX  */
    YYSYMBOL_AND = 34,                 /* AND  */
    YYSYMBOL_JOIN = 35,                /* JOIN  */
    YYSYMBOL_EXIT = 36,                /* EXIT  */
    YYSYMBOL_HELP = 37,                /* HELP  */
    YYSYMBOL_TXN_BEGIN = 38,           /* TXN_BEGIN  */
    YYSYMBOL_TXN_COMMIT = 39,          /* TXN_COMMIT  */
    YYSYMBOL_TXN_ABORT = 40,           /* TXN_ABORT  */
    YYSYMBOL_TXN_ROLLBACK = 41,        /* TXN_ROLLBACK  */
    YYSYMBOL_ORDER_BY = 42,            /* ORDER_BY  */
    YYSYMBOL_GROUP_BY = 43,            /* GROUP_BY  */
    YYSYMBOL_ENABLE_NESTLOOP = 44,     /* ENABLE_NESTLOOP  */
    YYSYMBOL_ENABLE_SORTMERGE = 45,    /* ENABLE_SORTMERGE  */
    YYSYMBOL_SET_OUTPUT_FILE_OFF = 46, /* SET_OUTPUT_FILE_OFF  */
    YYSYMBOL_LEQ = 47,                 /* LEQ  */
    YYSYMBOL_NEQ = 48,                 /* NEQ  */
    YYSYMBOL_GEQ = 49,                 /* GEQ  */
    YYSYMBOL_IN = 50,                  /* IN  */
    YYSYMBOL_T_EOF = 51,               /* T_EOF  */
    YYSYMBOL_IDENTIFIER = 52,          /* IDENTIFIER  */
    YYSYMBOL_VALUE_STRING = 53,        /* VALUE_STRING  */
    YYSYMBOL_FILE_PATH = 54,           /* FILE_PATH  */
    YYSYMBOL_VALUE_INT = 55,           /* VALUE_INT  */
    YYSYMBOL_VALUE_FLOAT = 56,         /* VALUE_FLOAT  */
    YYSYMBOL_VALUE_BOOL = 57,          /* VALUE_BOOL  */
    YYSYMBOL_58_ = 58,                 /* ';'  */
    YYSYMBOL_59_ = 59,                 /* '='  */
    YYSYMBOL_60_ = 60,                 /* '('  */
    YYSYMBOL_61_ = 61,                 /* ')'  */
    YYSYMBOL_62_ = 62,                 /* ','  */
    YYSYMBOL_63_ = 63,                 /* '.'  */
    YYSYMBOL_64_ = 64,                 /* '*'  */
    YYSYMBOL_65_ = 65,                 /* '<'  */
    YYSYMBOL_66_ = 66,                 /* '>'  */
    YYSYMBOL_67_ = 67,                 /* '+'  */
    YYSYMBOL_68_ = 68,                 /* '-'  */
    YYSYMBOL_YYACCEPT = 69,            /* $accept  */
    YYSYMBOL_start = 70,               /* start  */
    YYSYMBOL_stmt = 71,                /* stmt  */
    YYSYMBOL_txnStmt = 72,             /* txnStmt  */
    YYSYMBOL_dbStmt = 73,              /* dbStmt  */
    YYSYMBOL_setStmt = 74,             /* setStmt  */
    YYSYMBOL_ddl = 75,                 /* ddl  */
    YYSYMBOL_dml = 76,                 /* dml  */
    YYSYMBOL_fieldList = 77,           /* fieldList  */
    YYSYMBOL_colNameList = 78,         /* colNameList  */
    YYSYMBOL_field = 79,               /* field  */
    YYSYMBOL_type = 80,                /* type  */
    YYSYMBOL_valueList = 81,           /* valueList  */
    YYSYMBOL_value = 82,               /* value  */
    YYSYMBOL_condition = 83,           /* condition  */
    YYSYMBOL_optWhereClause = 84,      /* optWhereClause  */
    YYSYMBOL_whereClause = 85,         /* whereClause  */
    YYSYMBOL_col = 86,                 /* col  */
    YYSYMBOL_colList = 87,             /* colList  */
    YYSYMBOL_op = 88,                  /* op  */
    YYSYMBOL_expr = 89,                /* expr  */
    YYSYMBOL_setClauses = 90,          /* setClauses  */
    YYSYMBOL_setClause = 91,           /* setClause  */
    YYSYMBOL_selector = 92,            /* selector  */
    YYSYMBOL_tableList = 93,           /* tableList  */
    YYSYMBOL_opt_order_clause = 94,    /* opt_order_clause  */
    YYSYMBOL_order_clause = 95,        /* order_clause  */
    YYSYMBOL_order_clauses = 96,       /* order_clauses  */
    YYSYMBOL_opt_asc_desc = 97,        /* opt_asc_desc  */
    YYSYMBOL_opt_group_clause = 98,    /* opt_group_clause  */
    YYSYMBOL_group_clause = 99,        /* group_clause  */
    YYSYMBOL_group_clauses = 100,      /* group_clauses  */
    YYSYMBOL_opt_having_conds = 101,   /* opt_having_conds  */
    YYSYMBOL_having_conds = 102,       /* having_conds  */
    YYSYMBOL_set_knob_type = 103,      /* set_knob_type  */
    YYSYMBOL_tbName = 104,             /* tbName  */
    YYSYMBOL_colName = 105,            /* colName  */
    YYSYMBOL_filePath = 106            /* filePath  */
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
#define YYFINAL 53
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 38
/* YYNRULES -- Number of rules.  */
#define YYNRULES 117
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 230

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK 312

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
    2,  2,  60, 61, 64, 67, 62, 68, 63, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  58, 65, 59, 66, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
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
    49, 50, 51, 52, 53, 54, 55, 56, 57};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] = {
    0,   62,  62,  67,  72,  77,  82,  90,  91,  92,  93,  94,  98,  102, 106,
    110, 117, 124, 131, 135, 139, 143, 147, 151, 155, 162, 166, 170, 174, 178,
    185, 189, 196, 200, 207, 214, 218, 222, 226, 233, 237, 244, 248, 252, 256,
    263, 267, 271, 278, 279, 286, 290, 297, 301, 305, 309, 313, 317, 321, 325,
    329, 333, 337, 341, 345, 349, 353, 357, 361, 365, 369, 373, 377, 381, 388,
    392, 399, 403, 407, 411, 415, 419, 423, 430, 434, 441, 445, 452, 456, 460,
    464, 471, 475, 479, 483, 487, 494, 498, 502, 508, 512, 519, 520, 521, 525,
    529, 532, 537, 541, 547, 548, 554, 558, 565, 566, 569, 571, 573};
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
                                      "LOAD",
                                      "WHERE",
                                      "UPDATE",
                                      "SET",
                                      "SELECT",
                                      "INT",
                                      "CHAR",
                                      "FLOAT",
                                      "DATETIME",
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
                                      "SET_OUTPUT_FILE_OFF",
                                      "LEQ",
                                      "NEQ",
                                      "GEQ",
                                      "IN",
                                      "T_EOF",
                                      "IDENTIFIER",
                                      "VALUE_STRING",
                                      "FILE_PATH",
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
                                      "filePath",
                                      YY_NULLPTR};

static const char* yysymbol_name(yysymbol_kind_t yysymbol) {
    return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-141)

#define yypact_value_is_default(Yyn) ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-116)

#define yytable_value_is_error(Yyn) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    83,   8,    20,   31,   -48,  12,   19,   -15,  -48,  -20,  9,    -141,
    -141, -141, -141, -141, -141, -141, -141, 42,   16,   -141, -141, -141,
    -141, -141, -141, 66,   -48,  -48,  -48,  -48,  -141, -141, -48,  -48,
    -141, 74,   62,   -141, -141, 37,   39,   41,   43,   48,   49,   -141,
    -141, 55,   105,  68,   112,  -141, -141, -48,  73,   76,   -141, 79,
    138,  129,  -48,  106,  98,   119,  11,   119,  119,  14,   -48,  106,
    106,  -141, 106,  106,  106,  115,  14,   -141, -141, -141, -22,  -141,
    97,   -141, 101,  116,  128,  113,  132,  131,  134,  133,  136,  -141,
    -24,  -141, 175,  -141, -14,  -141, 96,   -5,   -141, 10,   110,  -141,
    165,  103,  106,  -141, 135,  106,  177,  178,  106,  179,  106,  180,
    106,  181,  -48,  -48,  189,  106,  -141, 106,  -141, 146,  -141, -141,
    -141, -141, 106,  -141, -141, -141, -141, -141, 44,   -141, 14,   -141,
    -141, -141, -141, -141, -141, -141, 126,  -141, -141, 117,  147,  106,
    106,  149,  106,  150,  106,  151,  106,  -141, -141, 196,  197,  -141,
    -141, 159,  -141, -141, 110,  -141, 104,  -141, -141, -141, 110,  110,
    -141, 193,  -141, -141, 194,  -141, 198,  -141, 199,  -141, 14,   14,
    204,  162,  -141, 163,  53,   -141, -141, 106,  106,  106,  106,  -141,
    -141, 158,  -141, 191,  209,  -141, -141, -141, -141, -141, -141, -141,
    -141, 14,   14,   14,   -141, -141, 7,    -141, 166,  -141, -141, -141,
    14,   -141};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] = {
    0,   0,   0,   0,  0,   0,  0,   0,   0,   0,  0,  4,   3,   12,  13,  14,
    15,  6,   5,   0,  0,   10, 7,   11,  8,   9,  16, 0,   0,   0,   0,   0,
    115, 20,  0,   0,  117, 0,  0,   113, 114, 0,  0,  0,   0,   0,   116, 91,
    74,  92,  0,   0,  55,  1,  2,   0,   0,   0,  19, 0,   0,   48,  0,   0,
    0,   0,   0,   0,  0,   0,  0,   0,   0,   23, 0,  0,   0,   0,   0,   26,
    24,  116, 48,  85, 0,   17, 0,   0,   0,   0,  0,  0,   0,   0,   0,   75,
    48,  93,  54,  53, 0,   30, 0,   0,   32,  0,  0,  50,  49,  0,   0,   27,
    0,   0,   67,  57, 0,   73, 0,   69,  0,   71, 0,  0,   105, 0,   18,  0,
    35,  0,   37,  38, 34,  21, 0,   22,  43,  41, 42, 44,  0,   39,  0,   80,
    79,  81,  82,  76, 77,  78, 0,   86,  90,  0,  0,  0,   0,   0,   0,   0,
    0,   0,   0,   95, 94,  0,  109, 52,  31,  0,  33, 25,  0,   51,  0,   83,
    84,  45,  0,   0,  89,  59, 66,  56,  65,  72, 61, 68,  63,  70,  0,   0,
    28,  0,   40,  0,  0,   87, 88,  0,   0,   0,  0,  106, 107, 104, 111, 110,
    0,   29,  36,  46, 47,  58, 64,  60,  62,  0,  0,  0,   108, 112, 103, 99,
    96,  102, 101, 98, 0,   100};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -141, -141, -141, -141, -141, -141, -141, 56,   -141, 153, 100,  -141, 57,
    -85,  -140, -76,  -141, -10,  -141, -141, -141, -141, 122, -141, -141, -141,
    5,    -141, -141, -141, 17,   -141, -141, -141, -141, 15,  -58,  -141};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] = {
    0,   19,  20,  21,  22,  23,  24,  25,  100, 103, 101, 132, 140,
    141, 107, 79,  108, 109, 49,  150, 177, 82,  83,  50,  96,  209,
    223, 224, 227, 166, 204, 205, 192, 207, 41,  51,  52,  37};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    48,  78,  173, 78,  32,  84,  111, 87,   90,  92,  94,  122, 26,  98,  99,
    225, 102, 104, 104, 33,  124, 226, 34,   38,  39,  40,  28,  152, 42,  43,
    44,  45,  35,  42,  43,  44,  45,  30,   123, 36,  110, 27,  53,  56,  57,
    58,  59,  126, 127, 60,  61,  206, 84,   29,  153, 154, 133, 134, 157, 95,
    159, 46,  161, 46,  31,  175, 46,  167,  180, 102, 73,  135, 134, 47,  54,
    88,  170, 80,  221, 55,  86,  89,  91,   93,  62,  97,  1,   194, 2,   63,
    3,   4,   5,   197, 198, 6,   64,  182,  183, 65,  185, 66,  187, 67,  189,
    171, 172, 7,   68,  8,   9,   10,  -115, 5,   212, 172, 6,   69,  70,  11,
    12,  13,  14,  15,  16,  128, 129, 130,  131, 17,  8,   71,  10,  74,  18,
    72,  75,  163, 164, 76,  176, 213, 214,  215, 216, 42,  43,  44,  45,  77,
    143, 144, 145, 146, 78,  85,  112, 136,  81,  137, 138, 139, 147, 136, 113,
    137, 138, 139, 148, 149, 136, 46,  137,  138, 139, 106, 116, 114, 46,  136,
    203, 137, 138, 139, 178, 179, 174, 81,   136, 115, 137, 138, 139, 117, 118,
    119, 120, 121, 125, 142, 155, 156, 158,  160, 162, 165, 169, 203, 181, 222,
    184, 186, 188, 190, 193, 191, 199, 200,  222, 208, 217, 201, 202, 210, 211,
    218, 219, 168, 228, 105, 195, 196, 151,  229, 220};

static const yytype_uint8 yycheck[] = {
    10,  25,  142, 25,  52,  63,  82,  65,  66,  67,  68,  35, 4,   71,  72,
    8,   74,  75,  76,  4,   96,  14,  10,  8,   44,  45,  6,  112, 19,  20,
    21,  22,  13,  19,  20,  21,  22,  6,   62,  54,  62,  33, 0,   28,  29,
    30,  31,  61,  62,  34,  35,  191, 110, 33,  112, 113, 61, 62,  116, 69,
    118, 52,  120, 52,  33,  150, 52,  125, 153, 127, 55,  61, 62,  64,  58,
    64,  134, 62,  218, 13,  65,  66,  67,  68,  10,  70,  3,  172, 5,   27,
    7,   8,   9,   178, 179, 12,  59,  155, 156, 60,  158, 60, 160, 60,  162,
    61,  62,  24,  60,  26,  27,  28,  63,  9,   61,  62,  12, 62,  13,  36,
    37,  38,  39,  40,  41,  29,  30,  31,  32,  46,  26,  63, 28,  60,  51,
    23,  60,  122, 123, 60,  150, 199, 200, 201, 202, 19,  20, 21,  22,  11,
    47,  48,  49,  50,  25,  57,  59,  53,  52,  55,  56,  57, 59,  53,  63,
    55,  56,  57,  65,  66,  53,  52,  55,  56,  57,  60,  63, 61,  52,  53,
    190, 55,  56,  57,  67,  68,  60,  52,  53,  61,  55,  56, 57,  61,  63,
    61,  63,  61,  23,  34,  23,  23,  23,  23,  23,  16,  60, 217, 61,  219,
    61,  61,  61,  17,  55,  18,  23,  23,  228, 15,  62,  23, 23,  61,  61,
    34,  17,  127, 62,  76,  174, 174, 110, 228, 217};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] = {
    0,   3,   5,   7,   8,   9,   12,  24,  26,  27,  28,  36,  37,  38,  39,
    40,  41,  46,  51,  70,  71,  72,  73,  74,  75,  76,  4,   33,  6,   33,
    6,   33,  52,  104, 10,  13,  54,  106, 104, 44,  45,  103, 19,  20,  21,
    22,  52,  64,  86,  87,  92,  104, 105, 0,   58,  13,  104, 104, 104, 104,
    104, 104, 10,  27,  59,  60,  60,  60,  60,  62,  13,  63,  23,  104, 60,
    60,  60,  11,  25,  84,  104, 52,  90,  91,  105, 57,  104, 105, 64,  104,
    105, 104, 105, 104, 105, 86,  93,  104, 105, 105, 77,  79,  105, 78,  105,
    78,  60,  83,  85,  86,  62,  84,  59,  63,  61,  61,  63,  61,  63,  61,
    63,  61,  35,  62,  84,  23,  61,  62,  29,  30,  31,  32,  80,  61,  62,
    61,  53,  55,  56,  57,  81,  82,  34,  47,  48,  49,  50,  59,  65,  66,
    88,  91,  82,  105, 105, 23,  23,  105, 23,  105, 23,  105, 23,  104, 104,
    16,  98,  105, 79,  60,  105, 61,  62,  83,  60,  82,  86,  89,  67,  68,
    82,  61,  105, 105, 61,  105, 61,  105, 61,  105, 17,  18,  101, 55,  82,
    76,  81,  82,  82,  23,  23,  23,  23,  86,  99,  100, 83,  102, 15,  94,
    61,  61,  61,  105, 105, 105, 105, 62,  34,  17,  99,  83,  86,  95,  96,
    8,   14,  97,  62,  95};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] = {
    0,  69, 70,  70,  70,  70,  70,  71,  71,  71,  71,  71,  72, 72, 72,
    72, 73, 74,  75,  75,  75,  75,  75,  75,  75,  76,  76,  76, 76, 76,
    77, 77, 78,  78,  79,  80,  80,  80,  80,  81,  81,  82,  82, 82, 82,
    83, 83, 83,  84,  84,  85,  85,  86,  86,  86,  86,  86,  86, 86, 86,
    86, 86, 86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86, 86, 87,
    87, 88, 88,  88,  88,  88,  88,  88,  89,  89,  90,  90,  91, 91, 91,
    91, 92, 92,  93,  93,  93,  94,  94,  95,  96,  96,  97,  97, 97, 98,
    98, 99, 100, 100, 101, 101, 102, 102, 103, 103, 104, 105, 106};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] = {
    0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 6, 3, 2, 6, 6, 4,
    4, 7, 4, 5, 7, 8, 1, 3, 1, 3, 2, 1, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 5, 5,
    0, 2, 1, 3, 5, 3, 3, 1, 6, 4, 8, 6, 8, 6, 8, 6, 8, 6, 6, 4, 6, 4, 6, 4,
    6, 4, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 5, 4, 3, 1, 1, 1, 3, 3,
    3, 0, 2, 1, 3, 1, 1, 0, 3, 0, 1, 1, 3, 0, 2, 1, 3, 1, 1, 1, 1, 1};

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
#line 1735 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 3: /* start: HELP  */
#line 68 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = std::make_shared<Help>();
            YYACCEPT;
        }
#line 1744 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 4: /* start: EXIT  */
#line 73 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = nullptr;
            YYACCEPT;
        }
#line 1753 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 5: /* start: T_EOF  */
#line 78 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = nullptr;
            YYACCEPT;
        }
#line 1762 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 6: /* start: SET_OUTPUT_FILE_OFF  */
#line 83 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            parse_tree = std::make_shared<SetOutputFileOff>();
            YYACCEPT;
        }
#line 1771 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 12: /* txnStmt: TXN_BEGIN  */
#line 99 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnBegin>();
        }
#line 1779 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 13: /* txnStmt: TXN_COMMIT  */
#line 103 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnCommit>();
        }
#line 1787 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 14: /* txnStmt: TXN_ABORT  */
#line 107 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnAbort>();
        }
#line 1795 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 15: /* txnStmt: TXN_ROLLBACK  */
#line 111 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<TxnRollback>();
        }
#line 1803 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 16: /* dbStmt: SHOW TABLES  */
#line 118 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<ShowTables>();
        }
#line 1811 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 17: /* setStmt: SET set_knob_type '=' VALUE_BOOL  */
#line 125 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SetStmt>(
                (yyvsp[-2].sv_setKnobType), (yyvsp[0].sv_bool));
        }
#line 1819 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 18: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 132 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<CreateTable>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
        }
#line 1827 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 19: /* ddl: DROP TABLE tbName  */
#line 136 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
        }
#line 1835 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 20: /* ddl: DESC tbName  */
#line 140 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
        }
#line 1843 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 21: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 144 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<CreateIndex>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
        }
#line 1851 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 22: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 148 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str),
                                                          (yyvsp[-1].sv_strs));
        }
#line 1859 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 23: /* ddl: SHOW INDEX FROM tbName  */
#line 152 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
        }
#line 1867 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 24: /* ddl: LOAD filePath INTO tbName  */
#line 156 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<LoadData>((yyvsp[-2].sv_str),
                                                         (yyvsp[0].sv_str));
        }
#line 1875 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 25: /* dml: INSERT INTO tbName VALUES '(' valueList ')'  */
#line 163 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-4].sv_str),
                                                           (yyvsp[-1].sv_vals));
        }
#line 1883 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 26: /* dml: DELETE FROM tbName optWhereClause  */
#line 167 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str),
                                                           (yyvsp[0].sv_conds));
        }
#line 1891 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 27: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 171 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<UpdateStmt>(
                (yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses),
                (yyvsp[0].sv_conds));
        }
#line 1899 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 28: /* dml: SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds  */
#line 175 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SelectStmt>(
                (yyvsp[-5].sv_cols), (yyvsp[-3].sv_strs), (yyvsp[-2].sv_conds),
                (yyvsp[-1].sv_groupbys), (yyvsp[0].sv_conds));
        }
#line 1907 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 29: /* dml: SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds opt_order_clause  */
#line 179 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_node) = std::make_shared<SelectStmt>(
                (yyvsp[-6].sv_cols), (yyvsp[-4].sv_strs), (yyvsp[-3].sv_conds),
                (yyvsp[-2].sv_groupbys), (yyvsp[-1].sv_conds),
                (yyvsp[0].sv_orderbys));
        }
#line 1915 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 30: /* fieldList: field  */
#line 186 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_fields) =
                std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
        }
#line 1923 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 31: /* fieldList: fieldList ',' field  */
#line 190 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_fields).push_back((yyvsp[0].sv_field));
        }
#line 1931 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 32: /* colNameList: colName  */
#line 197 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
        }
#line 1939 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 33: /* colNameList: colNameList ',' colName  */
#line 201 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 1947 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 34: /* field: colName type  */
#line 208 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str),
                                                        (yyvsp[0].sv_type_len));
        }
#line 1955 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 35: /* type: INT  */
#line 215 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
        }
#line 1963 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 36: /* type: CHAR '(' VALUE_INT ')'  */
#line 219 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
        }
#line 1971 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 37: /* type: FLOAT  */
#line 223 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) =
                std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
        }
#line 1979 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 38: /* type: DATETIME  */
#line 227 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, 25);
        }
#line 1987 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 39: /* valueList: value  */
#line 234 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_vals) =
                std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
        }
#line 1995 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 40: /* valueList: valueList ',' value  */
#line 238 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_vals).push_back((yyvsp[0].sv_val));
        }
#line 2003 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 41: /* value: VALUE_INT  */
#line 245 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
        }
#line 2011 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 42: /* value: VALUE_FLOAT  */
#line 249 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
        }
#line 2019 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 43: /* value: VALUE_STRING  */
#line 253 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
        }
#line 2027 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 44: /* value: VALUE_BOOL  */
#line 257 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_val) = std::make_shared<BoolLit>((yyvsp[0].sv_bool));
        }
#line 2035 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 45: /* condition: col op expr  */
#line 264 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cond) = std::make_shared<BinaryExpr>(
                (yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
        }
#line 2043 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 46: /* condition: col op '(' dml ')'  */
#line 268 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cond) = std::make_shared<BinaryExpr>(
                (yyvsp[-4].sv_col), (yyvsp[-3].sv_comp_op),
                (yyvsp[-1].sv_node));
        }
#line 2051 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 47: /* condition: col op '(' valueList ')'  */
#line 272 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cond) = std::make_shared<BinaryExpr>(
                (yyvsp[-4].sv_col), (yyvsp[-3].sv_comp_op),
                (yyvsp[-1].sv_vals));
        }
#line 2059 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 48: /* optWhereClause: %empty  */
#line 278 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2065 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 49: /* optWhereClause: WHERE whereClause  */
#line 280 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) = (yyvsp[0].sv_conds);
        }
#line 2073 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 50: /* whereClause: condition  */
#line 287 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) =
                std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
        }
#line 2081 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 51: /* whereClause: whereClause AND condition  */
#line 291 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
        }
#line 2089 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 52: /* col: tbName '.' colName AS colName  */
#line 298 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                (yyvsp[-4].sv_str), (yyvsp[-2].sv_str), (yyvsp[0].sv_str));
        }
#line 2097 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 53: /* col: colName AS colName  */
#line 302 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-2].sv_str),
                                                   (yyvsp[0].sv_str));
        }
#line 2105 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 54: /* col: tbName '.' colName  */
#line 306 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
        }
#line 2113 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 55: /* col: colName  */
#line 310 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
        }
#line 2121 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 56: /* col: COUNT '(' '*' ')' AS colName  */
#line 314 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>("", "*", (yyvsp[0].sv_str), "COUNT");
        }
#line 2129 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 57: /* col: COUNT '(' '*' ')'  */
#line 318 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>("", "*", "count_row", "COUNT");
        }
#line 2137 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 58: /* col: SUM '(' tbName '.' colName ')' AS colName  */
#line 322 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "SUM");
        }
#line 2145 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 59: /* col: SUM '(' tbName '.' colName ')'  */
#line 326 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "sum_" + (yyvsp[-1].sv_str), "SUM");
        }
#line 2153 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 60: /* col: MAX '(' tbName '.' colName ')' AS colName  */
#line 330 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "MAX");
        }
#line 2161 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 61: /* col: MAX '(' tbName '.' colName ')'  */
#line 334 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "max_" + (yyvsp[-1].sv_str), "MAX");
        }
#line 2169 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 62: /* col: MIN '(' tbName '.' colName ')' AS colName  */
#line 338 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "MIN");
        }
#line 2177 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 63: /* col: MIN '(' tbName '.' colName ')'  */
#line 342 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "min_" + (yyvsp[-1].sv_str), "MIN");
        }
#line 2185 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 64: /* col: COUNT '(' tbName '.' colName ')' AS colName  */
#line 346 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str),
                                      (yyvsp[0].sv_str), "COUNT");
        }
#line 2193 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 65: /* col: COUNT '(' tbName '.' colName ')'  */
#line 350 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) =
                std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str),
                                      "count_" + (yyvsp[-1].sv_str), "COUNT");
        }
#line 2201 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 66: /* col: SUM '(' colName ')' AS colName  */
#line 354 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "SUM");
        }
#line 2209 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 67: /* col: SUM '(' colName ')'  */
#line 358 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "sum_" + (yyvsp[-1].sv_str), "SUM");
        }
#line 2217 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 68: /* col: MAX '(' colName ')' AS colName  */
#line 362 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "MAX");
        }
#line 2225 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 69: /* col: MAX '(' colName ')'  */
#line 366 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "max_" + (yyvsp[-1].sv_str), "MAX");
        }
#line 2233 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 70: /* col: MIN '(' colName ')' AS colName  */
#line 370 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "MIN");
        }
#line 2241 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 71: /* col: MIN '(' colName ')'  */
#line 374 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "min_" + (yyvsp[-1].sv_str), "MIN");
        }
#line 2249 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 72: /* col: COUNT '(' colName ')' AS colName  */
#line 378 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str),
                                                   (yyvsp[0].sv_str), "COUNT");
        }
#line 2257 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 73: /* col: COUNT '(' colName ')'  */
#line 382 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_col) = std::make_shared<Col>(
                "", (yyvsp[-1].sv_str), "count" + (yyvsp[-1].sv_str), "COUNT");
        }
#line 2265 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 74: /* colList: col  */
#line 389 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols) =
                std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
        }
#line 2273 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 75: /* colList: colList ',' col  */
#line 393 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols).push_back((yyvsp[0].sv_col));
        }
#line 2281 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 76: /* op: '='  */
#line 400 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_EQ;
        }
#line 2289 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 77: /* op: '<'  */
#line 404 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_LT;
        }
#line 2297 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 78: /* op: '>'  */
#line 408 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_GT;
        }
#line 2305 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 79: /* op: NEQ  */
#line 412 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_NE;
        }
#line 2313 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 80: /* op: LEQ  */
#line 416 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_LE;
        }
#line 2321 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 81: /* op: GEQ  */
#line 420 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_GE;
        }
#line 2329 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 82: /* op: IN  */
#line 424 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_comp_op) = SV_OP_IN;
        }
#line 2337 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 83: /* expr: value  */
#line 431 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
        }
#line 2345 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 84: /* expr: col  */
#line 435 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
        }
#line 2353 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 85: /* setClauses: setClause  */
#line 442 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{
                (yyvsp[0].sv_set_clause)};
        }
#line 2361 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 86: /* setClauses: setClauses ',' setClause  */
#line 446 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
        }
#line 2369 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 87: /* setClause: colName '=' colName '+' value  */
#line 453 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-4].sv_str), (yyvsp[0].sv_val), SV_OP_ADD);
        }
#line 2377 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 88: /* setClause: colName '=' colName '-' value  */
#line 457 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-4].sv_str), (yyvsp[0].sv_val), SV_OP_SUB);
        }
#line 2385 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 89: /* setClause: colName '=' colName value  */
#line 461 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-3].sv_str), (yyvsp[0].sv_val), SV_OP_ADD);
        }
#line 2393 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 90: /* setClause: colName '=' value  */
#line 465 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_set_clause) = std::make_shared<SetClause>(
                (yyvsp[-2].sv_str), (yyvsp[0].sv_val));
        }
#line 2401 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 91: /* selector: '*'  */
#line 472 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_cols) = {};
        }
#line 2409 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 93: /* tableList: tbName  */
#line 480 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
        }
#line 2417 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 94: /* tableList: tableList ',' tbName  */
#line 484 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 2425 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 95: /* tableList: tableList JOIN tbName  */
#line 488 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_strs).push_back((yyvsp[0].sv_str));
        }
#line 2433 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 96: /* opt_order_clause: ORDER BY order_clauses  */
#line 495 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys) = (yyvsp[0].sv_orderbys);
        }
#line 2441 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 97: /* opt_order_clause: %empty  */
#line 498 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2447 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 98: /* order_clause: col opt_asc_desc  */
#line 503 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby) = std::make_shared<OrderBy>(
                (yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
        }
#line 2455 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 99: /* order_clauses: order_clause  */
#line 509 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys) =
                std::vector<std::shared_ptr<OrderBy>>{(yyvsp[0].sv_orderby)};
        }
#line 2463 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 100: /* order_clauses: order_clauses ',' order_clause  */
#line 513 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderbys).push_back((yyvsp[0].sv_orderby));
        }
#line 2471 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 101: /* opt_asc_desc: ASC  */
#line 519 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_ASC;
        }
#line 2477 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 102: /* opt_asc_desc: DESC  */
#line 520 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_DESC;
        }
#line 2483 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 103: /* opt_asc_desc: %empty  */
#line 521 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_orderby_dir) = OrderBy_DEFAULT;
        }
#line 2489 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 104: /* opt_group_clause: GROUP BY group_clauses  */
#line 526 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys) = (yyvsp[0].sv_groupbys);
        }
#line 2497 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 105: /* opt_group_clause: %empty  */
#line 529 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2503 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 106: /* group_clause: col  */
#line 533 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupby) = std::make_shared<GroupBy>((yyvsp[0].sv_col));
        }
#line 2511 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 107: /* group_clauses: group_clause  */
#line 538 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys) =
                std::vector<std::shared_ptr<GroupBy>>{(yyvsp[0].sv_groupby)};
        }
#line 2519 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 108: /* group_clauses: group_clauses ',' group_clause  */
#line 542 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_groupbys).push_back((yyvsp[0].sv_groupby));
        }
#line 2527 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 109: /* opt_having_conds: %empty  */
#line 547 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        { /* ignore*/
        }
#line 2533 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 110: /* opt_having_conds: HAVING having_conds  */
#line 549 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) = (yyvsp[0].sv_conds);
        }
#line 2541 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 111: /* having_conds: condition  */
#line 555 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds) =
                std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
        }
#line 2549 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 112: /* having_conds: having_conds AND condition  */
#line 559 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
        }
#line 2557 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 113: /* set_knob_type: ENABLE_NESTLOOP  */
#line 565 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_setKnobType) = EnableNestLoop;
        }
#line 2563 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

        case 114: /* set_knob_type: ENABLE_SORTMERGE  */
#line 566 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"
        {
            (yyval.sv_setKnobType) = EnableSortMerge;
        }
#line 2569 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"
        break;

#line 2573 "/home/fleurs/Repositories/rmdb/src/parser/yacc.tab.cpp"

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

#line 574 "/home/fleurs/Repositories/rmdb/src/parser/yacc.y"

%{
#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;
%}

// request a pure (reentrant) parser
%define api.pure full
// enable location in error handler
%locations
// enable verbose syntax error message
%define parse.error verbose

// keywords
%token SHOW TABLES CREATE TABLE DROP DESC INSERT INTO VALUES DELETE FROM ASC ORDER GROUP BY HAVING SUM COUNT MAX MIN AS LOAD
WHERE UPDATE SET SELECT INT CHAR FLOAT DATETIME INDEX AND JOIN EXIT HELP TXN_BEGIN TXN_COMMIT TXN_ABORT TXN_ROLLBACK ORDER_BY GROUP_BY ENABLE_NESTLOOP ENABLE_SORTMERGE
SET_OUTPUT_FILE_OFF
// non-keywords
%token LEQ NEQ GEQ IN T_EOF

// type-specific tokens
%token <sv_str> IDENTIFIER VALUE_STRING FILE_PATH
%token <sv_int> VALUE_INT
%token <sv_float> VALUE_FLOAT
%token <sv_bool> VALUE_BOOL

// specify types for non-terminal symbol
%type <sv_node> stmt dbStmt ddl dml txnStmt setStmt
%type <sv_field> field
%type <sv_fields> fieldList
%type <sv_type_len> type
%type <sv_comp_op> op
%type <sv_expr> expr
%type <sv_val> value
%type <sv_vals> valueList
%type <sv_str> tbName colName filePath
%type <sv_strs> tableList colNameList
%type <sv_col> col
%type <sv_cols> colList selector
%type <sv_set_clause> setClause
%type <sv_set_clauses> setClauses
%type <sv_cond> condition
%type <sv_conds> whereClause optWhereClause having_conds opt_having_conds
%type <sv_orderby> order_clause
%type <sv_orderbys> order_clauses opt_order_clause
%type <sv_orderby_dir> opt_asc_desc
%type <sv_groupby> group_clause
%type <sv_groupbys> group_clauses opt_group_clause
%type <sv_setKnobType> set_knob_type

%%
start:
        stmt ';'
    {
        parse_tree = $1;
        YYACCEPT;
    }
    |   HELP
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
    |   EXIT
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    |   T_EOF
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    | SET_OUTPUT_FILE_OFF
    {
        parse_tree = std::make_shared<SetOutputFileOff>();
        YYACCEPT;
    }
    ;

stmt:
        dbStmt
    |   ddl
    |   dml
    |   txnStmt
    |   setStmt
    ;

txnStmt:
        TXN_BEGIN
    {
        $$ = std::make_shared<TxnBegin>();
    }
    |   TXN_COMMIT
    {
        $$ = std::make_shared<TxnCommit>();
    }
    |   TXN_ABORT
    {
        $$ = std::make_shared<TxnAbort>();
    }
    | TXN_ROLLBACK
    {
        $$ = std::make_shared<TxnRollback>();
    }
    ;

dbStmt:
        SHOW TABLES
    {
        $$ = std::make_shared<ShowTables>();
    }
    ;

setStmt:
        SET set_knob_type '=' VALUE_BOOL
    {
        $$ = std::make_shared<SetStmt>($2, $4);
    }
    ;

ddl:
        CREATE TABLE tbName '(' fieldList ')'
    {
        $$ = std::make_shared<CreateTable>($3, $5);
    }
    |   DROP TABLE tbName
    {
        $$ = std::make_shared<DropTable>($3);
    }
    |   DESC tbName
    {
        $$ = std::make_shared<DescTable>($2);
    }
    |   CREATE INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<CreateIndex>($3, $5);
    }
    |   DROP INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<DropIndex>($3, $5);
    }
    |   SHOW INDEX FROM tbName
    {
        $$ = std::make_shared<ShowIndex>($4);
    }
    |   LOAD filePath INTO tbName
    {
        $$ = std::make_shared<LoadData>($2, $4);
    }
    ;

dml:
        INSERT INTO tbName VALUES '(' valueList ')'
    {
        $$ = std::make_shared<InsertStmt>($3, $6);
    }
    |   DELETE FROM tbName optWhereClause
    {
        $$ = std::make_shared<DeleteStmt>($3, $4);
    }
    |   UPDATE tbName SET setClauses optWhereClause
    {
        $$ = std::make_shared<UpdateStmt>($2, $4, $5);
    }
    |   SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds
    {
        $$ = std::make_shared<SelectStmt>($2, $4, $5, $6, $7);
    }
    |   SELECT selector FROM tableList optWhereClause opt_group_clause opt_having_conds opt_order_clause
    {
        $$ = std::make_shared<SelectStmt>($2, $4, $5, $6, $7, $8);
    }
    ;

fieldList:
        field
    {
        $$ = std::vector<std::shared_ptr<Field>>{$1};
    }
    |   fieldList ',' field
    {
        $$.push_back($3);
    }
    ;

colNameList:
        colName
    {
        $$ = std::vector<std::string>{$1};
    }
    | colNameList ',' colName
    {
        $$.push_back($3);
    }
    ;

field:
        colName type
    {
        $$ = std::make_shared<ColDef>($1, $2);
    }
    ;

type:
        INT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(std::int64_t));
    }
    |   CHAR '(' VALUE_INT ')'
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_STRING, $3);
    }
    |   FLOAT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(double));
    }
    | DATETIME
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_STRING, 25);
    }
    ;

valueList:
        value
    {
        $$ = std::vector<std::shared_ptr<Value>>{$1};
    }
    |   valueList ',' value
    {
        $$.push_back($3);
    }
    ;

value:
        VALUE_INT
    {
        $$ = std::make_shared<IntLit>($1);
    }
    |   VALUE_FLOAT
    {
        $$ = std::make_shared<FloatLit>($1);
    }
    |   VALUE_STRING
    {
        $$ = std::make_shared<StringLit>($1);
    }
    |   VALUE_BOOL
    {
        $$ = std::make_shared<BoolLit>($1);
    }
    ;

condition:
        col op expr
    {
        $$ = std::make_shared<BinaryExpr>($1, $2, $3);
    }
    |   col op '(' dml ')'
    {
        $$ = std::make_shared<BinaryExpr>($1, $2, $4);
    }
    |   col op '(' valueList ')'
    {
        $$ = std::make_shared<BinaryExpr>($1, $2, $4);
    }
    ;

optWhereClause:
        /* epsilon */ { /* ignore*/ }
    |   WHERE whereClause
    {
        $$ = $2;
    }
    ;

whereClause:
        condition 
    {
        $$ = std::vector<std::shared_ptr<BinaryExpr>>{$1};
    }
    |   whereClause AND condition
    {
        $$.push_back($3);
    }
    ;

col:
        tbName '.' colName AS colName
    {
        $$ = std::make_shared<Col>($1, $3, $5);
    }
    |   colName AS colName
    {
        $$ = std::make_shared<Col>("", $1, $3);
    }
    |    tbName '.' colName
    {
        $$ = std::make_shared<Col>($1, $3);
    }
    |   colName
    {
        $$ = std::make_shared<Col>("", $1);
    }
    |   COUNT '(' '*' ')' AS colName
    {
        $$ = std::make_shared<Col>("", "*", $6, "COUNT");
    }
    |   COUNT '(' '*' ')'
    {
        $$ = std::make_shared<Col>("", "*", "count_row", "COUNT");
    }
    |   SUM '(' tbName '.' colName ')' AS colName
    {
        $$ = std::make_shared<Col>($3, $5, $8, "SUM");
    }
    |   SUM '(' tbName '.' colName ')'
    {
        $$ = std::make_shared<Col>($3, $5, "sum_" + $5, "SUM");
    }
    |   MAX '(' tbName '.' colName ')' AS colName
    {
        $$ = std::make_shared<Col>($3, $5, $8, "MAX");
    }
    |   MAX '(' tbName '.' colName ')'
    {
        $$ = std::make_shared<Col>($3, $5, "max_" + $5, "MAX");
    }
    |   MIN '(' tbName '.' colName ')' AS colName
    {
        $$ = std::make_shared<Col>($3, $5, $8, "MIN");
    }
    |   MIN '(' tbName '.' colName ')'
    {
        $$ = std::make_shared<Col>($3, $5, "min_" + $5, "MIN");
    }
    |   COUNT '(' tbName '.' colName ')' AS colName
    {
        $$ = std::make_shared<Col>($3, $5, $8, "COUNT");
    }
    |   COUNT '(' tbName '.' colName ')'
    {
        $$ = std::make_shared<Col>($3, $5, "count_" + $5, "COUNT");
    }
    |   SUM '(' colName ')' AS colName
    {
        $$ = std::make_shared<Col>("", $3, $6, "SUM");
    }
    |   SUM '(' colName ')'
    {
        $$ = std::make_shared<Col>("", $3, "sum_" + $3, "SUM");
    }
    |   MAX '(' colName ')' AS colName
    {
        $$ = std::make_shared<Col>("", $3, $6, "MAX");
    }
    |   MAX '(' colName ')'
    {
        $$ = std::make_shared<Col>("", $3, "max_" + $3, "MAX");
    }
    |   MIN '(' colName ')' AS colName
    {
        $$ = std::make_shared<Col>("", $3, $6, "MIN");
    }
    |   MIN '(' colName ')'
    {
        $$ = std::make_shared<Col>("", $3, "min_" + $3, "MIN");
    }
    |   COUNT '(' colName ')' AS colName
    {
        $$ = std::make_shared<Col>("", $3, $6, "COUNT");
    }
    |   COUNT '(' colName ')'
    {
        $$ = std::make_shared<Col>("", $3, "count" + $3, "COUNT");
    }
    ;

colList:
        col
    {
        $$ = std::vector<std::shared_ptr<Col>>{$1};
    }
    |   colList ',' col
    {
        $$.push_back($3);
    }
    ;

op:
        '='
    {
        $$ = SV_OP_EQ;
    }
    |   '<'
    {
        $$ = SV_OP_LT;
    }
    |   '>'
    {
        $$ = SV_OP_GT;
    }
    |   NEQ
    {
        $$ = SV_OP_NE;
    }
    |   LEQ
    {
        $$ = SV_OP_LE;
    }
    |   GEQ
    {
        $$ = SV_OP_GE;
    }
    |   IN
    {
        $$ = SV_OP_IN;
    }
    ;

expr:
        value
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    |   col
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    ;

setClauses:
        setClause
    {
        $$ = std::vector<std::shared_ptr<SetClause>>{$1};
    }
    |   setClauses ',' setClause
    {
        $$.push_back($3);
    }
    ;

setClause:
         colName '=' colName '+' value
    {
        $$ = std::make_shared<SetClause>($1, $5, SV_OP_ADD);
    }
    |    colName '=' colName '-' value
    {
        $$ = std::make_shared<SetClause>($1, $5, SV_OP_SUB);
    }
    |    colName '=' colName value
    {
        $$ = std::make_shared<SetClause>($1, $4, SV_OP_ADD);
    }
    |    colName '=' value
    {
        $$ = std::make_shared<SetClause>($1, $3);
    }
    ;

selector:
        '*'
    {
        $$ = {};
    }
    |   colList
    ;

tableList:
        tbName
    {
        $$ = std::vector<std::string>{$1};
    }
    |   tableList ',' tbName
    {
        $$.push_back($3);
    }
    |   tableList JOIN tbName
    {
        $$.push_back($3);
    }
    ;

opt_order_clause:
    ORDER BY order_clauses  
    { 
        $$ = $3; 
    }
    |   /* epsilon */ { /* ignore*/ }
    ;

order_clause:
      col  opt_asc_desc 
    { 
        $$ = std::make_shared<OrderBy>($1, $2);
    }
    ;
order_clauses:
      order_clause
    {
        $$ = std::vector<std::shared_ptr<OrderBy>>{$1};
    }
    |	order_clauses ',' order_clause
    {
    	$$.push_back($3);
    }
    ; 

opt_asc_desc:
    ASC          { $$ = OrderBy_ASC;     }
    |  DESC      { $$ = OrderBy_DESC;    }
    |       { $$ = OrderBy_DEFAULT; }
    ;

opt_group_clause:
    GROUP BY group_clauses
    {
        $$ = $3;
    }
    |   /* epsilon */ { /* ignore*/ }
    ;
group_clause:
      col
    {
        $$ = std::make_shared<GroupBy>($1);
    }
group_clauses:
      group_clause
    {
        $$ = std::vector<std::shared_ptr<GroupBy>>{$1};
    }
    |	group_clauses ',' group_clause
    {
        $$.push_back($3);
    }
    ;
opt_having_conds:
       /* epsilon */ { /* ignore*/ }
    |   HAVING having_conds
    {
        $$ = $2;
    }
    ;
having_conds:
        condition 
    {
        $$ = std::vector<std::shared_ptr<BinaryExpr>>{$1};
    }
    |   having_conds AND condition
    {
        $$.push_back($3);
    }
    ;

set_knob_type:
    ENABLE_NESTLOOP { $$ = EnableNestLoop; }
    |   ENABLE_SORTMERGE { $$ = EnableSortMerge; }
    ;

tbName: IDENTIFIER;

colName: IDENTIFIER;

filePath: FILE_PATH;
%%

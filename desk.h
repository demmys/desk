#include <stdio.h>

/*
 * Storage
 */
#define DEFAULT_PAGE_SIZE 1024

typedef union{
    long l_dummy;
    double d_dummy;
    void *p_dummy;
} Cell;

typedef struct MemoryPage_tag{
    int cell_num;
    int use_cell_num;
    struct MemoryPage_tag *next;
    Cell cell[1];
} MemoryPage;

typedef MemoryPage *MemoryPageList;

typedef struct {
    MemoryPageList *page_list;
    int current_page_size;
} Storage;

Storage *open_storage(int page_size){
    Storage storage;

    storage = malloc(sizeof(Storage));
    storage -> page_list = NULL;
    if(page_size > 0){
        storage -> current_page_size = page_size;
    } else{
        storage -> current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}

void *storage_malloc(Storage storage, size_t size){
    int cell_num;
    MemoryPage *new_page;
    void *p;

    cell_num = ((size - 1) / sizeof(Cell)) + 1;

    if(storage -> page_list && (storage -> page_list -> use_cell_num < storage -> page_list -> cell_num)){
        p = &(storage -> page_list -> cell[storage -> page_list -> use_cell_num]);
        storage -> page_list -> use_cell_num += cell_num;
    } else{
        int alloc_cell_num;

        alloc_cell_num = max(cell_num, storage -> current_page_size);
        new_page = malloc(sizeof(MemoryPage) + sizeof(Cell) * (alloc_cell_num - 1));
        new_page -> next = storage -> page_list;
        new_page -> cell_num = alloc_cell_num;
        storage -> page_list = new_page;
        p = &(new_page -> cell[0]);
        new_page -> use_cell_num = cell_num;
    }

    return p;
}

void dispose_storage(Storage storage){
    MemoryPage *temp;
    
    while(storage -> page_list){
        temp = storage -> page_list -> next;
        free(storage -> page_list);
        storage -> page_list = temp;
    }
    free(storage);
}

/*
 * Compiler
 */
typedef struct {
    Storage compile_storage;
    Statement *main_statement; // FunctionList *function_list;
    //int function_count;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
} Compiler;

/*
 * Expression
 */
typedef struct Expression_tag{
    BasicType type; // TypeSpecifier *type;
    ExpressionKind kind;
    //int line_number;
    union{
        int int_value;
        double float_value;
        char char_value;
        BinaryExpression binary_expression;
        struct Expression_tag *minus_expression;
    } u;
} Expression;

// TypeSpecifier
/*
typedef struct{
    BasicType basic_type;
    TypeDerive *derive;
} TypeSpecifier;

typedef struct TypeDerive_tag{
    DeriveTag tag;
    union{
        FunctionDerive function_d;
    } u;
    struct TypeDerive_tag *next;
} TypeDerive;

typedef enum{
    FUNCTION_DERIVE
} DeriveTag;

typedef struct{
    ParameterList *parameter_list;
} FunctionDerive;

typedef struct ParameterList_tag{
    char *name;
    TypeSpecifier *type;
    int line_number;
    struct ParameterList_tag *next;
} ParameterList;
*/

typedef enum{
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} BasicType;

// ExpressionKind
typedef enum{
    INT_EXPRESSION = 1,
    FLOAT_EXPRESSION,
    CHAR_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    MINUS_EXPRESSION,
    EXPRESSION_KIND_COUNT_PLUS_1
} ExpressionKind;

// BinaryExpression
typedef struct {
    Expression  *left;
    Expression  *right;
} BinaryExpression;

/*
 * Statement
 */
typedef struct{
    StatementType type;
    //int line_number;
    union{
        Expression *expression_s;
    } u;
} Statement;

typedef enum{
    EXPRESSION_STATEMENT = 1,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

/*
 * function prototype
 */
// TODO
void main_define(Statement *statement){
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler -> main_statement){
        compile_error();
    } else{
        compiler -> main_statement = statement;
    }
}

Statement *create_expression_statement(Expression *expression){
    Statement *st;

    st = alloc_statement(EXPRESSION_STATEMENT);
    st -> u.expression_s = expression;

    return st;
}

Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right){
    Expression *exp;

    exp = alloc_expression(operator);
    exp -> u.binary_expression.left = left;
    exp -> u.binary_expression.right = right;

    return exp;
}

Expression *create_minus_expression(Expression *operand){
    Expression *exp;

    exp = alloc_expression(MINUS_EXPRESSION);
    exp -> u.minus_expression = operand;

    return exp;
}

/*
 * storage allocable
 */
Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;

    exp = compiler_storage_malloc(sizeof(Expression));
    exp -> type = NULL;
    exp -> kind = kind;
    //exp -> line_number = get_current_compiler() -> current_line_number;
    //
    return exp;
}

static Statement* alloc_statement(StatementType type){
    Statement *st;

    st = compiler_storage_malloc(sizeof(Statement));
    st -> type = type;
    //st -> line_number = get_current_compiler() -> current_line_number;
    
    return st;
}

/*
 * utilities
 */
static Compiler *static_current_compiler;

Compiler *get_current_compiler(){
    return static_current_compiler;
}

void set_current_compiler(Compiler *compiler){
    static_current_compiler = compiler;
}

void *compiler_storage_malloc(size_t size){
    void *p;
    Compiler *compiler;

    compiler = get_current_compiler();
    p = storage_malloc(compiler -> compile_storage, size);

    return p;
}

Compiler *create_compiler(){
    Storage storage;
    Compiler *compiler;
    
    storage = open_storage(0);
    compiler = storage_malloc(storage, sizeof(struct Compiler));
    compiler -> compile_storage = storage;
    compiler -> main_statement = NULL;
    compiler -> current_line_number = 1;
    set_current_compiler(compiler);
    return compiler;
}

void dispose_compiler(Compiler *compiler){
    free(compiler -> main_statement);
    dispose_storage(compiler -> compile_storage);
}

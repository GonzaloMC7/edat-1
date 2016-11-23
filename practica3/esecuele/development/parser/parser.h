/* PARSER MODULE */
#include "../database/database.h"

/* parses the query to create an operation that executes it */
operation_t* parser_operation(char* statement, database_t* database);

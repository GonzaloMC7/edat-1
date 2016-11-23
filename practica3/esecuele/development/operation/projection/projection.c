#include "../operation.h"
#include "../../type/type.h"
#include "projection.h"

/* INTERNALS OF THE PROJECTION MODULE */

void*
projection_project(projection_t* projection, struct operation_* operation) {
    return projection->project(operation, projection->args);
}

void
projection_close(projection_t* projection) {
    projection->close(projection->args);
    free(projection);
}

type_t
projection_type(projection_t* projection) {
    return projection->type;
}

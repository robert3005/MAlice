module.exports = (() ->
    Types = {
        NODE_ROOT: "ROOT",
        NODE_OP: "OPERATOR",
        NODE_VAR: "VARIABLE",
        NODE_VAR_ARRAY: "ARRAY",
        NODE_VAR_ARRAY_ELEM: "ARRAY_ELEM",
        NODE_CONST: "CONSTANT",
        NODE_TYPE: "TYPE",
        NODE_RETURN: "RETURN",
        NODE_IO: "IO",
        NODE_LOOP: "WHILE",
        NODE_FUN: "FUNCTION",
        NODE_FUN_DEF: "FUNCTION_DEFINITION",
        NODE_FUN_CALL: "FUNCTION_CALL",
        NODE_LOOK_DEF: "LOOKING_GLASS",
        NODE_LOOK: "THROUGH_LOOKING_GLASS",
        NODE_IF: "IF",
        NODE_ELSE_IF: "ELSE_IF",
        NODE_ELSE: "ELSE",
        NODE_END_IF: "END_IF",

        OP_NONE: "NO_OP",
        OP_ADD: "ADD",
        OP_OR: "OR",
        OP_XOR: "XOR",
        OP_AND: "AND",
        OP_SUB: "SUBTRACT",
        OP_MUL: "MULTIPLY",
        OP_DIV: "DIVIDE",
        OP_MOD: "MOD",
        OP_NOT: "NOT",
        OP_NEG: "NEGATE",
        OP_LOR: "LOGICAL_OR",
        OP_LAND: "LOGICAL_AND",
        OP_EQ: "EQUAL",
        OP_UEQ: "NOT_EQUAL",
        OP_LT: "LESS_THAN",
        OP_GT: "GREATER_THAN",
        OP_LTE: "LESS_THAN_EQUAL",
        OP_GTE: "GREATER_THAN_EQUAL"

        intOperators: [@OP_ADD, @OP_OR, @OP_XOR, @OP_AND, @OP_SUB, @OP_MUL, @OP_DIV, @OP_MOD, @OP_NEG, @OP_NOT]
        boolOperators: [@OP_NOT, @OP_LOR, @OP_LAND, @OP_EQ, @OP_UEQ, @OP_LT, @OP_GT, @OP_LTE, @OP_GTE]

        TYPE_ARRAY: "array"
        TYPE_SENTENCE: "sentence"
        TYPE_LETTER: "letter"
        TYPE_NUMBER: "number"
    }
)()
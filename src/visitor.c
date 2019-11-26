#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "visitor.h"

#define printm(...) //printf(__VA_ARGS__)

FILE *fp;
int ssa_counter = 0;

void visit_file (AST *root) {
	char *filename = root->list.first->ast->decl.function.token->filename;
	filename[strlen(filename) - 2] = '\0';
	filename = strcat(filename, ".ll");
	fp = fopen(filename, "w");

	printm(">>> file\n");
	printm("file has %d declarations\n", root->list.num_items);

	for (ListNode *ptr = root->list.first; ptr != NULL; ptr = ptr->next) {
		switch (ptr->ast->decl.type) {
		case FUNCTION_DECLARATION:
			visit_function_decl(ptr->ast); break;
		case VARIABLE_DECLARATION:
			visit_var_decl(ptr->ast); break;
		default:
			fprintf(stderr, "UNKNOWN DECLARATION TYPE %c\n", ptr->ast->decl.type);
			break;
		}
	}
	printm("<<< file\n");

	fclose(fp);
}

void visit_function_decl (AST *ast) {
	printm(">>> function_decl\n");
	AST *params = ast->decl.function.param_decl_list;
	int type = ast->decl.function.type;

	fprintf(fp, "\ndefine ");

	if (type == TYPE_INT) {
		fprintf(fp, "i32 ");
	} else if (type == TYPE_VOID) {
		fprintf(fp, "void ");
	}

	fprintf(fp, "@%s(", ast->decl.function.id->id.string);

	if (params != NULL) {
		for (ListNode *param = params->list.first; param != NULL; param = param->next) {
			printm("  param");

			if (param->ast->decl.variable.type == TYPE_INT) {
				fprintf(fp, "i32");

				if (param->next != NULL) {
					fprintf(fp, ", ");
				}
			} else {
				printf("Invalid variable type!\n");
			}
		}
		printm("\n");
	}

	fprintf(fp, ") #0 {\n");

	ExprResult ret = { 0, VOID_CONSTANT };
	if (ast->decl.function.stat_block != NULL) {
		ret = visit_stat_block(ast->decl.function.stat_block, params, ast->decl.function.type);
	}

	switch (ret.type) {
	case INTEGER_CONSTANT:
		fprintf(fp, "\tret i32 %ld\n}\n", ret.int_value);
		break;
	case VOID_CONSTANT:
		fprintf(fp, "\tret void\n}\n");
		break;
	case LLIR_REGISTER:
		fprintf(fp, "\tret i32 %%%ld\n}\n", ret.ssa_register);
		break;
	default:
		printf("Invalid function return type!\n");
		break;
	}

	printm("<<< function_decl\n");	
}

// what is surrounded by { }
ExprResult visit_stat_block (AST *stat_block, AST *params, int return_type) {
	printm(">>> stat_block\n");
	ExprResult ret = { 0, VOID_CONSTANT };

	ssa_counter = 1;

	// alloca and store for params
	if (params != NULL) {
		int i = 0;
		ssa_counter += params->list.num_items;

		for (ListNode *param = params->list.first; param != NULL; param = param->next) {
			if (param->ast->decl.variable.type == TYPE_INT) {
				param->ast->decl.variable.id->id.type = NO_TYPE;
				param->ast->decl.variable.id->id.ssa_register = ssa_counter;
				fprintf(fp, "\t%%%d = alloca i32, align 4\n", ssa_counter);
				ssa_counter++;
				fprintf(fp, "\tstore i32 %%%d, i32* %%%ld, align 4\n", i, param->ast->decl.variable.id->id.ssa_register);
				i++;
			} else {
				printf("Invalid variable type!\n");
			}
		}
	}

	// Visit variable declarations
	ListNode *ptr = stat_block->list.first;
	for (; ptr != NULL; ptr = ptr->next) {
		if (ptr->ast->stat.type != VARIABLE_DECLARATION) {
			break;
		}
		ret = visit_stat(ptr->ast);
	}

	// Visit other statements
	for (ListNode *ptr2 = ptr; ptr2 != NULL; ptr2 = ptr2->next) {
		ret = visit_stat(ptr2->ast);
	}

	printm("<<< stat_block\n");
	return  ret;
}


ExprResult visit_stat (AST *stat) {
	printm(">>> statement\n");
	ExprResult ret = { 0, VOID_CONSTANT };

	switch (stat->stat.type) {
	case VARIABLE_DECLARATION:
		visit_var_decl(stat); break;
	case ASSIGN_STATEMENT:
		visit_assign_stat(stat); break;
	case RETURN_STATEMENT:
		ret = visit_return_stat(stat); break;
	case EXPRESSION_STATEMENT:
		visit_expr(stat->stat.expr.expr); break;
	default: fprintf(stderr, "UNKNOWN STATEMENT TYPE %c\n", stat->stat.type); break;
	}

	printm("<<< statement\n");
	return ret;
}

void visit_var_decl (AST *ast) {
	printm(">>> var_decl\n");
	AST *id = ast->decl.variable.id;

	if (id->id.flags == IS_GLOBAL) {
		if (ast->decl.variable.expr != NULL) {
			fprintf(fp, "@%s = global int32 ", id->id.string);

			ExprResult expr = visit_expr(ast->decl.variable.expr);

			if (expr.type == INTEGER_CONSTANT) {
				fprintf(fp, "%ld, align 4\n", expr.int_value);
			} else {
				//
			}
		} else {
			fprintf(fp, "@%s = common global int32 0, align 4\n", id->id.string);
		}
	} else {
		if (ast->decl.variable.type == TYPE_INT) {
			id->id.type = NO_TYPE;
			id->id.ssa_register = ssa_counter;
			fprintf(fp, "\t%%%d = alloca i32, align 4\n", ssa_counter);
			ssa_counter++;
		} else {
			printf("Invalid variable type!\n");
		}

		if (ast->decl.variable.expr != NULL) {
			ExprResult expr = visit_expr(ast->decl.variable.expr);

			if (expr.type == LLIR_REGISTER) {
				fprintf(fp, "\tstore i32 %%%ld, i32 %%%ld, align 4\n", expr.ssa_register, id->id.ssa_register);
			} else {
				fprintf(fp, "\tstore i32 %ld, i32 %%%ld, align 4\n", expr.int_value, id->id.ssa_register);
			}
		} else {
			if (ast->decl.variable.type == TYPE_INT) {
				fprintf(fp, "\tstore i32 0, i32 %%%ld, align 4\n", id->id.ssa_register);
			} else {
				printf("Invalid variable type!\n");
			}
		}
	}

	printm("<<< var_decl\n");
}


ExprResult visit_return_stat (AST *ast) {
	printm(">>> return stat\n");
	ExprResult ret = { 0, VOID_CONSTANT };

	if (ast->stat.ret.expr) {
		ret = visit_expr(ast->stat.ret.expr);
	}

	printm("<<< return stat\n");
	return ret;
}

void visit_assign_stat (AST *assign) {
	printm(">>> assign stat\n");
	ExprResult expr = visit_expr(assign->stat.assign.expr);

	fprintf(fp, "\tstore i32 ");
	visit_operand(expr);
	fprintf(fp, ", i32 %s, align 4\n", id_ref(assign->stat.assign.id->id));

	printm("<<< assign stat\n");
}

ExprResult visit_expr (AST *expr) {
	printm(">>> expression\n");
	ExprResult ret = {};

	switch (expr->expr.type) {
	case BINARY_EXPRESSION:
		switch (expr->expr.binary_expr.operation) {
		case '+':
			ret = visit_add(expr); break;
		case '-':
			ret = visit_sub(expr); break;
		case '*':
			ret = visit_mul(expr); break;
		case '/':
			ret = visit_div(expr); break;
		case '%':
			ret = visit_mod(expr); break;
		default:
			fprintf(stderr, "UNKNOWN OPERATOR %c\n", expr->expr.binary_expr.operation); break;
		}
		break;
	case UNARY_MINUS_EXPRESSION:
		ret = visit_unary_minus(expr); break;
	case LITERAL_EXPRESSION:
		ret = visit_literal(expr); break;
	case IDENTIFIER_EXPRESSION:
		ret = visit_id(expr->expr.id.id); break;
	case FUNCTION_CALL_EXPRESSION:
		ret = visit_function_call(expr); break;
	default:
		fprintf(stderr, "UNKNOWN EXPRESSION TYPE %c\n", expr->expr.type);
		break;
	}

	printm("<<< expression\n");
	return ret;
}

// não implementar
ExprResult visit_function_call (AST *ast) {
	printm(">>> function_call\n");
	ExprResult ret = {}, arg_expr[20]; //instead of alloca
	AST *arg_list = ast->expr.function_call.expr_list;

	if (arg_list != NULL) {
		int i = 0;
		for (ListNode *ptr = arg_list->list.first; ptr != NULL; ptr = ptr->next) {
			arg_expr[i++] = visit_expr(ptr->ast);
		}
	}

	printm("<<< function_call\n");
	return ret;
}

ExprResult visit_id (AST *ast) {
	printm(">>> identifier\n");
	ExprResult ret = {};

	ret.type = LLIR_REGISTER;
	ret.ssa_register = ast->id.ssa_register;

	printm("<<< identifier\n");
	return ret;
}

ExprResult visit_literal (AST *ast) {
	printm(">>> literal\n");
	ExprResult ret = {};

	ret.type = INTEGER_CONSTANT;
	ret.int_value = ast->expr.literal.int_value;

	printm("<<< literal\n");
	return ret;
}

ExprResult visit_unary_minus (AST *ast) {
	printm(">>> unary_minus\n");
	ExprResult expr, ret = {};

	expr = visit_expr(ast->expr.unary_minus.expr);

	if (ast->expr.unary_minus.expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.unary_minus.expr->expr.id.id->id));
		expr.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (expr.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = - expr.int_value;
	} else {
		fprintf(fp, "\t%%%d = sub nsw i32 0,", ssa_counter);
		visit_operand(expr);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< unary_minus\n");
	return ret;
}

ExprResult visit_add (AST *ast) {
	printm(">>> add\n");
	ExprResult left, right, ret = {};

	left  = visit_expr(ast->expr.binary_expr.left_expr);
	right = visit_expr(ast->expr.binary_expr.right_expr);

	if (ast->expr.binary_expr.left_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.left_expr->expr.id.id->id));
		left.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (ast->expr.binary_expr.right_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.right_expr->expr.id.id->id));
		right.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = left.int_value + right.int_value;
	} else {
		fprintf(fp, "\t%%%d = add nsw i32 ", ssa_counter);
		visit_operand(left);
		fprintf(fp, ", ");
		visit_operand(right);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< add\n");
	return ret;
}

ExprResult visit_sub (AST *ast) {
	printm(">>> sub\n");
	ExprResult left, right, ret = {};

	left  = visit_expr(ast->expr.binary_expr.left_expr);
	right = visit_expr(ast->expr.binary_expr.right_expr);

	if (ast->expr.binary_expr.left_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.left_expr->expr.id.id->id));
		left.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (ast->expr.binary_expr.right_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.right_expr->expr.id.id->id));
		right.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = left.int_value - right.int_value;
	} else {
		fprintf(fp, "\t%%%d = sub nsw i32 ", ssa_counter);
		visit_operand(left);
		fprintf(fp, ", ");
		visit_operand(right);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< sub\n");
	return ret;
}

ExprResult visit_mul (AST *ast) {
	printm(">>> mul\n");
	ExprResult left, right, ret = {};

	left  = visit_expr(ast->expr.binary_expr.left_expr);
	right = visit_expr(ast->expr.binary_expr.right_expr);

	if (ast->expr.binary_expr.left_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.left_expr->expr.id.id->id));
		left.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (ast->expr.binary_expr.right_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.right_expr->expr.id.id->id));
		right.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = left.int_value * right.int_value;
	} else {
		fprintf(fp, "\t%%%d = mul nsw i32 ", ssa_counter);
		visit_operand(left);
		fprintf(fp, ", ");
		visit_operand(right);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< mul\n");
	return ret;
}

ExprResult visit_div (AST *ast) {
	printm(">>> div\n");
	ExprResult left, right, ret = {};

	left  = visit_expr(ast->expr.binary_expr.left_expr);
	right = visit_expr(ast->expr.binary_expr.right_expr);

	if (ast->expr.binary_expr.left_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.left_expr->expr.id.id->id));
		left.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (ast->expr.binary_expr.right_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.right_expr->expr.id.id->id));
		right.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = left.int_value / right.int_value;
	} else {
		fprintf(fp, "\t%%%d = sdiv i32 ", ssa_counter);
		visit_operand(left);
		fprintf(fp, ", ");
		visit_operand(right);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< div\n");
	return ret;
}

ExprResult visit_mod (AST *ast) {
	printm(">>> mod\n");
	ExprResult left, right, ret = {};

	left  = visit_expr(ast->expr.binary_expr.left_expr);
	right = visit_expr(ast->expr.binary_expr.right_expr);

	if (ast->expr.binary_expr.left_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.left_expr->expr.id.id->id));
		left.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (ast->expr.binary_expr.right_expr->expr.type == IDENTIFIER_EXPRESSION) {
		fprintf(fp, "\t%%%d = load i32, i32* %s, align 4\n", ssa_counter, id_ref(ast->expr.binary_expr.right_expr->expr.id.id->id));
		right.ssa_register = ssa_counter;
		ssa_counter++;
	}

	if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT) {
		ret.type = INTEGER_CONSTANT;
		ret.int_value = left.int_value % right.int_value;
	} else {
		fprintf(fp, "\t%%%d = srem i32 ", ssa_counter);
		visit_operand(left);
		fprintf(fp, ", ");
		visit_operand(right);
		fprintf(fp, "\n");
		ret.type = LLIR_REGISTER;
		ret.ssa_register = ssa_counter++;
	}

	printm("<<< mod\n");
	return ret;
}

void visit_operand(ExprResult expr) {
	switch (expr.type) {
	case INTEGER_CONSTANT:
		fprintf(fp, "%ld", expr.int_value);
		break;
	case LLIR_REGISTER:
		fprintf(fp, "%%%ld", expr.ssa_register);
		break; 
	default:
		printf("Invalid operand type!\n");
		break;
	}
}

const char* id_ref(Identifier id) {
	static char ref[20];

	if (id.flags == IS_GLOBAL) {
		sprintf(ref, "@%s", id.string);
	} else {
		sprintf(ref, "%%%ld", id.ssa_register);
	}

	return ref;
}
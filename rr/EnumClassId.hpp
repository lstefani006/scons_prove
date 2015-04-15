#ifndef __EnumClassId_hpp__
#define __EnumClassId_hpp__

enum Expr_IdType
{	ExprNum_Id = 1,
	ExprId_Id = 2,
	ExprStr_Id = 3,
	ExprBin_Id = 4,
	ExprUni_Id = 5,
	ExprCast_Id = 6,
	ExprDot_Id = 7,
	ExprPost_Id = 8,
	ExprPre_Id = 9,
	ExprNew_Id = 10,
	ExprNewArray_Id = 11,
	ExprArray_Id = 12,
	ExprCall_Id = 13,
	ExprList_Id = 14,
};

enum Stmt_IdType
{	StmtIf_Id = 1,
	StmtWhile_Id = 2,
	StmtFor_Id = 3,
	StmtForEach_Id = 4,
	StmtExpr_Id = 5,
	StmtList_Id = 6,
	StmtDecl_Id = 7,
};

enum Type_IdType
{	TypeArray_Id = 1,
	TypeId_Id = 2,
	TypeDot_Id = 3,
};

enum Decl_IdType
{	DeclFun_Id = 1,
	DeclVar_Id = 2,
	DeclEnum_Id = 3,
	DeclList_Id = 4,
	DeclClass_Id = 5,
};

#endif // __EnumClassId_hpp__

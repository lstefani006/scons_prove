#ifndef __CheckType_ExprVisitor___
#define __CheckType_ExprVisitor___
#include "c.hpp"

struct ExprVisitor_CheckType {
	static Type * CheckType_Expr(Expr *v, Context *ctx);
private:
	static Type * CheckType_ExprNum(ExprNum *v, Context *ctx);
	static Type * CheckType_ExprId(ExprId *v, Context *ctx);
	static Type * CheckType_ExprStr(ExprStr *v, Context *ctx);
	static Type * CheckType_ExprBin(ExprBin *v, Context *ctx);
	static Type * CheckType_ExprUni(ExprUni *v, Context *ctx);
	static Type * CheckType_ExprCast(ExprCast *v, Context *ctx);
	static Type * CheckType_ExprDot(ExprDot *v, Context *ctx);
	static Type * CheckType_ExprPost(ExprPost *v, Context *ctx);
	static Type * CheckType_ExprPre(ExprPre *v, Context *ctx);
	static Type * CheckType_ExprNew(ExprNew *v, Context *ctx);
	static Type * CheckType_ExprNewArray(ExprNewArray *v, Context *ctx);
	static Type * CheckType_ExprArray(ExprArray *v, Context *ctx);
	static Type * CheckType_ExprCall(ExprCall *v, Context *ctx);
	static Type * CheckType_ExprList(ExprList *v, Context *ctx);
};

/*
Type * ExprVisitor_CheckType::CheckType_ExprNum(ExprNum *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprId(ExprId *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprStr(ExprStr *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprBin(ExprBin *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprUni(ExprUni *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprCast(ExprCast *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprDot(ExprDot *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprPost(ExprPost *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprPre(ExprPre *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprNew(ExprNew *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprNewArray(ExprNewArray *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprArray(ExprArray *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprCall(ExprCall *v, Context *ctx) {}
Type * ExprVisitor_CheckType::CheckType_ExprList(ExprList *v, Context *ctx) {}
*/
inline Type * ExprVisitor_CheckType::CheckType_Expr(Expr *v, Context *ctx) {
	switch (v->getType()) {
	case ExprNum_Id:
		return CheckType_ExprNum((ExprNum *)v, ctx);
	case ExprId_Id:
		return CheckType_ExprId((ExprId *)v, ctx);
	case ExprStr_Id:
		return CheckType_ExprStr((ExprStr *)v, ctx);
	case ExprBin_Id:
		return CheckType_ExprBin((ExprBin *)v, ctx);
	case ExprUni_Id:
		return CheckType_ExprUni((ExprUni *)v, ctx);
	case ExprCast_Id:
		return CheckType_ExprCast((ExprCast *)v, ctx);
	case ExprDot_Id:
		return CheckType_ExprDot((ExprDot *)v, ctx);
	case ExprPost_Id:
		return CheckType_ExprPost((ExprPost *)v, ctx);
	case ExprPre_Id:
		return CheckType_ExprPre((ExprPre *)v, ctx);
	case ExprNew_Id:
		return CheckType_ExprNew((ExprNew *)v, ctx);
	case ExprNewArray_Id:
		return CheckType_ExprNewArray((ExprNewArray *)v, ctx);
	case ExprArray_Id:
		return CheckType_ExprArray((ExprArray *)v, ctx);
	case ExprCall_Id:
		return CheckType_ExprCall((ExprCall *)v, ctx);
	case ExprList_Id:
		return CheckType_ExprList((ExprList *)v, ctx);
	default:
		assert(false);
		break;
	}
}

inline Type * CheckType(Expr *v, Context *ctx) {
	return ExprVisitor_CheckType::CheckType_Expr(v, ctx);
}
#endif // __CheckType_ExprVisitor__

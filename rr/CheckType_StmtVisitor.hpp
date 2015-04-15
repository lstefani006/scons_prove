#ifndef __CheckType_StmtVisitor___
#define __CheckType_StmtVisitor___
#include "c.hpp"

struct StmtVisitor_CheckType {
	static Type * CheckType_Stmt(Stmt *v, Context *ctx);
private:
	static Type * CheckType_StmtIf(StmtIf *v, Context *ctx);
	static Type * CheckType_StmtWhile(StmtWhile *v, Context *ctx);
	static Type * CheckType_StmtFor(StmtFor *v, Context *ctx);
	static Type * CheckType_StmtForEach(StmtForEach *v, Context *ctx);
	static Type * CheckType_StmtExpr(StmtExpr *v, Context *ctx);
	static Type * CheckType_StmtList(StmtList *v, Context *ctx);
	static Type * CheckType_StmtDecl(StmtDecl *v, Context *ctx);
};

/*
Type * StmtVisitor_CheckType::CheckType_StmtIf(StmtIf *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtWhile(StmtWhile *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtFor(StmtFor *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtForEach(StmtForEach *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtExpr(StmtExpr *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtList(StmtList *v, Context *ctx) {}
Type * StmtVisitor_CheckType::CheckType_StmtDecl(StmtDecl *v, Context *ctx) {}
*/
inline Type * StmtVisitor_CheckType::CheckType_Stmt(Stmt *v, Context *ctx) {
	switch (v->getType()) {
	case StmtIf_Id:
		return CheckType_StmtIf((StmtIf *)v, ctx);
	case StmtWhile_Id:
		return CheckType_StmtWhile((StmtWhile *)v, ctx);
	case StmtFor_Id:
		return CheckType_StmtFor((StmtFor *)v, ctx);
	case StmtForEach_Id:
		return CheckType_StmtForEach((StmtForEach *)v, ctx);
	case StmtExpr_Id:
		return CheckType_StmtExpr((StmtExpr *)v, ctx);
	case StmtList_Id:
		return CheckType_StmtList((StmtList *)v, ctx);
	case StmtDecl_Id:
		return CheckType_StmtDecl((StmtDecl *)v, ctx);
	default:
		assert(false);
		break;
	}
}

inline Type * CheckType(Stmt *v, Context *ctx) {
	return StmtVisitor_CheckType::CheckType_Stmt(v, ctx);
}
#endif // __CheckType_StmtVisitor__

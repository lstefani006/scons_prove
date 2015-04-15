#include <assert.h>
#include <string.h>
#include "CheckType_ExprVisitor.hpp"
#include "CheckType_DeclVisitor.hpp"

Type * ExprVisitor_CheckType::CheckType_ExprNum(ExprNum *v, Context *ctx) {
	return new TypeId(strdup("int"));
}
// ritorno copia del tipo associato alla variable.
Type * ExprVisitor_CheckType::CheckType_ExprId(ExprId *v, Context *ctx) {
	DeclVar *dv = ctx->findVar(v->id);
	return dv->t->clone();
}
Type * ExprVisitor_CheckType::CheckType_ExprStr(ExprStr *v, Context *ctx) {
	return new TypeId(strdup("str"));
}
Type * ExprVisitor_CheckType::CheckType_ExprBin(ExprBin *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprUni(ExprUni *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprCast(ExprCast *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprDot(ExprDot *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprPost(ExprPost *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprPre(ExprPre *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprNew(ExprNew *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprNewArray(ExprNewArray *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprArray(ExprArray *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprCall(ExprCall *v, Context *ctx) {
	return nullptr;
}
Type * ExprVisitor_CheckType::CheckType_ExprList(ExprList *v, Context *ctx) {
	return nullptr;
}

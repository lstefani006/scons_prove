#include <assert.h>
#include "CheckType_DeclVisitor.hpp"
#include "CheckType_TypeVisitor.hpp"
#include "CheckType_ExprVisitor.hpp"


Type * DeclVisitor_CheckType::CheckType_DeclFun(DeclFun *v, Context *ctx) {
	ctx->push(v);
	ctx->pop();
	return nullptr;
}

Type * DeclVisitor_CheckType::CheckType_DeclVar(DeclVar *v, Context *ctx) {
	ctx->add(v);
	if (v->t == nullptr)
		v->t = CheckType(v->e, ctx);
	return v->t;
}
Type * DeclVisitor_CheckType::CheckType_DeclEnum(DeclEnum *v, Context *ctx) {
	return nullptr;
}
Type * DeclVisitor_CheckType::CheckType_DeclList(DeclList *v, Context *ctx) {
	return nullptr;
}
Type * DeclVisitor_CheckType::CheckType_DeclClass(DeclClass *v, Context *ctx) {
	ctx->push(v);
	
	for (auto d : v->dd->v)
		CheckType(d, ctx);
	
	ctx->pop();
	return nullptr;
}

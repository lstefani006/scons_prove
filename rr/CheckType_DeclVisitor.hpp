#ifndef __CheckType_DeclVisitor___
#define __CheckType_DeclVisitor___
#include "c.hpp"

struct DeclVisitor_CheckType {
	static Type * CheckType_Decl(Decl *v, Context *ctx);
private:
	static Type * CheckType_DeclFun(DeclFun *v, Context *ctx);
	static Type * CheckType_DeclVar(DeclVar *v, Context *ctx);
	static Type * CheckType_DeclEnum(DeclEnum *v, Context *ctx);
	static Type * CheckType_DeclList(DeclList *v, Context *ctx);
	static Type * CheckType_DeclClass(DeclClass *v, Context *ctx);
};

/*
Type * DeclVisitor_CheckType::CheckType_DeclFun(DeclFun *v, Context *ctx) {}
Type * DeclVisitor_CheckType::CheckType_DeclVar(DeclVar *v, Context *ctx) {}
Type * DeclVisitor_CheckType::CheckType_DeclEnum(DeclEnum *v, Context *ctx) {}
Type * DeclVisitor_CheckType::CheckType_DeclList(DeclList *v, Context *ctx) {}
Type * DeclVisitor_CheckType::CheckType_DeclClass(DeclClass *v, Context *ctx) {}
*/
inline Type * DeclVisitor_CheckType::CheckType_Decl(Decl *v, Context *ctx) {
	switch (v->getType()) {
	case DeclFun_Id:
		return CheckType_DeclFun((DeclFun *)v, ctx);
	case DeclVar_Id:
		return CheckType_DeclVar((DeclVar *)v, ctx);
	case DeclEnum_Id:
		return CheckType_DeclEnum((DeclEnum *)v, ctx);
	case DeclList_Id:
		return CheckType_DeclList((DeclList *)v, ctx);
	case DeclClass_Id:
		return CheckType_DeclClass((DeclClass *)v, ctx);
	default:
		assert(false);
		break;
	}
}

inline Type * CheckType(Decl *v, Context *ctx) {
	return DeclVisitor_CheckType::CheckType_Decl(v, ctx);
}
#endif // __CheckType_DeclVisitor__

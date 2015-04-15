#ifndef __CheckType_TypeVisitor___
#define __CheckType_TypeVisitor___
#include "c.hpp"

struct TypeVisitor_CheckType {
	static Type * CheckType_Type(Type *v, Context *ctx);
private:
	static Type * CheckType_TypeArray(TypeArray *v, Context *ctx);
	static Type * CheckType_TypeId(TypeId *v, Context *ctx);
	static Type * CheckType_TypeDot(TypeDot *v, Context *ctx);
};

/*
Type * TypeVisitor_CheckType::CheckType_TypeArray(TypeArray *v, Context *ctx) {}
Type * TypeVisitor_CheckType::CheckType_TypeId(TypeId *v, Context *ctx) {}
Type * TypeVisitor_CheckType::CheckType_TypeDot(TypeDot *v, Context *ctx) {}
*/
inline Type * TypeVisitor_CheckType::CheckType_Type(Type *v, Context *ctx) {
	switch (v->getType()) {
	case TypeArray_Id:
		return CheckType_TypeArray((TypeArray *)v, ctx);
	case TypeId_Id:
		return CheckType_TypeId((TypeId *)v, ctx);
	case TypeDot_Id:
		return CheckType_TypeDot((TypeDot *)v, ctx);
	default:
		assert(false);
		break;
	}
}

inline Type * CheckType(Type *v, Context *ctx) {
	return TypeVisitor_CheckType::CheckType_Type(v, ctx);
}
#endif // __CheckType_TypeVisitor__

#ifndef __Dump_TypeVisitor___
#define __Dump_TypeVisitor___
#include "c.hpp"

struct TypeVisitor_Dump {
	static std::string Dump_Type(Type *v, int arg);
private:
	static std::string Dump_TypeArray(TypeArray *v, int arg);
	static std::string Dump_TypeId(TypeId *v, int arg);
	static std::string Dump_TypeDot(TypeDot *v, int arg);
};

/*
std::string TypeVisitor_Dump::Dump_TypeArray(TypeArray *v, int arg) {}
std::string TypeVisitor_Dump::Dump_TypeId(TypeId *v, int arg) {}
std::string TypeVisitor_Dump::Dump_TypeDot(TypeDot *v, int arg) {}
*/
inline std::string TypeVisitor_Dump::Dump_Type(Type *v, int arg) {
	switch (v->getType()) {
	case TypeArray_Id:
		return Dump_TypeArray((TypeArray *)v, arg);
	case TypeId_Id:
		return Dump_TypeId((TypeId *)v, arg);
	case TypeDot_Id:
		return Dump_TypeDot((TypeDot *)v, arg);
	default:
		assert(false);
		break;
	}
}

inline std::string Dump(Type *v, int arg) {
	return TypeVisitor_Dump::Dump_Type(v, arg);
}
#endif // __Dump_TypeVisitor__

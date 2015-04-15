#ifndef __Dump_DeclVisitor___
#define __Dump_DeclVisitor___
#include "c.hpp"

struct DeclVisitor_Dump {
	static std::string Dump_Decl(Decl *v, int arg);
private:
	static std::string Dump_DeclFun(DeclFun *v, int arg);
	static std::string Dump_DeclVar(DeclVar *v, int arg);
	static std::string Dump_DeclEnum(DeclEnum *v, int arg);
	static std::string Dump_DeclList(DeclList *v, int arg);
	static std::string Dump_DeclClass(DeclClass *v, int arg);
};

/*
std::string DeclVisitor_Dump::Dump_DeclFun(DeclFun *v, int arg) {}
std::string DeclVisitor_Dump::Dump_DeclVar(DeclVar *v, int arg) {}
std::string DeclVisitor_Dump::Dump_DeclEnum(DeclEnum *v, int arg) {}
std::string DeclVisitor_Dump::Dump_DeclList(DeclList *v, int arg) {}
std::string DeclVisitor_Dump::Dump_DeclClass(DeclClass *v, int arg) {}
*/
inline std::string DeclVisitor_Dump::Dump_Decl(Decl *v, int arg) {
	switch (v->getType()) {
	case DeclFun_Id:
		return Dump_DeclFun((DeclFun *)v, arg);
	case DeclVar_Id:
		return Dump_DeclVar((DeclVar *)v, arg);
	case DeclEnum_Id:
		return Dump_DeclEnum((DeclEnum *)v, arg);
	case DeclList_Id:
		return Dump_DeclList((DeclList *)v, arg);
	case DeclClass_Id:
		return Dump_DeclClass((DeclClass *)v, arg);
	default:
		assert(false);
		break;
	}
}

inline std::string Dump(Decl *v, int arg) {
	return DeclVisitor_Dump::Dump_Decl(v, arg);
}
#endif // __Dump_DeclVisitor__

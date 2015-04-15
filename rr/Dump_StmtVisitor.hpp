#ifndef __Dump_StmtVisitor___
#define __Dump_StmtVisitor___
#include "c.hpp"

struct StmtVisitor_Dump {
	static std::string Dump_Stmt(Stmt *v, int arg);
private:
	static std::string Dump_StmtIf(StmtIf *v, int arg);
	static std::string Dump_StmtWhile(StmtWhile *v, int arg);
	static std::string Dump_StmtFor(StmtFor *v, int arg);
	static std::string Dump_StmtForEach(StmtForEach *v, int arg);
	static std::string Dump_StmtExpr(StmtExpr *v, int arg);
	static std::string Dump_StmtList(StmtList *v, int arg);
	static std::string Dump_StmtDecl(StmtDecl *v, int arg);
};

/*
std::string StmtVisitor_Dump::Dump_StmtIf(StmtIf *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtWhile(StmtWhile *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtFor(StmtFor *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtForEach(StmtForEach *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtExpr(StmtExpr *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtList(StmtList *v, int arg) {}
std::string StmtVisitor_Dump::Dump_StmtDecl(StmtDecl *v, int arg) {}
*/
inline std::string StmtVisitor_Dump::Dump_Stmt(Stmt *v, int arg) {
	switch (v->getType()) {
	case StmtIf_Id:
		return Dump_StmtIf((StmtIf *)v, arg);
	case StmtWhile_Id:
		return Dump_StmtWhile((StmtWhile *)v, arg);
	case StmtFor_Id:
		return Dump_StmtFor((StmtFor *)v, arg);
	case StmtForEach_Id:
		return Dump_StmtForEach((StmtForEach *)v, arg);
	case StmtExpr_Id:
		return Dump_StmtExpr((StmtExpr *)v, arg);
	case StmtList_Id:
		return Dump_StmtList((StmtList *)v, arg);
	case StmtDecl_Id:
		return Dump_StmtDecl((StmtDecl *)v, arg);
	default:
		assert(false);
		break;
	}
}

inline std::string Dump(Stmt *v, int arg) {
	return StmtVisitor_Dump::Dump_Stmt(v, arg);
}
#endif // __Dump_StmtVisitor__

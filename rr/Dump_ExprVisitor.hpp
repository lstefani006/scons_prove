#ifndef __Dump_ExprVisitor___
#define __Dump_ExprVisitor___
#include "c.hpp"

struct ExprVisitor_Dump {
	static std::string Dump_Expr(Expr *v, int arg);
private:
	static std::string Dump_ExprNum(ExprNum *v, int arg);
	static std::string Dump_ExprId(ExprId *v, int arg);
	static std::string Dump_ExprStr(ExprStr *v, int arg);
	static std::string Dump_ExprBin(ExprBin *v, int arg);
	static std::string Dump_ExprUni(ExprUni *v, int arg);
	static std::string Dump_ExprCast(ExprCast *v, int arg);
	static std::string Dump_ExprDot(ExprDot *v, int arg);
	static std::string Dump_ExprPost(ExprPost *v, int arg);
	static std::string Dump_ExprPre(ExprPre *v, int arg);
	static std::string Dump_ExprNew(ExprNew *v, int arg);
	static std::string Dump_ExprNewArray(ExprNewArray *v, int arg);
	static std::string Dump_ExprArray(ExprArray *v, int arg);
	static std::string Dump_ExprCall(ExprCall *v, int arg);
	static std::string Dump_ExprList(ExprList *v, int arg);
};

/*
std::string ExprVisitor_Dump::Dump_ExprNum(ExprNum *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprId(ExprId *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprStr(ExprStr *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprBin(ExprBin *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprUni(ExprUni *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprCast(ExprCast *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprDot(ExprDot *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprPost(ExprPost *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprPre(ExprPre *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprNew(ExprNew *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprNewArray(ExprNewArray *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprArray(ExprArray *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprCall(ExprCall *v, int arg) {}
std::string ExprVisitor_Dump::Dump_ExprList(ExprList *v, int arg) {}
*/
inline std::string ExprVisitor_Dump::Dump_Expr(Expr *v, int arg) {
	switch (v->getType()) {
	case ExprNum_Id:
		return Dump_ExprNum((ExprNum *)v, arg);
	case ExprId_Id:
		return Dump_ExprId((ExprId *)v, arg);
	case ExprStr_Id:
		return Dump_ExprStr((ExprStr *)v, arg);
	case ExprBin_Id:
		return Dump_ExprBin((ExprBin *)v, arg);
	case ExprUni_Id:
		return Dump_ExprUni((ExprUni *)v, arg);
	case ExprCast_Id:
		return Dump_ExprCast((ExprCast *)v, arg);
	case ExprDot_Id:
		return Dump_ExprDot((ExprDot *)v, arg);
	case ExprPost_Id:
		return Dump_ExprPost((ExprPost *)v, arg);
	case ExprPre_Id:
		return Dump_ExprPre((ExprPre *)v, arg);
	case ExprNew_Id:
		return Dump_ExprNew((ExprNew *)v, arg);
	case ExprNewArray_Id:
		return Dump_ExprNewArray((ExprNewArray *)v, arg);
	case ExprArray_Id:
		return Dump_ExprArray((ExprArray *)v, arg);
	case ExprCall_Id:
		return Dump_ExprCall((ExprCall *)v, arg);
	case ExprList_Id:
		return Dump_ExprList((ExprList *)v, arg);
	default:
		assert(false);
		break;
	}
}

inline std::string Dump(Expr *v, int arg) {
	return ExprVisitor_Dump::Dump_Expr(v, arg);
}
#endif // __Dump_ExprVisitor__

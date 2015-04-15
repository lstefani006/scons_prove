#ifndef __BaseVisitor_hpp__
#define __BaseVisitor_hpp__

struct ExprVisitor {
	virtual ~ExprVisitor() {}
	virtual void visit(ExprNum &) = 0;
	virtual void visit(ExprId &) = 0;
	virtual void visit(ExprStr &) = 0;
	virtual void visit(ExprBin &) = 0;
	virtual void visit(ExprUni &) = 0;
	virtual void visit(ExprCast &) = 0;
	virtual void visit(ExprDot &) = 0;
	virtual void visit(ExprPost &) = 0;
	virtual void visit(ExprPre &) = 0;
	virtual void visit(ExprNew &) = 0;
	virtual void visit(ExprNewArray &) = 0;
	virtual void visit(ExprArray &) = 0;
	virtual void visit(ExprCall &) = 0;
	virtual void visit(ExprList &) = 0;
};

inline void ExprNum::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprId::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprStr::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprBin::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprUni::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprCast::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprDot::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprPost::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprPre::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprNew::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprNewArray::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprArray::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprCall::accept(ExprVisitor &v) { v.visit(*this); }
inline void ExprList::accept(ExprVisitor &v) { v.visit(*this); }

struct StmtVisitor {
	virtual ~StmtVisitor() {}
	virtual void visit(StmtIf &) = 0;
	virtual void visit(StmtWhile &) = 0;
	virtual void visit(StmtFor &) = 0;
	virtual void visit(StmtForEach &) = 0;
	virtual void visit(StmtExpr &) = 0;
	virtual void visit(StmtList &) = 0;
	virtual void visit(StmtDecl &) = 0;
};

inline void StmtIf::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtWhile::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtFor::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtForEach::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtExpr::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtList::accept(StmtVisitor &v) { v.visit(*this); }
inline void StmtDecl::accept(StmtVisitor &v) { v.visit(*this); }

struct TypeVisitor {
	virtual ~TypeVisitor() {}
	virtual void visit(TypeArray &) = 0;
	virtual void visit(TypeId &) = 0;
	virtual void visit(TypeDot &) = 0;
};

inline void TypeArray::accept(TypeVisitor &v) { v.visit(*this); }
inline void TypeId::accept(TypeVisitor &v) { v.visit(*this); }
inline void TypeDot::accept(TypeVisitor &v) { v.visit(*this); }

struct DeclVisitor {
	virtual ~DeclVisitor() {}
	virtual void visit(DeclFun &) = 0;
	virtual void visit(DeclVar &) = 0;
	virtual void visit(DeclEnum &) = 0;
	virtual void visit(DeclList &) = 0;
	virtual void visit(DeclClass &) = 0;
};

inline void DeclFun::accept(DeclVisitor &v) { v.visit(*this); }
inline void DeclVar::accept(DeclVisitor &v) { v.visit(*this); }
inline void DeclEnum::accept(DeclVisitor &v) { v.visit(*this); }
inline void DeclList::accept(DeclVisitor &v) { v.visit(*this); }
inline void DeclClass::accept(DeclVisitor &v) { v.visit(*this); }

#endif

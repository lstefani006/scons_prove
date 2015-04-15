#ifndef __p_hpp__
#define __p_hpp__

#include <vector>
#include <string>
#include <stack>
#include <stdlib.h>
#include <string.h>

#include "EnumClassId.hpp"

struct Type;
struct Expr;
struct ExprList;
struct Stmt;
struct Decl;
struct DeclVar;
struct DeclFun;
struct DeclClass;
struct VarList;


struct Context {
	void push(DeclFun *v);// { st.push(v); }
	void push(DeclClass *v);// { st.push(v); }
	void pop() { st.pop(); }
	
	void add(DeclVar *v);
	
	DeclVar * findVar(const std::string &id);
	
	std::stack<Decl*> st;
};

#define visitor_base(b) \
	virtual b##_IdType getType() const = 0;\
	virtual void accept(struct b##Visitor &v)

#define visitor(b, c) \
	b##_IdType getType() const { return c##_Id; } \
	virtual void accept(struct b##Visitor &v)

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


struct Type {
	virtual ~Type() {}
	virtual Type * clone() const = 0;
	visitor_base(Type) = 0;
};
struct TypeArray : public Type {
	TypeArray(Type *b) : b(b) {}
	Type *b;
	~TypeArray() { delete b; }
	virtual TypeArray * clone() const { return new TypeArray(b->clone()); }

	visitor(Type, TypeArray);
};
struct TypeId : public Type {
	TypeId(char *id) : id(id) { free(id); }
	std::string id;
	~TypeId() {}
	virtual TypeId * clone() const { return new TypeId(strdup(id.c_str())); }

	visitor(Type, TypeId);
};
struct TypeDot : public Type {
	TypeDot(Type *left, char *id) : left(left), id(id) { free(id); }
	Type *left;
	std::string id;
	~TypeDot() { delete left; }

	virtual TypeDot * clone() const { return new TypeDot(left->clone(), strdup(id.c_str())); }
	visitor(Type, TypeDot);
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

struct Expr {
	virtual ~Expr() {}
	visitor_base(Expr) = 0;
};
struct ExprNum : public Expr {
	ExprNum(int n) : n(n) {}
	virtual ~ExprNum() {}
	int n;
	visitor(Expr, ExprNum);
};
struct ExprId : public Expr {
	ExprId(char *id) : id(id) { free(id); }
	virtual ~ExprId() {}
	std::string id;
	visitor(Expr, ExprId);
};
struct ExprStr : public Expr {
	ExprStr(char *str) : str(str) { free(str); }
	virtual ~ExprStr() {}
	std::string str;
	visitor(Expr, ExprStr);
};
struct ExprBin : public Expr {
	ExprBin(Expr *a, const char *op, Expr *b): a(a), op(op), b(b) {}
	virtual ~ExprBin() { delete a; delete b; }
	Expr *a;
	std::string op;
	Expr *b;
	visitor(Expr, ExprBin);
};
struct ExprUni : public Expr {
	ExprUni(const char *op, Expr *a): op(op), a(a) {}
	virtual ~ExprUni() { delete a; }
	std::string op;
	Expr *a;
	visitor(Expr, ExprUni);
};
struct ExprCast : public Expr {
	ExprCast(Type *t, Expr *a): t(t), a(a) {}
	virtual ~ExprCast() { delete t; delete a; }
	Type *t;
	Expr *a;
	visitor(Expr, ExprCast);
};
struct ExprDot : public Expr {
	ExprDot(Expr *a, Expr *b): a(a), b(b) {}
	virtual ~ExprDot() { delete a; delete b; }
	Expr *a;
	Expr *b;
	visitor(Expr, ExprDot);
};
struct ExprPost : public Expr {
	ExprPost(Expr *a, const char *op): a(a), op(op) {}
	virtual ~ExprPost() { delete a; }
	Expr *a;
	std::string op;
	visitor(Expr, ExprPost);
};
struct ExprPre : public Expr {
	ExprPre(const char *op, Expr *a): op(op), a(a) {}
	virtual ~ExprPre() { delete a; }
	std::string op;
	Expr *a;
	visitor(Expr, ExprPre);
};

struct ExprNew : public Expr {
	ExprNew(Type *t, ExprList *el): t(t), el(el) {}
	virtual ~ExprNew();
	Type *t;
	ExprList *el;
	visitor(Expr, ExprNew);
};
struct ExprNewArray : public Expr {
	ExprNewArray(Type *t, ExprList *el): t(t), el(el) {}
	virtual ~ExprNewArray();
	Type *t;
	ExprList *el;
	visitor(Expr, ExprNewArray);
};
struct ExprArray : public Expr {
	ExprArray(Expr *e, ExprList *el): e(e), el(el) {}
	~ExprArray();
	Expr *e;
	ExprList *el;
	visitor(Expr, ExprArray);
};
struct ExprCall : public Expr {
	ExprCall(Expr *e, ExprList *el): e(e), el(el) {}
	~ExprCall();
	Expr *e;
	ExprList *el;
	visitor(Expr, ExprCall);
};
struct ExprList : public Expr {
	~ExprList() { for (auto e : el) delete e; }
	ExprList * add(Expr *e) { el.push_back(e); return this; }
	std::vector<Expr *> el;
	visitor(Expr, ExprList);
};
//////////////////////////////////////////////////
//////////////////////////////////////////////////

struct Stmt {
	virtual ~Stmt() {}
	visitor_base(Stmt) = 0;
};
struct StmtIf : public Stmt {
	StmtIf(Stmt *e, Stmt *sa, Stmt *sb): e(e), sa(sa), sb(sb) {}
	~StmtIf() { delete e; delete sa; delete sb; }
	Stmt *e;
	Stmt *sa, *sb;
	visitor(Stmt, StmtIf);
};
struct StmtWhile : public Stmt {
	StmtWhile(Stmt *e, Stmt *s): e(e), s(s) {}
	~StmtWhile() { delete e; delete s; }
	Stmt *e;
	Stmt *s;
	visitor(Stmt, StmtWhile);
};
struct StmtFor : public Stmt {
	StmtFor(Stmt *ea, Expr *eb, Expr *ec, Stmt *s): ea(ea), eb(eb), ec(ec), s(s) {}
	~StmtFor() { delete ea; delete eb; delete ec; delete s; }
	Stmt *ea;
	Expr *eb, *ec;
	Stmt *s;
	visitor(Stmt, StmtFor);
};
struct StmtForEach : public Stmt {
	StmtForEach(Stmt *ea, Expr *eb, Stmt *s): ea(ea), eb(eb), s(s) {}
	~StmtForEach() { delete ea; delete eb; delete s; }
	Stmt *ea;
	Expr *eb;
	Stmt *s;
	visitor(Stmt, StmtForEach);
};
struct StmtExpr : public Stmt {
	StmtExpr(Expr *e): e(e) {}
	~StmtExpr() { delete e; }
	Expr *e;
	visitor(Stmt, StmtExpr);
};
struct StmtList : public Stmt {
	~StmtList() { for (auto s : sl) delete s; }
	StmtList * add(Stmt *s) { sl.push_back(s); return this; }
	std::vector<Stmt *> sl;
	visitor(Stmt, StmtList);
};
struct StmtDecl : public Stmt {
	StmtDecl(Decl *d) : d(d) {}
	~StmtDecl();
	Decl *d;
	visitor(Stmt, StmtDecl);
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////

struct Decl {
	virtual ~Decl() {}
	visitor_base(Decl) = 0;
};

struct DeclFun : public Decl {
	DeclFun(char *id, VarList *vv, Type *t, Stmt *m) : id(id), vv(vv), t(t), m(m) { free(id); }
	~DeclFun();

	std::string id;
	VarList *vv;
	Type *t;
	Stmt *m;
	visitor(Decl, DeclFun);
};

struct DeclVar : public Decl {
	DeclVar(char *id, Type *t, Expr *e) : id(id), t(t), e(e) { free(id); }
	~DeclVar() { delete t; delete e; }

	std::string id;
	Type *t;
	Expr *e;
	visitor(Decl, DeclVar);
};

struct DeclEnum : public Decl {
	DeclEnum() {}
	DeclEnum * add(char *ei) { this->ei.push_back(ei); free(ei); return this; }
	DeclEnum * setName(char *id) { this->id = id; free(id); return this; }

	std::string id;
	std::vector<std::string> ei;
	visitor(Decl, DeclEnum);
};

struct DeclList : public Decl {
	std::vector<Decl *> v;
	~DeclList() { for (auto t : v) delete t; }
	DeclList * add(Decl *s) { v.push_back(s); return this; }
	visitor(Decl, DeclList);
};


struct DeclClass : public Decl {
	DeclClass(char *id, DeclList *dd) : id(id), dd(dd) { free(id); }
	~DeclClass() { delete dd; }

	std::string id;
	DeclList *dd;
	visitor(Decl, DeclClass);
};

struct VarList : std::vector<DeclVar *> {
	~VarList() { for (auto t : *this) delete t; }
	VarList * add(DeclVar *s) { this->push_back(s); return this; }
};

struct Pgm : std::vector<Decl *> {
	Pgm * add(Decl *d) { this->push_back(d); return this; }
	~Pgm() { for (auto p : *this) delete p; }
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

inline StmtDecl::~StmtDecl() { delete d; }

inline DeclFun::~DeclFun() {
	delete vv;
	delete t;
	delete m;
}

inline ExprNew::~ExprNew() {
	delete t;
	delete el;
}
inline ExprNewArray::~ExprNewArray() {
	delete t;
	delete el;
}
inline ExprCall::~ExprCall() {
	delete e;
	delete el;
}
inline ExprArray::~ExprArray() { delete e; delete el; }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#include "BaseVisitor.hpp"

#endif

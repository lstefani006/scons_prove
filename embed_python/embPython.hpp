#include <stdlib.h>
#include <stdio.h>

#ifndef __EMBPYTHON_HPP__
#define __EMBPYTHON_HPP__

extern "C" {
	typedef struct _object PyObject;
}

#define PY_Undef(ty) \
	private: \
		ty(const ty &); \
		void operator = (const ty&)

namespace EmbPython {

	class EMBPYTHON_API PY_Exception {
	public:
		PY_Exception(const char *str);
		~PY_Exception();
		PY_Exception(const PY_Exception &ex);
		void operator = (const PY_Exception &ex);
		const char *Msg() const;
	private:
		char *msg;
	};

	struct PY_Object;
	struct PY_Int;
	struct PY_String;
	struct PY_Bool;
	struct PY_Tuple;
	struct PY_Callable;
	struct PY_List;
	struct PY_Set;
	struct PY_Dict;

#define PY_Wrap(t, a, b) t &a = *((t *)(&b))

	struct EMBPYTHON_API PY_Object {
		PY_Object();
		PY_Object(PyObject *v);
		PY_Object(const PY_Object &v);
		void operator = (const PY_Object &v);
		~PY_Object(); // NON deve essere virtuale
		void Print(FILE *s = stdout) const;
		PY_String Str() const;
		PY_String Repr() const;

		void DecRef() const;
		PyObject *IncRef() const;
		PyObject *Ref() const;
		int GetRefCnt() const;

		bool IsNull() const { return _v == NULL; }

		bool IsBool()      const;
		bool IsInt()       const;
		bool IsString()    const;
		bool IsTuple()     const;
		bool IsList()      const;
		bool IsSet()       const;
		bool IsDict()      const;
		bool IsCallable()  const;

		PY_Object GetAttr(const char *attr) const;

		PY_Bool      ToBool() const;
		PY_Int       ToInt() const;
		PY_String    ToString() const;
		PY_Tuple     ToTuple() const;
		PY_List      ToList() const;
		PY_Set       ToSet() const;
		PY_Dict      ToDict() const;
		PY_Callable  ToCallable() const;

		/*
		c 	char A C char becomes a Python string of length 1
		d 	double A C double becomes a Python float
		D 	Py_Complex A C Py_Complex becomes a Python complex
		i 	int A C int becomes a Python int
		l 	long A C long becomes a Python int
		N 	PyObject* Passes a Python object and steals a reference
		O 	PyObject* Passes a Python object and INCREFs it as per normal rules
		O& 	convert + void* Arbitrary conversion (see below)
		s 	char* C null-terminated char* to Python string, or NULL to None
		s# 	char* + int
		C 	char* and length to Python string, or NULL to None
		u 	Py_UNICODE* C wide (UCS-2) null-terminated string to Python Unicode, or NULL to None
		u# 	Py_UNICODE* + int 
		C 	wide (UCS-2) string and length to Python Unicode, or NULL to None
		(...) as per ...  Build Python tuple from C values
		[...] as per ...  Build Python list from C values
		{...} as per ...  Build Python dictionary from C values, alternating keys and values (must be an even number of C values)
		*/
		PY_Object BuildValue(const char *fmt, ...);

	protected:
		mutable PyObject *_v;
	};
#define PY_F(ty) \
private: \
	 void operator = (const ty &v); \
public: \
	ty(PyObject *v) : PY_Object(v) {} \
	ty(const ty &v) { _v = v.IncRef(); }

	struct EMBPYTHON_API PY_String : public PY_Object
	{
		PY_F(PY_String)
		PY_String(const char *str);
	};
	struct EMBPYTHON_API PY_Int : public PY_Object
	{
		PY_F(PY_Int)
		PY_Int(long n);
	};
	struct EMBPYTHON_API PY_Bool : public PY_Object
	{
		PY_F(PY_Bool)
		PY_Bool(bool n);
	};

	struct EMBPYTHON_API PY_Tuple : public PY_Object
	{
		PY_F(PY_Tuple);
		PY_Tuple(int n);

		void SetItem(int i, const PY_Object &v);
		void Parse(const char *fmt, ...);
		bool ParseArgsAndKeywords(PY_Object &kwds, const char *fmt, const char **names, ...);
		bool ParseArgs(const char *fmt, const char **names, ...);
	};
	struct EMBPYTHON_API PY_List : public PY_Object
	{
		PY_F(PY_List);
		PY_List(int n);
		void SetItem(int i, const PY_Object &v);
		void Append(const PY_Object &v);
		int Size();
	};
	struct EMBPYTHON_API PY_Set : public PY_Object
	{
		PY_F(PY_Set);
		PY_Set();

		void Clear();
		bool Contains(const PY_Object &v);

		void Add(const PY_Object &v);
		int Size();
	};
	struct EMBPYTHON_API PY_Dict : public PY_Object
	{
		PY_F(PY_Dict);
		PY_Dict();
		void SetItem(const PY_Object &k, const PY_Object &v);
		PY_Object GetItem(const PY_Object &key);
	};

	struct EMBPYTHON_API PY_Callable : public PY_Object 
	{
		PY_F(PY_Callable);
		PY_Callable(const char *callableName);
		PY_Object Call(PY_Tuple &t) const;
	};

	struct EMBPYTHON_API PY_Import : public PY_Object
	{
		PY_F(PY_Import);
		PY_Import(const PY_String &s);
	};

	struct EMBPYTHON_API  PY_InitModule {
		PY_InitModule();
		void Add(const char *funcName, PyObject * (*f0)(PyObject *self), const char *doc = NULL);
		void Add(const char *funcName, PyObject * (*f1)(PyObject *self, PyObject *args), const char *doc = NULL);
		void Add(const char *funcName, PyObject * (*f2)(PyObject *self, PyObject *args, PyObject *kwds), const char *doc = NULL);
		~PY_InitModule();

		void InitModule(const char *module);
	private:
		int   top;
		int   sz;
		void  *md;

		PY_Undef(PY_InitModule);
	};

	//////////////////////////////////////////////////////////////

	struct EMBPYTHON_API PY_Init {
		PY_Init(int ac, char **av, bool multithread = false);
		~PY_Init();
		static int Run_SimpleString(const char *scriptBody);
		static int Run_AnyFile(FILE *f, const char *fName);
		PY_Undef(PY_Init);
	};
	// da chiamare all'inizio di ogni nuovo thread
	struct EMBPYTHON_API PY_Thread {
		PY_Thread();
		~PY_Thread();
		PY_Undef(PY_Thread);
	};
	// da chiamare prima di eseguire del codice Python
	struct EMBPYTHON_API PY_Execute {
		PY_Execute();
		~PY_Execute();
		PY_Undef(PY_Execute);
	};

	////////////////////////////
#undef PY_Undef
#undef PY_F
}

#endif

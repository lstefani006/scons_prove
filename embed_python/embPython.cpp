#include <Python.h>
#include "embPython.hpp"

namespace EmbPython {

	PY_Exception::PY_Exception(const char *str) { msg = strdup(str); }
	PY_Exception::~PY_Exception() { free(msg); }
	PY_Exception::PY_Exception(const PY_Exception &ex) { msg = strdup(ex.msg); }
	void PY_Exception::operator = (const PY_Exception &ex) { free(msg); msg = strdup(ex.msg); }
	const char *PY_Exception::Msg() const { return msg; }

	///////////////////////////////////////////////////////////
	PY_Object::PY_Object() { _v = NULL; }
	PY_Object::PY_Object(PyObject *v) { if (v) Py_INCREF(v); _v = v; }
	PY_Object::PY_Object(const PY_Object &v) { _v = v.IncRef(); }
	void PY_Object::operator = (const PY_Object &v) { DecRef(); _v = v.IncRef(); }

	PY_Object::~PY_Object() { DecRef(); }
	void PY_Object::Print(FILE *s) const { PyObject_Print(_v, s, 0); }
	void PY_Object::DecRef() const { if (_v) Py_DECREF(_v); _v = NULL; }
	PyObject *PY_Object::IncRef() const { if (_v) Py_INCREF(_v); return _v; }
	PyObject *PY_Object::Ref() const { return _v; }
	int PY_Object::GetRefCnt() const { return Py_REFCNT(_v); }

	bool PY_Object::IsBool()      const { return PyBool_Check(_v); }
	bool PY_Object::IsInt()       const { return PyInt_Check(_v); }
	bool PY_Object::IsString()    const { return PyString_Check(_v); }
	bool PY_Object::IsTuple()     const { return PyTuple_Check(_v); }
	bool PY_Object::IsList()      const { return PyList_Check(_v); }
	bool PY_Object::IsSet()       const { return PySet_Check(_v); }
	bool PY_Object::IsCallable()  const { return PyCallable_Check(_v); }

	PY_Object PY_Object::GetAttr(const char *attr) const {
		PyObject *v = PyObject_GetAttrString(_v, attr);
		if (v == NULL) {
			PyErr_Print();
			throw PY_Exception("attr not found");
		}
		return PY_Object(v);
	}
	PY_String PY_Object::Str() const {
		PyObject *s = PyObject_Str(_v);
		return PY_String(s);
	}
	PY_String PY_Object::Repr() const {
		PyObject *s = PyObject_Repr(_v);
		return PY_String(s);
	}

	PY_Int       PY_Object::ToInt()      const { return PY_Int(_v); }
	PY_String    PY_Object::ToString()   const { return PY_String(_v); }
	PY_Bool      PY_Object::ToBool()     const { return PY_Bool(_v); }
	PY_Tuple     PY_Object::ToTuple()    const { return PY_Tuple(_v); }
	PY_List      PY_Object::ToList ()    const { return PY_List(_v); }
	PY_Set       PY_Object::ToSet ()     const { return PY_Set(_v); }
	PY_Callable  PY_Object::ToCallable() const { return PY_Callable(_v); }

	PY_Object PY_Object::BuildValue(const char *fmt, ...)
	{
		va_list va;
		va_start(va, fmt);
		PyObject *r = Py_VaBuildValue(fmt, va);
		return PY_Object(r);
	}

	///////////////////////////////////////////////////////////
	PY_String::PY_String(const char *str) { _v = PyString_FromString(str); }
	PY_Int::PY_Int(long n) { _v = PyInt_FromLong(n); }
	PY_Bool::PY_Bool(bool n) { _v = PyBool_FromLong(n ? 1 : 0); }

	///////////////////////////////////////////////////////////
	PY_Tuple::PY_Tuple(int n) { _v = PyTuple_New(n); }
	void PY_Tuple::SetItem(int i, const PY_Object &v) { PyTuple_SetItem(_v, i, v.IncRef()); }
	void PY_Tuple::Parse(const char *fmt, ...)
	{
		va_list va;
		va_start(va, fmt);
		int r = PyArg_VaParse(_v, fmt, va);
		va_end(va);
		if (r == 0)
			throw PY_Exception("Unable parse tuple");
	}
	bool PY_Tuple::ParseArgsAndKeywords(PY_Object &kwrds, const char *fmt, const char **names, ...)
	{
		va_list va;
		va_start(va, names);
		int r = PyArg_VaParseTupleAndKeywords(_v, kwrds.Ref(), fmt, (char **)names, va);
		va_end(va);
		return r != 0;
	}
	bool PY_Tuple::ParseArgs(const char *fmt, const char **names, ...)
	{
		va_list va;
		va_start(va, names);
		int r = PyArg_VaParseTupleAndKeywords(_v, NULL, fmt, (char **)names, va);
		va_end(va);
		return r != 0;
	}
	///////////////////////////////////////////////////////////
	PY_List::PY_List(int n) { _v = PyList_New(n); }

	void PY_List::SetItem(int i, const PY_Object &v) { PyList_SetItem(_v, i, v.IncRef()); }
	void PY_List::Append(const PY_Object &v) { PyList_Append(_v, v.IncRef()); }
	int PY_List::Size() { return PyList_Size(_v); }

	///////////////////////////////////////////////////////////
	PY_Set::PY_Set() { _v = PySet_New(NULL); } // TODO che cosa e` l'argomento di _New(??)
	void PY_Set::Clear() { PySet_Clear(_v); }
	bool PY_Set::Contains(const PY_Object &v) { return PySet_Contains(_v, v.Ref()) > 0; }
	void PY_Set::Add(const PY_Object &v) { PySet_Add(_v, v.Ref()); }
	int PY_Set::Size() { return PySet_Size(_v); }

	PY_Callable::PY_Callable(const char *callableName) {
		_v = PyObject_GetAttrString(NULL, callableName);
		if (!PyCallable_Check(_v))
			throw PY_Exception("Not a callable object");
	}

	PY_Object PY_Callable::Call(PY_Tuple &t) const {
		PyObject *ret = PyObject_CallObject(_v, t.Ref());
		if (ret == NULL) {
			PyErr_Print();
			throw PY_Exception("error calling");
		}
		PY_Object r(ret);
		return r;
	}

	///////////////////////////////////////////////////////////
	PY_Dict::PY_Dict() { _v = PyDict_New(); }
	void PY_Dict::SetItem(const PY_Object &k, const PY_Object &v) { PyDict_SetItem(_v, k.Ref(), v.Ref()); }
	PY_Object PY_Dict::GetItem(const PY_Object &k)
	{ 
		PyObject *r = PyDict_GetItem(_v, k.Ref());
		return PY_Object(r);
	}
	///////////////////////////////////////////////////////////
	PY_Import::PY_Import(const PY_String &s) { 
		_v = PyImport_Import(s.Ref()); 
		if (_v == NULL)
		{
			PyErr_Print();
			throw PY_Exception("module not found");
		}
	}


	///////////////////////////////////////////////////////////

	PY_InitModule::PY_InitModule() {
		this->sz = 0;
		this->md = NULL;
	}
	void PY_InitModule::Add(const char *funcName, PyObject * (*f0)(PyObject *self), const char *doc)
	{
		sz += 1;
		PyMethodDef *a = (PyMethodDef *)realloc(md, sz * sizeof(PyMethodDef));
		a[sz-1].ml_name = (char *)funcName;
		a[sz-1].ml_meth = (PyCFunction)f0;
		a[sz-1].ml_flags = METH_NOARGS;
		a[sz-1].ml_doc = doc == NULL ? funcName : doc;
		md = a;
	}
	void PY_InitModule::Add(const char *funcName, PyObject * (*f1)(PyObject *self, PyObject *args), const char *doc)
	{
		sz += 1;
		PyMethodDef *a = (PyMethodDef *)realloc(md, sz * sizeof(PyMethodDef));
		a[sz-1].ml_name = (char *)funcName;
		a[sz-1].ml_meth = (PyCFunction)f1;
		a[sz-1].ml_flags = METH_VARARGS;
		a[sz-1].ml_doc = doc == NULL ? funcName : doc;
		md = a;
	}
	void PY_InitModule::Add(const char *funcName, PyObject * (*f2)(PyObject *self, PyObject *args, PyObject *kwds), const char *doc)
	{
		sz += 1;
		PyMethodDef *a = (PyMethodDef *)realloc(md, sz * sizeof(PyMethodDef));
		a[sz-1].ml_name = (char *)funcName;
		a[sz-1].ml_meth = (PyCFunction)f2;
		a[sz-1].ml_flags = METH_VARARGS | METH_KEYWORDS;
		a[sz-1].ml_doc = doc == NULL ? funcName : doc;
		md = a;
	}
	PY_InitModule::~PY_InitModule() {
		free(md);
	}

	void PY_InitModule::InitModule(const char *module) {
		sz += 1;
		PyMethodDef *a = (PyMethodDef *)realloc(md, sz * sizeof(PyMethodDef));
		a[sz-1].ml_name = NULL;
		a[sz-1].ml_meth = NULL;
		a[sz-1].ml_flags = 0;
		a[sz-1].ml_doc = NULL;
		md = a;
		Py_InitModule(module, a);
	}
 
	////////////////////////////////////////////////////
	static PyThreadState *G_ThreadState = NULL;  // Thread state del PROGRAMMA
	static bool G_multithread = false;
	PY_Init::PY_Init(int ac, char **av, bool multithread) {
		assert(sizeof(PY_Object) == sizeof(PyObject *));

		Py_SetProgramName(av[0]);
		Py_Initialize();
		PySys_SetArgv(ac, av);

		if (multithread) {
			G_multithread = multithread;
			PyEval_InitThreads();                // initialize thread support
			G_ThreadState = PyThreadState_Get(); // save a pointer to the main PyThreadState object
			PyEval_ReleaseLock();                // release the lock
		}
	}
	int PY_Init::Run_SimpleString(const char *scriptBody) { return PyRun_SimpleString(scriptBody); }
	int PY_Init::Run_AnyFile(FILE *f, const char *fName) { return PyRun_AnyFile(f, fName); }

	PY_Init::~PY_Init() {
		if (G_multithread)
			PyEval_AcquireLock();   // shut down the interpreter
		Py_Finalize();
		G_ThreadState = NULL;
	}
	////////////////////////////////////////////////////
#if defined(__linux__)
	static __thread PyThreadState *TH_ThreadState = NULL;
#else
	static declspec(thread) PyThreadState *TH_ThreadState = NULL;
#endif
	PY_Thread::PY_Thread() {
		assert(TH_ThreadState == NULL);
		assert(G_ThreadState != NULL);
		PyEval_AcquireLock();                           // get the global lock
		PyInterpreterState *is = G_ThreadState->interp; // get a reference to the PyInterpreterState
		TH_ThreadState = PyThreadState_New(is);         // create a thread state object for this thread
		PyEval_ReleaseLock();                           // free the lock
	}
	PY_Thread::~PY_Thread() {
		PyEval_AcquireLock();                 // grab the lock
		PyThreadState_Swap(NULL);             // swap my thread state out of the interpreter
		PyThreadState_Clear(TH_ThreadState);  // clear out any cruft from thread state object
		PyThreadState_Delete(TH_ThreadState); // delete my thread state object
		TH_ThreadState = NULL;
		PyEval_ReleaseLock();                 // release the lock
	}

	////////////////////////////////////////////////////
	PY_Execute::PY_Execute() {
		assert(TH_ThreadState != NULL);
		PyEval_AcquireLock();              // grab the global interpreter lock
		PyThreadState_Swap(TH_ThreadState); // swap in my thread state
	}
	PY_Execute::~PY_Execute() {
		PyThreadState_Swap(NULL); // clear the thread state
		PyEval_ReleaseLock();     // release our hold on the global interpreter
	}
}

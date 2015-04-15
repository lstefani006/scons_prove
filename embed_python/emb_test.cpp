#include <stdlib.h>
#include <stdio.h>
#include "embPython.hpp"

using namespace EmbPython;

static PyObject* func_na(PyObject *self)
{
	return PY_String("ciao da func_na").IncRef();
}
static PyObject* func_a(PyObject *self, PyObject *args_)
{
	PY_Wrap(PY_Tuple, args, args_);

	int x, y = 0, z = 0;
	const char *na[] = {"x", "y", "z", NULL};
	if (!args.ParseArgs("i|ii:func_a", na, &x, &y, &z))
		return NULL;
	return PY_String("ciao da func_a").IncRef();
}
static PyObject* func_ak(PyObject *self, PyObject *args_, PyObject *ka_)
{
	PY_Wrap(PY_Tuple, args, args_);
	PY_Wrap(PY_Object, ka, ka_);

	if (args.IsTuple()) { printf("111111\n"); }
	if (ka.IsSet()) { printf("111111\n"); }

	args.Print();printf("\n");
	ka.Print();printf("\n");

	int x, y = 0, z = 0;
	const char *na[] = {"x", "y", "z", NULL};
	if (!args.ParseArgsAndKeywords(ka, "i|ii:func_ak", na, &x, &y, &z))
		return NULL;
	return PY_String("ciao da func_ak").IncRef();
}

int main(int argc, char *argv[])
{
	try {
		const char *strModule = "ee";
		const char *strFunc =  "per";

		PY_Init py(argc, argv);

		PY_InitModule emb;
		emb.Add("func_na", func_na);
		emb.Add("func_a",  func_a);
		emb.Add("func_ak", func_ak);
		emb.InitModule("emb");

		PY_Import module(strModule);
		PY_Object f = module.GetAttr(strFunc);
		if (f.IsCallable()) {
			PY_Callable c = f.ToCallable();

			PY_List lst(0);
			lst.Append(PY_Int(1));
			lst.Append(PY_Int(2));
			lst.Append(PY_Int(3));

			PY_Dict dd;
			dd.SetItem(PY_String("AAAA"), PY_Int(3));
			dd.SetItem(PY_String("BBBB"), PY_Int(4));
			dd.SetItem(PY_String("CCCC"), PY_String("11"));
			dd.SetItem(PY_String("DDDD"), PY_Bool(true));

			PY_Tuple t(3);
			PY_Int a(5);
			PY_Int b(6);
			t.SetItem(0, a);
			t.SetItem(1, b);
			t.SetItem(2, dd);

			PY_Object r = c.Call(t);
			r.Print();
			printf("\n");
			printf("t=%d\n", t.GetRefCnt());
		}
	} catch (PY_Exception &ex) {
		printf("ex - %s\n", ex.Msg());
	}

	return 0;
}

/*

int TEST()
{
	HANDLE h = CreateFile("test.txt", GENERIC_READ | GENERIC_WRITE, 0, 0,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h != INVALID_HANDLE_VALUE)
	{
		int fd = _open_osfhandle((intptr_t)h, _O_APPEND | _O_RDONLY);
		if(fd != -1)
		{
			FILE* f = _fdopen(fd, "a+");
			if(f != 0)
			{
				char rbuffer[256];
				memset(rbuffer, 0, 256);
				fread(rbuffer, 1, 255, f);
				printf("read: %s\n", rbuffer);
				fseek(f, 0, SEEK_CUR); // Switch from read to write
				const char* wbuffer = " --- Hello World! --- \n";
				fwrite(wbuffer, 1, strlen(wbuffer), f);
				fclose(f); // Also calls _close()
			}
			else
			{
				_close(fd); // Also calls CloseHandle()
			}
		}
		else
		{
			CloseHandle(h);
		}
	}
}
*/

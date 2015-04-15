
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <mongoose.h>
#include <embPython.hpp>

static int PyCgi(struct mg_connection *conn);

static int event_handler(struct mg_connection *conn, enum mg_event ev) {
	switch (ev) {
		case MG_AUTH:
			//`MG_AUTH` - Mongoose asks whether this connection is authorized. If event
			// handler returns `MG_FALSE`, then Mongoose does not serve the request but
			// sends authorization request to the client. 
			// If `MG_TRUE` is returned, then Mongoose continues on with the request.
			return MG_TRUE;

		case MG_REQUEST:
			// `MG_REQUEST` - Mongoose asks event handler to serve the request. If
			// event handler serves the request by sending a reply,
			// it should return `MG_TRUE`. 
			// Otherwise, it should return `MG_FALSE` which tells Mongoose that request is not
			// served and Mongoose should serve it. 
			// If event handler decides to serve the request, but doesn't have
			// all the data at the moment, it should return `MG_MORE`. That tells
			// Mongoose to keep the connection open after callback returns.
			// `mg_connection::connection_param` pointer is a placeholder to keep
			// user-specific data. For example, handler could decide to open a DB
			// connection and store DB connection handle in `connection_param`.
			if (strcmp(conn->uri, "/leo") == 0) {

				  mg_send_header(conn, "Content-Type", "text/html; charset=utf-8");
				  mg_printf_data(conn, "<html>\n");
				  mg_printf_data(conn, "<body>\n");
				  mg_printf_data(conn, "ciao\n");
				  mg_printf_data(conn, "</body>\n");
				  mg_printf_data(conn, "</html>\n");

				  //mg_send_data();
				  //mg_printf_data();

				  return MG_TRUE;
			}
			else if (strncmp(conn->uri, "/tg/", 4) == 0)
			{
				return PyCgi(conn);
			}
			return MG_FALSE;

		case MG_POLL:
			// `MG_POLL` is sent to every connection on every iteration of
			// `mg_poll_server()`. Event handler should return `MG_FALSE` to ignore
			// this event. If event handler returns `MG_TRUE`, then Mongoose assumes
			// that event handler has finished sending data, and Mongoose will
			// close the connection.
			return MG_FALSE;

		case MG_HTTP_ERROR:
			// `MG_HTTP_ERROR` sent when Mongoose is about to send HTTP error back
			// to the client. Event handler can choose to send a reply itself, in which
			// case event handler must return `MG_TRUE`. Otherwise, event handler must
			// return `MG_FALSE`
			return MG_FALSE;

		case MG_CLOSE:
			// `MG_CLOSE` is sent when the connection is closed. This event is used
			// to cleanup per-connection state stored in `connection_param`
			// if it was allocated.
			return MG_FALSE;
	}
	return MG_FALSE;
}

using namespace EmbPython;

static PyObject* func_set_status_code(PyObject *self, PyObject *args_, PyObject *ka_);
static PyObject* func_send_header(PyObject *self, PyObject *args_, PyObject *ka_);
static PyObject* func_send_data(PyObject *self, PyObject *args_, PyObject *ka_);


int main(int ac, char **av) {

	try {

		struct mg_server *server = mg_create_server(NULL, event_handler);
		char b[512];
		const char *pwd = getcwd(b, 512);
		strcat(b, "/wwwroot");
		printf("document_root %s\n", b);

		enum GCI_Type { Php, Python, Turbogear };

		GCI_Type ty = Turbogear;

		mg_set_option(server, "document_root", b);
		mg_set_option(server, "listening_port", "8080");

		//mg_set_option(server, "cgi_pattern", "/py**$");
		mg_set_option(server, "cgi_pattern", "**.py$|**.php$");
		mg_set_option(server, "cgi_interpreter", "/usr/bin/python");

		/*
#ifdef WIN32
if (ty == Python)
mg_set_option(server, "cgi_interpreter", "D:\\prove\\php\\php-cgi.exe");
else if (ty == Turbogear)
mg_set_option(server, "cgi_interpreter", "python.exe");
else
mg_set_option(server, "cgi_interpreter", "python.exe");
#else
if (ty == Python)
mg_set_option(server, "cgi_interpreter", "/usr/bin/python");
else if (ty == Turbogear)
		//mg_set_option(server, "cgi_interpreter", "/home/leo/tmp/scons_prove/wwwroot/cgi2scgi");
		mg_set_option(server, "cgi_interpreter", "/usr/bin/python");
		else
		mg_set_option(server, "cgi_interpreter", "/usr/bin/php-cgi");
#endif
if (ty == Python)
mg_set_option(server, "cgi_pattern", "**.py$");
else if (ty == Turbogear)
mg_set_option(server, "cgi_pattern", "**.py$");
else
mg_set_option(server, "cgi_pattern", "**.php$");
*/

		for (;;) {
			mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
		}
		mg_destroy_server(&server);
	}
	catch (PY_Exception &ex) {
		fprintf(stderr, "ex - %s\n", ex.Msg());
	}

	return 0;
}

static PyObject* func_set_status_code(PyObject *self, PyObject *args_, PyObject *ka_) {
	PY_Wrap(PY_Tuple, args, args_);
	PY_Wrap(PY_Object, ka, ka_);

	char *ri  = NULL;
	int n;
	const char *na[] = {"ri", "status_code", NULL};
	if (!args.ParseArgsAndKeywords(ka, "si:mg_set_status_code", na, &ri, &n))
		return NULL;

	struct mg_connection *conn = NULL;
	sscanf(ri, "%p", &conn);
	conn->status_code = n;
	printf("mg_set_status_code(%d)\n", n);

	return PY_String("ciao da mg_set_status_code").IncRef();
}

static PyObject* func_send_header(PyObject *self, PyObject *args_, PyObject *ka_){
	PY_Wrap(PY_Tuple, args, args_);
	PY_Wrap(PY_Object, ka, ka_);

	char *ri  = NULL;
	char *name  = NULL;
	char *v  = NULL;
	const char *na[] = {"ri", "name", "v", NULL};
	if (!args.ParseArgsAndKeywords(ka, "sss:mg_send_header", na, &ri, &name, &v))
		return NULL;

	struct mg_connection *conn = NULL;
	sscanf(ri, "%p", &conn);
	mg_send_header(conn, name, v);
	printf("mg_send_header(%s, %s)\n", name, v);

	return PY_String("ciao da send_header").IncRef();
}
static PyObject* func_send_data(PyObject *self, PyObject *args_, PyObject *ka_){
	PY_Wrap(PY_Tuple, args, args_);
	PY_Wrap(PY_Object, ka, ka_);

	char *ri  = NULL;
	char *data  = NULL;
	const char *na[] = {"ri", "data", NULL};
	if (!args.ParseArgsAndKeywords(ka, "ss:mg_send_data", na, &ri, &data))
		return NULL;

	struct mg_connection *conn = NULL;
	sscanf(ri, "%p", &conn);
	mg_send_data(conn, data, strlen(data));

	printf("mg_send_data(%s)\n", data);

	return PY_String("ciao da send_data").IncRef();
}

static int PyCgi(struct mg_connection *ri)
{
	try {
		char *av[] = { (char *)"mg", NULL };
		PY_Init py(1, av, false);
		PY_Init::Run_SimpleString("import os, sys");
		PY_Init::Run_SimpleString("sys.path.append(os.path.abspath(os.getenv('BASE')) + '/wwwroot')");
		PY_InitModule emb;
		emb.Add("mg_set_status_code", func_set_status_code);
		emb.Add("mg_send_header", func_send_header);
		emb.Add("mg_send_data", func_send_data);
		emb.InitModule("emb");

		PY_Import module("mgpy");
		PY_Object G_c = module.GetAttr("request");
		PY_Dict env;

		const char *s, *slash;
		char *p;
		//// char **opts = conn->config_options;
		int  i;

		if ((s = getenv("SERVER_NAME")) != NULL)
			env.SetItem(PY_String("SERVER_NAME"), PY_String(s));
		else
			env.SetItem(PY_String("SERVER_NAME"), PY_String(ri->local_ip));

		//#env.SetItem(PY_String("SERVER_ROOT"), PY_String(opts[DOCUMENT_ROOT]));
		//#env.SetItem(PY_String("DOCUMENT_ROOT"), PY_String(opts[DOCUMENT_ROOT]));
		env.SetItem(PY_String("SERVER_SOFTWARE"), PY_String("Mongoose/99"));

		// Prepare the environment block
		env.SetItem(PY_String("GATEWAY_INTERFACE"), PY_String("CGI/1.1"));
		env.SetItem(PY_String("SERVER_PROTOCOL"), PY_String("HTTP/1.1"));
		env.SetItem(PY_String("REDIRECT_STATUS"), PY_String("200")); // For PHP


		env.SetItem(PY_String("REQUEST_METHOD"), PY_String(ri->request_method));
		env.SetItem(PY_String("REMOTE_ADDR"), PY_String(ri->remote_ip));

		// TODO(lsm): fix this for IPv6 case
		//addenv(blk, "SERVER_PORT=%d", ri->remote_port);
		char b[256];
		sprintf(b, "%d", ri->remote_port);
		env.SetItem(PY_String("REMOTE_PORT"), PY_String(b));
		env.SetItem(PY_String("SERVER_PORT"), PY_String(b));

		sprintf(b, "%s%s%s", ri->uri,
				ri->query_string == NULL ? "" : "?",
				ri->query_string == NULL ? "" : ri->query_string);
		env.SetItem(PY_String("REQUEST_URI"), PY_String(b));

		// SCRIPT_NAME
		//#if (conn->path_info != NULL) {
		//#	sprintf(b, "%.*s", (int) (strlen(ri->uri) - strlen(conn->path_info)), ri->uri);
		//#	env.SetItem(PY_String("SCRIPT_NAME"), PY_String(b));
		//#	env.SetItem(PY_String("PATH_INFO"), PY_String(conn->path_info));
		//#} else {
		//#	s = strrchr(prog, '/');
		//#	slash = strrchr(ri->uri, '/');
		//#	sprint(b, "%.*s%s", slash == NULL ? 0 : (int) (slash - ri->uri), ri->uri, s == NULL ? prog : s);
		//#	env.SetItem(PY_String("SCRIPT_NAME"), PY_String(b));
		//#}
		env.SetItem(PY_String("SCRIPT_NAME"), PY_String(ri->uri));
		env.SetItem(PY_String("PATH_INFO"), PY_String(ri->uri));

		env.SetItem(PY_String("PATH_TRANSLATED"), PY_String("/usr/bin/python"));
		
		//#env.SetItem(PY_String("SCRIPT_FILENAME"), PY_String(prog));
		//#env.SetItem(PY_String("HTTPS"), PY_String(conn->ns_conn->ssl != NULL ? "on" : "off"));

		if ((s = mg_get_header(ri, "Content-Type")) != NULL)
			env.SetItem(PY_String("CONTENT_TYPE"), PY_String(s));

		if (ri->query_string != NULL)
			env.SetItem(PY_String("QUERY_STRING"), PY_String(ri->query_string));

		if ((s = mg_get_header(ri, "Content-Length")) != NULL)
			env.SetItem(PY_String("CONTENT_LENGTH"), PY_String(s));

		/*
		   addenv2(blk, "PATH");
		   addenv2(blk, "TMP");
		   addenv2(blk, "TEMP");
		   addenv2(blk, "TMPDIR");
		   addenv2(blk, "PERLLIB");
		   addenv2(blk, ENV_EXPORT_TO_CGI);

#if defined(_WIN32)
addenv2(blk, "COMSPEC");
addenv2(blk, "SYSTEMROOT");
addenv2(blk, "SystemDrive");
addenv2(blk, "ProgramFiles");
addenv2(blk, "ProgramFiles(x86)");
addenv2(blk, "CommonProgramFiles(x86)");
#else
addenv2(blk, "LD_LIBRARY_PATH");
#endif // _WIN32
*/

		// Add all headers as HTTP_* variables
		for (int i = 0; i < ri->num_headers; i++) {
			char b1[256];
			char b2[256];
			sprintf(b1, "HTTP_%s",ri->http_headers[i].name); 
			sprintf(b2, "%s",ri->http_headers[i].value); 

			for (p = b1; *p; ++p){
				if (*p == '-') 
					*p = '_';
				*p = (char) toupper(* (unsigned char *) p);
			}

			printf("%s %s\n", b1, b2);
			env.SetItem(PY_String(b1), PY_String(b2));
		}

		if (G_c.IsCallable()) {
			PY_Callable c = G_c.ToCallable();

			PY_Tuple t(2);
			t.SetItem(0, env);

			char b[20];
			sprintf(b, "%p", ri);
			PY_String pri(b);
			t.SetItem(1, pri);

			PY_Object r = c.Call(t);
			printf("finito\n");
		}
		return MG_TRUE;
	}
	catch (PY_Exception &ex) {
		printf("ex - %s\n", ex.Msg());
		return MG_FALSE;
	}
}


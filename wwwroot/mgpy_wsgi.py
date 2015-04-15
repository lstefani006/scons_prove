import os, sys, emb
####################################################################

def run_with_cgi(ri, environ, application):
    environ['wsgi.input']        = sys.stdin
    environ['wsgi.errors']       = sys.stderr
    environ['wsgi.version']      = (1, 0)
    environ['wsgi.multithread']  = False
    environ['wsgi.multiprocess'] = True
    environ['wsgi.run_once']     = True
    if environ.get('HTTPS', 'off') in ('on', '1'):
        environ['wsgi.url_scheme'] = 'https'
    else:
        environ['wsgi.url_scheme'] = 'http'

    headers_set  = []
    headers_sent = []

    def write(data):
        if not headers_set:
             raise AssertionError("write() before start_response()")

        elif not headers_sent:
             # Before the first output, send the stored headers
             status, response_headers = headers_sent[:] = headers_set
             http_version = environ.get('SERVER_PROTOCOL', 'HTTP/1.1')
             http_connection = environ.get('HTTP_CONNECTION','close')

             #if not isinstance(status, int):
             #    status = int(status.split(' ')[0])
             #if status != 200:
             #    emb.mg_set_status_code(ri, status)
             emb.mg_send_header(ri, "Connection", http_connection)
             for header in response_headers:
                 emb.mg_send_header(ri, header[0], header[1])

        emb.mg_send_data(ri, data)

    def start_response(status, response_headers, exc_info=None):
        if exc_info:
            try:
                if headers_sent:
                    # Re-raise original exception if headers sent
                    raise exc_info[0], exc_info[1], exc_info[2]
            finally:
                exc_info = None     # avoid dangling circular ref
        elif headers_set:
            raise AssertionError("Headers already set!")

        headers_set[:] = [status, response_headers]
        return write

    result = application(environ, start_response)
    try:
        for data in result:
            if data:    # don't send headers until body appears
                write(data)
        if not headers_sent:
            write('')   # send headers now if body was empty
    finally:
        if hasattr(result, 'close'):
            result.close()

############### END WSGI WRAPPER ##############

def build_env(url):
    assert isinstance(url, str)
    environ = {}
    environ['SERVER_SOFTWARE'] = 'Mongoose/99'
    environ['SERVER_NAME'] = 'leo'
    environ["GATEWAY_INTERFACE"] = "CGI/1.1"
    environ['REQUEST_METHOD'] = 'GET'
    environ['PATH_TRANSLATED'] = '/usr/bin/python'
    environ['REMOTE_ADDR'] = "127.0.0.1"
    environ['REMOTE_PORT'] = "63555"

    from urlparse import urlparse
    u = urlparse(url)
    if u.scheme == 'http':
        environ["SERVER_PROTOCOL"] = "HTTP/1.1"
    if u.path:
        environ['SCRIPT_NAME'] = u.path
        environ['PATH_INFO'] = u.path
    if u.query:
        environ['QUERY_STRING'] = u.query
    if u.path and u.query:
        environ['REQUEST_URI'] = u.path + "?" + u.query
    elif u.path:
        environ['REQUEST_URI'] = u.path

    if u.netloc:
        assert isinstance(u.netloc, str)
        if ':' in u.netloc:
            environ['HTTP_HOST'] = u.netloc.split(':')[0]
            environ['SERVER_PORT'] = u.netloc.split(':')[1]
        else:
            environ['HTTP_HOST'] = u.netloc
            environ['SERVER_PORT'] = '80'
    return environ

############### TEST ##############
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    yield "<html>\r\n"
    yield "<body>\r\n"
    yield 'Ciao\r\n'

    yield "<table>\r\n"
    for e in environ:
        yield '<tr><td>{0}</td><td>{1}</td></tr>\r\n'.format(e, environ[e])
    yield "</table>\r\n"

    yield "</body>\r\n"
    yield "</html>\r\n"

if __name__ == '__main__':
    env = build_env('http://localhost:8080/tg/login?a=4')
    print env
    run_with_cgi(0, env, app.wsgi_app)

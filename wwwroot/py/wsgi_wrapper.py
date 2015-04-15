#!/usr/bin/env python
#
# This script shows how to use Flask within a WSGI wrapper for CGI.
# Flask is a WSGI framework, so we need to translate CGI into WSGI.
#
# Authors: Athula Balachandran <abalacha@cs.cmu.edu>,
#          Charles Rang <rang@cs.cmu.edu>,
#          Wolfgang Richter <wolf@cs.cmu.edu>
import os, sys

# From Flask: http://flask.pocoo.org/docs/quickstart/
# From PEP 333: http://www.python.org/dev/peps/pep-0333/
############### BEGIN WSGI WRAPPER ##############
def run_with_cgi(application):
    environ = dict(os.environ.items())
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
             sys.stdout.write('%s %s\r\n' % (http_version, status))
             sys.stdout.write('Connection: %s\r\n' % (http_connection))
             for header in response_headers:
                 sys.stdout.write('%s: %s\r\n' % header)
             sys.stdout.write('\r\n')

        sys.stdout.write(data)
        sys.stdout.flush()

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
    os.environ['SERVER_SOFTWARE'] = 'Mongoose/99'
    os.environ['SERVER_NAME'] = 'leo'
    os.environ["GATEWAY_INTERFACE"] = "CGI/1.1"
    os.environ['REQUEST_METHOD'] = 'GET'
    os.environ['PATH_TRANSLATED'] = '/usr/bin/python'
    os.environ['REMOTE_ADDR'] = "127.0.0.1"
    os.environ['REMOTE_PORT'] = "63555"

    from urlparse import urlparse
    u = urlparse(url)
    if u.scheme == 'http':
        os.environ["SERVER_PROTOCOL"] = "HTTP/1.1"
    if u.path:
        os.environ['SCRIPT_NAME'] = u.path
        os.environ['PATH_INFO'] = u.path
    if u.query:
        os.environ['QUERY_STRING'] = u.query
    if u.path and u.query:
        os.environ['REQUEST_URI'] = u.path + "?" + u.query
    elif u.path:
        os.environ['REQUEST_URI'] = u.path

    if u.netloc:
        assert isinstance(u.netloc, str)
        if ':' in u.netloc:
            os.environ['HTTP_HOST'] = u.netloc.split(':')[0]
            os.environ['SERVER_PORT'] = u.netloc.split(':')[1]
        else:
            os.environ['HTTP_HOST'] = u.netloc
            os.environ['SERVER_PORT'] = '80'

############### TEST ##############
if __name__ == '__main__':
    def application(environ, start_response):
        start_response('200 OK', [('Content-Type', 'text/plain')])
        yield "Ciao 1\n"
        yield "Ciao 2\n"
    if not os.environ.has_key('SERVER_SOFTWARE'):
        build_env('http://localhost:8080/pippo.py/leo?a=4')
    run_with_cgi(application)

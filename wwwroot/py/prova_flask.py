from flask import Flask
from flask import request
from flask import abort, redirect, url_for, request
from flask import render_template_string

app = Flask(__name__)

@app.route('/py/prova_flask.py')
def prova_flask():
    if request.method == 'POST':
        pass
    #if request.args.get('a'):
    #    return "Ciao {0}".format(request.args.get('a'))
    s = render_template_string("""\
<html>
    <body>
        <table border=1>
        {% for e in env %}   <tr><td>{{e}}</td><td>{{env[e]}}</td></tr>
        {% endfor %}</table>
    </body>
</html>
    """, env=os.environ)
    return s

################################################################################

def build_env(url):
    assert isinstance(url, str)

    from urlparse import urlparse

    os.environ['SERVER_SOFTWARE'] = 'Mongoose/99'
    os.environ['SERVER_NAME'] = 'leo'
    os.environ["GATEWAY_INTERFACE"] = "CGI/1.1"
    os.environ['REQUEST_METHOD'] = 'GET'
    os.environ['PATH_TRANSLATED'] = '/usr/bin/python'
    os.environ['REMOTE_ADDR'] = "127.0.0.1"
    os.environ['REMOTE_PORT'] = "63555"

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
    return


if __name__ == '__main__':
    import os
    if not os.getenv('SERVER_SOFTWARE'):
        url = 'http://localhost:8080/py/prova_flask.py?a=4'
        build_env(url)

    import logging
    h = logging.FileHandler("leo.log", "a")
    h.setLevel(logging.DEBUG)
    h.setFormatter(logging.Formatter('%(asctime)s %(levelname)s: %(message)s', '[in %(pathname)s:%(lineno)d]'))
    app.logger.addHandler(h)
    app.debug = True

    for e in os.environ:
        ss = '{0} ==> {1}'.format(e, os.environ[e])
        app.logger.debug(ss)

    if False:
        import wsgiref.handlers
        wsgiref.handlers.CGIHandler().run(app.wsgi_app)
    else:
        import wsgi_wrapper
        wsgi_wrapper.run_with_cgi(app.wsgi_app)

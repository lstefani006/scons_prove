#!/usr/bin/python
import emb, os, sys, mgpy_wsgi

from flask import Flask
from flask import session
from flask import redirect
from flask import render_template_string
from flask import url_for, escape, request, abort

app = Flask(__name__)
# set the secret key. keep this really secret:
app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'

@app.route('/tg/login', methods=['GET', 'POST'])
def login():
    print '######################################'
    print request.method
    #if request.method == 'POST':
    #    pass
        #session.pop('username', None)
    #    if request.form['username'] == 'leo' and request.form['pwd'] == 'spectrum':
            #session['username'] = request.form['username']
    #        return """<html>ok</html>"""
    #        return redirect(url_for('leo'))
    return render_template_string('''\
<html>
<body>
<form action="" method="post">
    <p>Username:<input type='text' name='username'/>
    <p>Password:<input type='text' name='pwd'/>
    <p><input type='submit' value='Login'>
</form>
</body>
</html>
''')

@app.route('/tg/logout')
def logout():
    # remove the username from the session if it's there
    session.pop('username', None)
    return redirect(url_for('index'))

g_env = {}

@app.route('/tg/leo')
def leo():
    s = render_template_string("""\
<html>
    <body>
        <table border=1>
        {% for e in env %}   <tr><td>{{e}}</td><td>{{env[e]}}</td></tr>
        {% endfor %}</table>
    </body>
</html>
""", env=g_env)
    return s

def setupLog(application):
    import logging
    h = logging.FileHandler("leo.log", "a")
    h.setLevel(logging.DEBUG)
    h.setFormatter(logging.Formatter('%(asctime)s %(levelname)s: %(message)s', '[in %(pathname)s:%(lineno)d]'))
    application.logger.addHandler(h)
    application.debug = True

if __name__ == '__main__':
    env = None
    if len(sys.argv) == 2:    # un solo argomento
        arg = sys.argv[1]
        import pickle
        with open(arg) as f:
            env = pickle.load(f)
    if not env:
        env = mgpy_wsgi.build_env('http://localhost:8080/tg/login')
    assert isinstance(env, dict)
    g_env = env
    print env
    setupLog(app)
    mgpy_wsgi.run_with_cgi(0, env, app.wsgi_app)
    app.logger.debug('finito')

def request(env, ri):
    assert isinstance(env, dict)
    import pickle
    with open("env.pickle", "w") as f:
        pickle.dump(env, f)

    global g_env
    g_env = env.copy()
    setupLog(app)
    mgpy_wsgi.run_with_cgi(ri, env, app.wsgi_app)
    app.logger.debug('finito')

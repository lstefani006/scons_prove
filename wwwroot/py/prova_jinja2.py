#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# enable debugging
import cgitb
import jinja2
import os

cgitb.enable()

template = jinja2.Template("""\
<html>
    <body>
        <table border=1>
        {% for e in ev %}   <tr><td>{{e}}</td><td>{{ev[e]}}</td></tr>
        {% endfor %}</table>
    </body>
</html>
""")

import wsgiref.handlers

def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    s = template.render(ev=os.environ)
    return [str(s)]

if __name__ == '__main__':
    wsgiref.handlers.CGIHandler().run(application)

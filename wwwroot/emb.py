#!/usr/bin/python

# utilizzato solo da mgpy.py quando si lancia
# ./mgpy da command prompt
import os, sys

def mg_set_status_code(ri, status):
    print 'mg_set_status_code({0})'.format(status)

def mg_send_header(ri, k, v):
    print 'mg_send_header({0}, {1})'.format(k, v)

def mg_send_data(ri, data):
    print data.replace('\r\n', '')

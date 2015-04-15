#!/usr/bin/python
import os
import os.path
import sys
import re
import StringIO

class CppParser:
    def __init__(self, fname):
        self.fname = fname
        with open(fname) as f:
            self.rows = f.readlines()

    def findDerivedClasses(self, className):
        ret = []
        rg = r'^\s*((class)|(struct))\s+(\w+\s+)?(?P<der>\w+)\s*:\s*(((public)|(private)|(protected))\s+)?{0}\s*({{)?$'.format(className)
        pattern = re.compile(rg)

        for r in self.rows:
            m = pattern.match(r)
            if m:
                der = m.group('der')
                ret.append(der)
        return ret

    def findBaseClass(self, className):
        ret = None
        rg = r'\s*(class)|(struct)\s+(\w+\s+)?{0}\s*:\s*(((public)|(private)|(protected))\s+)?(?P<base>\w+)(\s*{{)?'.format(className)
        pattern = re.compile(rg)
        for r in self.rows:
            m = pattern.match(r)
            if m:
                ret = m.group('base')
                break
        return ret


def replace(s, *args, **kwargs):
    i = 0
    for v in args:
        s = s.replace("{"+str(i)+"}", v)
        i += 1
    for k, v in kwargs.iteritems():
        s = s.replace("{"+k+"}", v)
    return s;

def generateVisitor(className, fileName, fout=None):
    f = CppParser(fileName)

    if fout:
        w = open(fout, "w")
        w.write(replace('#ifndef __{f}__\n', f=args.fileout.replace(".", "_")))
        w.write(replace('#define __{f}__\n', f=args.fileout.replace(".", "_")))
        w.write("\n")
    else:
        w = sys.stdout

    for c in className:
        derClass = f.findDerivedClasses(c)
        w.write(replace("struct {className}Visitor {\n", className=c))
        w.write(replace("\tvirtual ~{className}Visitor() {}\n", className=c))
        for r in derClass:
            w.write(replace("\tvirtual void visit({0} &) = 0;\n", r))
        w.write("};\n")
        w.write("\n")
        for r in derClass:
            w.write(replace("inline void {0}::accept({1}Visitor &v) { v.visit(*this); }\n", r, c))
        w.write("\n")

    if w != sys.stdout:
        w.write('#endif\n')
        w.close()


def generateConcreteVisitor(hpp, className, fileName, fileNameOut, **kwargs):
    f = CppParser(fileName)
    derClass = f.findDerivedClasses(className)

    if fileNameOut:
        fout = open(fileNameOut, "w")
    else:
        fout = sys.stdout

    action = "Print"

    targ = "std::string arg"
    varg = "arg"
    tret = "std::string ret"
    vret = "ret"

    for k, v in kwargs.iteritems():
        if k == "action":
            action = v
        elif k == "targ":
            targ = v
        elif k == "varg":
            varg = v
        elif k == "tret":
            tret = v
        elif k == "vret":
            vret = v
        else:
            print 'invalid option ' + k
            sys.exit(1)

    if hpp:
        print >> fout, replace("#ifndef __{baseClass}Visitor_{action}__", baseClass=className, action=action)
        print >> fout, replace("#define __{baseClass}Visitor_{action}__", baseClass=className, action=action)
        print >> fout, replace('#include "{fileName}"', fileName=fileName)
        print >> fout
        code = """\
struct {baseClass}Visitor_{action} : {baseClass}Visitor {
    {targ};
    {tret};
:
    {baseClass}Visitor_{action}({targ}) : {varg}({varg}) {}

    static decltype({varg}) {action}({baseClass} *v, {targ}) {
        {baseClass}Visitor_{action} c({varg});
        v->accept(c);
        return c.{vret};
    }
    """
        code = replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret)
        print >> fout, code

        for d in derClass:
            code = """\
    void visit({derClass} &v);"""
            print >> fout, replace(code, derClass=d)
        print >> fout, "};"
        print >> fout
        print >> fout, "/*"
        for d in derClass:
            code = """\
void {baseClass}Visitor_{action}::visit({derClass} &v) {}"""
            print >> fout, replace(code, derClass=d, baseClass=className, action=action)
        print >> fout, "*/"
        print >> fout
        print >> fout, replace("#endif // __{baseClass}Visitor_{action}__", baseClass=className, action=action)
    else:
        print >> fout, replace('#include "{baseClass}Visitor_{action}.hpp"', baseClass=className, action=action)
        print >> fout
        for d in derClass:
            code = """\
void {baseClass}Visitor_{action}::visit({derClass} &v)
{
}
"""
            print >> fout, replace(code, derClass=d, baseClass=className, action=action)
##############################################

def checkFile(fileName):
    try:
        f = open(fileName, "r")
        f.close()
        return True
    except:
        pass
    return False

def generateConcreteVisitorSwitch(hpp, className, fileNameIn, fileNameOut, **kwargs):
    f = CppParser(fileNameIn)
    derClass = f.findDerivedClasses(className)

    action = "Print"

    targ = "std::string arg"
    varg = "arg"
    tret = "std::string ret"
    vret = "ret"

    for k, v in kwargs.iteritems():
        if k == "action":
            action = v
        elif k == "targ":
            targ = v
        elif k == "varg":
            varg = v
        elif k == "tret":
            tret = v
        elif k == "vret":
            vret = v
        else:
            print 'invalid option ' + k
            sys.exit(1)

    if hpp:
        # si sovrascrive sempre
        if fileNameOut:
            fout = open(fileNameOut, "w")
        else:
            fout = sys.stdout
    else:
        # si aggiunge la funzione quando manca
        if fileNameOut: 
            if checkFile(fileNameOut):
                fout = open(fileNameOut, "a")
                newfile = False
                print "Updating <" + fileNameOut + ">"
            else:
                fout = open(fileNameOut, "w")
                newfile = True
                print "Generating <" + fileNameOut + ">"
        else:
            fout = sys.stdout
            newfile = True


    if hpp:
        print >> fout, replace("#ifndef __{action}_{baseClass}Visitor___", baseClass=className, action=action)
        print >> fout, replace("#define __{action}_{baseClass}Visitor___", baseClass=className, action=action)
        print >> fout, replace('#include "{fileNameIn}"', fileNameIn=fileNameIn)
        print >> fout
        code = "struct {baseClass}Visitor_{action} {"
        print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret)

        code = "\tstatic {tret} {action}_{baseClass}({baseClass} *v, {targ});"
        print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret)

        print >> fout, "private:"
        for d in derClass:
            code = "\tstatic {tret} {action}_{derClass}({derClass} *v, {targ});"
            print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
        print >> fout, "};"
        print >> fout
        if True:
            print >> fout, "/*"
            for d in derClass:
                code = "{tret} {baseClass}Visitor_{action}::{action}_{derClass}({derClass} *v, {targ}) {}"
                print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
            print >> fout, "*/"
        if True:
            code = "inline {tret} {baseClass}Visitor_{action}::{action}_{baseClass}({baseClass} *v, {targ}) {"
            print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret)
            print >> fout, "\tswitch (v->getType()) {"
            for d in derClass:
                code = "\tcase {derClass}_Id:"
                print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
                code = "\t\treturn {action}_{derClass}(({derClass} *)v, {varg});"
                print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
            print >> fout, "\tdefault:"
            print >> fout, "\t\tassert(false);"
            print >> fout, "\t\tbreak;"
            print >> fout, "\t}"
            print >> fout, "}"
            print >> fout

            code = "inline {tret} {action}({baseClass} *v, {targ}) {"
            print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret)
            code = "\treturn {baseClass}Visitor_{action}::{action}_{baseClass}(v, {varg});"
            print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
            print >> fout, "}"
        print >> fout, replace("#endif // __{action}_{baseClass}Visitor__", baseClass=className, action=action)
    else:
        if newfile:
            print >> fout, "#include <assert.h>"
            print >> fout, replace('#include "{action}_{baseClass}Visitor.hpp"', baseClass=className, action=action)
            print >> fout
            for d in derClass:
                code = "{tret} {baseClass}Visitor_{action}::{action}_{derClass}({derClass} *v, {targ}) {"
                print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
                code = "\treturn {vret};"
                print >> fout, replace(code, baseClass=className, action=action, targ=targ, varg=varg, tret=tret, vret=vret, derClass=d)
                print >> fout, "}"

def grep(fileName, pattern):
    f = open(fileName, "r")
    for r in f:
        if pattern in r:
            return True
    return False

#################################################

def updateFile(fileName, f, marker):
    """legge il file fileName (stringa)
    cerca il marker e scrive al posto del marker f
    che puo` essere una lista di stringa o un file
    """
    fin  = open(fileName, "r")
    rg = r'^\s*//%visitor {0}\s*$'.format(marker)
    pattern = re.compile(rg)

    # il secondo //%visitor e' opzionale...
    # con due genero il codice dentro il blocco
    # con uno genero il codice e aggiungo il secondo //%visitor
    vis = 0
    if True:
        for row in fin:
            m = pattern.match(row)
            if m:
                vis += 1
        if vis == 0:
            print "cannot find '//%visitor {0}' marker".format(marker)
            exit(1)
        if vis > 2:
            print "too many '//%visitor {0}' marker".format(marker)
            exit(1)
    fin.close()

    fin  = open(fileName, "r")
    fout = open(fileName + ".tmp", "w")

    state = 0 # prima di //%visitor
    state = 1 # tra il primo e il secondo //%visitor
    state = 2 # dopo il secondo //%visitor

    state = 0
    for row in fin:
        m = pattern.match(row)
        if not m:
            if state == 0 or state == 2:
                fout.write(row)
            else:
                pass
                # butto via il pezzo generato prima
        else:
            state += 1
            if state == 1:
                fout.write(row)
                for a in f:
                    fout.write("%s\n" % a)
                fout.write(row)
            if vis == 1:
                state += 1

    fout.close()
    fin.close()

    if state == 0:
        print "cannot find '//%visitor {0}' marker".format(marker)
        os.delete(fileName + ".tmp")
        exit(1)
    os.rename(fileName + ".tmp", fileName)



#################################################

def base_visitor(args):
    generateVisitor(args.className, args.fileName, args.fileout)

def concrete_visitor(args):
    if not args.hpp and not args.cpp:
        args.hpp = True

    if args.hpp:
        generateConcreteVisitorSwitch(True, args.className, args.fileName, args.fileout, action=args.action, targ=args.targ, varg=args.varg, tret=args.tret, vret=args.vret)
    else:
        generateConcreteVisitorSwitch(False, args.className, args.fileName, args.fileout, action=args.action, targ=args.targ, varg=args.varg, tret=args.tret, vret=args.vret)

def gen_enum(args):
    cpp = CppParser(args.fileName)
    if args.fileout:
        with open(args.fileout, "w") as fout:
            ss = args.fileout.replace(".", "_")
            fout.write('#ifndef __{0}__\n'.format(ss))
            fout.write('#define __{0}__\n'.format(ss))
            fout.write('\n')
            for className in args.className:
                i = 1
                fout.write('enum {0}_IdType\n'.format(className))
                fout.write('{')
                for d in cpp.findDerivedClasses(className):
                    fout.write("\t{0}_Id = {1},\n".format(d, i))
                    i += 1
                fout.write('};\n\n')
            fout.write('#endif // __{0}__\n'.format(ss))
    else:
        ee = []
        for className in args.className:
            i = 1
            ee.append('enum {0}_IdType'.format(className))
            ee.append('{')
            for d in cpp.findDerivedClasses(className):
                ee.append("\t{0}_Id = {1},".format(d, i))
                i += 1
            ee.append('};\n')
        updateFile(args.fileName, ee, 'enum')

####################################################################3
import argparse

parser = argparse.ArgumentParser()
sps = parser.add_subparsers()

sp = sps.add_parser("base_visitor")
if True:
    sp.add_argument("--fileout", help="output file")
    sp.add_argument("fileName", help="the file to read")
    sp.add_argument("className", help="the base class", nargs='+')
    sp.set_defaults(func=base_visitor)

sp = sps.add_parser("enum")
if True:
    sp.add_argument("--fileout", help="output file")
    sp.add_argument("fileName", help="the file to read")
    sp.add_argument("className", help="the base class", nargs='+')
    sp.set_defaults(func=gen_enum)

sp = sps.add_parser("concrete_visitor")
if True:
    sp.add_argument("--hpp", help="generate .hpp file", action="store_true")
    sp.add_argument("--cpp", help="generate .cpp file", action="store_true")
    sp.add_argument("--action", help="action to perform - e.g. Print")
    sp.add_argument("--targ", help="in type  - std::string arg")
    sp.add_argument("--varg", help="in var   - arg")
    sp.add_argument("--tret", help="ret type - std::string ret")
    sp.add_argument("--vret", help="ret var  - ret")
    sp.add_argument("--fileout", help="output file")
    sp.add_argument("className", help="the base class")
    sp.add_argument("fileName", help="the file to read")
    sp.set_defaults(func=concrete_visitor)

args = parser.parse_args()

args.func(args)

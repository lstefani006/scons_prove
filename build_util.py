import os, platform, types
from SCons.Script import *

def error(fmt, *args):
    print 'error: ', fmt.format(*args)
    sys.exit(1)
def warn(fmt, *args):
    print 'warn: ', fmt.format(*args)

class Opt:

    def __init__(self):
        #self.__dict__.update(dictionary)
        #for k,v in dictionary.items():
        #    setattr(self, k, v)
        # controllo BASE

        # piattaforme supportate
        if platform.system() == 'Windows':
            self.win32 = True
            self.linux = False
        elif platform.system() == 'Linux':
            self.win32 = False
            self.linux = True
        else:
            error('Unknown platform {0}', platform.system())

        self.dirBase = os.getenv('BASE', None)
        if not self.dirBase:
            error("Please set BASE environment !")

        # var per capire la piattaforma
        # e le directory bin/lib (in win coincidono)
        self.platform = platform.system()
        if self.win32:
            self.dirBin = os.path.join(self.dirBase, 'bin')
            self.dirLib = os.path.join(self.dirBase, 'bin')
        elif self.linux:
            self.dirBin = os.path.join(self.dirBase, 'bin')
            self.dirLib = os.path.join(self.dirBase, 'lib')
        else:
            error('Unknown platform {0} while setting win32/linux', platform.system())

        # per linkare gli .exe/.dll con cl e non con link
        # mettere win32_link = Nome per linkare con link
        self.win32_link = None
        if self.win32:
            self.win32_link = "cl"

        self.dumpEnv = ARGUMENTS.get('dumpEnv', 0)     # per fare il dump
        self.release = ARGUMENTS.get('release', 0)     # per capire se compiliamo in debug o in release
        self.lang = None
        if self.linux:                                 # per attivare clang
            self.clang   = ARGUMENTS.get('clang', 0)

    def createEnv(self):
        """creo l'Enviroment cercando di impostare 
        le cose necessarie per la piattaforma
        """
        if self.win32:
            env = Environment(tools=['default'], ENV=os.environ, MSVC_VERSION='12.0', MSVS_SUITE='PROFESSIONAL')
        elif self.linux:
            if self.clang:
                env = Environment(CC='clang', CXX='clang++')
            else:
                env = Environment()

        # per comodita' e' bello avere if env.linux:
        setattr(env, 'linux', opt.linux)
        setattr(env, 'win32', opt.win32)

        # aggiungo alla classe e NON all'istanza la funzione ExportEnv/ImportEnv/MergeEnv
        def _ExportEnv(self, macro):
            if self.win32:
                self.Append(CCFLAGS=['"-D{0}=__declspec(dllexport)"'.format(macro)])
            elif self.linux:
                self.Append(CCFLAGS=['"-D{0}=__attribute__((visibility(\\"default\\")))"'.format(macro)])
        setattr(Environment, 'ExportEnv', _ExportEnv)

        def _ImportEnv(self, macro):
            if self.win32:
                self.Append(CCFLAGS=['"-D{0}=__declspec(dllimport)"'.format(macro)])
            elif self.linux:
                self.Append(CCFLAGS=['-D{0}=""'.format(macro)])
        setattr(Environment, 'ImportEnv', _ImportEnv)

        def _MergeEnv(self, srcEnv):
            if srcEnv.has_key('CCFLAGS'):
                self.AppendUnique(CCFLAGS=srcEnv['CCFLAGS'])
            if srcEnv.has_key('CPPDEFINES'):
                self.AppendUnique(CPPDEFINES=srcEnv['CPPDEFINES'])
            if srcEnv.has_key('CPPPATH'):
                self.AppendUnique(CPPPATH=srcEnv['CPPPATH'])
            if srcEnv.has_key('LIBS'):
                self.AppendUnique(LIBS=srcEnv['LIBS'])
            if srcEnv.has_key('LIBPATH'):
                self.AppendUnique(LIBPATH=srcEnv['LIBPATH'])
            if srcEnv.has_key('LINKFLAGS'):
                self.AppendUnique(LINKFLAGS=srcEnv['LINKFLAGS'])
        setattr(Environment, 'MergeEnv', _MergeEnv)

        if opt.linux:
            env.AppendUnique(CCFLAGS=['-pthread', '-Wall'])
            env.AppendUnique(LINKFLAGS=['-rdynamic'])
            env.AppendUnique(LINKFLAGS=['-pthread'])
            if opt.release:
                env.AppendUnique(CCFLAGS=['-O'])
            else:
                env.AppendUnique(CCFLAGS=['-g'])
                env.AppendUnique(LINKFLAGS=['-g'])
                env.AppendUnique(CCFLAGS=['-fvisibility=hidden'])

        elif opt.win32:
            env.AppendUnique(CCFLAGS=['/EHsc'])
            env['WINDOWS_EMBED_MANIFEST']=1
            if opt.release:
                env.AppendUnique(SHCCFLAGS=['/LD'])
                env.AppendUnique(CCFLAGS=['/Ox', '/MD'])
                if opt.win32_link == 'cl':
                    env['SHLINK']='cl'
                    env['SHLINKCOM']='$SHLINK $SOURCES /Fe$TARGET /LD /MD $_LIBFLAGS /link $_LIBDIRFLAGS /MANIFEST'
                    env['LINK']='cl'
                    env['LINKCOM']='$LINK $SOURCES /Fe$TARGET /MD $_LIBFLAGS /link $_LIBDIRFLAGS /MANIFEST'
            else:
                env.AppendUnique(CCFLAGS=['/Od', '/MDd', '/Zi', '/RTC1'])
                env.AppendUnique(SHCCFLAGS=['/LDd'])
                if opt.win32_link == 'cl':
                    env['SHLINK']='cl'
                    env['SHLINKCOM']='$SHLINK $SOURCES /Fe$TARGET /LDd /MDd $_LIBFLAGS /link $_LIBDIRFLAGS /debug'
                    env['LINK']='cl'
                    env['LINKCOM']='$LINK $SOURCES /Fe$TARGET /MDd $_LIBFLAGS /link $_LIBDIRFLAGS /debug'
        else:
            error('Unknow platform {0} while setting env', platform.system())

        env.AppendUnique(LIBPATH=[opt.dirLib])

        if self.dumpEnv:
            print(env.Dump())
        return env

######################################################################################################
opt = Opt()
env = opt.createEnv()

Export('env opt')

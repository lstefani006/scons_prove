import os, sys

basePath = os.getenv('BASE')
if not basePath:
    print('please $BASE to pwd')
    print('export BASE=`pwd`')
    sys.exit(1)

sys.path.append(os.path.abspath(basePath))

import build_util
Import('*')

env_mongoose = SConscript('mongoose/SConscript', 'env opt')
env_sqlite3  = SConscript('sqlite-amalgamation-3080500/SConscript', 'env opt')
env_epy      = SConscript('embed_python/SConscript', 'env opt')
env_leakfinder = SConscript('leakfinder/SConscript', 'env opt')

env = env.Clone()
env.MergeEnv(env_mongoose)
env.MergeEnv(env_sqlite3)
env.MergeEnv(env_epy)
a = env.Program('mongoose_server', ['mongoose_server.cpp'])
a = env.Install(opt.dirBin, a)
env.Default(a)

Return('env')

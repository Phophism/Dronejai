from SCons.Script import DefaultEnvironment
import glob, shutil, os

env = DefaultEnvironment()
# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

if not os.path.exists("extras/tmp"):
    os.makedirs("extras/tmp")

if not os.path.exists("extras/tmp/ESPert"):
    os.makedirs("extras/tmp/ESPert")

for file in glob.iglob('src/*.*'):
    print 'Copied %s' % (file)
    shutil.copy2(file, "extras/tmp/ESPert")

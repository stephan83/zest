def options(opt):
  opt.load('compiler_cxx boost')

def configure(conf):
  #conf.env.LIBPATH_TEST   = ['/usr/lib', '/usr/local/lib']
  conf.load('compiler_cxx')
  conf.check(
    features          = 'cxx cxxprogram',
    cxxflags          = '-O3 -Wall -mmacosx-version-min=10.5'
  )
  conf.check(
    lib               = 'pthread',
    uselib_store      = 'PTHREAD'
  )
  conf.check(
    lib               = 'log4cxx',
    uselib_store      = 'LOG4CXX'
  )
  conf.load('boost')
  conf.check_boost(
    lib               = 'date_time filesystem iostreams program_options regex '
                      + 'serialization system thread',
    static            = True
  )
  conf.env.append_unique('LINKFLAGS', ['-static -mmacosx-version-min=10.5']) 

def build(bld):
  bld.recurse('src')
  bld.install_files('${PREFIX}/bin', ['build/zest'])
  bld.install_files('${PREFIX}/etc/zest', ['conf/zest.conf', 'conf/log.conf'])

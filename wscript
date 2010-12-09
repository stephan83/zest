def options(opt):
  opt.load('compiler_c compiler_cxx boost')

def configure(conf):
  conf.load('compiler_c compiler_cxx')
  conf.check(
    features          = 'c cprogram',
    cflags            = '-O3 -Wall'
  )
  conf.check(
    features          = 'cxx cxxprogram',
    cxxflags          = '-O3 -Wall'
  )
  conf.check(
    lib               = 'pthread',
    uselib_store      = 'PTHREAD'
  )
  #conf.check(
  #  lib               = 'ctemplate',
  #  uselib_store      = 'CTEMPLATE'
  #)
  conf.load('boost')
  conf.check_boost(
    lib               = 'date_time filesystem iostreams log program_options regex '
                      + 'serialization system thread',
    static            = 'onlystatic'
  )

def build(bld):
  bld.recurse('src')
  bld.install_files('${PREFIX}/bin', ['build/zest'])
  bld.install_files('${PREFIX}/etc', ['conf/zest.conf'])

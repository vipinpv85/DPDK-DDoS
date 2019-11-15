project('ddos', 'c', version : '1.0.0', license : 'GPL')

# check the OS is supported, rather than going any further
supported_exec_envs = ['linux']
exec_env = host_machine.system()
if not supported_exec_envs.contains(exec_env)
        error('unsupported system type "@0@"'.format(exec_env))
endif

# define a handy variable for checking which OS we have.
# gives us "is_windows", "is_freebsd" and "is_linux"
foreach env:supported_exec_envs
        set_variable('is_' + env, exec_env == env)
endforeach

add_project_link_arguments('-Wl,--no-as-needed', language: 'c')
add_project_arguments('-include', 'rte_config.h', language: 'c')

c=meson.get_compiler('c')

c_version = c.version()
message('compiler version ' + c_version)

ddos_conf = configuration_data()
#ddos_conf.set('RTE_MAX_LCORE', get_option('max_lcores'))

# enable extra warnings and disable any unwanted warnings
warning_flags = [
        # -Wall is added by meson by default, so add -Wextra only
        '-Wextra',

        # additional warnings in alphabetical order
        '-Wcast-qual',
        '-Wdeprecated',
        '-Wformat-nonliteral',
        '-Wformat-security',
        '-Wmissing-declarations',
        '-Wmissing-prototypes',
        '-Wnested-externs',
        '-Wold-style-definition',
        '-Wpointer-arith',
        '-Wsign-compare',
        '-Wstrict-prototypes',
        '-Wundef',
        '-Wwrite-strings',

        # globally disabled warnings
        '-Wno-address-of-packed-member',
        '-Wno-packed-not-aligned',
        '-Wno-missing-field-initializers'
]

compiler = meson.get_compiler('c')

subdir('src')

env = environment()
dpdksdk = ''
#get_option('rtesdk')
dpdktarget = ''
#get_option('rtetarget')
dpdkfound = true

if dpdksdk.strip() == ''
	dpdkfound = false
endif

if dpdktarget.strip() == ''
	dpdkfound = false
endif

dpdksdk = ''
dpdktarget = ''

r = run_command('echo', ' $RTE_SDK')
if r.returncode() == 0
  dpdksdk = r.stdout().strip()
  errortxt = r.stderr().strip()
endif

r = run_command('echo', ' $RTE_SDK')
if r.returncode() == 0
  dpdktarget = r.stdout().strip()
  errortxt = r.stderr().strip()
endif

dpdkincdir = dpdksdk.strip() + dpdktarget.strip() + '/include'
#incdir = include_directories(dpdkincdir)

#dpdkdep = dependency('dpdk')
#executable('intercept', 'src/pkt-capture.c', dependencies : dpdkdep)
#executable('ddos', 'src/pkt-ddos.c', dependencies : dpdkdep)
#executable('ddos', 'src/pkt-ddos.c', include_directories : incdir, link_args : '-Wl,--linker-option')
executable('ddos', 'src/pkt-ddos.c', link_args : '-Wl,--linker-option')

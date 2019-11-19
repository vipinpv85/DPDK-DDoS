project('ddos', 'c', version : '1.0.0', license : 'GPL',
	default_options: ['enable_dpdk_sdk=static'])

# check the OS is supported, rather than going any further
supported_exec_envs = ['linux']
exec_env = host_machine.system()
if not supported_exec_envs.contains(exec_env)
	error('unsupported system type "@0@"'.format(exec_env))
endif


pcap_dep = dependency('pcap', version : '>=1.0')
dpdk_sdk = ''
#if get_option('enable_dpdk_sdk')
#	dpdk_sdk = 'I have to fetch the values'
#endif
message('dpdk_sdk: "' + dpdk_sdk + '"')

#get meson found compiler
compiler = meson.get_compiler('c')

message('building on: "' + exec_env + '"')
message('with compiler: "' + compiler.version() + '"')
message('host machine: "' + host_machine.system() + '"')

add_project_link_arguments('-Wl,--no-as-needed', language: 'c')
add_project_arguments('-include', 'rte_config.h', language: 'c')

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

subdir('src')

dpdksdk = ''
#get_option('rtesdk')
dpdktarget = ''
#get_option('rtetarget')

dpdkfound = true
if dpdksdk.strip() == ''
	dpdkfound = false
endif

r = run_command('echo', '$RTE_SDK')
if r.returncode() == 0
  dpdksdk = r.stdout().strip()
  errortxt = r.stderr().strip()
endif

r = run_command('echo', '$RTE_SDK')
if r.returncode() == 0
  dpdktarget = r.stdout().strip()
  errortxt = r.stderr().strip()
endif

message('dpdksdk: "' + dpdksdk + '"')
message('dpdktarget: "' + dpdktarget + '"')

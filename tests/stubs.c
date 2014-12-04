/*
 * (Manually built) list of stubs for various Na* symbols used by livestatus
 *
 * Note that if you want to test something that actually
 * makes use of these symbols you probably don't want to
 * link against this CU.
 *
 * Alternatively, you could do some linking magic (like
 * ld's -z muldefs), define a mock of the symbol you wish to use,
 * and make sure that symbol is resolved before the one defined here.
 *
 * But, I digress.
 * */
int grab_service_macros = 0;
int enable_event_handlers = 0;
int nagios_pid = 0;
int process_external_command1 = 0;
int check_host_freshness = 0;
int obsess_over_hosts = 0;
int program_start = 0;
int accept_passive_service_checks = 0;
int event_broker_options = 0;
int is_escalated_contact_for_host = 0;
int check_time_against_period = 0;
int obsess_over_services = 0;
int check_result_path = 0;
int is_contact_for_host = 0;
int enable_notifications = 0;
int num_objects = 0;
int find_hostgroup = 0;
int find_servicegroup = 0;
int command_list = 0;
int get_raw_command_line = 0;
int hostgroup_list = 0;
int contact_list = 0;
int is_contact_for_service = 0;
int check_service_freshness = 0;
int process_macros = 0;
int grab_host_macros = 0;
int find_contactgroup = 0;
int execute_host_checks = 0;
int find_command = 0;
int service_list = 0;
int log_file = 0;
int service_check_timeout = 0;
int last_log_rotation = 0;
int find_contact = 0;
int rotate_log_file = 0;
int find_service = 0;
int neb_register_callback = 0;
int interval_length = 0;
int process_performance_data = 0;
int contactgroup_list = 0;
int check_external_commands = 0;
int macro_user = 0;
int get_program_version = 0;
int execute_service_checks = 0;
int currently_running_host_checks = 0;
int log_archive_path = 0;
int currently_running_service_checks = 0;
int timeperiod_list = 0;
int is_escalated_contact_for_service = 0;
int enable_flap_detection = 0;
int host_check_timeout = 0;
int accept_passive_host_checks = 0;
int find_host = 0;
int servicegroup_list = 0;
int neb_deregister_callback = 0;
int clear_volatile_macros = 0;
int host_list = 0;

void nm_log(int prio, const char *fmt, ...) {}
char *qh_socket_path = 0;
int nsock_printf_nul(int sd, const char *fmt, ...) {};
int nsock_unix(const char *path, unsigned int flags) {};
const char *nsock_strerror(int code) {};

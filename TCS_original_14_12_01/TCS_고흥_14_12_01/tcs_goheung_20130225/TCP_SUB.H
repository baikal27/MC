extern int care_server(void);
extern int get_server_command(void);
extern int decode_command_string(int ns);
extern int write_error_code(int ns, int error_code);
extern int return_message(int ns);
extern int put_check_sum(void);
extern int com_check_sum(void);


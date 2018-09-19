extern int register_tcp_server(int tcp_server_port_no);
extern int tcp_server_write(char buffer[]);
extern int tcp_server_block_write(char buffer[], unsigned int size);
extern int quit_tcp_server(void);
extern int disconnect_tcp_client(void);
extern int disconnect_tcp_client(void);
extern void reserve_tcp_client(char client_name[]);


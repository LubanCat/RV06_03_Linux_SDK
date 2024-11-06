#ifndef SERVER_H
#define SERVER_H

int setup_serial_port(const char *port);
//将str和num组成json格式的字符串，并发送给客户端
int send_json_number(char *str,int num);

void handle_post_door_ctrl(const char *main_json_str);
void handle_out_light_ctrl(const char *main_json_str);
void handle_in_light_ctrl(const char *main_json_str);
void handle_ir_ctrl(const char *main_json_str);
void handle_weight_ctrl(const char *main_json_str);
void handle_clean_door_ctrl(const char *main_json_str);
void handle_update_ctrl(const char *main_json_str);

void handle_speaker_ctrl(const char *main_json_str);
void handle_file_transfer(const char *main_json_str);

void handle_machine_ctrl(const char *main_json_str);

void process_json(const char *json_str);

int server_start();
int test_json();

#endif /* SERVER_H */

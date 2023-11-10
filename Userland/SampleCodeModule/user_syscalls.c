#include <stdbool.h>
#include <stdint.h>
#include <user_lib.h>
#include <user_syscalls.h>

void sys_clear_screen() { sys_int_80((uint64_t)0, 0, 0, 0, 0, 0); }

void sys_write(char *c, int color) {
  sys_int_80((uint64_t)1, (uint64_t)c, color, 0, 0, 0);
}

void sys_read(char *buff) {
  sys_int_80((uint64_t)2, (uint64_t)buff, 0, 0, 0, 0);
}

void sys_get_hours(int *hours) {
  sys_int_80((uint64_t)3, (uint64_t)hours, 0, 0, 0, 0);
}

void sys_get_minutes(int *min) {
  sys_int_80((uint64_t)4, (uint64_t)min, 0, 0, 0, 0);
}

void sys_get_seconds(int *sec) {
  sys_int_80((uint64_t)5, (uint64_t)sec, 0, 0, 0, 0);
}

void sys_new_line() { sys_int_80((uint64_t)6, 0, 0, 0, 0, 0); }

void sys_write_dec(int c, int color) {
  sys_int_80((uint64_t)7, (uint64_t)c, color, 0, 0, 0);
}

void sys_beep(int freq, int time) {
  sys_int_80((uint64_t)8, (uint64_t)freq, (uint64_t)time, 0, 0, 0);
}

void sys_sleep(int ms) { sys_int_80((uint64_t)9, (uint64_t)ms, 0, 0, 0, 0); }

void sys_put_pixel(uint32_t color, uint32_t x, uint32_t y) {
  sys_int_80((uint64_t)10, (uint64_t)color, (uint64_t)x, (uint64_t)y, 0, 0);
}

void sys_get_screen_width(int *width) {
  sys_int_80((uint64_t)11, (uint64_t)width, 0, 0, 0, 0);
}

void sys_get_clean_buffer() { sys_int_80((uint64_t)12, 0, 0, 0, 0, 0); }

void sys_registers(uint64_t *registers, bool *isSaved) {
  sys_int_80((uint64_t)13, (uint64_t)registers, (uint64_t)isSaved, 0, 0, 0);
}

void sys_alloc(uint64_t **memory, int size) {
  sys_int_80((uint64_t)14, (uint64_t)memory, (uint64_t)size, 0, 0, 0);
}

void sys_create_process(void (*process)(unsigned int argc, char **argv),
                        int argc, char **argv, int foreground, int *fd) {
  sys_int_80((uint64_t)15, (uint64_t)process, (uint64_t)argc, (uint64_t)argv,
             (uint64_t)foreground, (uint64_t)fd);
}

void sys_kill_process(int pid) {
  sys_int_80((uint64_t)16, (uint64_t)pid, 0, 0, 0, 0);
}

void sys_get_pid() { sys_int_80((uint64_t)17, 0, 0, 0, 0, 0); }

void sys_get_info_processes(char *buffer) {
  sys_int_80((uint64_t)18, (uint64_t)buffer, 0, 0, 0, 0);
}

void sys_nice(int pid, int priority) {
  sys_int_80((uint64_t)19, (uint64_t)pid, (uint64_t)priority, 0, 0, 0);
}

void sys_block_process(int pid) {
  sys_int_80((uint64_t)20, (uint64_t)pid, 0, 0, 0, 0);
}

void sys_unblock_process(int pid) {
  sys_int_80((uint64_t)21, (uint64_t)pid, 0, 0, 0, 0);
}

void sys_giveup_cpu() { sys_int_80((uint64_t)22, 0, 0, 0, 0, 0); 

}

void sys_semWait(int id){
    sys_int_80((uint64_t)23, (uint64_t)id, 0, 0, 0, 0);
}

void sys_semPost(int id){
    sys_int_80((uint64_t)24, (uint64_t)id, 0, 0, 0, 0);
}

void sys_semClose(int id){
    sys_int_80((uint64_t)26, (uint64_t)id, 0, 0, 0, 0);
}

void sys_semOpen(char *id, int initialValue){
    sys_int_80((uint64_t)25, (uint64_t)id, (uint64_t)initialValue, 0, 0, 0);
}

void sys_sem(char *id, int initialValue){
    sys_int_80((uint64_t)27,0,0, 0, 0, 0);
}


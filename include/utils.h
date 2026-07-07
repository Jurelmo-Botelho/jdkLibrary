#ifndef UTILS_H
#define UTILS_H

int safe_read_int(int *value);
int safe_read_string(char *buffer, int size);
int confirm_action(const char *message);

#endif
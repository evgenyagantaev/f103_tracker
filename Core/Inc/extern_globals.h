#ifndef INC_EXTERN_GLOBALS_H_
#define INC_EXTERN_GLOBALS_H_

#include "main.h"

extern char usart1_buffer[2][USART1_BUFFER_LENGTH];
extern int usart1_write_buffer;
extern int usart1_read_buffer;
extern int usart1_write_index;
extern int usart1_new_message_ready_flag;
extern int usart1_message_length;
extern int usart1_old_message_saved;
extern int usart1_message_lost;
extern int usart1_received_messages;
extern int usart1_processed_messages;

#endif /* INC_EXTERN_GLOBALS_H_ */

#include "usart2_buffer_obj.h"
#include "usart2_buffer_interface.h"

void usart2_buffer_obj_write_reset()
{
	usart2_buffer_write_index = 0;
}

void usart2_buffer_obj_read_reset()
{
	usart2_buffer_read_index = 0;
}

void usart2_buffer_obj_write(char data)
{

	if(data == '\n')

	if(usart2_buffer_write_index < USART2_BUFFER_LENGTH)
	{
		usart2_buffer[usart2_buffer_write_index] = data;

		usart2_buffer_write_index++;
	}
}

char usart2_buffer_obj_read()
{
	char return_data = usart2_buffer[usart2_buffer_read_index];
	usart2_buffer_read_index++;

	return return_data;
}

char usart2_buffer_obj_read_last()
{
	char return_data = usart2_buffer[(usart2_buffer_write_index - 1)];

	return return_data;
}

void usart2_new_byte_flag_set()
{
	usart2_new_byte_flag = 1;
}
void usart2_new_byte_flag_reset()
{
	usart2_new_byte_flag = 0;
}
int usart2_new_byte_flag_get()
{
	return usart2_new_byte_flag;
}

void usart2_object_init()
{
	usart2_new_byte_flag_reset();
	usart2_buffer_obj_write_reset();
	usart2_buffer_obj_read_reset();
}


void usart2_buffer_action()
{
	if(usart2_new_byte_flag_get())
	{
		usart2_new_byte_flag_reset();


	}
}

void usart2_buffer_get_message(char *message_buffer)
{
	if(usart2_buffer_write_index < USART2_BUFFER_LENGTH)
	{
		usart2_buffer[usart2_buffer_write_index] = 0;

		int i;

		for(i=0; i<=usart2_buffer_write_index; i++)
			message_buffer[i] = usart2_buffer[i];

		usart2_buffer_obj_write_reset();
	}
}


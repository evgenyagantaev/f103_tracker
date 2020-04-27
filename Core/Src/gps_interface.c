#include "gps_obj.h"
#include "gps_interface.h"
#include "usart1_buffer_interface.h"

#include "ssd1306.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"

void gps_new_message_received_flag_set()
{
	gps_new_message_received_flag = 1;
}


void gps_new_message_received_flag_reset()
{
	gps_new_message_received_flag = 0;
}
int gps_new_message_received_flag_get()
{
	return gps_new_message_received_flag;
}

void gps_object_init()
{
	gps_new_message_received_flag_reset();
}


void gps_action()
{
	if(gps_new_message_received_flag_get())
	{
		gps_new_message_received_flag_reset();

		usart1_buffer_get_message(gps_message);

		if(strstr(gps_message, "GNRMC") != NULL)
		{

			if(strstr(gps_message, "A") != NULL)
			{
				int i = 0;
				int no_stop_condition = 1;
				while(no_stop_condition)
				{
					aux_message[i] = gps_message[49 + i];
					if(gps_message[49 + i] == ',')
					{
						aux_message[i] = 0;
						no_stop_condition = 0;
					}
					i++;
				}

				ssd1306_SetCursor(0,0);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,0);
				ssd1306_WriteString(aux_message, Font_11x18, White);
				ssd1306_UpdateScreen();

				strncpy(aux_message, &(gps_message[20]), 11);
				aux_message[11] = 0;

				ssd1306_SetCursor(0,22);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,22);
				ssd1306_WriteString(aux_message, Font_11x18, White);
				ssd1306_UpdateScreen();

				strncpy(aux_message, &(gps_message[35]), 11);
				aux_message[11] = 0;

				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString(aux_message, Font_11x18, White);
				ssd1306_UpdateScreen();
			}
			else
			{
				sprintf(aux_message, "no data");
				ssd1306_SetCursor(0,22);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,22);
				ssd1306_WriteString(aux_message, Font_11x18, White);
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_UpdateScreen();
			}

		}

	}


}


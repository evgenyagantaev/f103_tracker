#include "gps_obj.h"
#include "gps_interface.h"
#include "usart1_buffer_interface.h"

#include "ssd1306.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"

#include "extern_globals.h"

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
	gps_odd_even = 0;
}


void gps_action()
{
	if(usart1_message_lost)
	{
		ssd1306_SetCursor(0,0);
		ssd1306_WriteString("message lst", Font_11x18, White);
		ssd1306_UpdateScreen();

		// stop
		while(1);
	}

	if(usart1_new_message_ready_flag)
	{
		usart1_new_message_ready_flag = 0;
		usart1_processed_messages++;

		// copy message
		usart1_old_message_saved = 0;
		int i;
		for(i=0; i<=usart1_message_length; i++)
			gps_message[i] = usart1_buffer[usart1_read_buffer][i];
		usart1_old_message_saved = 1;


		if(gps_message[1]=='G' && gps_message[2]=='N' && gps_message[3]=='R' && gps_message[4]=='M' && gps_message[5]=='C')
		{
			gps_odd_even = (gps_odd_even + 1)%2;

			if(gps_message[18]=='A')
			{
				// read velocity
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

				/*
				int intpart;
				sscanf(aux_message, "%d.", &intpart);
				intpart = (int)((float)intpart*1.852);
				sprintf(aux_message, "%3d", intpart);
				*/
				aux_message[3] = 0;
				// write velocity
				ssd1306_SetCursor(0,0);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,0);
				ssd1306_WriteString(aux_message, Font_11x18, White);

				// read heading
				int start_position = i;
				no_stop_condition = 1;
				while(no_stop_condition)
				{
					aux_message[i - start_position] = gps_message[49 + i];
					if(gps_message[49 + i] == ',')
					{
						aux_message[i] = 0;
						no_stop_condition = 0;
					}
					i++;
				}

				/*
				sscanf(aux_message, "%d.", &intpart);
				intpart = (int)((float)intpart*1.852);
				sprintf(aux_message, "%3d", intpart);
				*/
				aux_message[3] = 0;
				// write heading
				ssd1306_SetCursor(55,0);
				ssd1306_WriteString(aux_message, Font_11x18, White);


				/*
				// read altitude
				strncpy(aux_message, &(gps_message[20]), 11);
				aux_message[11] = 0;

				ssd1306_SetCursor(0,22);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,22);
				ssd1306_WriteString(aux_message, Font_11x18, White);

				// read lattitude
				strncpy(aux_message, &(gps_message[35]), 11);
				aux_message[11] = 0;

				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString(aux_message, Font_11x18, White);
				*/
				ssd1306_UpdateScreen();

			}
			else
			{
				/*
				ssd1306_SetCursor(0,0);
				ssd1306_WriteString("          ", Font_11x18, White);
				ssd1306_SetCursor(0,22);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_SetCursor(0,22);
				if(gps_odd_even)
					ssd1306_WriteString("no data    ", Font_11x18, White);
				else
					ssd1306_WriteString("NO DATA    ", Font_11x18, White);
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_UpdateScreen();
				*/
			}

			sprintf(aux_message, "%5d %5d", usart1_received_messages, usart1_processed_messages);
			ssd1306_SetCursor(0,44);
			ssd1306_WriteString(aux_message, Font_11x18, White);
			ssd1306_UpdateScreen();

		}

	}// end if(usart1_new_message_ready_flag)


}


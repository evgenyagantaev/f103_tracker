#ifndef GPS_INTERFACE_H
#define GPS_INTERFACE_H

void gps_object_init();

void gps_new_message_received_flag_set();
void gps_new_message_received_flag_reset();
int gps_new_message_received_flag_get();

void gps_action();

#endif  // GPS_INTERFACE_H

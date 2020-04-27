#ifndef GPS_OBJ_H
#define GPS_OBJ_H

#include "main.h"

#define GPS_MESSAGE_LENGTH 128

static char gps_message[GPS_MESSAGE_LENGTH];

static int gps_new_message_received_flag;

static char aux_message[64];

#endif // GPS_OBJ_H

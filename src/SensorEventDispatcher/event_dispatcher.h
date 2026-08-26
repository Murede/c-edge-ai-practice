#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <stdint.h> 

// Events 
typedef enum
{
    SENSOR_EVENT_READY = 0,
    SENSOR_EVENT_ERROR,
    SENSOR_EVENT_TIMEOUT,
    SENSOR_EVENT_COUNT
} SensorEvent;

typedef enum 
{
    DISPATCHER_OK = 0,
    DISPATCHER_ERROR_INVALID_EVENT,
    DISPATCHER_ERROR_NULL_CALLBACK,
    DISPATCHER_ERROR_NOT_REGISTERED

} DispatcherError; 

// Callback Table Registration 
typedef void (*SensorCallback)(
    uint16_t value
);



DispatcherError event_register_callback(
    SensorEvent event,
    SensorCallback callback 
);

DispatcherError event_unregister_callback(
    SensorEvent event 
);

DispatcherError event_dispatch(
    SensorEvent event,
    uint16_t value
);

const char *dispatcher_error_to_string(
    DispatcherError error
);

#endif 
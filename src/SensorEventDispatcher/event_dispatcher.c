#include "event_dispatcher.h"

/*Private Callback Table*/
static SensorCallback callbacks[SENSOR_EVENT_COUNT] = {NULL};

/*Register Callback*/
DispatcherError event_register_callback(
    SensorEvent event,
    SensorCallback callback)
{
    if (event >= SENSOR_EVENT_COUNT)
    {
        return DISPATCHER_ERROR_INVALID_EVENT;
    }

    if (callback == NULL)
    {
        return DISPATCHER_ERROR_NULL_CALLBACK;
    }

    callbacks[event] = callback;

    return DISPATCHER_OK;
}

/*Unregister Callback*/
DispatcherError event_unregister_callback(
    SensorEvent event)
{
    if (event >= SENSOR_EVENT_COUNT)
    {
        return DISPATCHER_ERROR_INVALID_EVENT;
    }

    if (callbacks[event] == NULL)
    {
        return DISPATCHER_ERROR_NOT_REGISTERED;
    }

    callbacks[event] = NULL;

    return DISPATCHER_OK;
}

DispatcherError event_dispatch(
    SensorEvent event,
    uint16_t value)
{
    if (event >= SENSOR_EVENT_COUNT)
    {
        return DISPATCHER_ERROR_INVALID_EVENT;
    }

    if (callbacks[event] == NULL)
    {
        return DISPATCHER_ERROR_NOT_REGISTERED;
    }

    callbacks[event](value);

    return DISPATCHER_OK;
}

/*
 * Convert a DispatcherError value into a human-readable string.
 * Useful for debugging and displaying dispatcher operation results.
 */
const char *dispatcher_error_to_string(
    DispatcherError error)
{
    switch (error)
    {
    case DISPATCHER_OK:
        return "DISPATCHER_OK";

    case DISPATCHER_ERROR_INVALID_EVENT:
        return "DISPATCHER_ERROR_INVALID_EVENT";

    case DISPATCHER_ERROR_NULL_CALLBACK:
        return "DISPATCHER_ERROR_NULL_CALLBACK";

    case DISPATCHER_ERROR_NOT_REGISTERED:
        return "DISPATCHER_ERROR_NOT_REGISTERED";

    default:
        return "DISPATCHER_ERROR_UNKNOWN";
    }
}
#include <stdio.h>
#include "event_dispatcher.h"

void handle_ready(uint16_t value)
{
    printf(
        "Sensor READY: %u\n",
        (unsigned int)value
    );
} 

void handle_error(uint16_t value)
{
    printf(
        "Sensor ERROR: %u\n",
        (unsigned int)value
    );
}

void handle_timeout(uint16_t value)
{
    printf(
        "Sensor TIMEOUT: %u\n",
        (unsigned int)value
    );
}

// Alternate READY handler used to prove callback replacement.
void handle_ready_alternate(uint16_t value)
{
    printf(
        "Alternate READY handler: %u\n",
        (unsigned int)value
    );
}

int main(void)
{
    DispatcherError result; 

    // Register READY callback 
    result = event_register_callback(SENSOR_EVENT_READY, 
        handle_ready
    );

    // Print READY callback Result 
    printf(
        "Register READY: %s\n",
        dispatcher_error_to_string(result)
    );

    // Register Error callback
    result = event_register_callback(SENSOR_EVENT_ERROR, 
        handle_error
    );

    // Print ERROR callback Result 
    printf(
        "Register ERROR: %s\n",
        dispatcher_error_to_string(result)
    );

    // Register TIMEOUT callback
    result = event_register_callback(SENSOR_EVENT_TIMEOUT, 
        handle_timeout
    );

    // Print TIMEOUT callback Result 
    printf(
        "Register TIMEOUT: %s\n",
        dispatcher_error_to_string(result)
    );

    // =====================================================
    // Phase 2: Dispatch Registered Events
    // =====================================================

    // Dispatch READY event
    result = event_dispatch(
        SENSOR_EVENT_READY,
        2048u
    );

    printf(
        "Dispatch READY result: %s\n",
        dispatcher_error_to_string(result)
    );


    // Dispatch ERROR event
    result = event_dispatch(
        SENSOR_EVENT_ERROR,
        5u
    );

    printf(
        "Dispatch ERROR result: %s\n",
        dispatcher_error_to_string(result)
    );


    // Dispatch TIMEOUT event
    result = event_dispatch(
        SENSOR_EVENT_TIMEOUT,
        1000u
    );

    printf(
        "Dispatch TIMEOUT result: %s\n",
        dispatcher_error_to_string(result)
    );

    // =====================================================
    // Phase 3: Callback Replacement and Unregistering
    // =====================================================
    //
    // This phase proves that:
    // 1. Registering another handler for the same event replaces
    //    the previously stored function pointer.
    // 2. Unregistering an event clears its callback slot.
    // 3. Dispatching an unregistered event fails safely.


    // Replace READY callback.
    result = event_register_callback(
        SENSOR_EVENT_READY,
        handle_ready_alternate
    );

    printf(
        "Replace READY callback: %s\n",
        dispatcher_error_to_string(result)
    );


    // Dispatch READY again.
    // This should now call handle_ready_alternate() instead
    // of the original handle_ready().
    result = event_dispatch(
        SENSOR_EVENT_READY,
        3000u
    );

    printf(
        "Dispatch replaced READY: %s\n",
        dispatcher_error_to_string(result)
    );


    // Unregister the ERROR callback.
    result = event_unregister_callback(
        SENSOR_EVENT_ERROR
    );

    printf(
        "Unregister ERROR: %s\n",
        dispatcher_error_to_string(result)
    );


    // Attempt to dispatch ERROR after its callback was removed.
    // The dispatcher should detect the NULL callback slot and
    // return DISPATCHER_ERROR_NOT_REGISTERED.
    result = event_dispatch(
        SENSOR_EVENT_ERROR,
        7u
    );

    printf(
        "Dispatch unregistered ERROR: %s\n",
        dispatcher_error_to_string(result)
    );

    // =====================================================
    // Phase 4: Defensive / Invalid Input Testing
    // =====================================================
    //
    // Deliberately provide invalid inputs to verify that the
    // dispatcher rejects them instead of indexing outside the
    // callback table or calling invalid function pointers.


    // -----------------------------------------------------
    // NULL callback registration
    // -----------------------------------------------------
    //
    // READY is a valid event, but NULL is not a valid handler
    // to register.

    result = event_register_callback(
        SENSOR_EVENT_READY,
        NULL
    );

    printf(
        "NULL callback test: %s\n",
        dispatcher_error_to_string(result)
    );


    // -----------------------------------------------------
    // Invalid event registration
    // -----------------------------------------------------
    //
    // SENSOR_EVENT_COUNT represents the number of valid events.
    // It is therefore one position past the final valid event
    // and should be rejected.

    result = event_register_callback(
        SENSOR_EVENT_COUNT,
        handle_ready
    );

    printf(
        "Invalid registration event: %s\n",
        dispatcher_error_to_string(result)
    );


    // -----------------------------------------------------
    // Invalid event dispatch
    // -----------------------------------------------------

    result = event_dispatch(
        SENSOR_EVENT_COUNT,
        123u
    );

    printf(
        "Invalid dispatch event: %s\n",
        dispatcher_error_to_string(result)
    );


    // -----------------------------------------------------
    // Invalid event unregister
    // -----------------------------------------------------

    result = event_unregister_callback(
        SENSOR_EVENT_COUNT
    );

    printf(
        "Invalid unregister event: %s\n",
        dispatcher_error_to_string(result)
    );


    // -----------------------------------------------------
    // Unregister an already-unregistered callback
    // -----------------------------------------------------
    //
    // ERROR was removed during Phase 3, so attempting to remove
    // it again should return DISPATCHER_ERROR_NOT_REGISTERED.

    result = event_unregister_callback(
        SENSOR_EVENT_ERROR
    );

    printf(
        "Unregister ERROR again: %s\n",
        dispatcher_error_to_string(result)
    );

    return 0; 
}
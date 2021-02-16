
#include <stdint.h>

extern void midi_control_event_callback( uint8_t controller, uint8_t value);

void midi_control_process_byte( uint8_t recv)
{
    static uint8_t state = 0;
    static uint8_t counter = 0;
    static uint8_t controller = 0;

    if ( recv&0x80 )
    {
        state = recv;
        counter = 0;
    }
    else
    {
        counter++;
        if (counter&0x01)
        {
            controller = recv;
        }
        else
        {
            if ((state&0xf0) == 0xb0)
            {
                switch(controller)
                {
                case 91: midi_control_event_callback(0x0,recv); break;
                case 93: midi_control_event_callback(0x1,recv); break;
                case  5: midi_control_event_callback(0x2,recv); break;
                case 10: midi_control_event_callback(0x3,recv); break;
                case 71: midi_control_event_callback(0x4,recv); break;
                case 74: midi_control_event_callback(0x5,recv); break;
                case 84: midi_control_event_callback(0x6,recv); break;
                case  7: midi_control_event_callback(0x7,recv); break;
                case  1: midi_control_event_callback(0x8,recv); break;
                default: break;
                }
            }
        }

    }

}

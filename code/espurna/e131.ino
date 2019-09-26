#ifdef _E131_H
/*

E1.31 Streaming ACN Plugin
Adam Honse (CalcProgrammer1) 2019

*/

//*---------------------------------------------------------------------------
//* E1.31 includes
//*---------------------------------------------------------------------------

#include <ESPAsyncE131.h>
//#include <vector>

//typedef struct {
//    unsigned int channel;
//    e131_callback_f fn = NULL;
//} e131_callback_t;
//std::vector<e131_callback_t> _callbacks;

//*----------------------------------------------------------------------------
//* Private
//*----------------------------------------------------------------------------
//*----------------------------------------------------------------------------
//* E1.31 globals
//*----------------------------------------------------------------------------
bool _e131_enabled = false;
bool _e131_initialized = false;

ESPAsyncE131        e131(2);       // ESPAsyncE131 with X buffers

//*----------------------------------------------------------------------------
//* E1.31 setup
//*----------------------------------------------------------------------------
void E131Setup() {
    _e131_enabled = 1;//getSetting("E131_EN", E131_ENABLE).toInt() == 1;
    _e131_initialized = 0;
    //* Register plugin loop to espurna main loop
    //*------------------------------------------
    espurnaRegisterLoop(_E131Loop);

    DEBUG_MSG_P(PSTR("[E131] E131 setup code finished \n"));
}
//* end of plugin setup
//*----------------------------------------------------------------------------

//void e131Register(e131_callback_f e131_function, unsigned int e131_channel)
//{
//e131_callback_t cb;
//
//cb.channel = e131_channel;
//cb.fn = e131_function;
//
//_callbacks.push_back(cb);
//}

//*----------------------------------------------------------------------------
//* E131 loop
//*----------------------------------------------------------------------------
void _E131Loop() {

    //* if plugin disabled dont run the coded
    if (_e131_enabled != 1) return;

    //* Initializing multicast mode must be done when the WiFi is connected, so
    //* set a flag to track when WiFi is connected and disconnected
    if (WiFi.status() == WL_CONNECTED) {
        if(_e131_initialized == 0) {
            e131.begin(E131_MULTICAST, 1, 1);
            _e131_initialized = 1;
        }
    }
    else {
        _e131_initialized = 0;
    }

    if(!e131.isEmpty())
    {
        e131_packet_t pkt;
        e131.pull(&pkt);

        lightChannel(0, pkt.property_values[1]);
        lightChannel(1, pkt.property_values[2]);
        lightChannel(2, pkt.property_values[3]);
        lightChannel(3, pkt.property_values[4]);
        lightUpdate(false, false, false);
    }

}

bool E131Enabled() {
    return _e131_enabled;
}
#endif  //#ifdef _E131_H

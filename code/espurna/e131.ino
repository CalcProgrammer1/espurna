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
bool         _e131_enabled         = false;
bool         _e131_multicast       = false;
unsigned int _e131_universe        = 1;
bool         _e131_light_0_enabled = false;
unsigned int _e131_light_0_channel = 1;
bool         _e131_light_1_enabled = false;
unsigned int _e131_light_1_channel = 2;
bool         _e131_light_2_enabled = false;
unsigned int _e131_light_2_channel = 3;
bool         _e131_light_3_enabled = false;
unsigned int _e131_light_3_channel = 4;
bool         _e131_light_4_enabled = false;
unsigned int _e131_light_4_channel = 5;
bool         _e131_initialized     = false;

ESPAsyncE131        e131(2);       // ESPAsyncE131 with X buffers

#if WEB_SUPPORT

bool _e131WebSocketOnKeyCheck(const char * key, JsonVariant& value) {
    return (strncmp(key, "e131", 4) == 0);
}

void _e131WebSocketOnVisible(JsonObject& root) {
    root["e131Visible"] = 1;
}

void _e131WebSocketOnData(JsonObject& root) {

}

void _e131WebSocketOnConnected(JsonObject& root) {
    root["e131Enabled"]         = getSetting("e131Enabled",       0);
    root["e131Multicast"]       = getSetting("e131Multicast",     0);
    root["e131Universe"]        = getSetting("e131Universe",      1);
    root["e131Light0Enabled"]   = getSetting("e131Light0Enabled", 0);
    root["e131Light0Channel"]   = getSetting("e131Light0Channel", 1);
    root["e131Light1Enabled"]   = getSetting("e131Light1Enabled", 0);
    root["e131Light1Channel"]   = getSetting("e131Light1Channel", 2);
    root["e131Light2Enabled"]   = getSetting("e131Light2Enabled", 0);
    root["e131Light2Channel"]   = getSetting("e131Light2Channel", 3);
    root["e131Light3Enabled"]   = getSetting("e131Light3Enabled", 0);
    root["e131Light3Channel"]   = getSetting("e131Light3Channel", 4);
    root["e131Light4Enabled"]   = getSetting("e131Light4Enabled", 0);
    root["e131Light4Channel"]   = getSetting("e131Light4Channel", 5);
}

#endif

//*----------------------------------------------------------------------------
//* E1.31 setup
//*----------------------------------------------------------------------------
void E131Setup() {
    _e131_initialized = 0;


    _e131_enabled               = getSetting("e131Enabled",       0).toInt() == 1;
    _e131_multicast             = getSetting("e131Multicast",     0).toInt() == 1;
    _e131_universe              = getSetting("e131Universe",      1).toInt();
    _e131_light_0_enabled       = getSetting("e131Light0Enabled", 0).toInt() == 1;
    _e131_light_0_channel       = getSetting("e131Light0Channel", 1).toInt();
    _e131_light_1_enabled       = getSetting("e131Light1Enabled", 0).toInt() == 1;
    _e131_light_1_channel       = getSetting("e131Light1Channel", 2).toInt();
    _e131_light_2_enabled       = getSetting("e131Light2Enabled", 0).toInt() == 1;
    _e131_light_2_channel       = getSetting("e131Light2Channel", 3).toInt();
    _e131_light_3_enabled       = getSetting("e131Light3Enabled", 0).toInt() == 1;
    _e131_light_3_channel       = getSetting("e131Light3Channel", 4).toInt();
    _e131_light_4_enabled       = getSetting("e131Light4Enabled", 0).toInt() == 1;
    _e131_light_4_channel       = getSetting("e131Light4Channel", 5).toInt();

    //* Register plugin loop to espurna main loop
    //*------------------------------------------
    espurnaRegisterLoop(_E131Loop);

    #if WEB_SUPPORT
        wsRegister()
            .onVisible(_e131WebSocketOnVisible)
            .onConnected(_e131WebSocketOnConnected)
            .onData(_e131WebSocketOnData)
            .onKeyCheck(_e131WebSocketOnKeyCheck);
    #endif

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
            if(_e131_multicast) {
                e131.begin(E131_MULTICAST, _e131_universe, 1);
            }
            else {
                e131.begin(E131_UNICAST);
            }
            
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

        if(_e131_light_0_enabled) {
            lightChannel(0, pkt.property_values[_e131_light_0_channel]);
        }

        if(_e131_light_1_enabled) {
            lightChannel(1, pkt.property_values[_e131_light_1_channel]);
        }

        if(_e131_light_2_enabled) {
            lightChannel(2, pkt.property_values[_e131_light_2_channel]);
        }

        if(_e131_light_3_enabled) {
            lightChannel(3, pkt.property_values[_e131_light_3_channel]);
        }

        if(_e131_light_4_enabled) {
            lightChannel(4, pkt.property_values[_e131_light_4_channel]);
        }

        lightUpdate(false, false, false);
    }

}

bool E131Enabled() {
    return _e131_enabled;
}
#endif  //#ifdef _E131_H

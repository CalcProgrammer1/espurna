
#ifdef _E131_H
/*

E1.31 Streaming ACN Plugin
GPL V2 - Adam Honse (CalcProgrammer1) 2019

*/

//*for better readability and usage of this template file the writing style is:
//* (//*) for comments
//* (//) for commented code (just delete the // to activate)
//* This plugin templat allows web interface provisioning the fronand
//* HTML, JS, CSS needs to be coded into the original core (on current design)

//*---------------------------------------------------------------------------
//* Plugin includes
//*---------------------------------------------------------------------------

#include <ESPAsyncE131.h>

//*----------------------------------------------------------------------------
//* Private
//*----------------------------------------------------------------------------
//*----------------------------------------------------------------------------
//* Plugin globals
//*----------------------------------------------------------------------------
bool _e131_enabled = false;

ESPAsyncE131        e131(1);       // ESPAsyncE131 with X buffers

//*----------------------------------------------------------------------------
//* Plugin helper functions
//*----------------------------------------------------------------------------

//*----------------------------------------------------------------------------
//* Plugin setup
//*----------------------------------------------------------------------------
void E131Setup() {
    _e131_enabled = 1;//getSetting("E131_EN", E131_ENABLE).toInt() == 1;

    //* Register plugin loop to espurna main loop
    //*------------------------------------------
    espurnaRegisterLoop(_E131Loop);

    //* Set up E131 for multicast
    e131.begin(E131_UNICAST);

    DEBUG_MSG_P(PSTR("[E131] E131 setup code finished \n"));
}
//* end of plugin setup
//*----------------------------------------------------------------------------

//*----------------------------------------------------------------------------
//* E131 loop
//*----------------------------------------------------------------------------
void _E131Loop() {

    //* if plugin disabled dont run the coded
    if (_e131_enabled != 1) return;

    if(!e131.isEmpty())
    {
        e131_packet_t pkt;
        e131.pull(&pkt);

        lightChannel(0, pkt.property_values[1]);
        lightChannel(1, pkt.property_values[2]);
        lightChannel(2, pkt.property_values[3]);
        lightChannel(3, pkt.property_values[4]);
        lightUpdate(true, true);
    }

}

//*----------------------------------------------------------------------------
//* Public - Plugin API
//*----------------------------------------------------------------------------
//* Here goes ublic plugin API definitions and coded
//* currently plugin enabled query implemented
//* use this as template to create additionl API calls
//* to Set/Get internal plugin data or use plugin functions
bool E131Enabled() {
    return _e131_enabled;
}
#endif  //#ifdef _E131_H

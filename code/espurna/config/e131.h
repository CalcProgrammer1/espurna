#ifndef _E131_H
#define _E131_H

using e131_callback_f = std::function<void(unsigned char)>;

//------------------------------------------------------------
//* Plugin public interface
//------------------------------------------------------------
//* declare the plugin setup function (used by custom.h)
void E131Setup();
//* get plugin enabled state
bool E131Enabled();

#endif
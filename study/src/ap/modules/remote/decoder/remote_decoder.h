#ifndef MODULE_REMOTE_DECODER_H_
#define MODULE_REMOTE_DECODER_H_

#include "../common/rf_def.c"

remote_type_t decodeRemotesType(uint32_t raw);
uint32_t decodeRemotesAddress(uint32_t raw);
remote_btn_t decodeRemotesButton(uint32_t raw);

#endif
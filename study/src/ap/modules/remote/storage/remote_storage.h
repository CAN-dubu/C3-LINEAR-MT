#ifndef AP_MODULE_MANAGER__H_
#define AP_MODULE_MANAGER__H_

#include <Preferences.h>
#include "../../ap_def.h"
#include "../common/rf_def.c"
#include "../decoder/remote_decoder.h"

bool remoteStorageSave(uint32_t raw);
void remoteStorageDeleteAll(uint8_t max_slot);
bool remoteInfoContained(uint32_t address);
#endif
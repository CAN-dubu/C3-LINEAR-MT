#ifndef REMOTE_STORAGE_H_ 
#define REMOTE_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../ap_def.h"
#include "../common/rf_def.h"
#include "../decoder/remote_decoder.h"

bool remoteStorageSave(uint32_t raw);
void remoteStorageDeleteAll(void);
bool remoteInfoContained(uint32_t address);

#ifdef __cplusplus
}
#endif

#endif
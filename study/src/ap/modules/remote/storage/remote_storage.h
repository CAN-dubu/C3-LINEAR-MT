#ifndef REMOTE_STORAGE_H_ 
#define REMOTE_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../ap_def.h"
#include "../common/rf_def.h"
#include "../decoder/remote_decoder.h"

typedef enum
{
  REMOTE_STORAGE_OK = 0,
  REMOTE_STORAGE_ERR_INVALID_DATA,
  REMOTE_STORAGE_ERR_ALREADY_EXISTS,
  REMOTE_STORAGE_ERR_FULL,
  REMOTE_STORAGE_ERR_FLASH_ACCESS,
} remote_storage_error_t;

remote_storage_error_t remoteStorageSave(uint32_t raw);
void remoteStorageDeleteAll(void);
bool remoteInfoContained(uint32_t address);
int getStoredRemoteNum(void);

#ifdef __cplusplus
}
#endif

#endif
#include <Preferences.h>
#include "remote_storage.h"

static bool remoteStorageGetSlot(uint8_t slot);
static bool remoteStorageDelete(uint8_t slot);
static void remoteSetNamespace(char *buf, uint8_t slot);
static uint16_t crc16_ccitt(uint8_t *data, uint32_t len);
static int remoteStorageGetNextSlot(void);

static Preferences prefs;

/**
 * @brief flash에 리모컨 정보를 저장하는 함수 (빈 공간에 명칭, 주소, crc를 저장)
 */
remote_storage_error_t remoteStorageSave(uint32_t raw)
{
  uint32_t address;
  int slot;
  char remote_name_space[16];
  
  if (raw == 0)
  {
    return REMOTE_STORAGE_ERR_INVALID_DATA;
  }

  address = decodeRemotesAddress(raw);
  
  if (remoteInfoContained(address))
  {
    return REMOTE_STORAGE_ERR_ALREADY_EXISTS;
  }

  slot = remoteStorageGetNextSlot();

  if (slot < 0)
  {
    return REMOTE_STORAGE_ERR_FULL; // FULL
  } 

  remoteSetNamespace(remote_name_space, (uint8_t)slot);

  if (!prefs.begin(remote_name_space, false)) // 읽기, 쓰기 전용으로 prefs열기
  {
    return REMOTE_STORAGE_ERR_FLASH_ACCESS;
  }
    
  uint16_t crc = crc16_ccitt((uint8_t *)&address, sizeof(address));

  prefs.putUInt("address", address); // key,value쌍으로 key에 "address"란 문자열을 넣음
  prefs.putUShort("crc", crc);
  
  prefs.end(); // prefs 닫기

  return REMOTE_STORAGE_OK;
}

/**
 * @brief 공장 초기화 함수
 */
void remoteStorageDeleteAll(void)
{
  for (int i=1; i<=MAX_REMOTE_CONTROLLER_NUM; i++)
  {
    remoteStorageDelete(i);
  }
}

bool remoteInfoContained(uint32_t raw) // @@수정필 -> raw데이터로 받아야함.
{
  if (raw == 0)
  {
    return false;
  }

  uint32_t address = decodeRemotesAddress(raw);

  char name_space[16];

  for (int i = 0; i < MAX_REMOTE_CONTROLLER_NUM; i++)
  {
    remoteSetNamespace(name_space, i);

    if (!prefs.begin(name_space, true)) // read-only
    {
      continue;
    }

    uint32_t stored_address = prefs.getUInt("address", 0);
    uint16_t stored_crc     = prefs.getUShort("crc", 0);

    prefs.end();

    if (stored_address == 0)
    {
      continue; // empty slot
    }

    uint16_t calc_crc =
      crc16_ccitt((uint8_t *)&stored_address, sizeof(stored_address));

    if (calc_crc != stored_crc)
    {
      continue; // 데이터 깨짐
    }

    if (stored_address == address)
    {
      return true; // FOUND
    }
  }

  return false;
}

int getStoredRemoteNum(void)
{
  return remoteStorageGetNextSlot() - 1;
}

/**
 * @brief static 함수들
 */
static bool remoteStorageGetSlot(uint8_t slot)
{
  char name_space[16];
  remoteSetNamespace(name_space, slot);

  if (!prefs.begin(name_space, true))
  {
    return false;
  }

  uint32_t address = prefs.getUInt("address", 0); // 저장된 address값 반환, 없다면 0 반환(두번째 인자)
  uint16_t crc_stored = prefs.getUShort("crc", 0);

  prefs.end();

  if (address == 0) // 비어있다면
  {
    return false; // false 반환
  }

  uint16_t crc_calc = 
    crc16_ccitt((uint8_t *)&address, sizeof(address));

  return (crc_calc == crc_stored); // 저장된 데이터가 깨져있다면 false 
}

static int remoteStorageGetNextSlot(void)
{
  for (int i = 0; i < MAX_REMOTE_CONTROLLER_NUM; i++)
  {
    if (remoteStorageGetSlot(i) == false)
    {
      return i;
    }
  }
  return -1; // FULL
}

static bool remoteStorageDelete(uint8_t slot)
{
  char name_space[16];
  remoteSetNamespace(name_space, slot);

  if (!prefs.begin(name_space, false))
  {
    return false;
  }
    
  prefs.clear();
  prefs.end();
  
  return true;
}

static void remoteSetNamespace(char *buf, uint8_t slot)
{
  sprintf(buf, "remote_%d", slot);
}

static uint16_t crc16_ccitt(uint8_t *data, uint32_t len)
{
  uint16_t crc = 0xFFFF;
  uint8_t *ptr = data;
  
  while (len--)
  {
    crc ^= *ptr++ << 8;
    for (uint8_t i = 0; i < 8; ++i)
    {
      if (crc & 0x8000)
        crc = (crc << 1) ^ 0x1021;
      else
        crc = crc << 1;
    }
  }
  return crc;
}

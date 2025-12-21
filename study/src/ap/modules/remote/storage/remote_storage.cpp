#include "remote_manager.h"


static Preferences prefs;

/**
 * @brief flash에 리모컨 정보를 저장하는 함수 (빈 공간에 명칭, 주소, crc를 저장)
 */
bool remoteStorageSave(uint32_t raw)
{
  uint32_t address;
  uint8_t slot;
  char remote_name_space[16];
  
  if (raw == 0)
  {
    return false;
  }

  address = decodeRemotesAddress(raw);
  
  if (remoteInfoContained(address))
  {
    return false;
  }

  slot = remoteStorageGetNextSlot();

  if (slot < 0)
  {
    return false; // FULL
  }

  makeNamespace(remote_name_space, slot);

  if (!prefs.begin(remote_name_space, false)) // 읽기, 쓰기 전용으로 prefs열기
  {
    return false;
  }
    
  uint16_t crc = crc16_ccitt((uint8_t *)&address, sizeof(address));

  prefs.putUInt("address", address); // key,value쌍으로 key에 "address"란 문자열을 넣음
  prefs.putUShort("crc", crc);
  
  prefs.end(); // prefs 닫기

  return true;
}

/**
 * @brief 공장 초기화 함수
 */
void remoteStorageDeleteAll(uint8_t max_slot)
{
  for (int i=1; i<=MAX_REMOTE_CONTROLLER_NUM; i++)
  {
    remoteStorageDelete(i);
  }
}

/**
 * @brief static 함수들
 */
static int remoteInfoContained(uint32_t address)
{
  uint32_t loaded_address = 0;

  for (int i=0; i<MAX_REMOTE_CONTROLLER_NUM; i++)
  {
    loaded_address = preferences.getUInt("address", 0);
    
    if (loaded_address == address)
    {
      return true;
    }
  }

  return false;
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

static bool remoteStorageGetSlot(uint8_t slot)
{
  char name_space[16];
  makeNamespace(name_space, slot);

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

bool remoteStorageDelete(uint8_t slot)
{
  char name_space[16];
  makeNamespace(name_space, slot);

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

#endif
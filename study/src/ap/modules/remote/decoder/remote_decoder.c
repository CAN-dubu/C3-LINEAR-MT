#include "remote_decoder.h"


/**
 * @brief 아래는 정보에 대한 개별 함수들
 */
remote_type_t decodeRemotesType(uint32_t raw)
{
  if (raw == 0)
  {
    return REMOTE_TYPE_UNKNOWN;
  }

  if (raw < 0x23900000)
  {
    return REMOTE_TYPE_A; 
  }
  else
  {
    return REMOTE_TYPE_B; 
  }
}

uint32_t decodeRemotesAddress(uint32_t raw)
{
  if (raw == 0)
  {
    return 0;
  }

  return raw & 0xFFFFFFF0;
}

uint8_t decodeRemotesButton(uint32_t raw)
{
  if (raw == 0)
  {
    return 0;
  }

  return raw & 0x0F;
}
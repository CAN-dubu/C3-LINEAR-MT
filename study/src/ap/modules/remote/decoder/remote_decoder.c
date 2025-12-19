#include "remote_decoder.h"

// /**
//  * @brief 리모컨 rf코드에서 추출가능한 정보들
//  */
// typedef struct
// {
//   remote_type_t type;
//   uint32_t      address;
//   remote_btn_t  button;
// } remote_decoded_t;

// /**
//  * @brief 정보를 decode해서 의미 storage에 전달
//  */
// remote_decoded_t getDecodedRemoteInfo(uint32_t raw)
// {
//   remote_decoded_t remote_decoded;

//   remote_decoded.type    = decodeRemotesType(raw);
//   remote_decoded.address = decodeRemotesAddress(raw);
//   remote_decoded.button  = decodeRemotesButton(raw);

//   return remote_decoded;
// }

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

remote_btn_t decodeRemotesButton(uint32_t raw)
{
  if (raw == 0)
  {
    return 0;
  }

  return raw % 0x0F;
}
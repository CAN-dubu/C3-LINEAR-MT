#ifndef RF_DEF_H_
#define RF_DEF_H_

/**
* @brief 리모컨 버튼 상수, TYPE B 구현 고민
*/
#define REMOTE_TYPE_A_BTN_A 0x01
#define REMOTE_TYPE_A_BTN_B 0x02
#define REMOTE_TYPE_A_BTN_C 0x04
#define REMOTE_TYPE_A_BTN_D 0x08  

#define REMOTE_TYPE_B_BTN_A 0x04 
#define REMOTE_TYPE_B_BTN_B 0x08 
#define REMOTE_TYPE_B_BTN_C 0x01
#define REMOTE_TYPE_B_BTN_D 0x02

/**
* @brief 리모컨 타입
*/
typedef enum
{
  REMOTE_TYPE_UNKNOWN,
  REMOTE_TYPE_A, // 현재 kc인증된 리모컨
  REMOTE_TYPE_B, // 판매중이 아닌 다른 타입의 리모컨
} remote_type_t;

#define MAX_REMOTE_CONTROLLER_NUM 20 // 저장 가능한 최대 리모컨 수 (적절히 조절)

/**
* @brief 에러 타입
*/


#endif

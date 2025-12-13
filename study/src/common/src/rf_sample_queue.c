#include "../Inc/rf_sample_queue.h"
#include "../Inc/qbuffer.h"
#include <stdio.h>

/**
 * @brief RF 리모컨 학습용 세션 큐 관리
 * 
 * @details 
 * (Queue push전 : 세션1, QueuePush후 : 새로운 세션2 (새로운 시작), 을 기준으로 세션을 구분 하고있다.)
 * 
 * 1. current_sample: 현재 수신 중인 "한 버튼 입력 세션"의 임시 버퍼
 *    - rx_packet[SAMPLE_MAX]: 세션 내 서로 다른 코드들의 빈도수 저장
 *    - total: 세션 내 총 수신 패킷 수
 * 
 * 2. sample_queue (qbuffer_t): 완성된 세션들을 FIFO로 관리
 *    - sample_buf[4]: 실제 세션 데이터 저장소 (최대 4개 세션 버퍼링)
 * 
 * 3. 세션 분리 기준:
 *    - 50ms 내 다른 코드 수신 → "버튼 전환" → sampleQueuePush()
 *    - 200ms 신호 없음 → "버튼 뗌" → 타임아웃 sampleQueuePush()
 * 
 * 4. 검증:
 *    - 세션 내 최다 코드 비율 >= 95% → 신호 일관성 검증 성공
 * 
 * @example
 * 버튼A 1초: current_sample.rx_packet[0]={0xAAAA, count=100}
 * 버튼A 뗌:  timeout checking -> 200ms 이상 응답이 없다면, 큐[0]에 세션1 저장 -> 새 세션 시작
 * 버튼B 전환 (if '100ms 이내' then): current_sample.rx_packet[1]={0xBBBB,count=10}으로 같은 current_sample내 count
 * 버튼B 다시 누름 (if '100ms 이후' then): 큐[1]에 세션2 저장 → vaildate 검사
 * 버튼B 떼고 100ms이내 버튼C 전환:  큐[2]에 세션2 저장 → vaildate 검사
 */

// static void currentQueueReset(void);

// #define SAMPLE_MAX 20
// #define NO_SIGNAL_TIME_OUT 100 

// typedef struct 
// {
//   uint32_t code;
//   uint8_t count;
// } rx_packet_t;

// typedef struct 
// {
//   uint8_t      total;
//   uint32_t     pre_time;
//   rx_packet_t  rx_packet[SAMPLE_MAX];
// } sample_t;

// #define SAMPLE_QUEUE_LENGTH 4 // 버튼은 최대 4개의 입력패턴이 있으므로

// static sample_t sample_buf[SAMPLE_QUEUE_LENGTH];
// static qbuffer_t sample_queue;

// static sample_t current_sample;

// bool rfSampleQueueInit(void)
// {
//   bool ret = true;

//   qbufferCreateBySize(&sample_queue, (uint8_t*)sample_buf, 
//                       sizeof(sample_t), SAMPLE_QUEUE_LENGTH);
//   memset(&current_sample, 0, sizeof(sample_t));
//   return ret;
// }

// /**
//  * @brief timeout만큼의 시간동안 무응답일시 강제 세션종료 (새 세션 시작)
//  */
// void rfSampleQueueUpdate(void)
// {
//   // 1) 세션 타임아웃 체크
//   if (current_sample.total > 0 &&
//       (millis() - current_sample.pre_time) > NO_SIGNAL_TIME_OUT)
//   {
//     sampleQueuePush();
//   }
// }

// /**
//  * @brief 들어온 데이터가 있다면, queue에 추가
//  */
// void rfSampleQueuePush(void)
// {
//   printf("[PUSH] total=%d\n", current_sample.total);
//   if (current_sample.total > 0)
//   {
//     qbufferWrite(&sample_queue, (uint8_t*)&current_sample, 1);
//   }
//   currentQueueReset();
// }

// /**
//  * @brief 
//  */
// void rfSampleQueueWrite(uint32_t code)
// {
//   uint32_t entered_time = millis();
  
//   if (current_sample.total > 0 && (entered_time - current_sample.pre_time < 50))
//   {
//     for (int i=0; i<SAMPLE_MAX; i++)
//     {
//       if (current_sample.rx_packet[i].code != 0 && current_sample.rx_packet[i].code != code)
//       {
//         sampleQueuePush();
//         break;
//       }
//     }
//   }

//   current_sample.pre_time = millis();
//   current_sample.total++;
  
//   // 이전 데이터와 동일한 경우
//   for (int i=0; i<SAMPLE_MAX; i++)
//   {
//     if (current_sample.rx_packet[i].code == code)
//     {
//       current_sample.rx_packet[i].count++; // 해당 인덱스의 카운트 값 증가.
//       return;
//     }
//   }

//   // code가 0으로 비어있을때, (이전 데이터와는 다른 경우)
//   for (int i = 0; i < SAMPLE_MAX; i++)
//   {
//     if (current_sample.rx_packet[i].code == 0)
//     {
//       current_sample.rx_packet[i].code = code;
//       current_sample.rx_packet[i].count = 1;
//       return;
//     }
//   }
// }

// bool rfSampleQueueGet(uint32_t *out_code)
// {
//   if (qbufferAvailable(&sample_queue) == 0) 
//   {
//     return false;
//   }
  
//   sample_t temp_buffer;
//   qbufferRead(&sample_queue, (uint8_t*)&temp_buffer, 1);
//   printf("[GET] total=%d\n", temp_buffer.total);
//   if (temp_buffer.total < 1) 
//   {
//     printf("[GET] total < 1 -> fail\n");
//     return false; 
//   }
  
//   // 최다 수신 코드 찾기
//   uint8_t max_count = 0;
//   uint32_t max_code = 0;
  
//   for (int i = 0; i < SAMPLE_MAX; i++)
//   {
//     if (temp_buffer.rx_packet[i].count > max_count)
//     {
//        printf("  code=0x%08lX, count=%d\n",  (unsigned long)temp_buffer.rx_packet[i].code,
//              temp_buffer.rx_packet[i].count);
//       max_count = temp_buffer.rx_packet[i].count;
//       max_code = temp_buffer.rx_packet[i].code;
//     }
//   }
  
//   // 95% 일관성 체크
//   if (max_count * 100 / temp_buffer.total >= 95)
//   {
//     *out_code = max_code;
//     return true;
//   }
  
//   return false;
// }

// static void currentQueueReset(void)
// {
//   current_sample.total = 0;
//   current_sample.pre_time = 0;

//   for (int i = 0; i < SAMPLE_MAX; i++) 
//   {
//     current_sample.rx_packet[i].code = 0;
//     current_sample.rx_packet[i].count = 0;
//   }
// }
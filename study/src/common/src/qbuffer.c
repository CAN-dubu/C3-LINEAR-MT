#include "../Inc/qbuffer.h"

/**
 * @brief size = 1인 qbuffer 생성
 */
bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
  bool ret = true;

  p_node->in    = 0;
  p_node->out   = 0;
  p_node->len   = length;
  p_node->size  = 1;
  p_node->p_buf = p_buf;

  return ret;
}

/**
 * @brief size를 입력받아 qbuffer 생성
 */
bool qbufferCreateBySize(qbuffer_t *p_node, uint8_t *p_buf, uint32_t size, uint32_t length)
{
  bool ret = true;

  p_node->in    = 0;
  p_node->out   = 0;
  p_node->len   = length;
  p_node->size  = size;
  p_node->p_buf = p_buf;

  return ret;
}

/**
 * @brief p_data 데이터를 length개 만큼 원형버퍼 p_node에 넣는다.
          넣을 공간이 없으면 false 리턴.
 */
bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t next_in;

  for (int i=0; i<length; i++)
  {
    next_in = (p_node->in + 1) % p_node->len; // 마지막까지 원소가 가득차면 첫번째 인덱스로 돌아가 다시 채우길 반복

    if (next_in != p_node->out) // 쓸 위치와 읽는 위치가 같다 = 버퍼 공간이 없다.
    {
      if (p_node->p_buf != NULL && p_data != NULL) // p_node내 버퍼가 정상적인지, 쓸 값은 이상이 없는지
      {
        uint8_t *p_buf;

        p_buf = &p_node->p_buf[p_node->in*p_node->size]; // 사용자가 지정한 버퍼 사이즈가 4바이트라면, 1바이트를 4칸 사용 (실제 주소 계산) 
        for (int j=0; j<p_node->size; j++) // (size = 4,in = 0 일때 p_buf[0]~ p_buf[3] 까지)
        {
          p_buf[j] = p_data[j];
        }
        p_node->in = next_in; // 반복
      }
    }
    else
    {
      ret = false;
      break;
    }
  }

  return ret;
}

bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;

  for (int i=0; i<length; i++)
  {
    if (p_node->p_buf != NULL && p_data != NULL)
    {
      uint8_t *p_buf;

      p_buf = &p_node->p_buf[p_node->out*p_node->size];
      for (int j=0; j<p_node->size; j++)
      {
        p_data[j] = p_buf[j];
      }

      p_data += p_node->size;
    }

    if (p_node->out != p_node->in)
    {
      p_node->out = (p_node->out + 1) % p_node->len;
    }
    else
    {
      ret = false;
      break;
    }
  }

  return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
  uint32_t ret;

  ret = (p_node->len + p_node->in - p_node->out) % p_node->len; // in과 out의 인덱스 차만큼 데이터가 존재하고 있으므로, 버퍼 속 남은 데이터 수를 반환

  return ret;
}

void qbufferFlush(qbuffer_t *p_node)
{
  p_node->in  =  0;
  p_node->out =  0;
}





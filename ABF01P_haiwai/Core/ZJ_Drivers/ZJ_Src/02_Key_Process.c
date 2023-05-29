#include "Key_Process.h"

extern uint8_t Continue_Times;

typedef enum
{
  _KEY_NONE,
  _REENTER,
  _NO_REENTER
} _Key_REENTER;

typedef enum
{
  _HAS_NO_KEY,
  _HAS_KEY_DOWN,
  _HAS_KEY_SURE,
  _HAS_KEY_WAITUP
} _Key_State;

typedef struct
{
  uint8_t Key_Short : 1;
  uint8_t Key_Long : 1;
  uint8_t Key_Short_Set : 1;
} _Key_Flag_Struct;

uint8_t KeyDownCallBack(uint16_t Key, uint16_t Times);

typedef struct
{
  uint8_t Long_Key_t;
  uint16_t SameKeyCntr;
  _Key_Input LastKey;
  _Key_Input PreDownKey;
  _Key_State KeyState;
  _Key_Input CurKey;
  uint8_t (*KeyDownCallBack)(uint16_t, uint16_t);
  void (*KeyUpCallBack)(uint16_t);
} struct_KeyInfo;

static void KeyUpCallBack(uint16_t Key);
static void DitherlessKey(struct_KeyInfo *pInfo);
_Key_Input Key_Scan(void);
static struct_KeyInfo g_KeyInfo1 = {0, 0, KEY_None, KEY_None, _HAS_NO_KEY, KEY_None, KeyDownCallBack, KeyUpCallBack};

static struct_KeyInfo Test_KeyInfo1 = {0, 0, KEY_None, KEY_None, _HAS_NO_KEY, KEY_None, KeyDownCallBack, KeyUpCallBack};
// extern _Key_Flag_Struct Key_Flag;
_Key_Flag_Struct Key_Flag;

extern void Key_Press_Process(_Key_Input key);
extern void Key_PutUp_Process(_Key_Input key);
extern void Key_Press_Long_Process(_Key_Input key);
extern void Key_Press_Continue(_Key_Input key);

/**************************************************************************************
 * FunctionName   : KeyUpCallBack
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void KeyUpCallBack(uint16_t Key)
{
  Key_Flag.Key_Short = 0;
  Key_Flag.Key_Long = 0;
}
/**************************************************************************************
 * FunctionName   : DitherlessKey(struct_KeyInfo* pInfo)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void DitherlessKey(struct_KeyInfo *pInfo)
{
  switch (pInfo->KeyState)
  {
  case _HAS_NO_KEY:
  {
    pInfo->SameKeyCntr = 0;
    if (pInfo->CurKey != KEY_None)
    {
      pInfo->KeyState = _HAS_KEY_DOWN;
    }
  }
  break;

  case _HAS_KEY_DOWN:
  {
    if (pInfo->PreDownKey == pInfo->CurKey)
    {
      pInfo->KeyState = _HAS_KEY_SURE;
    }
    else
    {
      pInfo->KeyState = _HAS_NO_KEY;
    }
  }
  break;

  case _HAS_KEY_SURE:
  {
    if (pInfo->CurKey == pInfo->PreDownKey)
    {
      pInfo->KeyState = _HAS_KEY_WAITUP;
      if (pInfo->KeyDownCallBack)
      {
        if (_REENTER == pInfo->KeyDownCallBack(pInfo->CurKey, pInfo->SameKeyCntr))
        {
          pInfo->KeyState = _HAS_KEY_SURE;
          ++pInfo->SameKeyCntr;
        }
      }
    }
    else
    {
      pInfo->KeyState = _HAS_NO_KEY;
    }
  }
  break;

  case _HAS_KEY_WAITUP:
  {
    if (pInfo->CurKey != pInfo->PreDownKey)
    {
      pInfo->KeyState = _HAS_NO_KEY;

      if (pInfo->KeyUpCallBack)
      {
        pInfo->KeyUpCallBack(pInfo->PreDownKey);
      }
    }
  }
  break;

  default:
    break;
  }

  pInfo->PreDownKey = pInfo->CurKey;

  return;
}
/**************************************************************************************
 * FunctionName   : Key_Stroke()
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Stroke(void)
{
  if (g_KeyInfo1.KeyState == _HAS_KEY_SURE)
  {
    if (Key_Flag.Key_Short == 0)
    {
      Key_Flag.Key_Short = 1;
      Key_Flag.Key_Short_Set = 1;
      g_KeyInfo1.LastKey = g_KeyInfo1.CurKey;
      Key_Press_Process(g_KeyInfo1.CurKey);
    }
  }
  else if (g_KeyInfo1.KeyState == _HAS_NO_KEY)
  {
    Key_Flag.Key_Short = 0;
    Key_Flag.Key_Long = 0;
    g_KeyInfo1.Long_Key_t = 0;
    if (Key_Flag.Key_Short_Set) // ???????????
    {
      Key_Flag.Key_Short_Set = 0;
      Key_PutUp_Process(g_KeyInfo1.LastKey);
      g_KeyInfo1.LastKey = KEY_None;
    }
  }
  else if (g_KeyInfo1.KeyState == _HAS_KEY_WAITUP)
  {
    if (Key_Flag.Key_Long == 0) // ????????????
    {
      Key_Flag.Key_Long = 1;
      Key_Flag.Key_Short_Set = 0;
      Key_Press_Long_Process(g_KeyInfo1.CurKey);
    }

    if (++g_KeyInfo1.Long_Key_t > Continue_Times)
    {
      g_KeyInfo1.Long_Key_t = 0;
      //				Key_Press_Continue(g_KeyInfo1.LastKey);
    }
  }
}

/**************************************************************************************
 * FunctionName   : Key_Stroke()
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Key_Stroke(void)
{
  if (Test_KeyInfo1.KeyState == _HAS_KEY_SURE)
  {
    if (Key_Flag.Key_Short == 0)
    {
      Key_Flag.Key_Short = 1;
      Key_Flag.Key_Short_Set = 1;
      Test_KeyInfo1.LastKey = Test_KeyInfo1.CurKey;
      Test_Key_Press_Process(Test_KeyInfo1.CurKey);
    }
  }
  else if (Test_KeyInfo1.KeyState == _HAS_NO_KEY)
  {
    Key_Flag.Key_Short = 0;
    Key_Flag.Key_Long = 0;
    Test_KeyInfo1.Long_Key_t = 0;
    if (Key_Flag.Key_Short_Set) // ???????????
    {
      Key_Flag.Key_Short_Set = 0;
      Test_Key_PutUp_Process(Test_KeyInfo1.LastKey);
      Test_KeyInfo1.LastKey = KEY_None;
    }
  }
  else if (Test_KeyInfo1.KeyState == _HAS_KEY_WAITUP)
  {
    if (Key_Flag.Key_Long == 0) // ????????????
    {
      Key_Flag.Key_Long = 1;
      Key_Flag.Key_Short_Set = 0;
      Test_Key_Press_Long_Process(Test_KeyInfo1.CurKey);
    }

    if (++Test_KeyInfo1.Long_Key_t > Continue_Times)
    {
      Test_KeyInfo1.Long_Key_t = 0;
      //				Key_Press_Continue(g_KeyInfo1.LastKey);
    }
  }
}
/**************************************************************************************
 * FunctionName   : Key_Stroke()
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Proc(void)
{
  g_KeyInfo1.CurKey = Key_Scan();
  DitherlessKey(&g_KeyInfo1);
  Key_Stroke();
}
/**************************************************************************************
 * FunctionName   : Key_Stroke()
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Key_Proc(void)
{
  Test_KeyInfo1.CurKey = Key_Scan();
  DitherlessKey(&Test_KeyInfo1);
  Test_Key_Stroke();
  g_KeyInfo1.CurKey = KEY_None;
  g_KeyInfo1.LastKey = KEY_None;
  g_KeyInfo1.Long_Key_t = 0;
  g_KeyInfo1.KeyState = _HAS_NO_KEY;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

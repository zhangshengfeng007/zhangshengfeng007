/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/**
* @brief  Convert an Integer to a string
* @param  str: The string
* @param  intnum: The intger to be converted
* @retval None
*/
void Int2Str(uint8_t* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;
  
  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;
    
    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}

/**
* @brief  Convert a string to an integer
* @param  inputstr: The string to be converted
* @param  intnum: The intger value
* @retval 1: Correct
*         0: Error
*/
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;
  
  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
      return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0;i < 11;i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  return res;
}

/**
* @brief  Get an integer from the HyperTerminal
* @param  num: The inetger
* @retval 1: Correct
*         0: Error
*/
uint32_t GetIntegerInput(int32_t * num)
{
  uint8_t inputstr[16];
  
  while (1)
  {
    GetInputString(inputstr);
    if (inputstr[0] == '\0') continue;
    if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
    {
      SerialPutString("User Cancelled \r\n");
      return 0;
    }
    
    if (Str2Int(inputstr, num) == 0)
    {
      SerialPutString("Error, Input again: \r\n");
    }
    else
    {
      return 1;
    }
  }
}

/**
* @brief  Test to see if a key has been pressed on the HyperTerminal
* @param  key: The key pressed
* @retval 1: Correct
*         0: Error
*/
uint32_t SerialKeyPressed(uint8_t *key)
{
  if((USART2->ISR & (1<<5))) //USART_FLAG_RXNE
  {
    *key = (uint8_t)(USART2->RDR & (uint8_t)0xFF);
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Get a key from the HyperTerminal
* @param  None
* @retval The Key Pressed
*/
uint8_t GetKey(void)
{
  uint8_t key = 0;
  uint32_t time_out = 0x5000;
  while (time_out--)/* Waiting for user input */
  {
    if (SerialKeyPressed((uint8_t*)&key)) break;
  }
  return key;
}

/**
* @brief  
* @param  
* @retval None
*/
//uint8_t SerialGetChar(void)
//{
//  while(!(USART1->ISR & (1<<5)));
//  return USART1->RDR;
//}

/**
* @brief  Print a character on the HyperTerminal
* @param  c: The character to be printed
* @retval None
*/
void SerialPutChar(uint8_t c)
{ 
  while(!(USART2->ISR & (1<<7)));
  USART2->TDR = c;
}

/**
* @brief  Print a string on the HyperTerminal
* @param  s: The string to be printed
* @retval None
*/
void Serial_PutString(uint8_t *s)
{
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}

/**
* @brief
* @param
* @retval None
*/
//int fputc(int ch,FILE *f)
//{
//  while((USART1->ISR & (1<<7)) == 0);
//  USART1->TDR = (uint8_t)ch;
//  return ch;
//}

/**
* @brief  Get Input string from the HyperTerminal
* @param  buffP: The input string
* @retval None
*/
void GetInputString (uint8_t * buffP)
{
  uint32_t bytes_read = 0;
  uint8_t c = 0;
  do
  {
    c = GetKey();
    if (c == '\r')
      break;
    if (c == '\b') /* Backspace */
    {
      if (bytes_read > 0)
      {
        SerialPutString("\b \b");
        bytes_read --;
      }
      continue;
    }
    if (bytes_read >= CMD_STRING_SIZE )
    {
      SerialPutString("Command string size overflow\r\n");
      bytes_read = 0;
      continue;
    }
    if (c >= 0x20 && c <= 0x7E)
    {
      buffP[bytes_read++] = c;
      SerialPutChar(c);
    }
  }
  while (1);
  SerialPutString(("\n\r"));
  buffP[bytes_read] = '\0';
}


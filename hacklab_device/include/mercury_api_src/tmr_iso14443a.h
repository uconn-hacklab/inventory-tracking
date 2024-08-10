/* ex: set tabstop=2 shiftwidth=2 expandtab cindent: */
#ifndef _TMR_ISO14443A_H
#define _TMR_ISO14443A_H
/** 
 *  @file tmr_iso14443a.h  
 *  @brief Mercury API - ISO14443A tag information and interfaces
 *  @author Anjali Panchal
 *  @date 1/30/2020
 */

/*
 * Copyright (c) 2023 Novanta, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef  __cplusplus
extern "C" {
#endif
#ifdef TMR_ENABLE_HF_LF
/** Defines ISO14443A protocol supported tagTypes */
typedef enum TMR_ISO14443A_TagType
{
  TMR_ISO14443A_TAGTYPE_AUTO_DETECT          = 0x00000001,
  TMR_ISO14443A_TAGTYPE_MIFARE_PLUS          = 0x00000002,
  TMR_ISO14443A_TAGTYPE_MIFARE_ULTRALIGHT    = 0x00000004,
  TMR_ISO14443A_TAGTYPE_MIFARE_CLASSIC       = 0x00000008,
  TMR_ISO14443A_TAGTYPE_NTAG                 = 0x00000010,
  TMR_ISO14443A_TAGTYPE_MIFARE_DESFIRE       = 0x00000020,
  TMR_ISO14443A_TAGTYPE_MIFARE_MINI          = 0x00000040,
  TMR_ISO14443A_TAGTYPE_ULTRALIGHT_NTAG      = 0x00000080,
}TMR_ISO14443A_TagType;

#endif /* TMR_ENABLE_HF_LF */
#ifdef __cplusplus
}
#endif

#endif /*_TMR_ISO14443A_H*/

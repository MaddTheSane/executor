//
//  SpeechManager.h
//  CocoaExecutor
//
//  Created by C.W. Betts on 8/4/14.
//  Copyright (c) 2014 C.W. Betts. All rights reserved.
//

#ifndef __CocoaExecutor__SpeechManager__
#define __CocoaExecutor__SpeechManager__

#include "SoundMgr.h"
#include "FileMgr.h"

namespace Executor
{
#pragma pack(push, 2)
typedef struct VoiceSpec
{
    GUEST_STRUCT;
    GUEST<OSType> creator;
    GUEST<OSType> id;
} VoiceSpec, *VoiceSpecPtr;

typedef struct VoiceFileInfo
{
    GUEST_STRUCT;
    FSSpec fileSpec;
    GUEST<uint16_t> resID;
} VoiceFileInfo;

typedef struct SpeechStatusInfo
{
    GUEST_STRUCT;
    GUEST<Boolean> outputBusy;
    GUEST<Boolean> outputPaused;
    GUEST<int32_t> inputBytesLeft;
    GUEST<int16_t> phonemeCode;
} SpeechStatusInfo;

typedef struct VoiceDescription
{
    GUEST_STRUCT;
    GUEST<int32_t> length;
    GUEST<VoiceSpec> voice;
    GUEST<int32_t> version;
    GUEST<Str63> name;
    GUEST<Str255> comment;
    GUEST<int16_t> gender;
    GUEST<int16_t> age;
    GUEST<int16_t> script;
    GUEST<int16_t> language;
    GUEST<int16_t> region;
    GUEST<int32_t> reserved[4];
} VoiceDescription;

typedef struct SpeechChannelRecord
{
    GUEST_STRUCT;
    LONGINT data[1];
} SpeechChannelRecord, *SpeechChannel;

typedef struct PhonemeInfo
{
    GUEST_STRUCT;
    GUEST<int16_t> opcode;
    GUEST<Str15> phStr;
    GUEST<Str31> exampleStr;
    GUEST<int16_t> hiliteStart;
    GUEST<int16_t> hiliteEnd;
} PhonemeInfo;

typedef struct PhonemeDescriptor
{
    GUEST_STRUCT;
    GUEST<int16_t> phonemeCount;
    GUEST<PhonemeInfo> thePhonemes[1];
} PhonemeDescriptor;

typedef struct SpeechXtndData
{
    GUEST_STRUCT;
    GUEST<OSType> synthCreator;
    GUEST<Byte> synthData[2];
} SpeechXtndData;

typedef struct DelimiterInfo
{
    GUEST_STRUCT;
    GUEST<Byte> startDelimiter[2];
    GUEST<Byte> endDelimiter[2];
} DelimiterInfo;

#pragma pack(pop)

NumVersion C_SpeechManagerVersion(void);
int16_t C_SpeechBusy(void);
int16_t C_SpeechBusySystemWide(void);
OSErr C_CountVoices(int16_t *numVoices);
OSErr C_DisposeSpeechChannel(SpeechChannel chan);
OSErr C_SpeakString(Str255 textToBeSpoken);

OSErr C_StopSpeech(SpeechChannel chan);
OSErr C_ContinueSpeech(SpeechChannel chan);

OSErr C_GetIndVoice(int16_t index, VoiceSpec *voice);
OSErr C_NewSpeechChannel(VoiceSpec *voice, SpeechChannel *chan);
OSErr C_StopSpeechAt(SpeechChannel chan, int32_t whereToStop);
OSErr C_PauseSpeechAt(SpeechChannel chan, int32_t whereToPause);
OSErr C_SetSpeechRate(SpeechChannel chan, Fixed rate);
OSErr C_GetSpeechRate(SpeechChannel chan, Fixed *rate);
OSErr C_SetSpeechPitch(SpeechChannel chan, Fixed pitch);
OSErr C_GetSpeechPitch(SpeechChannel chan, Fixed *pitch);
OSErr C_UseDictionary(SpeechChannel chan, Handle dictionary);
OSErr C_MakeVoiceSpec(OSType creator, OSType id, VoiceSpec *voice);
OSErr C_GetVoiceDescription(const VoiceSpec *voice, VoiceDescription *info, LONGINT infoLength);
OSErr C_GetVoiceInfo(const VoiceSpec *voice, OSType selector, void *voiceInfo);
OSErr C_SpeakText(SpeechChannel chan, const void *textBuf, ULONGINT textBytes);
OSErr C_SetSpeechInfo(SpeechChannel chan, OSType selector, const void *speechInfo);
OSErr C_GetSpeechInfo(SpeechChannel chan, OSType selector, void *speechInfo);
OSErr C_SpeakBuffer(SpeechChannel chan, const void *textBuf, ULONGINT textBytes, int32_t controlFlags);
OSErr C_TextToPhonemes(SpeechChannel chan, const void *textBuf, ULONGINT textBytes, Handle phonemeBuf, GUEST<LONGINT> *phonemeBytes);
}

#endif /* defined(__CocoaExecutor__SpeechManager__) */

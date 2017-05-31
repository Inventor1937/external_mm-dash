/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DASHPLAYER_DECODER_H_

#define DASHPLAYER_DECODER_H_

#include "DashPlayerRenderer.h"
#include "DashPlayer.h"
#include <media/stagefright/foundation/AHandler.h>
#include "OMX_QCOMExtns.h"

namespace android {

struct ABuffer;
struct MediaCodec;

struct DashPlayer::Decoder : public AHandler {
    Decoder(const sp<AMessage> &notify,
            const sp<Surface> &nativeWindow = NULL);

    void configure(const sp<MetaData> &meta);
    void init();

    void signalFlush();
    void signalResume();
    void initiateShutdown();


    enum {
        kWhatFillThisBuffer      = 'flTB',
        kWhatDrainThisBuffer     = 'drTB',
        kWhatOutputFormatChanged = 'fmtC',
        kWhatFlushCompleted      = 'flsC',
        kWhatShutdownCompleted   = 'shDC',
        kWhatEOS                 = 'eos ',
        kWhatError               = 'err ',
    };

protected:
    virtual ~Decoder();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        kWhatCodecNotify        = 'cdcN',
        kWhatConfigure          = 'conf',
        kWhatInputBufferFilled  = 'inpF',
        kWhatRenderBuffer       = 'rndr',
        kWhatFlush              = 'flus',
        kWhatShutdown           = 'shuD',
    };

    sp<AMessage> mNotify;
    sp<Surface> mNativeWindow;

    sp<AMessage> mInputFormat;
    sp<AMessage> mOutputFormat;
    sp<MediaCodec> mCodec;
    sp<ALooper> mCodecLooper;
    sp<ALooper> mDecoderLooper;

    Vector<sp<ABuffer> > mInputBuffers;
    Vector<sp<ABuffer> > mOutputBuffers;

    void handleError(int32_t err);
    bool handleAnInputBuffer();
    bool handleAnOutputBuffer();

    void requestCodecNotification();
    bool isStaleReply(const sp<AMessage> &msg);

    int mLogLevel;

    sp<AMessage> makeFormat(const sp<MetaData> &meta);

    void onConfigure(const sp<AMessage> &format);
    void onFlush();
    void onInputBufferFilled(const sp<AMessage> &msg);
    void onRenderBuffer(const sp<AMessage> &msg);
    void onShutdown();

    int32_t mBufferGeneration;
    AString mComponentName;


    DISALLOW_EVIL_CONSTRUCTORS(Decoder);
};

struct DashPlayer::CCDecoder : public RefBase{
    enum {
        kWhatClosedCaptionData,
        kWhatTrackAdded,
    };

    CCDecoder(const sp<AMessage> &notify);

    size_t getTrackCount() const;
    sp<AMessage> getTrackInfo(size_t index) const;
    status_t selectTrack(size_t index, bool select);
    bool isSelected() const;
    void decode(const sp<ABuffer> &accessUnit);
    void display(int64_t timeUs);
    void flush();
    void decode(OMX_U8 *pictureUserData, OMX_U32 pictureUserDataSize, int64_t mediaTimeUs);
    int getSelectedTrack();

private:
    sp<AMessage> mNotify;
    KeyedVector<int64_t, sp<ABuffer> > mCCMap;
    size_t mCurrentChannel;
    int32_t mSelectedTrack;
    int32_t mTrackIndices[4];
    Vector<size_t> mFoundChannels;

    bool isTrackValid(size_t index) const;
    int32_t getTrackIndex(size_t channel) const;
    //bool extractFromSEI(const sp<ABuffer> &accessUnit);
    bool extractPictureUserData(OMX_U8 *pictureUserDatae, OMX_U32 pictureUserDataSize, int64_t mediaTimeUs);
    sp<ABuffer> filterCCBuf(const sp<ABuffer> &ccBuf, size_t index);
    void printmCCMap();
    void PrintCCTypeCombo(bool cc_valid,uint8_t cc_type);

    int32_t mCurrentWidth;
    int32_t mCurrentHeight;
    int32_t mColorFormat;

    int mLogLevel;

    DISALLOW_EVIL_CONSTRUCTORS(CCDecoder);
};


}  // namespace android

#endif  // DASHPLAYER_DECODER_H_

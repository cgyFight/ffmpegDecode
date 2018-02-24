#ifndef FFDECODE_COMMON_H
#define FFDECODE_COMMON_H

#ifdef FFMPEGDECODE_EXPORTS
#define FFMPEGDECODE_API __declspec(dllexport)
#else
#define FFMPEGDECODE_API __declspec(dllimport)
#endif

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/mem.h"
#include "libavutil/fifo.h"
#include "libswscale/swscale.h"
}

#include "inttypes.h"


#define SAFE_FREE(p) do \
{\
	if (p) {\
	free(p);\
	p = NULL;\
	}\
} while (0);

//½âÂëºóµÄ
typedef struct StructDecoded
{
	StructDecoded() {};
	int w;
	int h;
} StructDecoded;

typedef struct StructBeforDecode
{
	StructBeforDecode():decodeBuf(NULL){}
	uint8_t *decodeBuf;
	int frameSize;
}StructBeforDecode;

#endif
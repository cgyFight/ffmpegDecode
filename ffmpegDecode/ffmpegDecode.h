#ifndef FFMPEGDECODE_H
#define FFMPEGDECODE_H

#include "ffdecodecommon.h"

class FFMPEGDECODE_API CffmpegDecode {
public:
	CffmpegDecode(void) ;
	~CffmpegDecode(void) {SAFE_FREE(yuv420buf);SAFE_FREE(rgb32buf)};

	bool decodeInit();
	void codecDestroy();
	bool decodeOneSlice(StructBeforDecode in, StructDecoded &out ); //bool decodeOneSlice(uint8_t *buf, int frameSize, StructDecoded &out );

	//在成功调用decodeOneSlice后调用
	void getYUV420data(char **outData, int *size);
	bool getRGB32data(char **outData, int *size);

private:
	//默认大小是1920 * 1080p的大小，如果需要改变的话调用以下函数
	void changeYUV420BufSize(size_t size); 
	void changeRGB32BufSize(size_t size); 

	AVCodec *codec;
	AVCodecParserContext *parser;
	AVCodecContext *c;
	AVFrame *frame;
	AVPacket *pkt;

	char *yuv420buf;
	char *rgb32buf;
	unsigned int yuv420bufSize;
	unsigned int rgb32bufSize;
};
#endif

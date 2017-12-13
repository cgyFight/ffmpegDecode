#include "stdafx.h"
#include "ffmpegDecode.h"

CffmpegDecode::CffmpegDecode()
{
	yuv420buf = (char *)malloc(PIC_SIZE_RGB32_1080P);
	rgb32buf = (char *)malloc(PIC_SIZE_RGB32_1080P);
	return;
}

bool CffmpegDecode::decodeInit()
{
	avcodec_register_all();
	pkt = av_packet_alloc();
	if (!pkt) {
		printf("packet alloc error\n");
		return false;
	}

	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec) {
		printf("find decode error\n");
		return false;
	}

	c = avcodec_alloc_context3(codec);
	if (!c) {
		printf("alloc context error\n");
		return false;
	}

	if (0 > avcodec_open2(c, codec, NULL)) {
		printf("codec open err\n");
		return false;
	}

	frame = av_frame_alloc();
	if (!frame) {
		printf("frame alloc err\n");
		return false;
	}
	return true;
}

void CffmpegDecode::codecDestroy()
{
	//av_parser_close(parser);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);
}

bool CffmpegDecode :: decodeOneSlice(StructBeforDecode in, StructDecoded &out )
{
	pkt->size = in.frameSize;
	pkt->data = in.decodeBuf;

	int ret = 0;
	//new
	ret = avcodec_send_packet(c, pkt);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		return false;
	else if (ret < 0) {
		fprintf(stderr, "Error during decoding\n");
		return false;
	}

	ret = avcodec_receive_frame(c, frame);

	if (ret < 0) {
		printf("decode err\n");
		return false;
	}

	out.h = c->height;
	out.w = c->width;

	return true;
}

void CffmpegDecode::getYUV420data(char **outData, int *size)
{
	*size = c->width * c->height * 3 / 2;

	int i = 0;
	int p = 0;
	for(i=0; i<c->height; i++)   
	{   
		memcpy(yuv420buf+p,frame->data[0] + i * frame->linesize[0], c->width);   
		p += c->width;   
	}   
	for(i=0; i<c->height/2; i++)   
	{   
		memcpy(yuv420buf+p,frame->data[1] + i * frame->linesize[1], c->width/2);  
		p += c->width/2;   
	}   
	for(i=0; i<c->height/2; i++)   
	{   
		memcpy(yuv420buf+p,frame->data[2] + i * frame->linesize[2], c->width/2);  
		p += c->width/2;   
	} 
	*outData = yuv420buf;
	return ;
}

bool CffmpegDecode::getRGB32data(char **outData, int *size)
{
	SwsContext *swsctx = sws_getContext(c->width, c->height, AV_PIX_FMT_YUV420P,c->width,c->height, AV_PIX_FMT_RGB32, 0, NULL, NULL,NULL);
	if (!swsctx) {
		printf("transe rgb error\n");
		return false;
	}

	uint8_t *dst_data[4] = {0};
	int dst_linesize[4] = {0};

	int ret;
	ret = av_image_alloc(dst_data, dst_linesize, c->width, c->height, AV_PIX_FMT_RGB32 , 16);
	if (ret < 0) {
		printf("img alloc erro\n");
		return false;
	}

	ret = sws_scale(swsctx,  frame->data, frame->linesize, 0,c->height,dst_data, dst_linesize);

	*size = c->height * c->width * 4;

	memcpy(rgb32buf, dst_data[0], *size);

	*outData = rgb32buf;

	sws_freeContext(swsctx);
	av_freep(&dst_data[0]);
	return true;
}

void CffmpegDecode::changeYUV420BufSize(size_t size)
{
	SAFE_FREE(yuv420buf);
	yuv420buf = (char *)malloc(size);
}

void CffmpegDecode::changeRGB32BufSize(size_t size)
{
	SAFE_FREE(rgb32buf);
	rgb32buf = (char *)malloc(size);
}
# ffmpegDecode

以前的代码不见了，随手再写一个简单的解码用，可以获取420yuv以及rgb32

接口如下：

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

    //默认大小是1920 * 1080p的大小，如果需要改变的话调用以下函数
    void changeYUV420BufSize(size_t size); 
    void changeRGB32BufSize(size_t size); 
private:
    AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *c;
    AVFrame *frame;
    AVPacket *pkt;

    char *yuv420buf;
    char *rgb32buf;
};
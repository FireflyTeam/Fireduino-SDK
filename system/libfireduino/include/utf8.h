#ifdef __UTF8_H__

#define __UTF8_H__

#ifdef __cplusplus
extern "C" {
#endif

int wchar_to_utf8(const unsigned short *wbuf, char *buf, int buf_len);

int wchar_from_utf8(unsigned short *wbuf, int wbuf_len, const char *buf);


#ifdef __cplusplus
}
#endif

#endif // __UTF8_H__

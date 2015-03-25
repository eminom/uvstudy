
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "StreamBuffer.h"
#include "StreamState.h"
#include "client_proc_t.h"
#include "uv.h"

extern uv_loop_t *loop;

client_proc_t* createClientProcessor()
{
    client_proc_t* rv = (client_proc_t*)malloc(sizeof(client_proc_t));
    uv_tcp_init(loop, (uv_tcp_t*)rv);
    rv->sb = new StreamBuffer;
    rv->so = new StreamStateObj(rv->sb, rv);
    return rv;
}

void destroyClientProcessor(client_proc_t *ptr)
{
    uv_close((uv_handle_t*)ptr, 0);
    delete ptr->so;
    delete ptr->sb;
    free(ptr);
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    } else {
        // printf("written to client\n");
    }
    free(req);
}

void sendStreamBuffer(client_proc_t *ptr, int typecode, const char *buffer, int length)
{
	uv_stream_t *stream = (uv_stream_t*)ptr;
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

    int final_length = length + 4 + 2;
    char *ex_buffer = (char*)malloc(final_length);
    *((int*)ex_buffer) = final_length;
    *((short*)(ex_buffer + 4)) = typecode;
    if(length>0){
        memcpy(ex_buffer + 6, buffer, length);
    }
    uv_buf_t wrbuf = uv_buf_init(ex_buffer, final_length);
    uv_write(req, stream, &wrbuf, 1, echo_write);
    //printf("Something is sent to client(%d)\n", final_length);
    free(ex_buffer);
}

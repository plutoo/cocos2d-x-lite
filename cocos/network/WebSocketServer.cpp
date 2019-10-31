
#include <iostream>
#include <cassert>

#ifndef WSS_EXTERNAL_TEST_BUILD

#include "WebSocketServer.h"
#include "platform/CCApplication.h"
#include "base/CCScheduler.h"


namespace cocos2d {
namespace network {

#define RUN_IN_GAMETHREAD(task) \
    do { \
        cocos2d::Application::getInstance()->getScheduler()->performFunctionInCocosThread([=]() { \
        task; \
    });\
    } while(0)

#define DISPATCH_CALLBACK_IN_GAME_LOOP() do {\
        data->setCallback([callback](const std::string& msg) { \
            auto wrapper = [callback, msg]() {callback(msg); }; \
            RUN_IN_GAMETHREAD(wrapper()); \
        }); \
    }while(0)

#else

#include "WebSocketServer.h"
#include "uv.h"
#include <cstdio>
#include <cstring>

#define RUN_IN_GAMETHREAD(task) \
    do { \
        task; \
    } while(0)

#define DISPATCH_CALLBACK_IN_GAME_LOOP() do {\
        data->setCallback([callback](const std::string& msg) { \
            auto wrapper = [callback, msg]() {callback(msg); }; \
            RUN_IN_GAMETHREAD(wrapper()); \
        }); \
    }while(0)
//#define CCLOG(fmt, ...) do {\ 
//        char _buf[256] = { 0 }; \
//        snprintf(_buf, 256, fmt, ##__VA_ARGS__); \
//        std::cout << "[CCLOG]" << _buf << std::endl; \
//    } while(0)

#define CCLOG printf
#endif



#ifdef WS_LOG_FUN

void LOG_EVENT(const char* fn) 
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M:%S", timeinfo);

    std::cout << buffer << " [fn] " << fn << std::endl;
}
#define LOGE() LOG_EVENT(__FUNCTION__)

#else
//#define LOGE() CCLOG("WSS: %s", __FUNCTION__)
#define LOGE()
#endif





#define MAX_MSG_PAYLOAD 2048
#define SEND_BUFF 1024

struct AsyncTaskData {
    std::mutex mtx;
    std::list<std::function<void()> > tasks;
};

static void scheduler_task_cb(uv_async_t* asyn)
{
    AsyncTaskData* data = (AsyncTaskData*)asyn->data;
    std::lock_guard<std::mutex> guard(data->mtx);
    while (!data->tasks.empty())
    {
        data->tasks.front()();
        data->tasks.pop_front();
    }

}
static void async_init(uv_loop_t* loop, uv_async_t* async)
{
    memset(async, 0, sizeof(uv_async_t));
    uv_async_init(loop, async, scheduler_task_cb);
    async->data = new AsyncTaskData();
}

static void schedule_async_task(uv_async_t* asyn, std::function<void()> func)
{

    AsyncTaskData* data = (AsyncTaskData*)asyn->data;
    {
        std::lock_guard<std::mutex> guard(data->mtx);
        data->tasks.emplace_back(func);
    }
    uv_async_send(asyn);
}

#if USE_EXT_LIBUV_LOOP
struct timer_count {
    int c = 0;
    int max = 1;
};
static void timer_callback(uv_timer_t* timer) {
    struct timer_count* d = (struct timer_count*)timer->data;
    if (d->c >= d->max) {
        uv_timer_stop(timer);
    }
    d->c++;
    CCLOG("TIMER LOG \n");
}
#endif


DataFrag::DataFrag(const std::string& data) :_isBinary(false)
{
    _underlying_data.resize(data.size() + LWS_PRE);
    memcpy(getData(), data.c_str(), data.length());
}

DataFrag::DataFrag(const void* data, int len, bool isBinary) :_isBinary(isBinary)
{
    _underlying_data.resize(len + LWS_PRE);
    memcpy(getData(), data, len);
}

DataFrag::~DataFrag()
{
}

void DataFrag::append(unsigned char* p, int len)
{
    //_underlying_data.emplace_back(_underlying_data.end(), p, len);
    _underlying_data.insert(_underlying_data.end(), p, p + len);
}

int DataFrag::slice(unsigned char** p, int len) {
    *p = getData() + _consumed;
    if (_consumed + len > size()) {
        return size() - _consumed;
    }
    return len;
}

int DataFrag::consume(int len) {
    _consumed = len + _consumed > size() ? size() : len + _consumed;
    return _consumed;
}

int DataFrag::remain() const {
    return size() - _consumed;
}


std::string DataFrag::toString()
{
    return std::string((char*)getData(), size());
}

//static int websocket_server_callback(struct lws* wsi, enum lws_callback_reasons reason,
//    void* user, void* in, size_t len);

static struct lws_protocols protocols[] = {
    {
        "", //protocol name
        WebSocketServer::websocket_server_callback,
        sizeof(int),
        0
    },
    {
        nullptr, nullptr, 0
    }
};

static const struct lws_extension exts[] = {
    {
        "permessage-deflate",
        lws_extension_callback_pm_deflate,
        "permessage-deflate; client_on_context_takeover; client_max_window_bits"
    },
    {
        "deflate-frame",
        lws_extension_callback_pm_deflate,
        "deflate-frame"
    },
    {
        nullptr, nullptr, nullptr
    }
};


WebSocketServer::~WebSocketServer()
{

    if (_ctx) {

        if (_onclose) _onclose("");

        lws_context_destroy(_ctx);
        lws_context_destroy2(_ctx);
        _ctx = nullptr;
    }

    delete _subthread;
}

bool WebSocketServer::close(std::function<void(const std::string & errorMsg)> callback)
{
    lws_libuv_stop(_ctx);
    return true;
}

bool WebSocketServer::closeAsync(std::function<void(const std::string & errorMsg)> callback)
{
    schedule_async_task(&_async, [this, callback]() {
        this->close(callback);
        });
    return true;
}


bool WebSocketServer::listen(int port, const std::string& host, std::function<void(const std::string & errorMsg)> callback)
{

    lws_set_log_level(-1, nullptr);

    _host = host;
#if USE_EXT_LIBUV_LOOP
    _loop = (uv_loop_t*)malloc(sizeof(*_loop));
    memset(_loop, 0, sizeof(*_loop));
    uv_loop_init(_loop);

    uv_timer_init(_loop, &_timer);
    struct timer_count tc;
    tc.max = 20;
    _timer.data = &tc;
    uv_timer_start(&_timer, timer_callback, 0, 10);
    uv_run(_loop, UV_RUN_DEFAULT);

#endif
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = port;
    info.iface = _host.empty() ? nullptr : _host.c_str();
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.extensions = exts;
    // info.options = LWS_SERVER_OPTION_VALIDATE_UTF8
    //     |LWS_SERVER_OPTION_LIBUV
    //     |LWS_SERVER_OPTION_SKIP_SERVER_CANONICAL_NAME
    //     ;
    info.options = LWS_SERVER_OPTION_LIBUV
        ;
    info.timeout_secs = 3; //
    info.max_http_header_pool = 1;
    info.user = this;

    _ctx = lws_create_context(&info);

    if (!_ctx) {
        if (callback) {
            RUN_IN_GAMETHREAD(callback("Error: Failed to create lws_context!"));
        }
        RUN_IN_GAMETHREAD(if (_onerror)_onerror("websocket listen error!"));
        return false;
    }
    uv_loop_t* loop = nullptr;
#if USE_EXT_LIBUV_LOOP
    loop = _loop;
#endif
    if (lws_uv_initloop(_ctx, loop, 0)) {
        if (callback) {
            RUN_IN_GAMETHREAD(callback("Error: Failed to create libuv loop!"));
        }
        RUN_IN_GAMETHREAD(if (_onerror)_onerror("websocket listen error, failed to create libuv loop!"));
        return false;
    }
#if USE_EXT_LIBUV_LOOP
    assert(lws_uv_getloop(_ctx, 0) == loop);
#else 
    loop = lws_uv_getloop(_ctx, 0);
#endif

    _booted = true;
    async_init(loop, &_async);
    RUN_IN_GAMETHREAD(if(_onlistening)_onlistening(""));
    RUN_IN_GAMETHREAD(if(_onbegin)_onbegin());
    RUN_IN_GAMETHREAD(if(callback)callback(""));

#if USE_EXT_LIBUV_LOOP
    uv_run(loop, UV_RUN_DEFAULT);
#else
    lws_libuv_run(_ctx, 0);
#endif

    uv_close((uv_handle_t*)&_async, nullptr);
#if USE_EXT_LIBUV_LOOP
    uv_loop_close(loop);
#endif
    RUN_IN_GAMETHREAD(if(_onend)_onend());

    return true;
}

bool WebSocketServer::listenAsync(int port, const std::string& host, std::function<void(const std::string & errorMsg)> callback)
{
    if (this->_subthread) {
        return false;
    }

    this->_subthread = new std::thread([=]() {
        this->listen(port, host, callback);
        });

    return true;
}


std::vector<std::shared_ptr<Connection>> WebSocketServer::getConnections() const
{
    return {};
}

void WebSocketServer::onCreateClient(struct lws* wsi)
{
    LOGE();
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(wsi);

    char ip[221] = { 0 };
    char addr[221] = { 0 };
    //lws_get_peer_addresses(wsi, lws_get_socket_fd(wsi), ip, 220, addr, 220);

    _conns.emplace(wsi, conn);
    
    RUN_IN_GAMETHREAD(if(_onconnection) _onconnection(conn));
    
    conn->onConnected();
}

void WebSocketServer::onDestroyClient(struct lws* wsi)
{
    LOGE();
    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        itr->second->finallyClosed();
        _conns.erase(itr);
    }
}
void WebSocketServer::onCloseClient(struct lws* wsi)
{
    LOGE();
    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        itr->second->setClosed();
    }
}

void WebSocketServer::onCloseClientInit(struct lws* wsi, void* in, int len)
{
    LOGE();
    int16_t code;
    char* msg = nullptr;

    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        if (len > 2) {
            code = ntohs(*(int16_t*)in);
            msg = (char*)in + sizeof(code);
            std::string cp(msg, len - sizeof(code));
            itr->second->onCloseInit(code, cp);
        }
    }
}

void WebSocketServer::onClientReceive(struct lws* wsi, void* in, int len)
{
    LOGE();
    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        itr->second->onDataReceive(in, len);
    }
}
int WebSocketServer::onClientWritable(struct lws* wsi)
{
    LOGE();
    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        return itr->second->onDrainData();
    }
    return 0;
}

void WebSocketServer::onClientHTTP(struct lws* wsi)
{
    LOGE();
    auto itr = _conns.find(wsi);
    if (itr != _conns.end()) {
        itr->second->onHTTP();
    }
}

Connection::Connection(struct lws* wsi) : _wsi(wsi)
{
    LOGE();
    uv_loop_t* loop = lws_uv_getloop(lws_get_context(wsi), 0);
    async_init(loop, &_async);
}


Connection::~Connection()
{
    LOGE();
    //uv_close((uv_handle_t*)&_async, nullptr);
}

bool Connection::send(std::shared_ptr<DataFrag> data)
{
    {
        std::lock_guard<std::mutex> guard(_sendQueueMtx);
        _sendQueue.emplace_back(data);
    }
    onDrainData();
    return true;
}


bool Connection::sendText(const std::string& text, std::function<void(const std::string&)> callback)
{
    LOGE();
    std::shared_ptr<DataFrag> data = std::make_shared<DataFrag>(text);
    if (callback) {
        DISPATCH_CALLBACK_IN_GAME_LOOP();
    }
    send(data);
    return true;
}

bool Connection::sendTextAsync(const std::string& text, std::function<void(const std::string&)> callback)
{
    LOGE();
    std::shared_ptr<DataFrag> data = std::make_shared<DataFrag>(text);
    if (callback) {
        DISPATCH_CALLBACK_IN_GAME_LOOP();
    }
    schedule_async_task(&_async, [this, data]() {
        this->send(data);
        });

    return true;
}

bool Connection::sendBinary(const void* in, size_t len, std::function<void(const std::string&)> callback)
{
    LOGE();
    std::shared_ptr<DataFrag> data = std::make_shared<DataFrag>(in, len);
    if (callback) {
        DISPATCH_CALLBACK_IN_GAME_LOOP();
    }
    send(data);
    return true;
}


bool Connection::sendBinaryAsync(const void* in, size_t len, std::function<void(const std::string&)> callback)
{
    LOGE();
    std::shared_ptr<DataFrag> data = std::make_shared<DataFrag>(in, len);
    if (callback) {
        DISPATCH_CALLBACK_IN_GAME_LOOP();
    }
    schedule_async_task(&_async, [this, data]() {
        this->send(data);
        });

    return true;
}


bool Connection::close(int code, std::string message)
{
    LOGE();
    if (!_wsi) return false;
    _readyState = ReadyState::CLOSING;
    _closeReason = message;
    _closeCode = code;
    setClosed();
    lws_callback_on_writable(_wsi);
    return true;
}


bool Connection::closeAsync(int code, std::string message)
{
    LOGE();
    _readyState = ReadyState::CLOSING;
    if (!_wsi) return false;
    schedule_async_task(&_async, [this, code, message]() {
        this->close(code, message);
        });
    return true;
}

void Connection::onConnected()
{
    _readyState = ReadyState::OPEN;
    RUN_IN_GAMETHREAD(if(_onconnect)_onconnect());
}

void Connection::onDataReceive(void* in, int len)
{
    LOGE();

    bool isFinal = lws_is_final_fragment(_wsi);
    bool isBinary = lws_frame_is_binary(_wsi);

    if (!_prevPkg) {
        _prevPkg = std::make_shared<DataFrag>(in, len, isBinary);
    }
    else {
        _prevPkg->append((unsigned char*)in, len);
    }

    if (isFinal) {
        //trigger event
        std::shared_ptr<DataFrag> fullpkg = _prevPkg;
        if (isBinary)
        {
            RUN_IN_GAMETHREAD(if(_onbinary)_onbinary(fullpkg));
        }
        if (!isBinary)
        {
            RUN_IN_GAMETHREAD(if(_ontext)_ontext(fullpkg));
        }

        RUN_IN_GAMETHREAD(if(_ondata)_ondata(fullpkg));
        
        _prevPkg.reset();
    }
}



int Connection::onDrainData()
{
    LOGE();
    if (!_wsi) return -1;
    if (_closed) return -1;
    if (_readyState == ReadyState::CLOSING) {
        return -1;
    }
    if (_readyState != ReadyState::OPEN) return 0;
    unsigned char* p = nullptr;
    int send_len = 0;
    int finish_len = 0;
    int flags = 0;

    std::vector<char> buff(SEND_BUFF + LWS_PRE);

    if (!_sendQueue.empty())
    {
        std::shared_ptr<DataFrag> frag = _sendQueue.front();

        send_len = frag->slice(&p, SEND_BUFF);

        if(frag->isFront())
        {
            if (frag->isBinary()) 
            {
                flags |= LWS_WRITE_BINARY;
            }
            if (frag->isString())  
            {
                flags |= LWS_WRITE_TEXT;
            }
        }

        if (frag->remain() != send_len)
        {
            // remain bytes > 0
            // not FIN
            flags |= LWS_WRITE_NO_FIN;
        }

        if (!frag->isFront()) {
            flags |= LWS_WRITE_CONTINUATION;
        }

        finish_len = lws_write(_wsi, p, send_len, (lws_write_protocol)flags);

        if (finish_len == 0)
        {
            frag->onFinish("Connection Closed");
            return -1; // connection closed? can cause mainloop exit
            //return 0; 
        }
        else if (finish_len < 0)
        {
            frag->onFinish("Send Error!");
            return -1; // connection closed? can cause mainloop exit
            //return 0;
        }
        else
        {
            frag->consume(finish_len);
        }


        if (frag->remain() == 0) {
            frag->onFinish("");
            _sendQueue.pop_front();
        }
        lws_callback_on_writable(_wsi);
    }

    return 0;

}

void Connection::onHTTP()
{
    if (!_wsi) return;

    _headers.clear();

    _readyState = ReadyState::CONNECTING;

    int n = 0, len;
    std::vector<char> buf(256);
    const char* c;
    do {

        lws_token_indexes idx = static_cast<lws_token_indexes>(n);
        c = (const char*)lws_token_to_string(idx);
        if (!c) {
            n++;
            continue;
        }
        len = lws_hdr_total_length(_wsi, idx);
        if (!len) {
            n++;
            continue;
        }
        else if (len + 1 > buf.size())
        {
            buf.resize(len + 1);
        }
        lws_hdr_copy(_wsi, buf.data(), buf.size(), idx);
        buf[len + 1] = '\0';
        _headers.emplace(std::string(c), std::string(buf.data()));
        n++;
    } while (c);

}

void Connection::onCloseInit(int code, const std::string& msg)
{
    _closeCode = code;
    _closeReason = msg;
}

void Connection::setClosed()
{
    if (_closed) return;
    lws_close_reason(_wsi, (lws_close_status)_closeCode, (unsigned char*)_closeReason.c_str(), _closeReason.length());
    _closed = true;
}

void Connection::finallyClosed()
{
    _readyState = ReadyState::CLOSED;
    //on wsi destroied
    if (_wsi)
    {
        RUN_IN_GAMETHREAD(if(_onclose)_onclose(_closeCode, _closeReason));
        RUN_IN_GAMETHREAD(if(_onend) _onend());
    }
}


std::vector<std::string> Connection::getProtocols() {
    std::vector<std::string> ret;
    if (_wsi) {
        //TODO 
        //const struct lws_protocols* protos = lws_get_protocol(_wsi);
        //while (protos && protos->name != nullptr)
        //{
        //    ret.emplace_back(protos->name);
        //    protos++;
        //}
    }
    return ret;
}

std::map<std::string, std::string> Connection::getHeaders()
{
    if (!_wsi) return {};
    return _headers;
}


int WebSocketServer::websocket_server_callback(struct lws* wsi, enum lws_callback_reasons reason,
    void* user, void* in, size_t len)
{
    int ret = 0;
    WebSocketServer* server = nullptr;
    lws_context* ctx = nullptr;

    if (wsi)
        ctx = lws_get_context(wsi);
    if (ctx)
        server = static_cast<WebSocketServer*>(lws_context_user(ctx));

    if (!server) {
        return 0;
    }
    

    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        break;
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        break;
    case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
        break;
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        break;
    case LWS_CALLBACK_CLOSED:
        break;
    case LWS_CALLBACK_CLOSED_HTTP:
        break;
    case LWS_CALLBACK_RECEIVE:
        server->onClientReceive(wsi, in, len);
        break;
    case LWS_CALLBACK_RECEIVE_PONG:
        break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
        server->onClientReceive(wsi, in, len);
        break;
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
        break;
    case LWS_CALLBACK_CLIENT_WRITEABLE:
        ret = server->onClientWritable(wsi);
        break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
        ret = server->onClientWritable(wsi);
        break;
    case LWS_CALLBACK_HTTP:
        //server->onClientHTTP(wsi);
        break;
    case LWS_CALLBACK_HTTP_BODY:
        break;
    case LWS_CALLBACK_HTTP_BODY_COMPLETION:
        break;
    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
        break;
    case LWS_CALLBACK_HTTP_WRITEABLE:
        break;
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        break;
    case LWS_CALLBACK_FILTER_HTTP_CONNECTION:
        break;
    case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
        break;
    case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        break;
    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
        break;
    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS:
        break;
    case LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION:
        break;
    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        break;
    case LWS_CALLBACK_CONFIRM_EXTENSION_OKAY:
        break;
    case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
        break;
    case LWS_CALLBACK_PROTOCOL_INIT:
        break;
    case LWS_CALLBACK_PROTOCOL_DESTROY:
        break;
    case LWS_CALLBACK_WSI_CREATE:
        server->onCreateClient(wsi);
        break;
    case LWS_CALLBACK_WSI_DESTROY:
        server->onDestroyClient(wsi);
        break;
    case LWS_CALLBACK_GET_THREAD_ID:
        break;
    case LWS_CALLBACK_ADD_POLL_FD:
        break;
    case LWS_CALLBACK_DEL_POLL_FD:
        break;
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
        break;
    case LWS_CALLBACK_LOCK_POLL:
        break;
    case LWS_CALLBACK_UNLOCK_POLL:
        break;
    case LWS_CALLBACK_OPENSSL_CONTEXT_REQUIRES_PRIVATE_KEY:
        break;
    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        server->onCloseClientInit(wsi, in, len);
        break;
    case LWS_CALLBACK_WS_EXT_DEFAULTS:
        break;
    case LWS_CALLBACK_CGI:
        break;
    case LWS_CALLBACK_CGI_TERMINATED:
        break;
    case LWS_CALLBACK_CGI_STDIN_DATA:
        break;
    case LWS_CALLBACK_CGI_STDIN_COMPLETED:
        break;
    case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
        break;
    case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
        server->onCloseClient(wsi);
        break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
        break;
    case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
        break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
        break;
    case LWS_CALLBACK_HTTP_BIND_PROTOCOL:
        break;
    case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
        break;
    case LWS_CALLBACK_CHECK_ACCESS_RIGHTS:
        break;
    case LWS_CALLBACK_PROCESS_HTML:
        break;
    case LWS_CALLBACK_ADD_HEADERS:
        break;
    case LWS_CALLBACK_SESSION_INFO:
        break;
    case LWS_CALLBACK_GS_EVENT:
        break;
    case LWS_CALLBACK_HTTP_PMO:
        break;
    case LWS_CALLBACK_CLIENT_HTTP_WRITEABLE:
        break;
    case LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION:
        break;
    case LWS_CALLBACK_RAW_RX:
        break;
    case LWS_CALLBACK_RAW_CLOSE:
        break;
    case LWS_CALLBACK_RAW_WRITEABLE:
        break;
    case LWS_CALLBACK_RAW_ADOPT:
        break;
    case LWS_CALLBACK_RAW_ADOPT_FILE:
        break;
    case LWS_CALLBACK_RAW_RX_FILE:
        break;
    case LWS_CALLBACK_RAW_WRITEABLE_FILE:
        break;
    case LWS_CALLBACK_RAW_CLOSE_FILE:
        break;
    case LWS_CALLBACK_SSL_INFO:
        break;
    case LWS_CALLBACK_CHILD_WRITE_VIA_PARENT:
        break;
    case LWS_CALLBACK_CHILD_CLOSING:
        break;
    case LWS_CALLBACK_CGI_PROCESS_ATTACH:
        break;
    case LWS_CALLBACK_USER:
        break;
    default:
        break;
    }
    return ret;
}

#ifndef WSS_EXTERNAL_TEST_BUILD
} // namespace network
} // namespace cocos2d
#endif
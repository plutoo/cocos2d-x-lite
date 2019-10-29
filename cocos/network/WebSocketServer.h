#pragma once




#ifndef EXT_BUILD
#include "platform/CCPlatformDefine.h"
#endif

#include "uv.h"
#include "libwebsockets.h"

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <map>
#include <list>
#include <unordered_map>
#include <thread>
#include <mutex>



#ifndef EXT_BUILD

namespace cocos2d {

    namespace network {

#else 

#define CC_DLL 

#endif

class Connection;
class WebSocketServer;

class DataFrag {
public:

    DataFrag(const std::string& data);

    DataFrag(const void* data, int len, bool isBinary = true);

    virtual ~DataFrag();

    void append(unsigned char* p, int len);

    int slice(unsigned char** p, int len);

    int consume(int len);

    int remain() const;

    inline bool isBinary() const { return _isBinary; }
    inline bool isString() const { return !_isBinary; }
    inline bool isFront() const { return _consumed == 0; }

    void setCallback(std::function<void(const std::string&)> callback)
    {
        _callback = callback;
    }

    void onFinish(const std::string& message) {
        if (_callback) {
            _callback(message);
        }
    }

    inline int size() const { return _underlying_data.size() - LWS_PRE; }

    std::string toString();

private:

    unsigned char* getData() { return _underlying_data.data() + LWS_PRE; }

    std::vector<unsigned char> _underlying_data;
    int _consumed = 0;
    bool _isBinary = false;
    std::function<void(const std::string&) > _callback;

};


class CC_DLL Connection {
public:

    Connection(struct lws* wsi);
    virtual ~Connection();

    enum class Event {
        close,
        error,
        text,
        binary,
        connect,
        //pong

    };

    bool send(std::shared_ptr<DataFrag> data);

    bool sendText(const std::string&, std::function<void(const std::string&)> callback = nullptr);
    bool sendTextAsync(const std::string&, std::function<void(const std::string&)> callback = nullptr);

    bool sendBinary(const void*, size_t len, std::function<void(const std::string&)> callback = nullptr);
    bool sendBinaryAsync(const void*, size_t len, std::function<void(const std::string&)> callback = nullptr);

    /** stream is not implemented*/
    //bool beginBinary();

    /** should implement in JS */
    // bool send();
    // bool sendPing(std::string)

    bool close(int code = 1000, std::string reasson = "close normal");
    bool closeAsync(int code = 1000, std::string reasson = "close normal");


    int getReadyState();

    /** not use full */
    //int getSocket();

    std::shared_ptr<WebSocketServer>& getServer();

    std::string& getPath();

    std::map<std::string, std::string> getHeaders();

    std::vector<std::string> getProtocols();

    inline void setOnClose(std::function<void(int, const std::string&)> cb)
    {
        _onclose = cb;
    }

    inline void setOnError(std::function<void(const std::string&)> cb)
    {
        _onerror = cb;
    }

    inline void setOnText(std::function<void(std::shared_ptr<DataFrag>)> cb)
    {
        _ontext = cb;
    }

    inline void setOnBinary(std::function<void(std::shared_ptr<DataFrag>)> cb)
    {
        _onbinary = cb;
    }

    inline void setOnConnect(std::function<void()> cb)
    {
        _onconnect = cb;
    }

    inline void scheduleSend() {
        if (_wsi) {
            lws_callback_on_writable(_wsi);
        }
    }


    void setClosed();

private:

    void onConnected();
    void onDataReceive(void* in, int len);
    int onDrainData();
    void onHTTP();
    void onCloseInit(int code, const std::string& msg);

    void finallyClosed();

    struct lws* _wsi = nullptr;
    std::map<std::string, std::string> _headers;
    std::mutex _sendQueueMtx;
    std::list<std::shared_ptr<DataFrag>> _sendQueue;
    std::list<std::shared_ptr<DataFrag>> _recvQueus;
    std::shared_ptr<DataFrag> _prevPkg;
    bool _closed = false;
    std::string _closeReason = "close connection";
    int         _closeCode = 1000;


    // Attention: do not reference **this** in callbacks
    std::function<void(int, const std::string&)> _onclose;
    std::function<void(const std::string&)> _onerror;
    std::function<void(std::shared_ptr<DataFrag>)> _ontext;
    std::function<void(std::shared_ptr<DataFrag>)> _onbinary;
    std::function<void()> _onconnect;
    //std::function<void()> _onpong;

    friend class WebSocketServer;


    uv_async_t _async;

};

class CC_DLL WebSocketServer {
public:

    WebSocketServer() = default;

    virtual ~WebSocketServer();

    enum class Event {
        listening,
        close,
        error,
        connection
    };

    bool listen(int port, const std::string& host = "", std::function<void(const std::string & errorMsg)> callback = nullptr);
    bool close(std::function<void(const std::string & errorMsg)> callback = nullptr);

    bool listenAsync(int port, const std::string& host = "", std::function<void(const std::string & errorMsg)> callback = nullptr);
    bool closeAsync(std::function<void(const std::string & errorMsg)> callback = nullptr);

    std::vector<std::shared_ptr<Connection>> getConnections() const;

    void setOnListening(std::function<void(const std::string&)> cb)
    {
        _onlistening = cb;
    }

    void setOnError(std::function<void(const std::string&)> cb)
    {
        _onerror = cb;
    }

    void setOnClose(std::function<void(const std::string&)> cb)
    {
        _onclose = cb;
    }

    void setOnConnection(std::function<void(std::shared_ptr<Connection>)> cb)
    {
        _onconnection = cb;
    }

protected:

    void onCreateClient(struct lws* wsi);
    void onDestroyClient(struct lws* wsi);
    void onCloseClient(struct lws* wsi);
    void onCloseClientInit(struct lws* wsi, void* in, int len);
    void onClientReceive(struct lws* wsi, void* in, int len);
    int onClientWritable(struct lws* wsi);
    void onClientHTTP(struct lws* wsi);
private:

    std::string _host;
    lws_context* _ctx = nullptr;
    std::unordered_map<struct lws*, std::shared_ptr<Connection> > _conns;

    // Attention: do not reference **this** in callbacks

    std::function<void(const std::string&)> _onlistening;
    std::function<void(const std::string&)> _onerror;
    std::function<void(const std::string&)> _onclose;
    std::function<void(std::shared_ptr<Connection>)> _onconnection;

    std::thread* _subthread = nullptr;

    uv_async_t _async;
    bool _booted = false;

    friend int websocket_server_callback(struct lws* wsi, enum lws_callback_reasons reason,
        void* user, void* in, size_t len);
};


#ifndef EXT_BUILD
    }
}
#endif


set(CC_SCRIPTING_SOURCES
    scripting/js-bindings/event/CustomEventTypes.h
    scripting/js-bindings/event/EventDispatcher.h
    scripting/js-bindings/jswrapper/SeApi.h
    scripting/js-bindings/manual/JavaScriptObjCBridge.h
    scripting/js-bindings/manual/jsb_cocos2dx_network_manual.h
    scripting/js-bindings/manual/jsb_global.h
    scripting/js-bindings/manual/jsb_platform.h
    scripting/js-bindings/jswrapper/config.hpp
    scripting/js-bindings/jswrapper/Value.hpp
    scripting/js-bindings/jswrapper/Object.hpp
    scripting/js-bindings/jswrapper/HandleObject.hpp
    scripting/js-bindings/jswrapper/State.hpp
    scripting/js-bindings/jswrapper/MappingUtils.hpp

    scripting/js-bindings/auto/jsb_cocos2dx_extension_auto.hpp
    scripting/js-bindings/auto/jsb_video_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_dragonbones_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_spine_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.hpp
    scripting/js-bindings/auto/jsb_gfx_auto.hpp
    scripting/js-bindings/auto/jsb_webview_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_audioengine_auto.hpp
    scripting/js-bindings/auto/jsb_cocos2dx_network_auto.hpp
    scripting/js-bindings/auto/jsb_renderer_auto.hpp

    scripting/js-bindings/manual/jsb_cocos2dx_manual.hpp
    scripting/js-bindings/manual/jsb_websocket.hpp
    scripting/js-bindings/manual/jsb_spine_manual.hpp
    scripting/js-bindings/manual/jsb_helper.hpp
    scripting/js-bindings/manual/jsb_renderer_manual.hpp
    scripting/js-bindings/manual/jsb_cocos2dx_extension_manual.hpp
    scripting/js-bindings/manual/jsb_xmlhttprequest.hpp
    scripting/js-bindings/manual/jsb_gfx_manual.hpp
    scripting/js-bindings/manual/jsb_opengl_manual.hpp
    scripting/js-bindings/manual/jsb_classtype.hpp
    scripting/js-bindings/manual/jsb_module_register.hpp
    scripting/js-bindings/manual/jsb_conversions.hpp
    scripting/js-bindings/manual/jsb_socketio.hpp
    scripting/js-bindings/manual/jsb_opengl_utils.hpp
    scripting/js-bindings/manual/jsb_node.hpp
    scripting/js-bindings/manual/jsb_dragonbones_manual.hpp

    scripting/js-bindings/auto/jsb_cocos2dx_auto.cpp
    scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.cpp
    scripting/js-bindings/auto/jsb_cocos2dx_spine_auto.cpp
    scripting/js-bindings/auto/jsb_cocos2dx_extension_auto.cpp
    scripting/js-bindings/auto/jsb_cocos2dx_network_auto.cpp
    scripting/js-bindings/auto/jsb_gfx_auto.cpp
    scripting/js-bindings/auto/jsb_renderer_auto.cpp
    scripting/js-bindings/event/EventDispatcher.cpp
    scripting/js-bindings/jswrapper/config.cpp
    scripting/js-bindings/jswrapper/HandleObject.cpp
    scripting/js-bindings/jswrapper/MappingUtils.cpp
    scripting/js-bindings/jswrapper/RefCounter.cpp
    scripting/js-bindings/jswrapper/State.cpp

    scripting/js-bindings/jswrapper/Value.cpp
    scripting/js-bindings/manual/jsb_classtype.cpp
    #scripting/js-bindings/manual/jsb_cocos2dx_extension_manual.cpp
    scripting/js-bindings/manual/jsb_cocos2dx_manual.cpp
    scripting/js-bindings/manual/jsb_cocos2dx_network_manual.cpp
    scripting/js-bindings/manual/jsb_conversions.cpp
    scripting/js-bindings/manual/jsb_gfx_manual.cpp
    scripting/js-bindings/manual/jsb_global.cpp
    scripting/js-bindings/manual/jsb_helper.cpp
    scripting/js-bindings/manual/jsb_module_register.cpp
    #scripting/js-bindings/manual/jsb_node.cpp
    scripting/js-bindings/manual/jsb_spine_manual.cpp
    scripting/js-bindings/manual/jsb_opengl_manual.cpp
    scripting/js-bindings/manual/jsb_opengl_utils.cpp
    scripting/js-bindings/manual/jsb_platfrom_win32.cpp
    scripting/js-bindings/manual/jsb_renderer_manual.cpp
    scripting/js-bindings/manual/jsb_xmlhttprequest.cpp
)

if(USE_SE_V8) 
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/jswrapper/v8/Base.h
        scripting/js-bindings/jswrapper/v8/HelperMacros.h
        scripting/js-bindings/jswrapper/v8/ObjectWrap.h
        scripting/js-bindings/jswrapper/v8/SeApi.h
        scripting/js-bindings/jswrapper/v8/Class.cpp
        scripting/js-bindings/jswrapper/v8/debugger/SHA1.cpp
        scripting/js-bindings/jswrapper/v8/Object.cpp
        scripting/js-bindings/jswrapper/v8/ObjectWrap.cpp
        scripting/js-bindings/jswrapper/v8/ScriptEngine.cpp
        scripting/js-bindings/jswrapper/v8/Utils.cpp
    )
endif()

if(USE_SE_SM)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/jswrapper/sm/Base.h
        scripting/js-bindings/jswrapper/sm/Class.cpp
        scripting/js-bindings/jswrapper/sm/Class.hpp
        scripting/js-bindings/jswrapper/sm/HelperMacros.h
        scripting/js-bindings/jswrapper/sm/Object.cpp
        scripting/js-bindings/jswrapper/sm/Object.hpp
        scripting/js-bindings/jswrapper/sm/ScriptEngine.cpp
        scripting/js-bindings/jswrapper/sm/ScriptEngine.hpp
        scripting/js-bindings/jswrapper/sm/SeApi.h
        scripting/js-bindings/jswrapper/sm/Utils.cpp
        scripting/js-bindings/jswrapper/sm/Utils.hpp
    )
endif()

if(USE_SE_JSC)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/jswrapper/jsc/Base.h
        scripting/js-bindings/jswrapper/jsc/Class.cpp
        scripting/js-bindings/jswrapper/jsc/Class.hpp
        scripting/js-bindings/jswrapper/jsc/EJConvert.h
        scripting/js-bindings/jswrapper/jsc/EJConvert.m
        scripting/js-bindings/jswrapper/jsc/EJConvertTypedArray.h
        scripting/js-bindings/jswrapper/jsc/EJConvertTypedArray.m
        scripting/js-bindings/jswrapper/jsc/HelperMacros.h
        scripting/js-bindings/jswrapper/jsc/Object.hpp
        scripting/js-bindings/jswrapper/jsc/Object.mm
        scripting/js-bindings/jswrapper/jsc/PlatformUtils.h
        scripting/js-bindings/jswrapper/jsc/PlatformUtils.mm
        scripting/js-bindings/jswrapper/jsc/ScriptEngine.hpp
        scripting/js-bindings/jswrapper/jsc/ScriptEngine.mm
        scripting/js-bindings/jswrapper/jsc/SeApi.h
        scripting/js-bindings/jswrapper/jsc/Utils.cpp
        scripting/js-bindings/jswrapper/jsc/Utils.hpp
    )
endif()

if(USE_SE_V8 AND USE_V8_DEBUGGER)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/jswrapper/v8/debugger/base64.h
        scripting/js-bindings/jswrapper/v8/debugger/env.h
        scripting/js-bindings/jswrapper/v8/debugger/http_parser.h
        scripting/js-bindings/jswrapper/v8/debugger/inspector_agent.h
        scripting/js-bindings/jswrapper/v8/debugger/inspector_io.h
        scripting/js-bindings/jswrapper/v8/debugger/inspector_socket.h
        scripting/js-bindings/jswrapper/v8/debugger/inspector_socket_server.h
        scripting/js-bindings/jswrapper/v8/debugger/node.h
        scripting/js-bindings/jswrapper/v8/debugger/node_debug_options.h
        scripting/js-bindings/jswrapper/v8/debugger/node_mutex.h
        scripting/js-bindings/jswrapper/v8/debugger/SHA1.h
        scripting/js-bindings/jswrapper/v8/debugger/util-inl.h
        scripting/js-bindings/jswrapper/v8/debugger/util.h
        scripting/js-bindings/jswrapper/v8/debugger/v8_inspector_protocol_json.h
        scripting/js-bindings/jswrapper/v8/debugger/SHA1.cpp 
        scripting/js-bindings/jswrapper/v8/debugger/util.cc 
        scripting/js-bindings/jswrapper/v8/debugger/env.cc 
        scripting/js-bindings/jswrapper/v8/debugger/inspector_agent.cc 
        scripting/js-bindings/jswrapper/v8/debugger/inspector_io.cc 
        scripting/js-bindings/jswrapper/v8/debugger/inspector_socket.cc 
        scripting/js-bindings/jswrapper/v8/debugger/inspector_socket_server.cc 
        scripting/js-bindings/jswrapper/v8/debugger/node.cc 
        scripting/js-bindings/jswrapper/v8/debugger/node_debug_options.cc 
        scripting/js-bindings/jswrapper/v8/debugger/http_parser.c
    )
endif()


if(USE_AUDIO) 
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/auto/jsb_cocos2dx_audioengine_auto.cpp
    )
endif()

if(USE_SOCKET)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/manual/jsb_socketio.cpp
        scripting/js-bindings/manual/jsb_websocket.cpp
    )
    if(USE_WEBSOCKET_SERVER)
        list(APPEND CC_SCRIPTING_SOURCES
            scripting/js-bindings/manual/jsb_websocket_server.hpp
            scripting/js-bindings/manual/jsb_websocket_server.cpp
            )
    endif()
endif()

if(USE_MIDDLEWARE)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.cpp
    )
endif()

if(USE_MIDDLEWARE AND USE_SPINE)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/manual/jsb_spine_manual.cpp
        scripting/js-bindings/auto/jsb_cocos2dx_spine_auto.cpp
    )
endif()

if(USE_MIDDLEWARE AND USE_DRAGONBONES)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/manual/jsb_dragonbones_manual.cpp
        scripting/js-bindings/auto/jsb_cocos2dx_dragonbones_auto.cpp
    )
endif()

if(ANDROID OR IOS)
    if(USE_VIDEO)
        list(APPEND CC_SCRIPTING_SOURCES
            scripting/js-bindings/auto/jsb_video_auto.cpp
        )
    endif()
    if(USE_WEB_VIEW)
        list(APPEND CC_SCRIPTING_SOURCES
            scripting/js-bindings/auto/jsb_webview_auto.cpp
        )
    endif()
else()
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/auto/jsb_video_auto.cpp
        scripting/js-bindings/auto/jsb_webview_auto.cpp
    )
endif()


if(ANDROID)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/manual/JavaScriptJavaBridge.h
        scripting/js-bindings/manual/jsb_platform_android.cpp
        scripting/js-bindings/manual/JavaScriptJavaBridge.cpp
    )
elseif(WINDOWS)

elseif(APPLE)
    list(APPEND CC_SCRIPTING_SOURCES
        scripting/js-bindings/manual/JavaScriptObjCBridge.h
        scripting/js-bindings/manual/JavaScriptObjCBridge.mm
    )
endif()



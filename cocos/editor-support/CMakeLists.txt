

set(EDITORLIB editorsupport)

set(CC_EDITORSUPPORT_SOURCES 
    editor-support/../scripting/js-bindings/manual/jsb_helper.cpp 
    editor-support/IOBuffer.cpp 
    editor-support/RenderInfoMgr.cpp 
    editor-support/MeshBuffer.cpp 
    editor-support/middleware-adapter.cpp 
    editor-support/TypedArrayPool.cpp 
    editor-support/IOTypedArray.cpp 
    editor-support/MiddlewareManager.cpp 
    editor-support/../scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.cpp
)

if(USE_SPINE)

list(APPEND CC_EDITORSUPPORT_SOURCES 
    editor-support/spine-creator-support/AttachmentVertices.cpp
    editor-support/spine-creator-support/CreatorAttachmentLoader.cpp
    editor-support/spine-creator-support/SpineAnimation.cpp
    editor-support/spine-creator-support/SpineRenderer.cpp
    editor-support/spine-creator-support/spine-cocos2dx.cpp
    editor-support/spine-creator-support/SkeletonDataMgr.cpp
    )

    include(editor-support/spine/CMakeLists.txt)
    list(APPEND CC_EDITORSUPPORT_SOURCES
        ${SPINE_SRC_FILES}
    )

endif()

if(USE_DRAGONBONES)
list(APPEND CC_EDITORSUPPORT_SOURCES
    editor-support/dragonbones/animation/Animation.cpp
    editor-support/dragonbones/animation/AnimationState.cpp
    editor-support/dragonbones/animation/BaseTimelineState.cpp
    editor-support/dragonbones/animation/TimelineState.cpp
    editor-support/dragonbones/animation/WorldClock.cpp
    editor-support/dragonbones/armature/Armature.cpp
    editor-support/dragonbones/armature/Bone.cpp
    editor-support/dragonbones/armature/Constraint.cpp
    editor-support/dragonbones/armature/DeformVertices.cpp
    editor-support/dragonbones/armature/Slot.cpp
    editor-support/dragonbones/armature/TransformObject.cpp
    editor-support/dragonbones/core/BaseObject.cpp
    editor-support/dragonbones/core/DragonBones.cpp
    editor-support/dragonbones/event/EventObject.cpp
    editor-support/dragonbones/factory/BaseFactory.cpp
    editor-support/dragonbones/geom/Point.cpp
    editor-support/dragonbones/geom/Transform.cpp
    editor-support/dragonbones/model/AnimationConfig.cpp
    editor-support/dragonbones/model/AnimationData.cpp
    editor-support/dragonbones/model/ArmatureData.cpp
    editor-support/dragonbones/model/BoundingBoxData.cpp
    editor-support/dragonbones/model/CanvasData.cpp
    editor-support/dragonbones/model/ConstraintData.cpp
    editor-support/dragonbones/model/DisplayData.cpp
    editor-support/dragonbones/model/DragonBonesData.cpp
    editor-support/dragonbones/model/SkinData.cpp
    editor-support/dragonbones/model/TextureAtlasData.cpp
    editor-support/dragonbones/model/UserData.cpp
    editor-support/dragonbones/parser/DataParser.cpp
    editor-support/dragonbones/parser/BinaryDataParser.cpp
    editor-support/dragonbones/parser/JSONDataParser.cpp
    editor-support/dragonbones-creator-support/CCArmatureDisplay.cpp
    editor-support/dragonbones-creator-support/CCFactory.cpp
    editor-support/dragonbones-creator-support/CCSlot.cpp
    editor-support/dragonbones-creator-support/CCTextureAtlasData.cpp
)
endif()

#include "CCTTFLabelAtlasCache.h"

#include "platform/CCFileUtils.h"

#include <cmath>
#include "base/ccConfig.h"
#if CC_ENABLE_TTF_LABEL_RENDERER

#define FTT_MAP_FONT_SIZE 0
#define FTT_USE_FONT_MAP_SIZE_POWEROFTWO 0

#define FTT_TEXTURE_SIZE 1024

namespace cocos2d {

    namespace {
        TTFLabelAtlasCache *_instance = nullptr;


        /**
        * Labels of different size can share a same FontAtlas by mapping font size.
        */
#if FTT_MAP_FONT_SIZE
        inline int mapFontSize(float x) {
#if FTT_USE_FONT_MAP_SIZE_POWEROFTWO
            // round to power of 2, which waste lots of memory?
            return 1 << (int)(::ceil(::log(x) / ::log(2.0))); 
#else
            if (x <= 8.0f) return 8;
            else if (x <= 16.0f) return 16;
            else if (x <= 32.0f) return 32;
            else if (x <= 48.0f) return 48;
            return (int)x;
#endif
        }
#else
#define mapFontSize(f) (int)(f)
#endif
    }


    TTFLabelAtals::TTFLabelAtals(const std::string &fontPath, float fontSize, LabelLayoutInfo *info)
        :_fontName(fontPath), _fontSize(fontSize), _info(info)
    {
        init();
    }


    void TTFLabelAtals::init()
    {
        assert(FileUtils::getInstance()->isFileExist(_fontName));
        _ttfFont = std::make_shared<FontFreeType>(_fontName, _fontSize, _info);
        _ttfFont->loadFont();
        _fontAtlas = std::make_shared<FontAtlas>(PixelMode::A8, FTT_TEXTURE_SIZE, FTT_TEXTURE_SIZE, _info->outlineSize > 0 || _info->bold);
        _fontAtlas->init();
    }

    TTFLabelAtlasCache * TTFLabelAtlasCache::getInstance()
    {
        if (!_instance)
        {
            _instance = new TTFLabelAtlasCache();
        }
        return _instance;
    }

    void TTFLabelAtlasCache::destroyInstance()
    {
        delete _instance;
        _instance = nullptr;
    }

    void TTFLabelAtlasCache::reset()
    {
        _cache.clear();
    }

    std::shared_ptr<TTFLabelAtals> TTFLabelAtlasCache::load(const std::string &font, float fontSizeF, LabelLayoutInfo *info)
    {
        int fontSize = mapFontSize(fontSizeF);
        std::string keybuffer = cacheKeyFor(font, fontSize, info);
#if CC_TTF_LABELATLAS_ENABLE_GC
        std::weak_ptr<TTFLabelAtals> &atlasWeak= _cache[keybuffer];
        std::shared_ptr<TTFLabelAtals> atlas = atlasWeak.lock();
        if (!atlas)
        {
            atlas = std::make_shared<TTFLabelAtals>(font, fontSize, info);
            atlasWeak = atlas;
        }
#else
        std::shared_ptr<TTFLabelAtals> &atlas = _cache[keybuffer];
        if (!atlas)
        {
            atlas = std::make_shared<TTFLabelAtals>(font, fontSize, info);
        }
#endif
        return atlas;
    }


    void TTFLabelAtlasCache::unload(TTFLabelAtals *atlas)
    {
        int fontSize = mapFontSize(atlas->_fontSize);
        std::string key = cacheKeyFor(atlas->_fontName, fontSize, atlas->_info);
        _cache.erase(key);
    }


    std::string TTFLabelAtlasCache::cacheKeyFor(const std::string &font, int fontSize, LabelLayoutInfo * info)
    {
        char keybuffer[512] = { 0 };
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(font);

        //NOTICE: fontpath may be too long
        snprintf(keybuffer, 511, "s:%d/sdf:%d/p:%s", fontSize, info->outlineSize > 0 || info->bold, fullPath.c_str());
        return keybuffer;
    }
}

#endif
#ifndef __IsometricTilemap_h__
#define __IsometricTilemap_h__

#include "Header.h"

class TileLayer;
class IsometricTilemap : public IIsometricTilemap {
public:
    virtual ~IsometricTilemap();

    static IsometricTilemap* CreateWithXml(const std::string& path);
    static void Release(IsometricTilemap* map);

    const std::unordered_map<std::string, ITileLayer* const>& Layers() const override { return _Layers; }
    ITileLayer* GetLayer(const std::string& name) const override;

    __forceinline void AddLayer(const std::string& layerName, ITileLayer* layer) {
        _Layers.insert({ layerName, layer });
    }

private:
    IsometricTilemap(const eTileType type, const std::string& name);

private:
    std::unordered_map<std::string, ITileLayer* const> _Layers;
};

#endif // __IsometricTilemap_h__

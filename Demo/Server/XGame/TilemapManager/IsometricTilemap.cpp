#include "IsometricTilemap.h"
#include "TileLayer.h"

IsometricTilemap::~IsometricTilemap() {
    for (auto it = _Layers.begin(); it != _Layers.end(); it++) {
        txdel it->second;
    }
}

IsometricTilemap* IsometricTilemap::CreateWithXml(const std::string& path) {
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile(path.c_str())) {
        ErrorLog(g_Engine, "Load IsometricTilemap xml %s error", path.c_str());
        return nullptr;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    std::string name = root->Attribute("Name");
    tinyxml2::XMLElement* layer = root->FirstChildElement("Layer");

    IsometricTilemap* iosmetricTilemap = txnew IsometricTilemap(eTileType::Isometric, name);

    while (nullptr != layer) {
        const std::string layerName = layer->Attribute("Name");
        const Vector2 cellSize(
            SafeString::StringToFloat(layer->Attribute("CellSizeX")),
            SafeString::StringToFloat(layer->Attribute("CellSizeY"))
        );

        std::unordered_set<Vector2Int> tileSet;
        tinyxml2::XMLElement* tile = layer->FirstChildElement("Tile");
        while (nullptr != tile) {
            Vector2Int grid(
                SafeString::StringToInt(tile->Attribute("X")),
                SafeString::StringToInt(tile->Attribute("Y"))
            );

            XASSERT(!tileSet.contains(grid), "wtf");
            tileSet.insert(grid);
            tile = tile->NextSiblingElement("Tile");
        }

        TileLayer* tileLayer = txnew TileLayer(iosmetricTilemap, layerName, cellSize, tileSet);
        iosmetricTilemap->AddLayer(layerName, tileLayer);
        layer = layer->NextSiblingElement("Layer");
    }

    return iosmetricTilemap;
}

void IsometricTilemap::Release(IsometricTilemap* map) {
    txdel map;
}

ITileLayer* IsometricTilemap::GetLayer(const std::string& name) const {
    auto itor = _Layers.find(name);
    if (_Layers.end() != itor)
        return itor->second;

    return nullptr;
}

IsometricTilemap::IsometricTilemap(const eTileType type, const std::string& name)
    : IIsometricTilemap(type, name) {

}

message(STATUS "EngineSourceGroup ${XENGINE_DIR}")

include_directories(
    "${XENGINE_DIR}/Api"
    "${XENGINE_DIR}/Api/PhysX"
    "${XENGINE_DIR}/Utils"
    "${XENGINE_DIR}/Utils/TinyXML"
    "${XENGINE_DIR}/Utils/Geometry"
)

source_group(Api FILES ${Api})
source_group(Api\\PhysX FILES ${PhysX})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\JsonCpp FILES ${JsonCpp})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils\\Excel FILES ${Excel})
source_group(Utils FILES ${Utils})

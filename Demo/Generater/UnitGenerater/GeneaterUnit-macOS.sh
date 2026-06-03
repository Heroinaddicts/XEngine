#!/bin/sh
set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

GENERATOR="$SCRIPT_DIR/UnitGenerater-macOS"
UNIT_XML_PATH="$SCRIPT_DIR/GameObjectTemplate.xml"
CPP_TEMPLATE_PATH="$SCRIPT_DIR/CppCodeTemplate"
CSHARP_TEMPLATE_PATH="$SCRIPT_DIR/CSharpCodeTemplate"
CPP_UNIT_CODE_PATH="$(cd "$SCRIPT_DIR/../../Server/XGame/Interface" && pwd)"
CSHARP_UNIT_CODE_PATH="$(cd "$SCRIPT_DIR/../../Client/Assets/GameUnit/Units" && pwd)"

if [ ! -x "$GENERATOR" ]; then
    chmod +x "$GENERATOR"
fi

echo "Generate C++ unit code to: $CPP_UNIT_CODE_PATH"
echo "Generate C# unit code to: $CSHARP_UNIT_CODE_PATH"

if [ "${1:-}" = "--dry-run" ]; then
    exit 0
fi

if [ "$#" -ne 0 ]; then
    echo "Usage: $0 [--dry-run]" >&2
    exit 1
fi

rm -f "$CSHARP_UNIT_CODE_PATH"/*.cs

"$GENERATOR" \
    --UnitXmlPath="$UNIT_XML_PATH" \
    --CppCodeTemplatePath="$CPP_TEMPLATE_PATH" \
    --CppUnitCodePath="$CPP_UNIT_CODE_PATH"

"$GENERATOR" \
    --UnitXmlPath="$UNIT_XML_PATH" \
    --CSharpCodeTemplatePath="$CSHARP_TEMPLATE_PATH" \
    --CSharpUnitCodePath="$CSHARP_UNIT_CODE_PATH"

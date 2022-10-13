#include "X3DObj.h"
#include "XFile.h"
#include "SafeString.h"

#define LINE_MAX_LEN 64
namespace XEngine {
    bool X3DObj::Load(const std::string& path) {
        XFile file;
        if (!file.Open(path)) {
            return false;
        }

        char buff[LINE_MAX_LEN] = { 0 };
        while (file.GetLine(buff, sizeof(buff))) {
            buff[LINE_MAX_LEN - 1] = 0;
            int len = strlen(buff);
            buff[len - 1] = 0;

            if (SafeString::StringStartWith(buff, "v ") && len >= 7) { // "v 0 0 0"
                std::vector<std::string> res;
                int count = SafeString::Split(buff + 2, " ", res);
                XASSERT(count == 3, "X3DObj format error %s", buff);
                if (count != 3) {
                    return false;
                }

                float x = SafeString::StringToFloat(res[0]);
                float y = SafeString::StringToFloat(res[1]);
                float z = SafeString::StringToFloat(res[2]);

                _vs.push_back(Vector3(x, y, z));
            }
            else if (SafeString::StringStartWith(buff, "vn ") && len >= 8) {  // "vn 0 0 0"
                std::vector<std::string> res;
                int count = SafeString::Split(buff + 3, " ", res);
                XASSERT(count == 3, "X3DObj format error %s", buff);
                if (count != 3) {
                    return false;
                }

                float x = SafeString::StringToFloat(res[0]);
                float y = SafeString::StringToFloat(res[1]);
                float z = SafeString::StringToFloat(res[2]);

                _vns.push_back(Vector3(x, y, z));
            }
            else if (SafeString::StringStartWith(buff, "vt ") && len >= 6) { // "vt 0 0"
                std::vector<std::string> res;
                int count = SafeString::Split(buff + 3, " ", res);
                XASSERT(count == 2, "X3DObj format error %s", buff);
                if (count != 2) {
                    return false;
                }

                float x = SafeString::StringToFloat(res[0]);
                float y = SafeString::StringToFloat(res[1]);
                _vts.push_back(Vector2(x, y));
            }
            else if (SafeString::StringStartWith(buff, "f ") && len >= 7) { // "f 0 0 0"
                std::vector<std::string> res;
                int count = SafeString::Split(buff + 2, " ", res);
                XASSERT(count >= 3, "X3DObj format error %s", buff);
                if (count < 3) {
                    return false;
                }
                std::vector<Facet> f;
                for (int i = 0; i < count; i++) {
                    Facet temp;
                    temp.u = SafeString::StringToInt(res[i]) - 1;
                    f.push_back(temp);
                }

                _fs.push_back(f);
            }
            else {
                XASSERT(false, "X3DObj format error %s", buff);
                return false;
            }
        }

        return true;
    }
}

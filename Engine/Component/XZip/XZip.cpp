#include "XZip.h"
#include "MiniZip/unzip.h"
#include "SafeString.h"
#include <windows.h>
#include <fstream>

namespace XEngine {
    struct UnzipTask {
        const std::string _File;
        const std::string _TargetFolder;
        const Api::iZipApi::fUnzipCallback _Callback;

        UnzipTask(const std::string& file, const std::string& folder, const Api::iZipApi::fUnzipCallback callback) : _File(file), _TargetFolder(folder), _Callback(callback) {}
        UnzipTask(const UnzipTask& target) : _File(target._File), _TargetFolder(target._TargetFolder), _Callback(target._Callback) {}
    };

    struct UnzipResult {
        const bool _Success;
        const UnzipTask* _Task;

        UnzipResult(const bool success, const UnzipTask* task) : _Success(success), _Task(task) {}
    };

    static SafeQueue::SpscQueue<UnzipTask*> s_TaskQueue;
    static SafeQueue::SpscQueue<UnzipResult*> s_ResultQueue;

    static Api::iEngine* s_Engine = nullptr;

    XZip* XZip::GetInstance() {
        static XZip s_Instance;
        return &s_Instance;
    }

    bool XZip::Initialize(Api::iEngine* const engine) {
        s_Engine = engine;
        Start(8);
        return true;
    }

    bool XZip::Launch(Api::iEngine* const engine) {
        return false;
    }

    void XZip::Release(Api::iEngine* const engine) {

    }

    void XZip::EarlyUpdate(Api::iEngine* const engine) {

    }

    void XZip::Update(Api::iEngine* const engine) {

    }

    void XZip::LaterUpdate(Api::iEngine* const engine) {
        UnzipResult* result = nullptr;
        while (s_ResultQueue.Pull(result)) {
            result->_Task->_Callback(result->_Success);
            xdel result->_Task;
            xdel result;
        }
    }

    void XZip::Unzip(const std::string& file, const std::string& folder, const fUnzipCallback fun) {
        s_TaskQueue.Push(xnew UnzipTask(file, folder, fun));
    }
    // 单个文件限制大小为 100M 
#define MAX_SRC_FILE_SIZE (100*1024*1024)
#define TEMP_BUFFER_SIZE (128*1024)
    void XZip::Run(void* context) {
        static unsigned char* s_TempBuffer = (unsigned char*)malloc(TEMP_BUFFER_SIZE);

        while (true) {
            UnzipTask* task = nullptr;

            if (s_TaskQueue.Pull(task)) {
                if (!SafeSystem::File::FolderExists(task->_TargetFolder)) {
                    if (!SafeSystem::File::CreateFolder(task->_TargetFolder)) {
                        XERROR(s_Engine, "Create Folder %s error", task->_TargetFolder.c_str());
                        s_ResultQueue.Push(xnew UnzipResult(false, task));
                        continue;
                    }
                }

                unzFile zipFile = unzOpen(task->_File.c_str());
                if (nullptr == zipFile) {
                    XERROR(s_Engine, "unzOpen %s error", task->_File.c_str());
                    s_ResultQueue.Push(xnew UnzipResult(false, task));
                    continue;
                }

                unz_global_info globalInfo;
                if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) { // 获取zip文件信息
                    XERROR(s_Engine, "unzGetGlobalInfo %s error", task->_File.c_str());
                    unzClose(zipFile);
                    s_ResultQueue.Push(xnew UnzipResult(false, task));
                    continue;
                }

                bool success = true;

                for (int i = 0; i < globalInfo.number_entry; i++) {
                    char fileName[512];
                    unz_file_info fileInfo;
                    if (unzGetCurrentFileInfo(zipFile, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0) != UNZ_OK) {
                        XERROR(s_Engine, "unzGetCurrentFileInfo %s error", task->_File.c_str());
                        success = false;
                        break;
                    }
                    std::string path = task->_TargetFolder + "/" + fileName;

                    if (fileInfo.external_fa == FILE_ATTRIBUTE_DIRECTORY || fileName[strlen(fileName) - 1] == '/') {
                        SafeSystem::File::CreateFolder(path);
                        TRACE(s_Engine, "Unzip Create folder %s", path.c_str());
                    }
                    else {
                        if (unzOpenCurrentFile(zipFile) != UNZ_OK) { // 打开当前文件
                            XERROR(s_Engine, "unzOpenCurrentFile %s error", fileName);
                            success = false;
                            break;
                        }

                        std::ofstream file(path, std::ios::binary);
                        if (!file) {
                            XERROR(s_Engine, "Open %s error", path.c_str());
                            success = false;
                            break;
                        }

                        int readSize = 0;
                        SafeMemory::Memset(s_TempBuffer, TEMP_BUFFER_SIZE, 0, TEMP_BUFFER_SIZE);
                        while ((readSize = unzReadCurrentFile(zipFile, s_TempBuffer, TEMP_BUFFER_SIZE)) != 0) { // 读取当前文件内容
                            if (readSize < 0) {
                                XERROR(s_Engine, "unzReadCurrentFile error", path.c_str());
                                success = false;
                                break;
                            }
                            else {
                                file.write(reinterpret_cast<const char*>(s_TempBuffer), readSize);
                                SafeMemory::Memset(s_TempBuffer, TEMP_BUFFER_SIZE, 0, TEMP_BUFFER_SIZE);
                            }
                        }
                        if (success) {
                            file.flush();
                        }
                        else {
                            break;
                        }
                        file.close();

                        TRACE(s_Engine, "Unzip Create file %s", path.c_str());
                    }

                    unzCloseCurrentFile(zipFile);
                    unzGoToNextFile(zipFile); // 关闭当前文件
                }

                unzClose(zipFile);
                s_ResultQueue.Push(xnew UnzipResult(success, task));
            }
            else {
                SafeSystem::Time::MillisecondSleep(10);
            }
        }
    }
}

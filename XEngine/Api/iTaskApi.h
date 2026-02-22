#ifndef __iTaskApi_h__
#define __iTaskApi_h__

#include "MultiSys.h"
#include "SafeSystem.h"
#include <functional>

namespace XEngine {
    namespace Api {
        class iTaskContext;
        typedef std::function<Int64(iTaskContext &)> fTask;
        typedef std::function<void(const Int64, const iTaskContext &)> fTaskCompleted;
        typedef std::function<void()> fTasksAllCompleted;


        enum class eTaskProcessorType {
            OrderedQueue = 0,  //丢入队列里排队等待线程池处理执行
            SeparateThread = 1 //立即启动新线程处理
        };

        class iTaskContext {
        public:
            virtual ~iTaskContext() {}

            template<typename T>
            T* Get() const {
                return (T*)this->GetContext();
            }

            template<typename T>
            void Set(T* const context) {
                this->SetContext(context);
            }

            virtual void* GetContext() const = 0;
            virtual void SetContext(void* const context) = 0;
        };

        class iTaskGroup {
        public:
            virtual ~iTaskGroup() {}

            virtual void AddTask(fTask task, fTaskCompleted callback) = 0;
            virtual void SetTasksAllCompletedCallback(fTasksAllCompleted callback) = 0;
            virtual void Start(const eTaskProcessorType type) = 0;
        };

        class iTaskApi {
        public:
            virtual ~iTaskApi() {}

            #define NOT_USE_MASK -1

            virtual void PushTask(const eTaskProcessorType processorType, fTask task, fTaskCompleted callback, const Int64 mask = NOT_USE_MASK) = 0;

            virtual iTaskGroup* CreateTaskGroup() = 0;
        };
    }
}

#endif //__iTaskApi_h__

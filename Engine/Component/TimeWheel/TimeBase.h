#ifndef __TimeBase_h__
#define __TimeBase_h__


namespace tcore {
	using namespace api;

	class tlist;
	class tbase {
	public:
		const int _id;
		const int64 _interval;
		const iContext _context;
		const std::string _file;
		const int _line;

		typedef tpool<tbase> TIMER_BASE_POOL;

		static tbase* Create(iTimer* timer, const int id, const iContext& context, int count, int64 interval, const char* file, const int line);

		void onTimer();
		void forceEnd();

		void pause(uint64 jiffies);
		void resume(uint64 jiffies);

		void release();

		inline bool isValid() const { return _valid; }
		inline bool isPolling() const { return _polling; }

		inline bool isPaused() const { return _paused; }

		inline uint64 getExpire() const { return _expire; }
		inline void setExpire(uint64 expire) { _expire = expire; }
		void adjustExpire(uint64 now);

		inline void setNext(tbase* next) { _next = next; }
		inline tbase* getNext() const { return _next; }

		inline void setPrev(tbase* prev) { _prev = prev; }
		inline tbase* getPrev() const { return _prev; }

		inline void setList(tlist* list) { _list = list; }
		inline tlist* getList() const { return _list; }

		inline iTimer* getTimer() const { return _timer; }

	private:
		friend TIMER_BASE_POOL;
		tbase(iTimer* timer, const int id, const iContext& context, int count, int64 interval, const char* file, const int line);
		virtual ~tbase() {}

	private:
		tlist* _list;
		tbase* _next;
		tbase* _prev;

		iTimer* const _timer;

		bool _valid;
		bool _polling;

		uint64 _expire;
		int _count;
		bool _started;

		uint64 _pauseTick;
		bool _paused;

	};
}

#endif //__tbase_h__


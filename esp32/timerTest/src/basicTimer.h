class basicTimer{
	public:
		basicTimer();
		void init();
		void start(unsigned int interval);
		void stop();
		void clear();
		bool expired();
		void check();

	private:
		bool _active;
		unsigned int _startTime;
		unsigned int _interval;
		bool _expired;
};
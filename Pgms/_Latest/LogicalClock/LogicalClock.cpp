class LogicalClock {
private:
	long			timeStamp;
	pthread_t		timerThread;
	volatile bool	running;

public:

	LogicalClock () : timeStamp(0), running(true){
	}

	~LogicalClock () {
		running = false;
		sleep(1000);
	}

	void startClock () {
		if(pthread_create(&timerThread, NULL, (void *) LogicalClock::runClock, NULL) != 0) {
			cout<<"Failed to create the thread"<<endl;
		}
	}

	static void runClock () {
		while(running) {
			timeStamp++;
			sleep(1000)
		}
	}
			
	void setTime (long time) {
		timeStamp = time;
	}

	long getTime () {
		return timeStamp;
	}

	void stopTimer () {
		running = false;
	}
} 


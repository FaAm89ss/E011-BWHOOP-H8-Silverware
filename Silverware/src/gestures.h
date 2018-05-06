
int gestures2( void);
int gesture_sequence( int gesture);
void gestures( void);

// warning: using if x>GESTURE_UDR to check pid gestures
enum gestures_enum{
    GESTURE_NONE = 0, 
    GESTURE_DDD,
    GESTURE_UUU,
    GESTURE_LLD,
    GESTURE_RRD,
    GESTURE_UDU,
    GESTURE_UDD,
    GESTURE_UDR,
    GESTURE_UDL,
		GESTURE_LDD,
		GESTURE_RDD,
		GESTURE_LUU,
		GESTURE_RUU,
		GESTURE_LLL,
		GESTURE_RRR,
		GESTURE_LLR,
		GESTURE_RRL,
		GESTURE_RLL,
		GESTURE_LRR,
    GESTURE_LLU,
    GESTURE_RRU,
    GESTURE_UUD,
    GESTURE_DDU
};



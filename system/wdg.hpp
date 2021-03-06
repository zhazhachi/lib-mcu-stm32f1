#ifndef __WDG_H
#define __WDG_H
#include "sys.hpp"

namespace iwdg{
	void config(uint8_t prer,uint16_t rlr);
	void feed(void);
}

namespace wwdg{
	void config(uint8_t tr,uint8_t wr,uint8_t fprer);
}

#endif //__WDG_H

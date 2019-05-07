#ifndef __LINK_USART_H
#define __LINK_USART_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "fiip-base/fiip.h"
#include "fiip-base/link/link.h"
#include "function/function.hpp"

void startUsart(uint8_t* dev, uint32_t speed);

#endif  //__LINK_USART_H

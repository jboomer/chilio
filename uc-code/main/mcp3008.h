#ifndef MCP3008_H
#define MCP3008_H

#include <stdint.h>

void mcp3008_init(void);

uint16_t mcp3008_get_raw(uint8_t channel);

#endif // #ifndef MCP3008_H

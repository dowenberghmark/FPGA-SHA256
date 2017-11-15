#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__


#include <cstdint>



struct chunk {
	char data[64];
};

struct buffer_header {
	uint32_t num_chunks;
	uint32_t ready_flag;
};

struct buffer {
	struct buffer_header buffer_header;
	struct chunk *chunks;
};

struct global_header {
	uint32_t start_processing_flag;
	uint32_t active_buffer_flag;
};

#endif

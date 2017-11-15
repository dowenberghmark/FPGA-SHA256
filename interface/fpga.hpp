#ifndef __FPGA_H_INCLUDED__
#define __FPGA_H_INCLUDED__

#include <iostream>
#include <cstdint>
#include <stdlib.h>
#include <stdexcept>

#include <unistd.h> // usleep

#include "defs.hpp"


class FPGA {
public:
	FPGA(void *g_header, void *buffer0, void *buffer1)	{
		if (!global_header || !buffer0 || !buffer1) {
			throw std::invalid_argument("received NULL pointer as argument");
		}

		global_header = (struct global_header *) g_header;

		buffers[0] = (struct buffer *) buffer0;
		buffers[1] = (struct buffer *) buffer1;
		buffers[0]->chunks = (struct chunk *)((char *) buffer0 + sizeof(struct buffer_header));
		buffers[1]->chunks = (struct chunk *)((char *) buffer1 + sizeof(struct buffer_header));
	}

	void run();

private:
	struct global_header *global_header;
	struct buffer *buffers[2];
};

// run this function as a thread. communicate through the shared buffers.
void FPGA::run() {
	// dummy fpga loop
	while (1) {

		while (!global_header->start_processing_flag) {
			// usleep(500);
		}
		// only for testing so we can quit nicely
		if (global_header->start_processing_flag == 1337) {
			break;
		}
		global_header->start_processing_flag = 0;

		uint32_t active_buf = global_header->active_buffer_flag;
		struct buffer *curr = buffers[active_buf];

		struct buffer_header *buffer_header = &curr->buffer_header;
		uint32_t num_chunks = buffer_header->num_chunks;
		uint32_t *ready_flag = &buffer_header->ready_flag;
		struct chunk *chunks = curr->chunks;

		// do sha-256 to chunks
		for (int i = 0; i < num_chunks; i++) {
			std::cout << "chunk: " << i << "\n";
			struct chunk c = chunks[i];
			for (int j = 0; j < 32; j++) {
				c.data[j] = i;
			}
		}

		std::cout << "buffer" << curr_i << " processed, num_chunks: " << num_chunks << " ready_flag: " << *ready_flag << "\n";

		*ready_flag = 1;
	}
}

#endif

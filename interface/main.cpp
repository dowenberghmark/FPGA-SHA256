#include <cstddef>
#include <thread>

#include <unistd.h> // usleep

#include "defs.hpp"
#include "fpga.hpp"


int main(int argc, char **argv)
{
	struct global_header *global_header = (struct global_header *) calloc(1, sizeof(struct global_header));
	struct buffer *buffer0 = (struct buffer *) calloc(10, 64);
	struct buffer *buffer1 = (struct buffer *) calloc(10, 64);

	global_header->active_buffer_flag = 0;
	global_header->start_processing_flag = 1;

	buffer0->buffer_header.num_chunks = 2;
	buffer1->buffer_header.num_chunks = 3;

	FPGA fpga(global_header, buffer0, buffer1);
	std::thread t(&FPGA::run, fpga);

	while (!buffer0->buffer_header.ready_flag) {
		std::cout << "waiting for buffer0 ready flag" << "\n";
		// usleep(500);
	}
	buffer0->buffer_header.ready_flag = 0;

	global_header->active_buffer_flag = 1;
	global_header->start_processing_flag = 1;

	while (!buffer1->buffer_header.ready_flag) {
		std::cout << "waiting for buffer1 ready flag" << "\n";
		// usleep(500);
	}
	buffer1->buffer_header.ready_flag = 0;

	global_header->active_buffer_flag = 0;
	global_header->start_processing_flag = 1;

	while (!buffer0->buffer_header.ready_flag) {
		std::cout << "waiting for buffer0 again ready flag" << "\n";
		usleep(500);
	}

	global_header->start_processing_flag = 1337;

	t.join();

	return 0;
}

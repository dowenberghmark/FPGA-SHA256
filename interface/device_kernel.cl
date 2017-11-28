struct chunk{
	char data[64];
};

#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void fpga_sha(global struct chunk *chunk_buffer,
                       const int n_elements)
{
	printf("here");
    for (int i = 0 ; i < n_elements ; i ++)
    {
        for (int j = 0 ; j < DATA_TO_TOUCH; j++) chunk_buffer[i].data[j] = chunk_buffer[i].data[j] + NUMBER_ONE;
    }
}

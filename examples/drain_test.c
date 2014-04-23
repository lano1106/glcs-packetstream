/**
 * \file examples/crc32.c
 * \brief example app which uses 'packetstream' to distribute CRC32 calculation jobs
 * \author Pyry Haulos <pyry.haulos@gmail.com>
 * \date 2007-2008
 * For conditions of distribution and use, see copyright notice in packetstream.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <packetstream.h>
#include "optimization.h"

#define BUFFER_SIZE (1024 * 1024 * 50)

int main(int argc, char *argv[])
{
	ps_buffer_t buffer;
	ps_bufferattr_t bufferattr;
	ps_packet_t packet;
	int i;
	int ret;
	size_t size;
	char *temp = (char *) malloc(1000);

	ps_bufferattr_init(&bufferattr);
	ps_bufferattr_setflags(&bufferattr, PS_BUFFER_STATS);
	ps_bufferattr_setsize(&bufferattr, BUFFER_SIZE);

	if (ps_buffer_init(&buffer, &bufferattr)) {
		printf("ps_buffer_create() failed\n");
		return 1;
	}
	ps_bufferattr_destroy(&bufferattr);

	ps_packet_init(&packet, &buffer);

	for (i = 0; i < 50; ++i) {
		size = 1000;
		if (unlikely((ret = ps_packet_open(&packet, PS_PACKET_WRITE)))) {
			if (ret != EINTR)
				printf("writer_thread(): ps_packet_open() failed\n");
			break;
		}

		if (unlikely((ret = ps_packet_setsize(&packet, size)))) {
			if (ret == EINTR)
				break;
			printf("writer_thread(): ps_packet_setsize() failed\n");
			if(likely(!ps_packet_cancel(&packet)))
				continue;
			else {
				printf("writer_thread(): ps_packet_cancel() failed\n");
				ps_buffer_cancel(&buffer);
				break;
			}
		}
		if (unlikely(ps_packet_write(&packet, temp, size))) {
			printf("writer_thread(): ps_packet_write() failed\n");
			ps_buffer_cancel(&buffer);
			break;
		}
		if (unlikely((ret = ps_packet_close(&packet)))) {
			if (ret != EINTR) {
				printf("writer_thread(): ps_packet_close() failed\n");
				ps_buffer_cancel(&buffer);
			}
			break;
		}
	}

	ps_packet_destroy(&packet);

	printf("Before drain:\n");
	ps_buffer_state_text(&buffer, stdout);
	ret = ps_buffer_drain(&buffer);
	printf("Have drained %d packets\nAfter drain:\n", ret);
	ps_buffer_state_text(&buffer, stdout);

	ps_buffer_destroy(&buffer);
	free(temp);

	return 0;
}


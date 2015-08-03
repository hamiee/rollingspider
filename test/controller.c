#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

static const char *const evval[3] = {
	"RELEASED",
	"PRESSED ",
	"REPEATED"
};


/*
	ev.time: time of the event (struct timeval type)
	ev.type: EV_KEY
        ev.code: KEY_*, key identifier; see complete list in /usr/include/linux/input.h
	ev.value: 0 if key release, 1 if key press, 2 if autorepeat keypress
   */

int main(void)
{
	const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
	struct input_event ev;
	ssize_t n;
	int fd;

	fd = open(dev, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
		return EXIT_FAILURE;
	}


	while (1) {
		n = read(fd, &ev, sizeof ev);
		if (n == (ssize_t)-1) {
			if (errno == EINTR)
				continue;
			else
				break;
		} else{
			if (n != sizeof ev) {
				errno = EIO;
				break;
			}
		}


		if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
			printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);

	}
	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));
	return EXIT_FAILURE;

}

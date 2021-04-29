#include <linux/unistd.h>

int main(void)
{
	// TODO: Write your code here
	char name[] = "Taewon Kang";
	char major[] = "Comp. Sci.";
	syscall(333);
	syscall(334, name, major);
	return 0;
}

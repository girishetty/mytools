#include <setjmp.h>

int main() {
#ifdef setjmp
#error "sdf"
#endif
	return 0;
}


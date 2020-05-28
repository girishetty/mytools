int main(int argc, char** argv) {
	int i = 0;
	for(;i<argc;i++)
		printf("Arg-%d size-%d is %s \n", i+1, strlen(argv[i]), argv[i]);
	return 0;
}


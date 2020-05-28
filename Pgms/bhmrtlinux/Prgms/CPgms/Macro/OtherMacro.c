#define f(a,b) a##b
#define g(a)   #a
#define h(a) g(a)

#define cat(a, b) a##b
#define CAT(a, b) cat(a, b)

int main()  {
	int a;
	printf("%s\n",h(f(1,2)));
	printf("%s\n",g(f(1,2)));
	/* printf("%s\n",#g(f(1,2))); */
	printf("String Cat 1 : %s\n", cat("Girish ", "Shetty"));
	printf("String Cat 2 : %s\n", CAT("Girish ", "Shetty"));
	printf("String Cat 3 : %s\n", CAT(C, AT("Girish ", "Shetty")));
        return 0;
}


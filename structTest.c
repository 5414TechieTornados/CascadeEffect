
typedef struct {
    int foo;
    int bar;
} example_bug_t;

example_bug_t foobar[2] = {
    { 1, 2 },
    { 2, 3 }
};

task main()
{

	for(int i = 0; i < sizeof(foobar); i ++)
	{
		wait1Msec(1);
	}
}

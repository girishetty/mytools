#ifndef TESTCASES_H
#define TESTCASES_H

// Sample inputs that show different types of symmetry
// All symmetries 
static const char* testCases1[] = {
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff"
};

static const char* testCases2[] = {
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt"
};

static const char* testCases3[] = {
"tttttttt",
"ffffffff",
"tttttttt",
"ffffffff",
"ffffffff",
"tttttttt",
"ffffffff",
"tttttttt"
};

static const char* testCases4[] = {
"tttttttt",
"tfffffft",
"tfffffft",
"tfffffft",
"tfffffft",
"tfffffft",
"tfffffft",
"tttttttt"
};

static const char* testCases5[] = {
"tfffffft",
"ftfffftf",
"fftfftff",
"fffttfff",
"fffttfff",
"fftfftff",
"ftfffftf",
"tfffffft"
};

static const char* testCases6[] = {
"fffttfff",
"fftfftff",
"ftfffftf",
"tfffffft",
"tfffffft",
"ftfffftf",
"fftfftff",
"fffttfff"
};

static const char* testCases7[] = {
"fffttfff",
"fffttfff",
"fffttfff",
"tttttttt",
"tttttttt",
"fffttfff",
"fffttfff",
"fffttfff"
};

static const char* testCases8[] = {
"ttfffftt",
"ttfffftt",
"ttfffftt",
"tttttttt",
"tttttttt",
"ttfffftt",
"ttfffftt",
"ttfffftt"
};

// Horizontal symmetry only
static const char* testCases9[] = {
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"tttttttt",
"tttttttt",
"tttttttt",
"tttttttt"
};

static const char* testCases10[] = {
"ttfffftt",
"ttfffftt",
"ffttttff",
"ffttttff",
"ttfffftt",
"ttfffftt",
"ffttttff",
"ffttttff"
};

static const char* testCases11[] = {
"tttttttt",
"fttttttf",
"ffttttff",
"fffttfff",
"tfffffft",
"ttfffftt",
"tttffttt",
"tttttttt"
};

static const char* testCases12[] = {
"tfffffft",
"ftfffftf",
"fftfftff",
"fffttfff",
"tfffffft",
"ftfffftf",
"fftfftff",
"fffttfff"
};

static const char* testCases13[] = {
"ttfffftt",
"ttfffftt",
"fttffttf",
"fttffttf",
"ffttttff",
"ffttttff",
"fffttfff",
"fffttfff"
};

static const char* testCases14[] = {
"tfffffft",
"ttfffftt",
"fttffttf",
"ffttttff",
"fffttfff",
"fffttfff",
"fffttfff",
"fffttfff"
};

static const char* testCases15[] = {
"ffttttff",
"fttffttf",
"fttffttf",
"ffttttff",
"fttttttf",
"ttfffftt",
"ttfffftt",
"ffttttff"
};

static const char* testCases16[] = {
"fffttfff",
"ffttttff",
"fttffttf",
"ttfffftt",
"tttttttt",
"tttttttt",
"ttfffftt",
"ttfffftt"
};

// Vertical symmetry only
static const char* testCases17[] = {
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff"
};

static const char* testCases18[] = {
"ttffttff",
"ttffttff",
"ffttfftt",
"ffttfftt",
"ffttfftt",
"ffttfftt",
"ttffttff",
"ttffttff"
};

static const char* testCases19[] = {
"fffffftt",
"ffffttff",
"ffttffff",
"ttffffff",
"ttffffff",
"ffttffff",
"ffffttff",
"fffffftt"
};

static const char* testCases20[] = {
"tttttttt",
"tttttttf",
"ttttttff",
"ffffffff",
"ffffffff",
"ttttttff",
"tttttttf",
"tttttttt"
};

static const char* testCases21[] = {
"ffttttff",
"fttttttf",
"ttfffftt",
"ttffffff",
"ttffffff",
"ttfffftt",
"fttttttf",
"ffttttff"
};

static const char* testCases22[] = {
"ttttffff",
"ttttttff",
"ttfffttf",
"ttfffftt",
"ttfffftt",
"ttfffttf",
"ttttttff",
"ttttffff"
};

static const char* testCases23[] = {
"tttttttt",
"tttttttt",
"ttffffff",
"tttttttt",
"tttttttt",
"ttffffff",
"tttttttt",
"tttttttt"
};

static const char* testCases24[] = {
"ttttttff",
"ttfffftt",
"ttfffftt",
"ttttttff",
"ttttttff",
"ttfffftt",
"ttfffftt",
"ttttttff"
};

// No symmetry 
static const char* testCases25[] = {
"ttttffff",
"ttttffff",
"ttttffff",
"ttttffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff"
};

static const char* testCases26[] = {
"ftffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff",
"ffffffff"
};

static const char* testCases27[] = {
"ftffffff",
"tffffttf",
"ftfffttf",
"tfffffff",
"ftttffff",
"fffttttf",
"fffffttf",
"tffftttf"
};

static const char* testCases28[] = {
"ttfffttf",
"ffttffft",
"tfffttff",
"fttffftt",
"fffttfff",
"ttfffttf",
"ffttffft",
"tfffttff"
};

static const char* testCases29[] = {
"tttttttt",
"tttttttt",
"fffffttf",
"ffffttff",
"fffttfff",
"ffttffff",
"fttfffff",
"ttffffff"
};

static const char* testCases30[] = {
"tttttttt",
"tttttttt",
"ttffffff",
"tttttttt",
"tttttttt",
"ttffffff",
"ttffffff",
"ttffffff"
};

static const char* testCases31[] = {
"ffttttff",
"fttttttf",
"ttfffftt",
"ttffffff",
"ttfftttt",
"ttffffft",
"tttttttf",
"ffttttff"
};

static const char* testCases32[] = {
"fffffftt",
"fffffftt",
"fffffftt",
"fffffftt",
"fffffftt",
"ttfffftt",
"fttffttf",
"ffttttff"
};


#endif //TESTCASES_H

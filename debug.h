#ifndef _DEBUG
#define DEBUG(x) do {} while (0)
#else
#define DEBUG(x) do {x;} while (0)
#endif

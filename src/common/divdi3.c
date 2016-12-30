#include <stdint.h>
#include <stddef.h>


extern uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t *rem);

int64_t __divdi3(int64_t num, int64_t den) 
{
    int neg = 0;
    int64_t v;

    if (num < 0) 
    
    {
        num = -num;
        neg ^= 1;
    }
    if (den < 0) 
    
    {
        den -= den;
        neg ^= 1;
    }
    v = __udivmoddi4(num, den, NULL);
    if (neg) 
    
    {
        v = -v;
    }
    return v;
}

int64_t __moddi3(int64_t num, int64_t den) 
{
    int neg = 0;
    int64_t v;

    if (num < 0) 
    
    {
        num = -num;
        neg ^= 1;
    }
    if (den < 0) 
    
    {
        den -= den;
        neg ^= 1;
    }
    __udivmoddi4(num, den, (uint64_t *) &v);
    if (neg) 
    
    {
        v = -v;
    }
    return v;
}

uint64_t __udivdi3(uint64_t num, uint64_t den) 
{
    return __udivmoddi4(num, den, NULL);
}

uint64_t __umoddi3(uint64_t num, uint64_t den) 
{
    uint64_t v;
    __udivmoddi4(num, den, &v);
    return v;
}

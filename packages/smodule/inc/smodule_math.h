#ifndef _SMODULE_MATH_
#define _SMODULE_MATH_

int smodule_math_add(int a, int b);
int smodule_math_sub(int a, int b);
int smodule_math_mul(int a, int b);
int smodule_math_div(int a, int b);

void *smodule_math_init(uint32_t smodule_addr, void *kpi_entry);

#endif


#ifndef APP_ALGO_ALGO_H
#define APP_ALGO_ALGO_H

#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} AlgoAxesData_t;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} AlgoAxesData_t_25HZ;

typedef struct
{
    AlgoAxesData_t  axes;
    int32_t         ppgSample;
    int32_t         envSample;
} AlgoInputData_t;

typedef struct
{
    int32_t         hrData;
    int32_t         reliability;
} AlgoOutputData_t;

typedef struct
{
    int32_t         hrData;
    int32_t         reliability;
} AlgoOutputData_t_25HZ;

typedef struct
{
    AlgoAxesData_t_25HZ axes;
    int32_t         ppgSample;
    int32_t         envSample;
} AlgoInputData_t_25HZ;

void Algo_Init_25HZ(void);
void Algo_Input_25HZ(AlgoInputData_t_25HZ *pInputData, int32_t gaptime);
void Algo_Output_25HZ(AlgoOutputData_t_25HZ *pOutputData);
void Algo_Version_25HZ(char *pVersionOutput);

void Algo_Init(void);
void Algo_Input(AlgoInputData_t *pInputData, int32_t gaptime);
void Algo_Output(AlgoOutputData_t *pOutputData);
void Algo_Version(char *pVersionOutput);



#endif /* APP_ALGO_ALGO_H_ */




#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#define CYCLES_64 uint64_t


CYCLES_64 read_timer();
void busy_fp(double*, double*);
//void busy_int(int*, int*);



double subnormalfp;
double normalfpa= 100.0;
double normalfpb= 12.0;
uint64_t inta = 100;
uint64_t intb = 45;

double* subnormalfp_ptr = &subnormalfp;
double* normalfpa_ptr = &normalfpa;
double* normalfpb_ptr = &normalfpb;
uint64_t* inta_ptr = &inta;
uint64_t* intb_ptr = &intb;

int main(argc, argv[])
{


  CYCLES_64 t1, t2;

  // buffer to store measured latencies
  const int BUFF_SIZE = 1000;
  CYCLES_64 buffer[BUFF_SIZE];
  
  // construct subnormal floating point integer
  const unsigned long lnDEN[2] = {0x0000001, 0x00000000};
  const double A_DENORMAL  = *( double* )lnDEN;
  subnormalfp = A_DENORMAL;
  
  
  
  
  
  for(int j=0; j<BUFF_SIZE; j++){
    t1 = read_timer();
    for(int i=0; i<20; i++){
        busy_fp(subnormalfp_ptr, normalfpa_ptr);
    }
    t2 = read_timer();
    buffer[j] = t2-t1;
  }

  // print out results
  for(int i=0; i<BUFF_SIZE; i++){
    printf("%ld,", buffer[i]);
  }
  printf("\n");

  return 0;   
}

inline void busy_fp(double* fpa_ptr, double* fpb_ptr){

    asm __volatile__(
            "mov %0, %%rax\n"
            "mov %1, %%rbx\n"
            "movsd (%%rax), %%xmm0\n"
            "movsd (%%rbx), %%xmm1\n"
            "movsd %%xmm1, %%xmm2\n"
            // mulsd: Multiply Scalar Double-Precision Floating-Point Value
            // 128-bit Legacy SSE version: The first source operand
            // and the destination operand are the same.
            // NOTE: Here I make all the destinations differently.
            // While this is unneccessary, since they are false dependency
            "mulsd %%xmm0, %%xmm2\n" // mul_1
            "movsd %%xmm1, %%xmm3\n"
            "mulsd %%xmm0, %%xmm3\n" // mul_2
            "movsd %%xmm1, %%xmm4\n"
            "mulsd %%xmm0, %%xmm4\n" // mul_3
            "movsd %%xmm1, %%xmm5\n"
            "mulsd %%xmm0, %%xmm5\n" // mul_4
            "movsd %%xmm1, %%xmm6\n"
            "mulsd %%xmm0, %%xmm6\n" // mul_5
            "movsd %%xmm1, %%xmm7\n"
            "mulsd %%xmm0, %%xmm7\n" // mul_6
            "movsd %%xmm1, %%xmm8\n"
            "mulsd %%xmm0, %%xmm8\n" // mul_7
            "movsd %%xmm1, %%xmm9\n"
            "mulsd %%xmm0, %%xmm9\n" // mul_8
            "movsd %%xmm1, %%xmm10\n"
            "mulsd %%xmm0, %%xmm10\n" // mul_9
            "movsd %%xmm1, %%xmm11\n"
            "mulsd %%xmm0, %%xmm11\n" // mul_10
            :
            : "m"(fpa_ptr), "m"(fpb_ptr)
            : "%xmm0", "%xmm1", "%xmm2", "memory",
              "%xmm3", "%xmm4", "%xmm5",
              "%xmm6", "%xmm7", "%xmm8",
              "%xmm9", "%xmm10", "%xmm11",
              "%rax", "%rbx"
        );
}



inline CYCLES_64 read_timer(){
    volatile uint64_t t;
    asm __volatile__(
        "lfence\n"
        // Guaranteed to clear the high-order 32 bits of RAX and RDX.
        "rdtsc\n"
        "shlq $32, %%rdx\n"
        "orq %%rdx, %%rax\n"
        : "=a" (t)
        :
        : "%rdx"
    );
    return t;
}

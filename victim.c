#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define CYCLES_64 uint64_t


CYCLES_64 read_timer();
void busy_fp(double*, double*);
void busy_int(uint64_t*, uint64_t*);
void busy_simd_div(double*, double*);
void busy_fp_noisy(double*, double*);
void busy_int_noisy(uint64_t*, uint64_t*);


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

uint64_t dummy[1000];
uint64_t* dummy_ptr = &dummy[0];

int main(int argc, char* argv[])
{

  if(argc<2){
    printf("please provide 1 argument: <mode> [sub_fp, normal_fp, int]\n");
    exit(1);
  }

  // construct subnormal floating point integer
  const unsigned long lnDEN[2] = {0x0000001, 0x00000000};
  const double A_DENORMAL  = *( double* )lnDEN;
  subnormalfp = A_DENORMAL;
  
  if (strcmp("sub_fp", argv[1])==0){
  
    while(1){  
      busy_fp(subnormalfp_ptr, normalfpa_ptr);
    }

  }else if(strcmp("normal_fp", argv[1])==0){
  
    while(1){
        busy_fp(normalfpb_ptr, normalfpa_ptr);
    }

  }else if (strcmp("sub_div", argv[1])==0){
  
    while(1){
            busy_simd_div(subnormalfp_ptr, normalfpa_ptr);
    }

  }else if(strcmp("normal_div", argv[1])==0){
  
	while(1){
            busy_simd_div(normalfpb_ptr, normalfpa_ptr);
        }
  }else if(strcmp("int", argv[1])==0){
  
    while(1){
      busy_int(inta_ptr, intb_ptr);
    }

  }else{
  
    printf("please provide 2 argument: <mode> [sub_fp, normal_fp, int]; <number of iterations>\n");
    exit(1);
  }

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



inline void busy_fp_noisy(double* fpa_ptr, double* fpb_ptr){

    asm __volatile__(
            "mov %0, %%rax\n"
            "mov %1, %%rbx\n"
            "mov %2, %%rcx\n"
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
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mulsd %%xmm0, %%xmm3\n" // mul_2
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm4\n"
            "mulsd %%xmm0, %%xmm4\n" // mul_3
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm5\n"
            "mulsd %%xmm0, %%xmm5\n" // mul_4
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm6\n"
            "mulsd %%xmm0, %%xmm6\n" // mul_5
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm7\n"
            "mulsd %%xmm0, %%xmm7\n" // mul_6
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm8\n"
            "mulsd %%xmm0, %%xmm8\n" // mul_7
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm9\n"
            "mulsd %%xmm0, %%xmm9\n" // mul_8
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm10\n"
            "mulsd %%xmm0, %%xmm10\n" // mul_9
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "movsd %%xmm1, %%xmm11\n"
            "mulsd %%xmm0, %%xmm11\n" // mul_10
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            "mov (%%rcx), %%rdx\n"
            "inc %%rcx\n"
            :
            : "m"(fpa_ptr), "m"(fpb_ptr), "m"(dummy_ptr)
            : "%xmm0", "%xmm1", "%xmm2", "memory",
              "%xmm3", "%xmm4", "%xmm5",
              "%xmm6", "%xmm7", "%xmm8",
              "%xmm9", "%xmm10", "%xmm11",
              "%rax", "%rbx", "%rcx", "%rdx"
        );
}



inline void busy_int(uint64_t* inta_ptr, uint64_t* intb_ptr){

    asm __volatile__(
            "mov %0, %%rsi\n"
            "mov %1, %%rdi\n"
            "mov (%%rsi), %%rbx\n"
            "mov (%%rdi), %%rcx\n"
            // MUL — Unsigned Multiply
            // MUL r/m64
            // Operand Size Source 1    Source 2    Destination
            // Quadword     RAX         r/m64       RDX:RAX
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_1
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_2
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_3
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_4
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_5
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_6
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_7
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_8
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_9
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_10
            :
            : "m"(inta_ptr), "m"(intb_ptr)
            : "memory",
              "%rax", "%rbx", "%rcx", "%rdx",
              "%rsi", "%rdi"
        );
}


inline void busy_int_noisy(uint64_t* inta_ptr, uint64_t* intb_ptr){

    asm __volatile__(
            "mov %0, %%rsi\n"
            "mov %1, %%rdi\n"
            "mov (%%rsi), %%rbx\n"
            "mov (%%rdi), %%rcx\n"
            "mov %2, %%rsi\n"
            // MUL — Unsigned Multiply
            // MUL r/m64
            // Operand Size Source 1    Source 2    Destination
            // Quadword     RAX         r/m64       RDX:RAX
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_1
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_2
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_3
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_4
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_5
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_6
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_7
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_8
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_9
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "mov %%rcx, %%rax\n"
            "mul %%rbx\n"    // mul_10
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            "inc %%rsi\n"
            "mov (%%rsi), %%rdi\n"
            :
            : "m"(inta_ptr), "m"(intb_ptr), "m"(dummy_ptr)
            : "memory",
              "%rax", "%rbx", "%rcx", "%rdx",
              "%rsi", "%rdi"
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



inline void busy_simd_div(double* fpa_ptr, double* fpb_ptr){

    asm __volatile__(
            "mov %0, %%rax\n"
            "mov %1, %%rbx\n"
            "movsd (%%rax), %%xmm0\n"
            "movsd (%%rbx), %%xmm1\n"
            "movsd %%xmm1, %%xmm2\n"
            // divsd: Divide Scalar Double-Precision Floating-Point Value
            // 128-bit Legacy SSE version: The first source operand
            // and the destination operand are the same.
            // NOTE: Here I make all the destinations differently.
            // While this is unneccessary, since they are false dependency
            "divsd %%xmm0, %%xmm2\n" // mul_1
            "movsd %%xmm1, %%xmm3\n"
            "divsd %%xmm0, %%xmm3\n" // mul_2
            "movsd %%xmm1, %%xmm4\n"
            "divsd %%xmm0, %%xmm4\n" // mul_3
            "movsd %%xmm1, %%xmm5\n"
            "divsd %%xmm0, %%xmm5\n" // mul_4
            "movsd %%xmm1, %%xmm6\n"
            "divsd %%xmm0, %%xmm6\n" // mul_5
            "movsd %%xmm1, %%xmm7\n"
            "divsd %%xmm0, %%xmm7\n" // mul_6
            "movsd %%xmm1, %%xmm8\n"
            "divsd %%xmm0, %%xmm8\n" // mul_7
            "movsd %%xmm1, %%xmm9\n"
            "divsd %%xmm0, %%xmm9\n" // mul_8
            "movsd %%xmm1, %%xmm10\n"
            "divsd %%xmm0, %%xmm10\n" // mul_9
            "movsd %%xmm1, %%xmm11\n"
            "divsd %%xmm0, %%xmm11\n" // mul_10
            :
            : "m"(fpa_ptr), "m"(fpb_ptr)
            : "%xmm0", "%xmm1", "%xmm2", "memory",
              "%xmm3", "%xmm4", "%xmm5",
              "%xmm6", "%xmm7", "%xmm8",
              "%xmm9", "%xmm10", "%xmm11",
              "%rax", "%rbx"
        );
}

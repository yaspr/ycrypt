/*
  Comparison between the compiler generated code and the hand-optimized
  routine from PRESENT
*/

//
#include <stdio.h>
#include <stdlib.h>

//
typedef unsigned long long Q;

//
#define F(i, n) for (i = 0; i < n; i++)

//
#define R(v, n) (((v) >> (n)) | ((v) << (64 - n)))

//
static inline void tsc_sync()
{
  //
  asm volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
}

//
Q rdtsc()
{
  //
  Q a, d;

  //
  asm volatile ("rdtsc" : "=a" (a), "=d" (d) : : "%rbx", "%rcx");

  //
  return ((d << 32) | a);
}

//
Q rdtscp()
{
  //
  Q a, d;

  //
  asm volatile ("rdtscp" : "=a" (a), "=d" (d) : : "%rbx", "%rcx");

  //
  return ((d << 32) | a);
}

//
Q rdtsc_start()
{
  //
  tsc_sync();
  
  //
  return rdtsc();
}

//
Q rdtsc_stop()
{
  //
  Q t = rdtscp();
  
  //
  tsc_sync();
  
  //
  return t;
}

/*

  gcc -O3
    11a0:       48 ba 00 00 10 00 20    movabs $0x30002000100000,%rdx ; r
    11a7:       00 30 00 
    11aa:       45 31 c0                xor    %r8d,%r8d
    11ad:       31 f6                   xor    %esi,%esi
    11af:       90                      nop
    11b0:       89 f1                   mov    %esi,%ecx   ; ecx --> j
    11b2:       48 89 f8                mov    %rdi,%rax   ; rax --> p
    11b5:       48 83 c6 01             add    $0x1,%rsi
    11b9:       48 d3 e8                shr    %cl,%rax    ; rax --> p << r
    11bc:       89 d1                   mov    %edx,%ecx
    11be:       48 83 c2 01             add    $0x1,%rdx   ; rdx --> r (r = r + 1)
    11c2:       83 e0 01                and    $0x1,%eax   ; 
    11c5:       48 c1 ca 10             ror    $0x10,%rdx
    11c9:       48 d3 e0                shl    %cl,%rax
    11cc:       49 09 c0                or     %rax,%r8
    11cf:       48 83 fe 40             cmp    $0x40,%rsi
    11d3:       75 db                   jne    11b0 <C_version+0x10>
    11d5:       4c 89 c0                mov    %r8,%rax
    11d8:       c3                      ret    
    11d9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

  icc -O3
  4012a0:       33 c0                   xor    %eax,%eax
  4012a2:       48 bf 00 00 10 00 20    movabs $0x30002000100000,%rdi
  4012a9:       00 30 00 
  4012ac:       33 f6                   xor    %esi,%esi
  4012ae:       48 ba 00 00 00 00 00    movabs $0x1000000000000,%rdx
  4012b5:       00 01 00 
  4012b8:       89 f1                   mov    %esi,%ecx
  4012ba:       41 b8 0a 00 00 00       mov    $0xa,%r8d
  4012c0:       49 d3 e8                shr    %cl,%r8
  4012c3:       89 f9                   mov    %edi,%ecx
  4012c5:       49 83 e0 01             and    $0x1,%r8
  4012c9:       4c 8d 4f 01             lea    0x1(%rdi),%r9
  4012cd:       48 c1 e7 30             shl    $0x30,%rdi
  4012d1:       ff c6                   inc    %esi
  4012d3:       49 d3 e0                shl    %cl,%r8
  4012d6:       48 03 fa                add    %rdx,%rdi
  4012d9:       49 c1 e9 10             shr    $0x10,%r9
  4012dd:       49 0b c0                or     %r8,%rax
  4012e0:       49 0b f9                or     %r9,%rdi
  4012e3:       83 fe 40                cmp    $0x40,%esi
  4012e6:       72 d0                   jb     4012b8 <C_version+0x18>
  4012e8:       c3                      ret    
  4012e9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

  clang -O3
    1140:       48 be 00 00 10 00 20    movabs $0x30002000100000,%rsi
    1147:       00 30 00 
    114a:       31 c0                   xor    %eax,%eax
    114c:       45 31 c9                xor    %r9d,%r9d
    114f:       90                      nop
    1150:       48 89 fa                mov    %rdi,%rdx
    1153:       44 89 c9                mov    %r9d,%ecx
    1156:       48 d3 ea                shr    %cl,%rdx
    1159:       83 e2 01                and    $0x1,%edx
    115c:       89 f1                   mov    %esi,%ecx
    115e:       48 d3 e2                shl    %cl,%rdx
    1161:       48 09 c2                or     %rax,%rdx
    1164:       48 83 c6 01             add    $0x1,%rsi
    1168:       49 89 f0                mov    %rsi,%r8
    116b:       49 c1 e8 10             shr    $0x10,%r8
    116f:       41 8d 49 01             lea    0x1(%r9),%ecx
    1173:       48 89 f8                mov    %rdi,%rax
    1176:       48 d3 e8                shr    %cl,%rax
    1179:       83 e0 01                and    $0x1,%eax
    117c:       44 89 c1                mov    %r8d,%ecx
    117f:       48 d3 e0                shl    %cl,%rax
    1182:       48 c1 c6 30             rol    $0x30,%rsi
    1186:       48 09 d0                or     %rdx,%rax
    1189:       48 83 c6 01             add    $0x1,%rsi
    118d:       48 c1 c6 30             rol    $0x30,%rsi
    1191:       49 83 c1 02             add    $0x2,%r9
    1195:       49 83 f9 40             cmp    $0x40,%r9
    1199:       75 b5                   jne    1150 <C_version+0x10>
    119b:       c3                      ret    
    119c:       0f 1f 40 00             nopl   0x0(%rax)

  aocc -O3
  2010f0:       48 be 00 00 10 00 20    movabs $0x30002000100000,%rsi
  2010f7:       00 30 00 
  2010fa:       31 c0                   xor    %eax,%eax
  2010fc:       45 31 c9                xor    %r9d,%r9d
  2010ff:       90                      nop
  201100:       48 89 fa                mov    %rdi,%rdx
  201103:       44 89 c9                mov    %r9d,%ecx
  201106:       48 d3 ea                shr    %cl,%rdx
  201109:       83 e2 01                and    $0x1,%edx
  20110c:       89 f1                   mov    %esi,%ecx
  20110e:       48 d3 e2                shl    %cl,%rdx
  201111:       48 09 c2                or     %rax,%rdx
  201114:       48 83 c6 01             add    $0x1,%rsi
  201118:       49 89 f0                mov    %rsi,%r8
  20111b:       49 c1 e8 10             shr    $0x10,%r8
  20111f:       41 8d 49 01             lea    0x1(%r9),%ecx
  201123:       48 89 f8                mov    %rdi,%rax
  201126:       48 d3 e8                shr    %cl,%rax
  201129:       83 e0 01                and    $0x1,%eax
  20112c:       44 89 c1                mov    %r8d,%ecx
  20112f:       48 d3 e0                shl    %cl,%rax
  201132:       48 c1 c6 30             rol    $0x30,%rsi
  201136:       48 09 d0                or     %rdx,%rax
  201139:       48 83 c6 01             add    $0x1,%rsi
  20113d:       48 c1 c6 30             rol    $0x30,%rsi
  201141:       49 83 c1 02             add    $0x2,%r9
  201145:       49 83 f9 40             cmp    $0x40,%r9
  201149:       75 b5                   jne    201100 <C_version+0x10>
  20114b:       c3                      ret    
  20114c:       0f 1f 40 00             nopl   0x0(%rax)

  icx -O3
  2010f0:       48 be 00 00 10 00 20    movabs $0x30002000100000,%rsi
  2010f7:       00 30 00 
  2010fa:       31 c0                   xor    %eax,%eax
  2010fc:       45 31 c9                xor    %r9d,%r9d
  2010ff:       90                      nop
  201100:       48 89 fa                mov    %rdi,%rdx
  201103:       44 89 c9                mov    %r9d,%ecx
  201106:       48 d3 ea                shr    %cl,%rdx
  201109:       83 e2 01                and    $0x1,%edx
  20110c:       89 f1                   mov    %esi,%ecx
  20110e:       48 d3 e2                shl    %cl,%rdx
  201111:       48 09 c2                or     %rax,%rdx
  201114:       48 83 c6 01             add    $0x1,%rsi
  201118:       49 89 f0                mov    %rsi,%r8
  20111b:       49 c1 e8 10             shr    $0x10,%r8
  20111f:       41 8d 49 01             lea    0x1(%r9),%ecx
  201123:       48 89 f8                mov    %rdi,%rax
  201126:       48 d3 e8                shr    %cl,%rax
  201129:       83 e0 01                and    $0x1,%eax
  20112c:       44 89 c1                mov    %r8d,%ecx
  20112f:       48 d3 e0                shl    %cl,%rax
  201132:       48 c1 c6 30             rol    $0x30,%rsi
  201136:       48 09 d0                or     %rdx,%rax
  201139:       48 83 c6 01             add    $0x1,%rsi
  20113d:       48 c1 c6 30             rol    $0x30,%rsi
  201141:       49 83 c1 02             add    $0x2,%r9
  201145:       49 83 f9 40             cmp    $0x40,%r9
  201149:       75 b5                   jne    201100 <C_version+0x10>
  20114b:       c3                      ret    
  20114c:       0f 1f 40 00             nopl   0x0(%rax)

*/

//
Q C_version(Q p)
{
  Q j = 0;
  
  Q t = 0;
  Q r = 0x0030002000100000; //0, 16, 32, 48, ...
  
  F(j, 64)
    {
      t |= ((p >> j) & 0x01) << (r & 0xFF);
      r = R(r + 1, 16);
    }

  return t;
}

//
Q ASM_version1(Q p)
{
  Q t = 0;
  
  __asm__ (
	   ".intel_syntax noprefix;\n\t"
	   
	   "mov ecx, 0x30201000;\n\t"  
	   "xor %[_t_], %[_t_];\n\t"   
	   "xor ebx, ebx;\n\t"
	   
	   "1:;\n\t"
	   
	   "shr %[_p_], 1;\n\t"        
	   "jnc 2f;\n\t"               
	   "bts %[_t_], rcx;\n\t"      
	   
	   "2:;\n\t"
	   "inc cl;\n\t"              
	   "ror ecx, 8;\n\t"          
	   
	   "add bl, 4;\n\t"          
	   "jne 1b;\n\t"
	   
	   ".att_syntax prefix;\n\t"
	   
	   : //output
	     [_t_] "=r" (t)
	   : //input
	     [_p_] "r" (p)
	   : //clobber
	     "cc", "memory", "rcx", "rbx"
	   );
  
  return t;
}

//
//
Q ASM_version2(Q p)
{
  Q t = 0;
  
  __asm__ (
	   ".intel_syntax noprefix;\n\t"
	   
	   "mov edx, 0x30201000;\n\t"
	   "xor %[_t_], %[_t_];\n\t"
	   
	   "1:;\n\t"
	   "shr %[_p_], 1;\n\t"
	   "jnc 2f;\n\t"
	   "bts %[_t_], rdx;\n\t"
	   
	   "2:;\n\t"

	   "inc dl;\n\t"
	   "ror edx, 8;\n\t"
	   
	   "add cl, 4;\n\t"          
	   "jne 1b;\n\t"
	   
	   ".att_syntax prefix;\n\t"
	   
	   : //output
	     [_t_] "=r" (t)
	   : //input
	     [_p_] "r" (p)
	   : //clobber
	     "cc", "memory", "rcx", "rdx"
	   );
  
  return t;
}

//
#define MAX_ITER 100000000

//
Q measure(char *title, Q (*k)(Q), Q p)
{
    //
  Q t = 0;
  Q a = 0, b = 0, e = 0;
  
  b = rdtsc_start();
  
  //
  for (Q i = 0; i < MAX_ITER; i++)
    t += k(p);
  
  a = rdtsc_stop();

  e = (a - b) / MAX_ITER;

  //
  if (title)
    printf("%s:\t", title);

  //
  printf("t = 0x%016llx; elapsed = %llu cycles\n", t, e); 

  return e;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 2)
    return 1;
  
  //
  Q p = atoll(argv[1]);
  
  //
  Q C   = measure("C version", C_version, p);
  
  //
  Q ASM1 = measure("ASM version1", ASM_version1, p);

  //
  Q ASM2 = measure("ASM version2", ASM_version2, p);
  
  //
  printf("Speedup C    vs ASM1: %lf %%\n", (((double)C / (double)ASM1) - 1.0) * 100.0);
  printf("Speedup C    vs ASM2: %lf %%\n", (((double)C / (double)ASM2) - 1.0) * 100.0);
  printf("Speedup ASM1 vs ASM2: %lf %%\n", (((double)ASM1 / (double)ASM2) - 1.0) * 100.0);
  
  //
  return 0;
}

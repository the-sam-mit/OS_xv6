#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

// IOBound Function
void IObound() {
  int i = 1;
  while(i < 10000){
    sleep(2);
    i++;
  }
}

// CPUBound Function
void CPUbound() {
  int j = 21, i = 1, k = 0;
  while(i<2000) {
    k = 0;
    while(k < 100000){
      j += k;
      k++;
    }
	k = 0;
    while(k < 100000){
      j -= k;
      k++;
    }
    i++;
  }
  printf(1, "%d\n", j);
}

// This process invokes a IOBound process
void new_ioprocess(int prio) {
  int f = fork();
  if (f < 0) {
    printf(1, "%d parent, ERROR in FORK:%d\n", getpid(), f);
  }

  if (f == 0) { 
    setprio(prio);
    printf(1, "%d with priority: %d\n", getpid(), getprio());
    int begin = uptime();
    IObound();
    printf(1, "%d began at %d, difference time of %d\n", getpid(), begin, uptime() - begin);
    exit();
  }
}

// This process invokes a CPUBound process
void newprocess(int prio) {
  int f = fork();
  if (f < 0) {
    printf(1, "%d parent, ERROR in FORK:%d\n", getpid(), f);
  }

  if (f == 0) { 
    setprio(prio);
    printf(1, "%d with priority: %d\n", getpid(), getprio());
    int begin = uptime();
    CPUbound();
    printf(1, "%d began at %d, difference time of %d\n", getpid(), begin, uptime() - begin);
    exit();
  }
}


void testcase1() {
  newprocess(1);
  newprocess(2);
  newprocess(11);
  while(wait()!=-1);
  return;
}


void testcase2() {
  new_ioprocess(1);
  new_ioprocess(8);
  while(wait()!=-1);
  return;
}

int
main(int argc, char *argv[])
{
  printf(1, "\nStarted testmyscheduler[PID : %d] at uptime %d\n\n\n", getpid(), uptime());
  setprio(10);
  printf(1, "\n\nRunning testcase 1 (3 cpu bound processes)\n", getpid());
  testcase1();
  printf(1, "\n\nRunning testcase 2 (2 io bound processes)\n", getpid());
  testcase2();
  exit();
}

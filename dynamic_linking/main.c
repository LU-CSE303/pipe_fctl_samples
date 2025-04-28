#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
int x[2] = {1, 2};
int y[2] = {3, 4};
int a[2], m[2];
int main(){
   void *handle;
   void (*addvec)(int*, int*, int*, int);
   void (*multvec)(int*, int*, int*, int);
   char *error;
   handle = dlopen("./libvector.so", RTLD_LAZY);// load shared library
   if(!handle){
      fprintf(stderr, "%s\n", dlerror()); exit(1);}
   addvec = dlsym(handle, "addvec");// get pointer to addvec()
   if((error = dlerror()) != NULL) { 
      fprintf(stderr, "%s\n", error); exit(1);}
   addvec(x, y, a, 2); // call addvec

   multvec = dlsym(handle, "multvec");// get pointer to multvec()
   if((error = dlerror()) != NULL) { 
      fprintf(stderr, "%s\n", error); exit(1);}
   multvec(x, y, m, 2); // call multvec

   printf("[%d %d] + [%d %d] = [%d %d]\n",x[0], x[1], y[0], y[1], a[0], a[1]);
   printf("[%d %d] * [%d %d] = [%d %d]\n",x[0], x[1], y[0], y[1], m[0], m[1]);
   if(dlclose(handle) < 0) {  // unload shared library
      fprintf(stderr, "%s\n", dlerror()); exit(1);}
   return 0;
}
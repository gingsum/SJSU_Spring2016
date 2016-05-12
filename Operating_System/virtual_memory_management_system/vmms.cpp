/* 
  vmms.cpp - This file contains the code for each of the memory functions as well as initialization of the "shared" memory.
*/

#include "vmms_error.h"
#include <string.h>

// my includes
#include <stdio.h> // printf
#include <stdlib.h>
#include <tchar.h> // TCHAR
#include <process.h> // getpid
#include <windows.h>  //GetLocalTime
#include <psapi.h> // process handle

#define MAX_PHY_SIZE 8196    // 8K Bytes     ** Hardcode for testing !!
#define MAX_TABLE_SIZE 1024  // 1K entries
#define DEFAULT_BOUNDRY 8    // minimum 8 byte allocation block

// ************************************************************************
// Global Shared variables (shared by multiple instances of the DLL)
// ************************************************************************

/* Global shared memory section */
#pragma data_seg (".SHARED")  // Simulated Physical Mem // Hardcoded for now !!
int byte_boundry = DEFAULT_BOUNDRY;
int mem_size = MAX_PHY_SIZE;            // size of simulated phy mem (in bytes)
int tab_size = MAX_TABLE_SIZE;          // size of memory table
char mem_start [MAX_PHY_SIZE] = {0};    // simulated Phy Memory

// my global variables
struct Map_Table{
  int pid;
  int request_size;
  int actual_size;
  int pos_from_head;
  unsigned long int client_address;
  long long last_reference;
} mt_list[MAX_TABLE_SIZE] = {0,0,0,0,0,0};

struct Free_Table{
  int size;
  int pos_from_head;
  //unsigned long int pos_ptr;
} ft_list[MAX_TABLE_SIZE] = {0,0};
int file_index = 0;
#pragma data_seg ()

// global file streams
FILE *mms_log;
FILE *vmms_mem;

// get process name
HANDLE pHandle;
TCHAR process_name[MAX_PATH];

  // function that returns process name
char* get_process_name(){
    pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _getpid());
    if(pHandle != NULL){
      if(GetModuleFileNameEx(pHandle, NULL, process_name, MAX_PATH) == 0){
        printf("failed to get module name.\n");
        return "";
        } else{
          // converts filepath to file name
          char* converted_process_name = (char *) process_name;
          int i = 0 ;
          int slash = 0;
            
          char * a;
          for (a=converted_process_name; *a; a++) {
            if(*a == '\\')
              slash = i;
              i++;
          }
          return converted_process_name+(slash+1);
        }
        CloseHandle(pHandle);
      } else {
        printf("failed to open process.\n");
        return "";
    }
}

// get local time and return as long long
long long get_time(){
  SYSTEMTIME st;              // set local time
  GetLocalTime(&st);
  char* timestamp;
  timestamp = (char*)malloc(4+2+2+2+2+2);
  sprintf(timestamp, "%d%d%d%d%d%d",
          st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
  return atoll(timestamp);
}

// function that logs the process
void log_malloc( char* operation, char* output, int size, int* error_code){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %x %d %d\n", get_time(), get_process_name(), 
          _getpid(), operation, (unsigned int) output, size, error_code[0]);
  fclose(mms_log);
}
void log_memset( char* operation, int output, char* dest_ptr, char c, int size){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x %c %d\n", get_time(), get_process_name(), 
          _getpid(), operation, output, (unsigned int)dest_ptr, c, size);
  fclose(mms_log);
}
void log_memcpy( char* operation, int output, char* dest_ptr, char* src_ptr, int size){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x %x %d\n", get_time(), get_process_name(), 
          _getpid(), operation, output, (unsigned int)dest_ptr, (unsigned int)src_ptr, size);
  fclose(mms_log); 
}
void log_print( char* operation, int output, char* src_ptr, int size){
    mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x %d\n", get_time(), get_process_name(), 
          _getpid(), operation, output, (unsigned int)src_ptr, size);
  fclose(mms_log);
}
void log_free( char* operation, int output, char* mem_ptr){
    mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x\n", get_time(), get_process_name(), 
          _getpid(), operation, output, (unsigned int)mem_ptr);
  fclose(mms_log);
}
void log_initialize( char* operation, int output, int boundry_size){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %d\n", get_time(), get_process_name(), 
          _getpid(), operation, output, boundry_size);
  fclose(mms_log);
}
void log_display_table( char* operation, int output, char* filename){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x\n", get_time(), get_process_name(), 
          _getpid(), operation, output, (unsigned int)filename);
  fclose(mms_log);
}
void log_display_memory( char* operation, int output, char* filename){
  mms_log = fopen("mms.log", "a");
  fprintf(mms_log, "%lld %s %d %s %d %x\n", get_time(), get_process_name(), 
          _getpid(), operation, output,(unsigned int) filename);
  fclose(mms_log);
}

// printing stuff
void print_list(){
  int k;
  printf("Memory Management Table\n");
  printf("pid\trequest_size\tposition_index\tactual_size\taddress\t\ttime\n");
  for(k = 0; k < tab_size; k++){
    if(mt_list[k].pid != 0 && mt_list[k].request_size != 0){
     printf("%d\t%d\t\t%d\t\t%d\t\t%x\t%lld\n", mt_list[k].pid, mt_list[k].request_size, 
            mt_list[k].pos_from_head, mt_list[k].actual_size, mt_list[k].client_address,
            mt_list[k].last_reference);
     }
  }
  int j;
  printf("\nFree Memory Table\n");
  printf("size\tposition_index\n");
  for(j = 0; j < tab_size; j++){
    if(ft_list[j].size != 0 ){
      printf("%d\t%d\n", ft_list[j].size, ft_list[j].pos_from_head);
    }  
  }
  printf("\n");
}

// flush <address> <bytes> <character representation> to VMMS.MEM
void flush_process(){
  vmms_mem = fopen("VMMS.MEM", "wb"); 
  int k;
  for(k = 0; k < tab_size; k++){
    if(mt_list[k].pid != 0 && mt_list[k].request_size != 0){
      int block_size = (mt_list[k].request_size/16 + 1)*16;
      int block;
      for(block = 0; block < block_size; block += 16){      
        char* memory_address = (char*) malloc(16);
        char* output = (char*) malloc (73);
        unsigned char hex[32]; 
        unsigned char chr[32];

        sprintf(memory_address, "%x", (unsigned int)&(mem_start[mt_list[k].pos_from_head]) + block);
        int i;
        // get the hex from memory
        for(i = block; i < block + 16; i++){
          if(mem_start[mt_list[k].pos_from_head + i] == 0){
            mem_start[mt_list[k].pos_from_head + i] = 0xFF;
          }
          if(mem_start[mt_list[k].pos_from_head + i] == -1){
            hex[i-block] = 0xFF;
          }else{
              hex[i-block] = mem_start[mt_list[k].pos_from_head + i];
          }
        }         
        // get the char from memory
        int j;
        for(j = block; j < block + 16; j++){   
          chr[j-block] = mem_start[mt_list[k].pos_from_head + j];
        }
        sprintf(output, "%s %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
                memory_address, hex[0], hex[1], hex[2], hex[3], hex[4], hex[5], hex[6], hex[7], hex[8], hex[9], 
                hex[10], hex[11], hex[12], hex[13], hex[14], hex[15], chr[0], chr[1],chr[2],chr[3],chr[4],
                chr[5], chr[6], chr[7], chr[8], chr[9], chr[10], chr[11], chr[12], chr[13], chr[14], chr[15]);
        fprintf(vmms_mem, "%s", output);
        mem_start[mt_list[k].actual_size -1] = NULL;
        free(output);
      }
    }
  }
  fclose(vmms_mem);
}

void print_flush(){
  vmms_mem = fopen("VMMS.MEM", "r"); 
  int k;
  for(k = 0; k < tab_size; k++){
    if(mt_list[k].pid != 0 && mt_list[k].request_size != 0){
      int block_size = (mt_list[k].request_size/16 + 1)*16;
      int block;
      for(block = 0; block < block_size; block += 16){      
        printf( "%x", (unsigned int)&(mem_start)[mt_list[k].pos_from_head] + block);
        int i;
        // get the hex from memory
        for(i = block; i < block + 16; i++){
          if(mem_start[mt_list[k].pos_from_head + i] == -1){
            printf(" %x", 0xFF);
          }else{
            printf(" %x", mem_start[mt_list[k].pos_from_head + i]);
          }
        }         
        // get the char from memory
        printf(" ");
        int j;
        for(j = block; j < block + 16; j++){   
          printf( "%c", mem_start[mt_list[k].pos_from_head + j]);
        }
        printf("\n");
      }
    }
  }
  fclose(vmms_mem); 
}

void merge_ft(){
  // sort the list by pos
    // go through the list looking for smallest pos
    // put smallest pos to the top and repeat for 2nd array
    // stop when the rest of the elements are 0
  int total_count = 0;
  int temp_size = 0;
  int temp_pos = 0;
  int temp_index = 0;



  int i;
  for(i = 0; i < tab_size; i++){
    int j;
    if(ft_list[i].size > 0){
      total_count++;
      for(j = i; j < tab_size; j++){
        if(ft_list[j].size > 0){
          if(ft_list[i].pos_from_head > ft_list[j].pos_from_head){
            temp_size = ft_list[i].size;
            temp_pos = ft_list[i].pos_from_head;
            
            ft_list[i].size = ft_list[j].size;
            ft_list[i].pos_from_head = ft_list[j].pos_from_head;


            ft_list[j].size = temp_size;
            ft_list[j].pos_from_head = temp_pos;
          }
        }
      }
    }
  }

  // merge consecutive elements
  int curr_size = ft_list[0].size;
  int curr_pos = ft_list[0].pos_from_head;
  int curr_index = 0;
  int k;
  for(k = 1; k < total_count; k++){
    if(curr_size + curr_pos == ft_list[k].pos_from_head){
      ft_list[curr_index].size = ft_list[curr_index].size + ft_list[k].size;
      ft_list[k].size = 0;
      ft_list[k].pos_from_head = 0;

      curr_size = ft_list[k].size;
    } else {
      curr_size = ft_list[k].size;
      curr_pos = ft_list[k].pos_from_head;
      curr_index = k;
    }
  }  
}

/* Here are the 5 exported functions for the application programs to use */
__declspec(dllexport) char* vmms_malloc (  int size, int* error_code );
__declspec(dllexport) int vmms_memset ( char* dest_ptr, char c, int size );
__declspec(dllexport) int vmms_memcpy ( char* dest_ptr, char* src_ptr, int size );
__declspec(dllexport) int vmms_print ( char* src_ptr, int size );
__declspec(dllexport) int vmms_free ( char* mem_ptr );

/* Here are several exported functions specifically for mmc.cpp */
__declspec(dllexport) int mmc_initialize (  int boundry_size );
__declspec(dllexport) int mmc_display_memtable ( char* filename );
__declspec(dllexport) int mmc_display_memory ( char* filename );
 

__declspec(dllexport) int mmc_initialize (  int boundry_size )
{
  int rc = VMMS_SUCCESS;
  byte_boundry = boundry_size;
  
  // set all memory to 0xFF
  int i;
  for(i = 0; i < mem_size; i++){
    mem_start[i] = (unsigned char)0xFF;
  }

  // initialize Mapping Table and Free Table
  // set all values to 0
  int j;
  for(j = 0; j < tab_size; j++){
    mt_list[j].pid = 0;
    mt_list[j].request_size = 0;
    mt_list[j].actual_size = 0;
    mt_list[j].pos_from_head = 0;
    mt_list[j].client_address = 0;
    mt_list[j].last_reference = 0;
    
    ft_list[j].size = 0;
    ft_list[j].pos_from_head = 0;
  }
  
  // set the head for first free memory position
  ft_list[0].size = mem_size;
  ft_list[0].pos_from_head = 0;
    
  // setup log file and virtual memory file
  mms_log = fopen("mms.log", "w");
  fclose(mms_log);
  vmms_mem = fopen("VMMS.MEM", "w");
  fclose(vmms_mem);  
  
  //log the first operation: mmc_initlialize
  log_initialize("mmc_initialize", rc, byte_boundry);
  
  return rc;
}

__declspec(dllexport) int mmc_display_memtable ( char* filename )
{
  int rc = VMMS_SUCCESS;
  // log the process: mmc_display_memtable
  print_list();
  log_display_table("mmc_display_memtable", rc, filename);
  return rc;
}

__declspec(dllexport) int mmc_display_memory ( char* filename )
{
  int rc = VMMS_SUCCESS;
  print_flush();
  log_display_memory("mmc_display_memory", rc, filename);
  
  return rc;
}

__declspec(dllexport) char* vmms_malloc (  int size, int* error_code )
{  
  *error_code = VMMS_SUCCESS;
  // check free_table for the smallest free memory that can fit size
  // if free table doesn't have size, return error code 100

  int i;
  int index_size = mem_size + 1;
  int index_temp_pos = 0;
  int index_smallest = 0;

  for(i = 0; i < tab_size; i++){
    if(ft_list[i].size > size && ft_list[i].size < index_size){
      index_smallest = i;
      index_size = ft_list[i].size;
      index_temp_pos = ft_list[i].pos_from_head;
    }
    if(index_size == mem_size + 1 && i == tab_size-1){
      error_code[0] = OUT_OF_MEM;
      //return NULL;
      break;
    }
  }
  // find the first free slot in mt_list 
  if ( error_code[0] != 100){
    int j;
    for(j = 0; j < tab_size; j++){
      if(mt_list[j].pid == 0 && mt_list[j].request_size == 0){
        mt_list[j].pid = _getpid();
        mt_list[j].request_size = size;
        mt_list[j].pos_from_head = ft_list[index_smallest].pos_from_head;
        if (size%byte_boundry == 0)
          mt_list[j].actual_size = ((size/byte_boundry)*byte_boundry);
        else
          mt_list[j].actual_size = ((size/byte_boundry + 1)*byte_boundry);
        mt_list[j].client_address = (unsigned long int) &(mem_start)[ft_list[index_smallest].pos_from_head];
        mt_list[j].last_reference = get_time();
        break;
      }
    }

    // now that the free pointer is occupied, free it and create a new one
    ft_list[index_smallest].size = 0;
    ft_list[index_smallest].pos_from_head = 0;

    // put the new memory into ft_list
    if (index_size - size > 0){
      ft_list[index_smallest].size = index_size - mt_list[j].actual_size;
      ft_list[index_smallest].pos_from_head = index_temp_pos + mt_list[j].actual_size; 
    } else {
      ft_list[index_smallest].size = 0;
      ft_list[index_smallest].pos_from_head = 0;
    }
  }

  // merge the ft_list and flush
  merge_ft();
  flush_process();

  // log and flush the process: vmms_malloc

  if( error_code[0] != 100){
    log_malloc("vmms_malloc", mem_start + index_temp_pos, size, error_code);
    return mem_start + index_temp_pos;
  }else{
    log_malloc("vmms_malloc", NULL, size, error_code);
    return NULL;
  }
}

__declspec(dllexport) int vmms_memset ( char* dest_ptr, char c, int size )
{
  int rc = VMMS_SUCCESS;
  // search mt_list for dest_ptr match, 
  int i;
  for(i = 0; i < tab_size; i++){
    if(mt_list[i].client_address != 0){
      unsigned long int temp_d = (unsigned long int) dest_ptr;
      if (mt_list[i].client_address + mt_list[i].request_size >= temp_d &&
          mt_list[i].client_address <= temp_d){                     
        // if found, check size
        if (mt_list[i].client_address + mt_list[i].request_size >= temp_d + size){
          // if size valid, copy c to mem_start @ dest_ptr with number equal to size
          int j;
          for(j = 0; j < size; j++){
            dest_ptr[j] = c;
          }
          break;
        } else {
          // else return 101
          rc = MEM_TOO_SMALL;
          break;
        }
      }
    }
    if (i == tab_size-1 || (unsigned long int )dest_ptr[0] > 0xFF){
        // if not found return 102
        rc = INVALID_DEST_ADDR;
        break;
    }
  } 
  
  // log the process: vmms_memset
  log_memset("vmms_memset", rc, dest_ptr, c, size);
  flush_process();
  
  return rc;
}

__declspec(dllexport) int vmms_memcpy ( char* dest_ptr, char* src_ptr, int size )
{
  int rc = VMMS_SUCCESS;

  // checks validity of the ptr
  int i;
  for(i = 0; i < tab_size; i++){
    if(mt_list[i].client_address != 0){
      unsigned long int temp_d = (unsigned long int) dest_ptr;
      if (mt_list[i].client_address + mt_list[i].request_size >= temp_d &&
          mt_list[i].client_address <= temp_d){                     
        // if found, check size
        if (mt_list[i].client_address + mt_list[i].request_size >= temp_d + size){
          // if size valid, copy c to mem_start @ dest_ptr with number equal to size
          strncpy(dest_ptr, src_ptr, size);
          break;
        } else {
          // else return 101
          rc = MEM_TOO_SMALL;
          break;
        }
      }
    }
    if (i == tab_size-1 || (unsigned long int )dest_ptr[0] > 0xFF || (unsigned long int )src_ptr[0] > 0xFF){
      // if not found return 102
      rc = INVALID_CPY_ADDR;
      break;
    }
  } 

  // log the process: vmms_memcpy
  log_memcpy("vmms_memcpy", rc, dest_ptr, src_ptr, size); 
  flush_process();
  return rc;
}

__declspec(dllexport) int vmms_print ( char* src_ptr, int size )
{
  int rc = VMMS_SUCCESS;

  if ((unsigned long int )src_ptr[0] > 0xFF){ // figure out what's invalid 
    rc = INVALID_CPY_ADDR;
   }else if(size == 0){
    int i;
    for (i = 0; src_ptr[i] != NULL; i++) {
      if (src_ptr[i] == -1)
        printf("%c", 0xFF);
      else        
        printf("%c", src_ptr[i]);  
    }
  } else {
    int j;
    for (j = 0; j < size; j++) {
      if (src_ptr[j] == -1)
        printf("%c", 0xFF);
      else        
        printf("%c", src_ptr[j]);
    }  
  } 

  // log the process: vmms_print
  log_print("vmms_print", rc, src_ptr, size);
  
  return rc;
}

__declspec(dllexport) int vmms_free ( char* mem_ptr )
{
  int rc = VMMS_SUCCESS;
  // find the mem_ptr from the table
  // if found, set 

  int i;
  unsigned long int temp_d = (unsigned long int) mem_ptr;
  for(i = 0; i < tab_size; i++){
    if(mt_list[i].client_address != 0){
      if (mt_list[i].client_address == temp_d){
        // find first empty element in ft_list and store the new memory in it
        // delete the element from mt_list by setting all it's values to 0
        // set mem_start elements to 0xFF
        int j;
        for(j = 0; j < tab_size; j++){
          if(ft_list[j].size == 0 && ft_list[j].pos_from_head == 0){
            ft_list[j].size = mt_list[i].actual_size;
            ft_list[j].pos_from_head = mt_list[i].pos_from_head;

            int k;
            for (k = 0; k < mt_list[i].actual_size; k++)
              mem_start[mt_list[i].pos_from_head + k] = 0xFF;
            
            mt_list[i].pid = 0;
            mt_list[i].request_size = 0;
            mt_list[i].actual_size = 0;
            mt_list[i].pos_from_head = 0;
            mt_list[i].client_address = 0;
            mt_list[i].last_reference = 0;

            break;
          }
        }
      }
    }
    if (i == tab_size-1 ){
      // if not found return 102
      rc = INVALID_MEM_ADDR;
      break;
    }
  } 

  merge_ft();
  flush_process();
  
  // log the process: vmms_free
  log_free("vmms_free", rc, mem_ptr);

  return rc;
}
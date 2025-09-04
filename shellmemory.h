
// changes for it in A3, need to adapt it for pages frames 1.2
#define frame_length 3

// for 2.1
#ifndef FRAME_S_SIZE
#define FRAME_S_SIZE 900 
#endif

#ifndef VAR_S_SIZE
#define VAR_S_SIZE 100
#endif

#define frame_counter (FRAME_S_SIZE / frame_length)
#define frame_size FRAME_S_SIZE
#define var_size VAR_S_SIZE
#define MEM_SIZE FRAME_S_SIZE


struct loading_struct {
    char *lines[frame_size];
    int used[frame_size];
};

extern struct loading_struct source_loading;

void mem_init ();
void init_source ();            // A2-1
char *mem_get_value (char *var);
void mem_set_value (char *var, char *value);

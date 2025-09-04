#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"
#include <dirent.h>             // needed for 3.1
#include <ctype.h>              // needed for 3.2 and 3.3
#include <sys/stat.h>
#include <sys/types.h>          // to create directories
#include<unistd.h>              // included for 3.4
#include <sys/wait.h>           // included for 5
#include <stdbool.h>            // to use in A3 1.1



int MAX_ARGS_SIZE = 6;

int badcommand () {
    printf ("Unknown Command\n");
    return 1;
}

// For source command only
int badcommandFileDoesNotExist () {
    printf ("Bad command: File not found\n");
    return 3;
}

int help ();
int quit ();
int set (char *var, char *value);
int print (char *var);
int source (char *script);
int badcommandFileDoesNotExist ();
int echo (char *arg);
int my_ls ();
int my_mkdir (char *d_name);
int my_touch (char *f_name);
int my_cd (char *d_name);
int run (char *args[], int args_size);
void run_fcfs ();
void run_rr (int slice);
void run_aging ();
void run_sjf ();
int exec (char *programs[], int counter_programs, char *policy, int back_mode);

// Interpret commands and their arguments
int interpreter (char *command_args[], int args_size) {
    int i;

    if (args_size < 1 || args_size > MAX_ARGS_SIZE) {
        return badcommand ();
    }

    for (i = 0; i < args_size; i++) {   // terminate args at newlines
        command_args[i][strcspn (command_args[i], "\r\n")] = 0;
    }

    if (strcmp (command_args[0], "help") == 0) {
        //help
        if (args_size != 1)
            return badcommand ();
        return help ();

    } else if (strcmp (command_args[0], "quit") == 0) {
        //quit
        if (args_size != 1)
            return badcommand ();
        return quit ();

    } else if (strcmp (command_args[0], "set") == 0) {
        //set
        if (args_size != 3)
            return badcommand ();
        return set (command_args[1], command_args[2]);

    } else if (strcmp (command_args[0], "print") == 0) {
        if (args_size != 2)
            return badcommand ();
        return print (command_args[1]);

    } else if (strcmp (command_args[0], "source") == 0) {
        if (args_size != 2)
            return badcommand ();
        return source (command_args[1]);
    }


    // alright implement exec for A2-2
    else if (strcmp (command_args[0], "exec") == 0) {
        // up to 3 files + command + policy + # = 6
        if (args_size > 6 || args_size < 3) {
            printf ("exec needs 2-4 arguments\n");
            return 1;
        }

        // add backgrod option
        int back_mode = 0;      // no as standart
        char *policy;
        // for no backgroud mode
        if (strcmp (command_args[args_size - 1], "#") != 0) {
            // verify if the policy is correct, if not give an error
            policy = command_args[args_size - 1];
            if (strcmp (policy, "FCFS") == 0 || strcmp (policy, "SJF") == 0
                || strcmp (policy, "AGING") == 0 || strcmp (policy, "RR") == 0
                || strcmp (policy, "RR30") == 0) {

                // if its a valid policy, we put the 3 program in an arra
                int current_program = 1;
                int counter_programs = args_size - 2;
                char *programs_arr[3] = { NULL, NULL, NULL };
                while (current_program <= counter_programs) {
                    programs_arr[current_program - 1] =
                        command_args[current_program];
                    current_program = current_program + 1;
                }
                // finally we run exec
                return exec (programs_arr, counter_programs, policy,
                             back_mode);
            }
        }

        else {
            back_mode = 1;
            // verify if the policy is correct, if not give an error
            policy = command_args[args_size - 2];
            if (strcmp (policy, "FCFS") == 0 || strcmp (policy, "SJF") == 0
                || strcmp (policy, "AGING") == 0 || strcmp (policy, "RR") == 0
                || strcmp (policy, "RR30") == 0) {

                // if its a valid policy, we put the 3 program in an arra
                int current_program = 1;
                int counter_programs = args_size - 3;
                char *programs_arr[3] = { NULL, NULL, NULL };
                while (current_program <= counter_programs) {
                    programs_arr[current_program - 1] =
                        command_args[current_program];
                    current_program = current_program + 1;
                }
                // finally we run exec
                return exec (programs_arr, counter_programs, policy,
                             back_mode);

            }
        }
        printf ("this not a valid policy man '%s'\n", policy);
        return 1;
    }



    // Im adding the call for our functions here 

// implement 1 -> echo  
    else if (strcmp (command_args[0], "echo") == 0) {

        // if there is no arguments, then it prints an empty line    
        if (args_size == 1) {
            echo ("");          // echo for a blank line 
            return 0;
        }
        // if the user has exactly one argument it being a string or a variable, call echo 
        if (args_size == 2) {
            return echo (command_args[1]);
        }
        // only one argument is being used in this case
        return badcommand ();
    }


    // impplement 3.1 -> my_ls
    else if (strcmp (command_args[0], "my_ls") == 0) {
        if (args_size == 1) {
            return my_ls ();
        }
        return badcommand ();
    }

    // implement 3.2 -> my_mkdir 
    else if (strcmp (command_args[0], "my_mkdir") == 0) {
        if (args_size == 2) {
            return my_mkdir (command_args[1]);
        }
        return badcommand ();
    }

    // implement 3.3 - > my_touch 
    else if (strcmp (command_args[0], "my_touch") == 0) {
        if (args_size == 2) {
            return my_touch (command_args[1]);
        }
        return badcommand ();
    }

    // implement 3.4 - > my_cd
    else if (strcmp (command_args[0], "my_cd") == 0) {
        if (args_size == 2) {
            return my_cd (command_args[1]);
        }
        return badcommand ();
    }

    // implementy 5 -> run 
    else if (strcmp (command_args[0], "run") == 0) {
        if (args_size >= 2) {
            return run (&command_args[1], args_size - 1);
        }
        return badcommand ();
    }

    // else -> wrong command or command doesn't exist

    else {
        return badcommand ();
    }
}
int help () {

    // note the literal tab characters here for alignment
    char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
source SCRIPT.TXT	Executes the file SCRIPT.TXT\n \
echo STRING            Display the String on a new life\n \
my_ls                  Display all files/directories in current directory\n \
my_mkdir D_NAME        Makes a new directory in current directory\n \
my_touch F_NAME        Makes a new file in current directory\n \
my_cd D_NAME           Go to the following directory\n \
run COM ARG1 ...       Uses the “fork-exec-wait” pattern";
    printf ("%s\n", help_string);
    return 0;
}

int quit () {
    printf ("Bye!\n");
    exit (0);
}

int set (char *var, char *value) {
    char *link = " ";

    // Hint: If "value" contains multiple tokens, you'll need to loop through them, 
    // concatenate each token to the buffer, and handle spacing appropriately. 
    // Investigate how `strcat` works and how you can use it effectively here.

    mem_set_value (var, value);
    return 0;
}


int print (char *var) {
    printf ("%s\n", mem_get_value (var));
    return 0;
}

// updatating source for A2-1
// debo read this : https://www.geeksforgeeks.org/process-table-and-process-control-block-pcb/

// struct pcb - 1.2
typedef struct PCB {
    int pid;
    int start_position;
    int length_script;
    int program_counter;
    // adding pointer to use in queue
    struct PCB *next;
    // for A2-4, have a way to keep track of job length score
    int job_length_score;
    int first_run;              // to fix annoying bug in backgroud : make sure first time, it runs first
    int page_table[10]; // for A3 - 1.4 

    // for 2.3 save the file name
    char name[100];

} PCB;

int pid_counter = 1;            // it starts at the first 

// struct ready queue - queue process -> 1.3
typedef struct ReadyQueue {
    PCB *head;
    PCB *tail;
} ReadyQueue;

// create our global queue 
ReadyQueue ready_queue = { NULL, NULL };

// to use enqueue need to have a way to queue and to dequeue 
// implement enqueue
// used : https://www.programiz.com/dsa/linked-list 
void enqueue (PCB * pcb) {
    // need to add to end of the queue
    // so next is null
    pcb->next = NULL;

    // if its not empty, head is not empty
    // so add pcb to last in queue 
    if (ready_queue.head != NULL) {
        ready_queue.tail->next = pcb;
        ready_queue.tail = pcb;
    }
    // if it is empty, pcb is head and tail of queue
    else {
        ready_queue.head = pcb;
        ready_queue.tail = pcb;
    }
}

// ofc needs dequeue as well
PCB *dequeue () {
// if its not empty remove first from the queue
    if (ready_queue.head != NULL) {
        // save head
        PCB *first_pcb;
        first_pcb = ready_queue.head;
        // remove it         
        ready_queue.head = ready_queue.head->next;
        PCB *new_head = ready_queue.head;
        if (new_head = NULL) {
            ready_queue.tail = NULL;    // queue is empty now 
        }
        // return what was removed
        return first_pcb;
    }
// if its empty return null
    return NULL;

}

// finally read to do new source 
// the new source A2-1.4
int source (char *script) {
    int errCode = 0;
    // replaced the full code with exec to make it simpler
    char *programs_arr[3] = { script, NULL, NULL };
    return exec(programs_arr, 1, "FCFS", 0);
}


// new struct to handle multiple equal files 
// so to keep in memory already loaded files
typedef struct {
    char *name;
    int start_position;
    int length_script;
} Already_loaded;



// for 2.3, to implement page fault and page handling
struct Mapping {
    PCB *pcb;
    int page_num;
};

// create frame_mapping
struct Mapping frame_mapping[frame_counter];


// for 1.2 and 1.3 for A3 - need to find the first available hole
int find_available() {
    int i = 0; 
    int length_frame = 3; // the frame size is 3
    while( i < frame_size) {
    // need to check if we find a frame free, so all 3 free inside the frame. 
        int n = 0; 
        while ( n < length_frame) {
            // break if used
	    if (source_loading.used[i + n]) {
                break;
            }
	    // so if all 3 places are free
            if ( n == 2) {
	         return i; // we have found a free frame 
	    }
    	    n++;
        }
        i = i + length_frame; // since every frame has size 3, go to next
    }

    return -1; // didnt find a free frame
}



// implement exec for A2-2
int exec (char *programs[], int counter_programs, char *policy, int back_mode) {

    // init frame mappping
    int w  = 0;
    while(w < frame_counter) {
         frame_mapping[w].page_num = -1;
         frame_mapping[w].pcb = NULL;
         w = w + 1;
    }	
	
    // create actual_pcb 
    PCB *actual_pcb = malloc (sizeof (PCB));


    // if 2 files have same name print an error and return 1
    // for A3, now we can have 2 files with same name
    // so I deleted 
    // A3 1.1 : Set up process sharing
    Already_loaded files_loaded[3]; // for now we only have 3 programs loaded max at same time 

    int counter_file = 0;
    int errCode = 0;
    while (counter_file < counter_programs) {
        // check if file was already loaded 
	bool file_not_loaded = true; // we assume it didn't  
        
	// base values for loaded
	int loaded = 0;
        int start_position = -1;
        int length_script = -1;
        int m = 0; 
	int loaded_counter = 0; // it starts at 0 files loaded with a maximun of 3 
        char *current_program = programs[counter_file]; // current program  

	while ( m < loaded_counter) {
	     // verify if the file name is in the files_loaded array
	     if ( strcmp (files_loaded[m].name, current_program) == 0) {
	            // ok so in this case we have a repeated file, save its info from the old file
		    file_not_loaded = false; 
		    loaded = 1;
		    start_position = files_loaded[m].start_position;
		    length_script = files_loaded[m].length_script;
		    break; 
	     }
	     m = m + 1;
	}
	
       
        // to keep track of page numbers for A3-1.4
        int page_num = 0;
        int pagetable[10];
        // set all to -1 as base
        for (int p = 0; p < 10; p++) {
                pagetable[p] = -1;
        } 

        // if its not loaded, we have to load it then like normal 
        if (file_not_loaded) { 	

        char line[MAX_USER_INPUT];
        int current = 0;
        FILE *p = fopen (current_program, "rt");        // the program is in a file
        // if file doesnt exist, we have an error
        if (p == NULL) {
            return badcommandFileDoesNotExist ();
        }

	// A3 modified, now takes into consideration frames
        // if frame_line = 0, need to find a new available frame
        current = find_available();
        // check if source_loading is full
        if (current == -1) {
            printf ("Error : memory is full ");
            fclose (p);
            return 1;
        }
            

        start_position = current;
        length_script = 0;
	int frame_line = 0;
	int num_lines = 0;
        fgets (line, MAX_USER_INPUT - 1, p);

        while (1) {

	   // update page table and update frame mapping
	   if ( current % 3 == 0 ) {
	       // in this case, then we are at the start of a frame/page
	       // save it in the pagetable
	       int frame_num = current / 3;
	       pagetable[page_num] = frame_num;
               // update frame mapping
	       frame_mapping[frame_num].page_num = page_num;
	       frame_mapping[frame_num].pcb = actual_pcb;

	       page_num++;
	   }

            // load the source code into the memory
            source_loading.used[current] = 1; // that line is used
            source_loading.lines[current] = strdup (line);    // store line in lines
            memset (line, 0, sizeof (line));
 
            // update position current and length_list
            length_script = length_script + 1;
            current ++;
	    num_lines++;

	    // make sure to stop at 2 pages
	    if ( num_lines == 6 ) {
	        // already 2 pages, so break
		break;
	    }

	    // check if needed to go to next available frame
	    // it should implement 1.3 from A3
	    frame_line++;
            if(frame_line == 3){
	        frame_line = 0;
		current = find_available(); 
		if (current == -1) {
                    printf ("Error : memory is full ");
                    fclose (p);
                    return 1;
                }
	    }

	    // if end of file, break
            if (feof (p)) {
                break;
            }

            fgets (line, MAX_USER_INPUT - 1, p);
        }

        fclose (p);


       // have to load its info in the struct files_loaded
       files_loaded[loaded_counter].start_position = start_position;
       files_loaded[loaded_counter].length_script = length_script;
       strcpy(files_loaded[loaded_counter].name, current_program);
       loaded_counter = loaded_counter + 1; // we have one more loaded file
					  
       } 

        // for every program 
        // make the pcb to put in the list
        // pcb is gonna be head and tail
        //PCB *actual_pcb = malloc (sizeof (PCB));
        // put its specs
        actual_pcb->pid = pid_counter++;
        actual_pcb->start_position = start_position;
        actual_pcb->length_script = length_script;
        actual_pcb->program_counter = 0;        // first program starts at 0
        actual_pcb->next = NULL;        // no next since its only one
        actual_pcb->job_length_score = length_script;   // for A2-4
        actual_pcb->first_run = 0;      // no backgroud mode so 0
	// for A3 - 1.4
	// update page table for pcb
	for (int k = 0; k < 10; k++) {
        actual_pcb->page_table[k] = pagetable[k];
       	}
        // save file name aswell	
        strcpy(actual_pcb->name, current_program);

        // enqueue each pcb 
        enqueue (actual_pcb);
        counter_file = counter_file + 1;
    }


    // load the rest of the file as a prog 0 to implement backgroud mode -> A2-5.2
    // so like we have to pull it in front of the other progs ...
    // This is achieved by converting the rest of the Shell input into a program and running i

    if (back_mode == 1) {

        // the pid of prog 0
        int last_pid = pid_counter + 1;

        int l = 0;
        // find first free space in the source_loading
        // so check the first free space in the memory
        while (source_loading.used[l] == 1) {
            l = l + 1;
        }

        // check if source_loading is full
        if (l == frame_size) {
            printf ("Error : memory is full ");
            return 1;
        }

        // so instead of reading from file, read from rest of the Shell input
        // the rest will be program 0
        int start_position_0 = l;
        int length_script_0 = 0;
        char line[MAX_USER_INPUT];

        fgets (line, MAX_USER_INPUT - 1, stdin);
        while (1) {
            // check if source_loading is full
            if (l == frame_size) {
                printf ("Error : memory is full ");
                return 1;
            }

            // load the source code into the memory
            source_loading.used[l] = 1; // that line is used
            source_loading.lines[l] = strdup (line);    // store line in lines
            memset (line, 0, sizeof (line));

            // update position l and length_list
            l = l + 1;          // update to next line in memory
            length_script_0 = length_script_0 + 1;

            // if end of the file, break
            if (fgets (line, MAX_USER_INPUT - 1, stdin) == NULL) {
                break;
            }
        }

        // time to make the prog0 
        PCB *pcb_prog0 = malloc (sizeof (PCB));
        // put its specs
        pcb_prog0->pid = last_pid;
        pcb_prog0->start_position = start_position_0;
        pcb_prog0->length_script = length_script_0;
        pcb_prog0->program_counter = 0; // first program starts at 0
        pcb_prog0->next = NULL; // no next since its only one
        pcb_prog0->job_length_score = length_script_0;  // for A2-4
        pcb_prog0->first_run = 1;       // since created in backgroud mode, first time it runs, it has to run first


        // it fully runs once no matter what and after it becomes another program in the queue according to the policy
        int size = pcb_prog0->length_script;
        int m = 0;
        while (m < size) {
            int memory_position = pcb_prog0->start_position + m;
            parseInput (source_loading.lines[memory_position]);
            m = m + 1;
        }

        // now queue it but in front of all other programs .... so time to make a new queue
        // if its not empty, head is not empty
        // so add pcb to last in queue
        if (ready_queue.head != NULL) {
            pcb_prog0->next = ready_queue.head;
            ready_queue.head = pcb_prog0;
        }
        // if it is empty, pcb is head and tail of queue
        else {
            ready_queue.head = pcb_prog0;
            ready_queue.tail = pcb_prog0;
        }

    }

    // so after doing it for every file, now need to chose one of the policies and run it 
    if (strcmp (policy, "FCFS") == 0) {
        run_fcfs ();
    }

    // for the normal RR
    else if (strcmp (policy, "RR") == 0) {
        run_rr (2);
    }

    // for the new one A2-5
    else if (strcmp (policy, "RR30") == 0) {
        run_rr (30);
    }

    else if (strcmp (policy, "SJF") == 0) {
        run_sjf ();
    }

    else if (strcmp (policy, "AGING") == 0) {
        run_aging ();
    }

    else {
        printf(" Not a policy my man ");
	return 1;
    }

    // clean up like on source 
    for (int i = 0; i < MEM_SIZE; i++) {
        if (source_loading.used[i]) {
            free (source_loading.lines[i]);
            source_loading.used[i] = 0;
            source_loading.lines[i] = NULL;
        }
    }
    return errCode;
}


// implement fcfs for A2-1.4 and 1-5
void run_fcfs () {
    PCB *actual_pcb;
    // like on class
    // Process ready: insert at tail of queue
    // Head of queue: “oldest” ready process
    // keep dequieuen until its enpty
    actual_pcb = dequeue ();
    while (actual_pcb != NULL) {
        // run every intruction in memory
        int length = actual_pcb->length_script;
        int program_count = actual_pcb->program_counter;
        while (program_count < length) {
            int memory_position = program_count + actual_pcb->start_position;
            // execute the intruction 
            parseInput (source_loading.lines[memory_position]);
            program_count = program_count + 1;
        }

        // freen and update pcb
        free (actual_pcb);
        actual_pcb = dequeue ();
    }
}

// prepare a compare_length for qsort just like on A1
int compare_length (const void *a, const void *b) {
    PCB *pcb_1 = *(PCB **) a;
    PCB *pcb_2 = *(PCB **) b;
    int length1 = pcb_1->length_script;
    int length2 = pcb_2->length_script;
    return length1 - length2;
}


void run_sjf () {
    PCB *actual_pcb;
    actual_pcb = dequeue ();
    // like on class
    // Insert in queue according to length
    // Head of queue: “shortest” process
    // keep dequieuen until its enpty
    // so need to sort using qsort

    // take all pcb
    PCB *pcb_array[3];
    int counter = 0;
    while (actual_pcb != NULL) {
        pcb_array[counter] = actual_pcb;
        counter = counter + 1;
        actual_pcb = dequeue ();
    }
    // sort by length 
    qsort (pcb_array, counter, sizeof (PCB *), compare_length);

    // put all pcb back
    int i = 0;
    while (i < counter) {
        enqueue (pcb_array[i]);
        i = i + 1;
    }

    // now we can run fcfs like normal 
    run_fcfs ();
}

// have to modify run to run with page fault and page handling
// A3 - 2.3
// implement function to handle page fault
void page_fault(PCB *pcb, int page_num) {

    // we need to get the filename to be able to open the file again and grab the next pages
    int i = 0; 
    char line[MAX_USER_INPUT];
    char* name = pcb->name;
    // open file
    FILE *p = fopen(name, "rt");
    
    // check if openned correctly, if not, return 
     if (!p) {
	printf( " didn't open the file correctly ");
        return;
    }

    // go to the page where want to start from in the file
    int location = page_num * 3;
    while( i < location) {
         if (fgets(line, MAX_USER_INPUT, p) == NULL) {
	     break; // so the file ended 
	 }
	 i = i + 1;
    }

    // find available frame 
    int available_frame = find_available();
    
    // check if frame store is full, if it is, we will need to have a victim page
     if (available_frame == -1) {
         // for now, the victim frame is random
          int victim_frame = rand() % frame_counter;
	  int victim_location = victim_frame * 3;
	  
	  // print the error of page fault
	  // already printed it before ; printf("Page fault!");
	  printf("Victim page contents:\n");
	  int j = 0;
	  // print the contents of the page line by line
	  while(j < 3) {
	       int location_line = victim_location + j;	  
	       // print the line	  
	       printf("%s", source_loading.lines[location_line]);
	       // free it and reset it
               free(source_loading.lines[location_line]);
               source_loading.lines[location_line] = NULL;
               source_loading.used[location_line] = 0;
	       j = j + 1;
	  } 
	 printf("End of victim page contents.\n");

         // find pcb is using the victim page, remove it from page table
	 PCB *pcb = frame_mapping[victim_frame].pcb;
         // if found pcb, remove victim page from the page table
	 if (pcb != NULL) {
            pcb->page_table[frame_mapping[victim_frame].page_num] = -1;
	 }

         //update available frame to the position where it was the victim frame 
	 available_frame = victim_frame * 3;	 

     }

     // after evicting or not
     // have to load 3 lines into the available frame
     int n = 0;
     // put frame in the page table for the pcb
     int frame_num = available_frame / 3;
     pcb->page_table[page_num] = frame_num;
     // put in memory
     while (n < 3) {
	// end of file
	if ( fgets(line, MAX_USER_INPUT, p) == NULL) {
	    break;
	}
	int loca_line = available_frame + n;
        source_loading.lines[loca_line] = strdup(line);
        source_loading.used[loca_line] = 1;
        n++;
    }

    // update frame_mapping
    frame_mapping[frame_num].page_num = page_num; 
    frame_mapping[frame_num].pcb = pcb;

    // close file
    fclose(p);

}



// modify to add the possibility of 30 instead of 2
// only changed 2 by slice 
void run_rr (int slice) {

    PCB *actual_pcb;
    actual_pcb = dequeue ();

    // when actual_pcb == NULL, all programs are done 
    while (actual_pcb != NULL) {
        int program_count = actual_pcb->program_counter;
        int length = actual_pcb->length_script;
        int instruction_counter = 0;    // 2 at time
        // run 2 intructions then go to next progra
	

        // implement page fault detection and page handling on RR	
        while (program_count < length && instruction_counter < slice) {
            int offset_prog = program_count % 3; // to get its distance from starting point
	    int page_num = program_count / 3; // to get its position 
	    int frame_num = actual_pcb->page_table[page_num];

            // check if we have the frame in memory. if not, then we have a page fault
	    if ( frame_num == -1 ) {
	         // we have a page fault
		 printf("Page fault!\n");
		 page_fault(actual_pcb, page_num);

                 // The current process P is interrupted and placed at the back of the ready queue,
		 actual_pcb->program_counter = program_count;
                 enqueue(actual_pcb);
                 actual_pcb = dequeue();
	         break;
	    }

            // compute position in memory
	    int frame_start = frame_num * 3;	
            int memory_position = offset_prog + frame_start;
            // execute the intruction
            parseInput (source_loading.lines[memory_position]);
            program_count = program_count + 1;
            instruction_counter = instruction_counter + 1;
        }
        // if program is not done yet, go back to the queue
        // to avoid infinite loop
        actual_pcb->program_counter = program_count;
        if (program_count < length) {
            enqueue (actual_pcb);
        }
        // if it done, you free it 
        else {
            free (actual_pcb);
        }
        // go to the next program 
        actual_pcb = dequeue ();
    }

}


// do A2 - 4

// prepare a compare_length for qsort just like on A1
int compare_score (const void *a, const void *b) {
    PCB *pcb_1 = *(PCB **) a;
    PCB *pcb_2 = *(PCB **) b;
    int length1 = pcb_1->job_length_score;
    int length2 = pcb_2->job_length_score;
    return length1 - length2;
}

void run_aging () {

    PCB *actual_pcb;
    PCB *pcb_array[100];        // to save the pcbs
    int i = 0;                  // also counter of how many pcbi
    int counter = 0;

    // do the aging
    // the scheduler “ages” all the jobs that are in the ready queue
    // not the job that was executing during that time slice
    // put everything from queue in array
    actual_pcb = dequeue ();
    while (actual_pcb != NULL) {
        // put actual_pcb in array
        pcb_array[i] = actual_pcb;
        i = i + 1;
        actual_pcb = dequeue ();
    }

    // loop until all programs are done 
    while (i > 0) {

        // now comes the sorting part
        qsort (pcb_array, i, sizeof (PCB *), compare_score);

        actual_pcb = pcb_array[0];

        // Run one instruction :  slice = 1
        int memory_position =
            actual_pcb->program_counter + actual_pcb->start_position;
        parseInput (source_loading.lines[memory_position]);
        actual_pcb->program_counter += 1;

        // now do the aging and prepare to the next loop
        int n = 0;
        while (n < i) {
            // dont update aging of actual_pcb
            if (pcb_array[n] != actual_pcb) {
                if (pcb_array[n]->job_length_score > 0) {
                    pcb_array[n]->job_length_score =
                        pcb_array[n]->job_length_score - 1;
                }
            }
            n = n + 1;
        }

        // need to check if some program is done 
        int program_count = actual_pcb->program_counter;
        int length = actual_pcb->length_script;
        if (program_count >= length) {
            // if it done, you free it
            free (actual_pcb);
            // actual pcb is first in the array, so cut him
            int j = 0;
            int m = 1;
            while (j < i - 1) {
                pcb_array[j] = pcb_array[m];
                j = j + 1;
                m = m + 1;
            }
            // one program less to worry about
            i = i - 1;
        }
    }

}


// gonna implement the functions after this 

int echo (char *arg) {
    // assuming we have an argument, it can be a string or a variable 
    // if it's a string, just print the value 
    if (arg[0] != '$') {
        printf ("%s\n", arg);
    }
    // if it's a variable, print the value of the variable 
    else {
        // save the variable without its $
        char var[100];          // we know its less than 100 letters    
        strcpy (var, arg + 1);
        // take the value of the variable
        char *value = mem_get_value ((char *) var);

        if (strcmp (value, "Variable does not exist") != 0) {
            printf ("%s\n", value);     // print the value
            free (value);
        } else {
            printf ("\n");      // if the variable doesnt exist, dont show the message
        }
    }
    return 0;
}


// implementing 3.1 to 3.4
// my_ls 

// to use qsort I read about how to use it in : https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int compare (const void *arg1, const void *arg2) {
    return strcmp (*(const char **) arg1, *(const char **) arg2);
}

int my_ls () {

    // get current directory
    DIR *current_dir;           // current directory 
    current_dir = opendir (".");        // get current directory

    // struct made to iterate
    // can take the name of each file out of it with d_name
    struct dirent *next_file;
    int limit = 500;
    // array of char*
    char **file_array = malloc (limit * sizeof (char *));       // can change later
    int i = 0;

    // Iterate tho all the files in the directory 
    while (1) {                 // it ends when iterated tho all the files in the mkdir

        // update next_file
        next_file = readdir (current_dir);
        // ends infinite loop when files end -> file = null
        if (next_file == NULL) {
            break;
        }

        // if capacity is at limit, double it 
        if (i >= limit) {
            limit = limit * 2;
            file_array = realloc (file_array, limit * sizeof (char *));
        }

        // save the names to be sorted in an array
        file_array[i] = strdup (next_file->d_name);
        i = i + 1;
    }

    // now to put it in order alphabetically : qsort thank god 
    qsort (file_array, i, sizeof (char *), compare);
    int j = 0;

    // print the sorted names
    while (j < i) {
        printf ("%s\n", file_array[j]);
        j = j + 1;
    }

    free (file_array);
    closedir (current_dir);     // since I opened the directory, need to close it before ending program
    return 0;
}

// my_mkdir 
// leanred about isalnum() on : https://www.geeksforgeeks.org/isalnum-function-c-language/
// learned about creating directories : https://www.geeksforgeeks.org/create-directoryfolder-cc-program/
// useful for 3.2 and 3.3 
int my_mkdir (char *d_name) {
    // verify if its the name of the file or a variable with $
    // if it is not a variable, create directory with d_name        
    if (d_name[0] != '$') {

        // if d_name not alphanumeric, then error 
        int i = 0;
        int size = strlen (d_name);
        while (i < size) {
            // check for alpha numeric 
            if (!isalnum (d_name[i])) {
                printf ("Bad command: my_mkdir\n");
                return 1;       // fail
            }
            i = i + 1;
        }
        // create the directory with d_name 
        mkdir (d_name, 0777);
        return 0;
    }

    // if it's a variable, create directory with the value of the variable
    else {
        // to save the name of variable without $
        char var[100];          // we know its less than 100
        strcpy (var, d_name + 1);

        // take the value of the variable
        char *value = mem_get_value ((char *) var);

        // if the variable does not exist, bad command
        if (strcmp (value, "Variable does not exist") == 0) {
            printf ("Bad command: my_mkdir\n");
            return 1;           // fail
        }

        // if d_name not alphanumeric, then error
        int i = 0;
        int size = strlen (value);
        while (i < size) {
            // check for alpha numeric
            if (!isalnum (value[i])) {
                printf ("Bad command: my_mkdir\n");
                free (value);
                return 1;       // fail
            }
            i = i + 1;
        }

        // create the directory with d_name
        mkdir (value, 0777);
        free (value);
        return 0;
    }

}

// 3.3 -> my_touch 
int my_touch (char *f_name) {

    // if f_name not alphanumeric, then error
    int i = 0;
    int size = strlen (f_name);
    while (i < size) {
        // check for alpha numeric
        if (!isalnum (f_name[i])) {
            printf ("Bad command: my_touch\n");
            return 1;           // fail
        }
        i = i + 1;
    }

    // Create the new file 
    FILE *f = fopen (f_name, "w");      // openned a file need to close
    fclose (f);                 // close file 
    return 0;
}

// 3.4 -> my_cd
// learned how to switch directory : https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/ 
int my_cd (char *d_name) {

    // if d_name not alphanumeric, then error
    int i = 0;
    int size = strlen (d_name);
    while (i < size) {
        // check for alpha numeric
        if (!isalnum (d_name[i])) {
            printf ("Bad command: my_cd\n");
            return 1;           // fail
        }
        i = i + 1;
    }

    // go to directory 
    if (chdir (d_name) == 0) {
        return 0;               // we went to the directoy named d_name 
    } else {
        printf ("Bad command: my_cd\n");
        return 1;               // the directory doesn't exist
    }

}

// 5 -> run 
// read this : https://www.geeksforgeeks.org/wait-system-call-c/
// got the solution for this after the deadline for A1 
int run (char *args[], int arg_size) {

    int com_status;

    // copy the args into a new NULL-terminated array.
    char **adj_args = calloc (arg_size + 1, sizeof (char *));
    for (int i = 0; i < arg_size; ++i) {
        adj_args[i] = args[i];
    }

    // like in class,
    // in parent returns pid of child
    // in child, returns 0
    // if fail, returns -1 
    pid_t pid;
    pid = fork ();              // so pid of child 

    if (pid < 0) {
        // fork failed. Report the error and move on.
        perror ("fork() failed");
        return 1;
    } else if (pid == 0) {
        // we are the new child process.
        execvp (adj_args[0], adj_args);
        perror ("exec failed");
        exit (1);
    } else {
        // we are the parent process.
        waitpid (pid, NULL, 0);
    }

    return 0;

}

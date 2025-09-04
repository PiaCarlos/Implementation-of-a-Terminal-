# UNIX Command Interpreter (C)

A **custom UNIX-like shell** written in **C**, supporting command interpretation, file management, scripting, and process scheduling. This project demonstrates core operating system concepts like multitasking, memory management, and scheduling, while providing a usable command-line interface.  

---

## ⚡ Features

### Shell Commands
- `help` – List all commands  
- `quit` – Exit the shell  
- `set VAR VALUE` – Store a variable in shell memory  
- `print VAR` – Print a stored variable  
- `echo VALUE` – Print text or variable values  
- `my_ls` – List files and directories alphabetically  
- `my_mkdir DIR_NAME` – Create a new directory  
- `my_touch FILE_NAME` – Create a new file  
- `my_cd DIR_NAME` – Change directory  
- `source SCRIPT.TXT` – Run commands from a script  
- `run PROGRAM ARG...` – Execute external programs using fork-exec  
- `exec PROGRAM1 PROGRAM2 ... POLICY [#]` – Run multiple programs with scheduling policies and optional background mode  

### Scheduling & Process Management
- **First-Come-First-Serve (FCFS)**  
- **Shortest Job First (SJF)**  
- **Round-Robin (RR)** with configurable time slice  
- **Aging** – Dynamic job prioritization  
- **PCB-based ready queue** with process tracking  

### Memory & Paging
- **Frame-based memory allocation** (3 lines per frame)  
- **Page tables** per process  
- **Page fault handling** with victim page replacement  

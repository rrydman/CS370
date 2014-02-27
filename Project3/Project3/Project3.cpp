// Project 3 - Linux Scheduler
// Ross Rydman - Feb 2014

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

// Globals
int clock = 0;
int pidCounter = 0;

class Process {
public:
	int pid;
	int arrivalTime;
	int niceValue;
	int startTime;
	int endTime;
	int priority;
	int timeslice;
	int totalCPUtime;
	int totalIOtime;
	vector<int> cpuBursts;
	vector<int> ioBursts;
	Process(int, int, vector<int>, vector<int>);
};

Process::Process(int arr, int nice, vector<int> cpuB, vector<int> ioB) {
	pid = pidCounter;
	pidCounter++;
	arrivalTime = arr;
	niceValue = nice;
	cpuBursts = cpuB;
	ioBursts = ioB;
	startTime = 0;
	endTime = 0;
	priority = 0;
	timeslice = 0;
	totalCPUtime = 0;
	totalIOtime = 0;
}

void Process::calc_priority() {}
void Process::calc_timeslice() {}

void swap_queues(){}

istream& read_input(istream& in, vector<Process>& processList){
	if (in){
		processList.clear();
		while (in) {
			string line = "";
			std::getline(in, line);
			if (line.compare("***") == 0){
				break;
			}
			else {
				// Construct process from input line
				vector<int> cpuB;
				vector<int> ioB;
				int nice;
				// First col is nice value
				stringstream (line) >> nice;
				int arrTime;
				// Second column is arrival time
				stringstream (line) >> arrTime;
				int cpuBurstsCount;
				// Third is number of cpu bursts (remember io bursts is cpu -1)
				stringstream (line) >> cpuBurstsCount;
				stringstream (line) >> cpuB[0];
				int count = cpuBurstsCount;
				int i = 1;
				while (count > 1) {
						stringstream (line) >> ioB[i-1];
						stringstream (line) >> cpuB[i];
						i++;
						count--;
				}
				// Construct process
				process1 = new Process(arrTime, nice, cpuB, ioB);
				// Add process to processList
				processList.push_back(process1);
			}
		}
		in.clear();
	}
	return in;
}

void print_arrival(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Enters ready queue (Priority: " << p->priority << ", TimeSlice: " << p->timeslice << ")" << endl;
}
void print_enter_CPU(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Enters the CPU" << endl;
}
void print_preempted(Process *preempter, Process *preemptee){
	cout << "[" << clock << "] <" preempter->pid << "> Preempts Process " << preemptee->pid << endl;
}
void print_finishes_all_CPU_bursts(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Finishes and moves to the Finished Queue" << endl;
}
void print_finishes_cpu_io(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Moves to the IO Queue" << endl;
}
void print_finishes_timeslice_expired(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Finishes its time slice and moves to the Expired Queue (Priority: " << p->priority << ", TimeSlice: " << p->timeslice << ")" << endl;
}
void print_finishes_io_expired(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Finishes IO and moves to the Expired Queue (Priority: " << p->priority << ", TimeSlice: " << p->timeslice << ")" << endl;
}
void print_finishes_io_active(Process *p){
	cout << "[" << clock << "] <" p->pid << "> Finishes IO and moves to the Ready Queue" << endl;
}
void print_queue_swap(){
	cout << "[" << clock << "] *** Queue Swap" << endl;
}
void print_report(vector<Process> *finishedQueue){
	float avgtat = 0.0F;
	float avgwt = 0.0F;
	float avgcpu = 0.0F;
	Process * p;
	for ( ){ //p in finishedQueue
		int tat = p->endTime - p->arrivalTime - p->totalIOtime;
		int tct = 0;
		int wt = tat - tct;
		float cut = tct/tat; // round to nearest tenth somehow
		// print stats for process
		cout << "Process <" p->pid << ">" << endl;
		cout << "Turnaround Time = " << tat << endl;
		cout << "Total Time = " << tct << endl;
		cout << "Waiting Time = " << wt << endl;
		cout << "Percentage of CPU Utilization Time = " << avgcpu << endl;
		avgtat += tat;
		avgwt += wt;
		avgcpu += cut;
	}
	int processCount = finishedQueue->size();
	avgtat = avgtat/processCount; // round to nearest tenth somehow
	avgwt = avgwt/processCount; // round to nearest tenth somehow
	avgcpu = avgcpu/processCount; // round to nearest tenth somehow
	cout << "Average Waiting Time: " << avgwt << endl;
	cout << "Average Turnaround Time: " << avgtat << endl;
	cout << "Average CPU Utilization: " << avgcpu << endl;
}

int main(){
	vector<Process> processList;
	read_input(cin, processList);
	vector<Process> startQueue;
	vector<Process> activeQueue;
	vector<Process> expiredQueue;
	vector<Process> ioQueue;
	vector<Process> finishedQueue;
	vector<Process> cpu;

	while (true){
		// Insert processes to the active queue if they are to start at this clock tick. (Calculate priority and time slice)
			// sort processList by startTime
			// for (process in processList)
				// calculate priority
				// calculate timeslice
				// if (startTime <= clock) insert process into active queue
		// If the cpu is empty the lowest priority process in the active queue is put into the cpu. If more than two processes have the same priority then FIFO strategy can be used.
		// Check if the lowest priority process in the active queue has a lower priority then the process in the cpu. If so then preempt. Preempted process goes back to the active queue.
		// Perform cpu (decrement the time slice of the process in cpu)
		// Perform IO (decrement the IO burst for all processes in the IO queue)
		// if there is a process in the cpu (call this process p)
		if (!cpu.empty()){
			Process p = cpu.back();
			// if p's current cpu burst is exhausted
			if () {
				// if p is done with all cpu bursts send to the finished queue
				if (p.cpuBursts.empty()){
					// send p to finished queue
				} 
				// if p is not done with all cpu bursts (which means there is an IO burst) send to the IO queue
				else {
					// send p to io queue
				}
			}
			// if p's timeslice is exhausted send to the expired queue and recalculate priority and timeslice
			if (p.timeslice == 0) {
				// send to expired
				// recalc priority
				// recalc timeslice
			}
		}
		// If there is any process in the IO queue that is finished with its IO burst (there can be more than one, call this process p)
			// If p's timeslice is exhausted move to the expired queue and recalculate the priority and timeslice
			// If p still has timeslice left insert p into the active queue.
		// If the startup queue, ready queue, expired queue, IO queue, and the CPU are all empty then break out of the while loop (simulation complete)
		if (startQueue.empty() && activeQueue.empty() && expiredQueue.empty() && ioQueue.empty() && cpu.empty()) break;
		// If the ready queue and cpu are empty and the expired queue is not empty then swap the expired/active queues
		if (activeQueue.empty() && cpu.empty() && !expiredQueue.empty()) {
			swap_queues();
			print_queue_swap();
		}
		clock++;
	}

	print_report();
	return 0;
}


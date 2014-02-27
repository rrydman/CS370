// Project 3 - Linux Scheduler
// Ross Rydman - Feb 2014

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

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
}

int calc_priority(){
	return 0;
}
int calc_timeslice(){
	return 0;
}

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
				// First col is nice value
				// Second column is arrival time
				// Third is number of cpu bursts (remember io bursts is cpu -1)
					// store this and use it to calculate iterations of cpu/io burst loads
				// Fourth is cpu burst time
					// cpuB[0] = this value;
				// Subsequent is io/cpu/io/cpu etc

			}

		}
		in.clear();
	}
	return in;
}

void print_arrival(){}
void print_enter_CPU(){}
void print_preempted(){}
void print_finished_CPU_bursts(){}
void print_move_to_io(){}
void print_finished_timeslice(){}
void print_finished_io_expired(){}
void print_finished_io_active(){}

void print_queueSwap(){
	cout << "[" << clock << "] *** Queue Swap" << endl;
}

void print_report(){
	cout << "Print report here..." << endl;
}

int main(){
	vector<Process> processList;
	read_input(cin, processList);
	// Calculate initial priorities
	// Calculate initial timeslices
	// Create queues
	vector<Process> startQueue;
	vector<Process> activeQueue;
	vector<Process> expiredQueue;
	vector<Process> ioQueue;
	vector<Process> finishedQueue;
	vector<Process> cpu;

	//while (true){
		// Insert processes to the active queue if they are to start at this clock tick. (Calculate priority and time slice)
		// If the cpu is empty the lowest priority process in the active queue is put into the cpu. If more than two processes have the same priority then FIFO strategy can be used.
		// Check if the lowest priority process in the active queue has a lower priority then the process in the cpu. If so then preempt. Preempted process goes back to the active queue.
		// Perform cpu (decrement the time slice of the process in cpu)
		// Perform IO (decrement the IO burst for all processes in the IO queue)
		// if there is a process in the cpu (call this process p)
			// if p's current cpu burst is exhausted
				// if p is done with all cpu bursts send to the finished queue
				// if p is not done with all cpu bursts (which means there is an IO burst) send to the IO queue
			// if p's timeslice is exhausted send to the expired queue and recalculate priority and timeslice
		// If there is any process in the IO queue that is finished with its IO burst (there can be more than one, call this process p)
			// If p's timeslice is exhausted move to the expired queue and recalculate the priority and timeslice
			// If p still has timeslice left insert p into the active queue.
		// If the startup queue, ready queue, expired queue, IO queue, and the CPU are all empty then break out of the while loop (simulation complete)
		// If the ready queue and cpu are empty and the expired queue is not empty then swap the expired/active queues
	//	clock++;
	//}

	print_report();
	return 0;
}


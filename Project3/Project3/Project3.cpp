/* ------------------------
CS370 Project 3 - Linux Scheduler
Ross Rydman - (c) Feb 2014
---------------------------

---------------------------
What is working:
	Queues using vectors (and iterators!)
	Process class
	Printing messages
	Input read correctly, output formatted correctly
	Relatively short main with most things broken out into functions
	Swapping active/expired, moving processes between queues, etc
---------------------------
What isn't working:
	Hopefully the figures are right - hard to check since the example output is from a different semester
	Rounding could work better
---------------------------
What I didn't do:
	Header files
	Air-tight memory management
	Error handling
---------------------------
To compile: g++ filename.cpp
To run: ./a.out < input.txt
--------------------------- */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

// Globals
int clck = 0;
int pidCounter = 0;

class Process {
public:
	int pid;
	int niceValue;
	int startTime;
	int endTime;
	int priority;
	int originalpriority;
	int timeslice;
	int totalCPUtime;
	int totalIOtime;
	vector<int> cpuBursts;
	vector<int> ioBursts;
	Process(int, int, vector<int>, vector<int>);
	void calc_priority();
	void calc_timeslice();
	~Process();
};

Process::Process(int start, int nice, vector<int> cpuB, vector<int> ioB) {
	pid = pidCounter;
	pidCounter++;
	niceValue = nice;
	cpuBursts = cpuB;
	ioBursts = ioB;
	startTime = start;
	endTime = 0;
	priority = 0;
	originalpriority = 0;
	timeslice = 0;
	totalCPUtime = 0;
	totalIOtime = 0;
}

Process::~Process() {}

void Process::calc_priority(void) {
	// First time calculating priority (use nice value)
	if (originalpriority == 0){ 
		priority = (int)(((niceValue + 20) / 39.0) * 30 + 0.5) + 105;
		originalpriority = priority;
		niceValue = 0;
	}
	// Not first time (use bonus)
	else { 
		int bonus = 0;
		if (totalCPUtime < totalIOtime){
			bonus = (int)(((1 - totalCPUtime / (double)totalIOtime) *(-5)) - 0.5);
		}
		else {
			bonus = (int)(((1 - totalIOtime / (double)totalCPUtime) * 5) + 0.5);
		}
		priority = originalpriority + bonus;
	}
}

void Process::calc_timeslice(void) {
	timeslice = (int)((1 - priority / 140.0) * 290 + 0.5) + 10;
}

bool predicate_starttime(const Process* p1, const Process* p2){ return p1->startTime < p2->startTime; }
bool predicate_priority(const Process* p1, const Process* p2){ return p1->priority < p2->priority; }
bool predicate_currentIOBleft(const Process* p1, const Process* p2){ return p1->ioBursts.front() < p2->ioBursts.front(); }
bool predicate_endtime(const Process* p1, const Process* p2){ return p1->endTime < p2->endTime; }

istream& read_input(istream& in, vector<Process*>& sQueue){
	if (in){
		sQueue.clear();
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
				int start;
				int cpuBurstsCount;
				int tmp;
				stringstream ss;
				ss.str(line);
				// Read in first 4 columns
				ss >> nice >> start >> cpuBurstsCount >> tmp;
				cpuB.push_back(tmp);
				int count = cpuBurstsCount;
				while (count > 1) {
						ss >> tmp;
						ioB.push_back(tmp);
						ss >> tmp;
						cpuB.push_back(tmp);
						count--;
				}
				// Construct process
				Process* process1 = new Process(start, nice, cpuB, ioB);
				// Add process to startQueue
				sQueue.push_back(process1);
			}
		}
		in.clear();
	}
	return in;
}

void print_arrival(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Enters ready queue (Priority: " << p.priority << ", TimeSlice: " << p.timeslice << ")" << endl;
}
void print_enter_CPU(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Enters the CPU" << endl;
}
void print_preempted(Process &preempter, Process &preemptee){
	cout << "[" << clck << "] <" << preempter.pid << "> Preempts Process " << preemptee.pid << endl;
}
void print_finishes_all_CPU_bursts(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Finishes and moves to the Finished Queue" << endl;
}
void print_finishes_cpu_io(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Moves to the IO Queue" << endl;
}
void print_finishes_timeslice_expired(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Finishes its time slice and moves to the Expired Queue (Priority: " << p.priority << ", TimeSlice: " << p.timeslice << ")" << endl;
}
void print_finishes_io_expired(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Finishes IO and moves to the Expired Queue (Priority: " << p.priority << ", TimeSlice: " << p.timeslice << ")" << endl;
}
void print_finishes_io_active(Process &p){
	cout << "[" << clck << "] <" << p.pid << "> Finishes IO and moves to the Ready Queue" << endl;
}
void print_queue_swap(){
	cout << "[" << clck << "] *** Queue Swap" << endl;
}

void print_report(vector<Process*> &finishedQueue){
	double avgtat = 0.0;
	double avgwt = 0.0;
	double avgcut = 0.0;
	cout << endl;
	vector<Process*>::iterator iter = finishedQueue.begin();
	if (!finishedQueue.empty()) {
		while (iter != finishedQueue.end()){ // for p in finishedQueue
			double tat = (*iter)->endTime - (*iter)->startTime - (*iter)->totalIOtime;
			double tct = (*iter)->totalCPUtime;
			double wt = tat - tct;
			double cut = 0.0;
			if (tat != 0.0) cut = tct / tat;
			// print stats for process
			cout << "Process <" << (*iter)->pid << ">" << endl;
			cout << "Turnaround Time = " << tat << endl;
			cout << "Total CPU Time = " << tct << endl;
			cout << "Waiting Time = " << wt << endl;
			cout << "Percentage of CPU Utilization Time = " << static_cast<double>(static_cast<int>(cut * 1000.)) / 1000. * 100<< endl; // one decimal place
			avgtat += tat;
			avgwt += wt;
			avgcut += cut;
			iter++;
		}
	}
	int processCount = finishedQueue.size();
	avgtat = static_cast<double>(static_cast<int>((avgtat / processCount) * 100.)) / 100.; // three decimal places
	avgwt = static_cast<double>(static_cast<int>((avgwt / processCount) * 100.)) / 100.; // three decimal places
	avgcut = static_cast<double>(static_cast<int>((avgcut / processCount) * 100000.)) / 100000 * 100.; // three decimal places
	cout << endl;
	cout << "Average Turnarount Time = " << avgtat << endl;
	cout << "Average Waiting Time = " << avgwt << endl;
	cout << "Average Percentage of CPU Utilization Time = " << avgcut << endl;
}

int main(){
	vector<Process*> startQueue;
	vector<Process*> activeQueue;
	vector<Process*> expiredQueue;
	vector<Process*> ioQueue;
	vector<Process*> finishedQueue;
	vector<Process*> cpu;
	read_input(cin, startQueue);
	sort(startQueue.begin(), startQueue.end(), predicate_starttime);

	while (true){
		// Insert processes to the active queue if they are to start at this clock tick. (Calculate priority and time slice)
		if (!startQueue.empty()){
			vector<Process*>::iterator startIter = startQueue.begin();
			while (startIter != startQueue.end()){
				if ((*startIter)->startTime == clck) {
					(*startIter)->calc_priority();
					(*startIter)->calc_timeslice();
					activeQueue.push_back((*startIter));
					print_arrival(*(*startIter));
					startIter = startQueue.erase(startIter);
				}
				else{
					startIter++;
				}
			}
		}
		// If the cpu is empty the lowest priority process in the active queue is put into the cpu. If more than two processes have the same priority then FIFO strategy can be used.
		sort(activeQueue.begin(), activeQueue.end(), predicate_priority); // BUG? May mess up FIFO if two with same priority
		if (cpu.empty() && !activeQueue.empty()){
			cpu.push_back(activeQueue.front());
			print_enter_CPU(*activeQueue.front());
			activeQueue.erase(activeQueue.begin());
		}
		// Check if the lowest priority process in the active queue has a lower priority then the process in the cpu. 
		// If so then preempt. Preempted process goes back to the active queue.
		if (!activeQueue.empty()){
			if (activeQueue.front()->priority < cpu.front()->priority){
				Process* preemptee = cpu.front();
				cpu.erase(cpu.begin());
				Process* preempter = activeQueue.front();
				activeQueue.erase(activeQueue.begin());
				activeQueue.push_back(preemptee);
				cpu.push_back(preempter);
				print_preempted(*preempter, *preemptee);
			}
		}
		// Perform CPU (decrement the time slice of the process in cpu)
		if (!cpu.empty()){
			cpu.front()->timeslice--;
			cpu.front()->cpuBursts.front()--;
			cpu.front()->totalCPUtime++;
		}
		// Perform IO (decrement the IO burst for all processes in the IO queue)
		if (!ioQueue.empty()) {
			vector<Process*>::iterator ioiter = ioQueue.begin();
			while (ioiter != ioQueue.end()){
				(*ioiter)->ioBursts.front()--;
				(*ioiter)->totalIOtime++;
				ioiter++;
			}
		}
		// if there is a process in the cpu (call this process p)
		if (!cpu.empty()){
			Process* p = cpu.front();
			// if p's current cpu burst is exhausted
			if (p->cpuBursts.front() == 0) {
				// Remove CPU burst
				p->cpuBursts.erase(p->cpuBursts.begin());
				// if p is done with all cpu bursts send to the finished queue
				if (p->cpuBursts.empty()){
					p->endTime = clck;
					finishedQueue.push_back(p);
					print_finishes_all_CPU_bursts(*p);
					cpu.erase(cpu.begin());
				} 
				// if p is not done with all cpu bursts (which means there is an IO burst) send to the IO queue
				else if (!p->cpuBursts.empty()){
					ioQueue.push_back(p);
					print_finishes_cpu_io(*p);
					cpu.erase(cpu.begin());
					sort(ioQueue.begin(), ioQueue.end(), predicate_currentIOBleft);
				}
			}
			// if p's timeslice is exhausted send to the expired queue and recalculate priority and timeslice
			else if (p->timeslice == 0) {
				p->calc_priority();
				p->calc_timeslice();
				expiredQueue.push_back(p);
				print_finishes_timeslice_expired(*p);
				cpu.erase(cpu.begin());
				sort(expiredQueue.begin(), expiredQueue.end(), predicate_priority);
			}
		}
		// If there is any process in the IO queue that is finished with its IO burst (there can be more than one, call this process p)
		if (!ioQueue.empty()){
			vector<Process*>::iterator ioQiter = ioQueue.begin();
			if (!ioQueue.empty()) {
				while (ioQiter != ioQueue.end()){
					// If current IO Burst is 0
					if ((*ioQiter)->ioBursts.front() == 0) {
						// Remove zero io burst from process
						(*ioQiter)->ioBursts.erase((*ioQiter)->ioBursts.begin());
						// If p's timeslice is exhausted move to the expired queue and recalculate the priority and timeslice
						if ((*ioQiter)->timeslice == 0){
							(*ioQiter)->calc_priority();
							(*ioQiter)->calc_timeslice();
							expiredQueue.push_back((*ioQiter));
							print_finishes_io_expired(*(*ioQiter));
							ioQiter = ioQueue.erase(ioQiter);
							sort(expiredQueue.begin(), expiredQueue.end(), predicate_priority);
						}
						// If p still has timeslice left insert p into the active queue.
						else{
							activeQueue.push_back((*ioQiter));
							print_finishes_io_active(*(*ioQiter));
							ioQiter = ioQueue.erase(ioQiter);
						}
					}
					else {
						ioQiter++;
					}
				}
			}
		}
		// If the startup queue, ready queue, expired queue, IO queue, and the CPU are all empty then break (done!)
		if (startQueue.empty() && activeQueue.empty() && expiredQueue.empty() && ioQueue.empty() && cpu.empty()) break;
		// If the ready queue and cpu are empty and the expired queue is not empty then swap the expired/active queues
		if (activeQueue.empty() && cpu.empty() && !expiredQueue.empty()) {
			activeQueue.swap(expiredQueue);
			print_queue_swap();
		}
		clck++;
	}

	print_report(finishedQueue);

	return 0;
}


#include <bits/stdc++.h>
using namespace std;

// Process structure 
struct Process {
    int processID;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int priority;
};

// Function that implements the priority round-robin scheduling with feedback
void priorityRoundRobinScheduling(queue<Process>& highPriorityQueue, queue<Process>& lowPriorityQueue, int timeQuantum, int maxWaitingTime) {
    // Two pointers to point at the two queues
    queue<Process>* currentQueue = &highPriorityQueue;
    queue<Process>* nextQueue = &lowPriorityQueue;

    while (!highPriorityQueue.empty() || !lowPriorityQueue.empty()) {
        if (currentQueue->empty()) {
            // Switch to the next queue if the current queue is empty
            swap(currentQueue, nextQueue);
        }

        Process currentProcess = currentQueue->front();
        currentQueue->pop();

        // Execute the process for the time quantum or until it finishes
        int executionTime = min(timeQuantum, currentProcess.remainingTime);
        currentProcess.remainingTime -= executionTime;

        // Increasing waiting time of all the processes that are currently present in both the queues
        int n = highPriorityQueue.size();
        int c = 0;
        while (c != n) {
            Process pro = highPriorityQueue.front();
            pro.waitingTime += executionTime;
            highPriorityQueue.pop();

            // Check if process in low-priority queue should be promoted
            if (pro.priority == 0 && pro.waitingTime >= maxWaitingTime) {
                pro.priority = 1;
                cout << "Process " << pro.processID << " promoted to high priority due to high waiting time." << endl;
            }

            highPriorityQueue.push(pro);
            c++;
        }

        n = lowPriorityQueue.size();
        c = 0;
        while (c != n) {
            Process pro = lowPriorityQueue.front();
            pro.waitingTime += executionTime;
            lowPriorityQueue.pop();

            // Check if process in low-priority queue should be promoted
            if (pro.priority == 0 && pro.waitingTime >= maxWaitingTime) {
                pro.priority = 1;
                highPriorityQueue.push(pro);
                cout << "Process " << pro.processID << " promoted to high priority due to high waiting time." << endl;
            } else {
                lowPriorityQueue.push(pro);
            }
            c++;
        }

        // If the process is not yet completed, push it back to the respective queue
        if (currentProcess.remainingTime > 0) {
            currentQueue->push(currentProcess);
        } else {
            // Process completed
            cout << "Process " << currentProcess.processID<< endl;
        }

        // Swapping to give chance to the other queue to avoid starvation
        swap(currentQueue, nextQueue);
    }
}

int main() {
    int numProcesses;
    int timeQuantum;
    int maxWaitingTime;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    cout << "Enter the maximum waiting time before promotion: ";
    cin >> maxWaitingTime;

    // Two separate queues for high and low priority
    queue<Process> highPriorityQueue;
    queue<Process> lowPriorityQueue;

    // Input process details
    for (int i = 0; i < numProcesses; ++i) {
        Process process;
        cout << "Enter details for Process " << i + 1 << endl;
        cout << "Process ID: ";
        cin >> process.processID;
        cout << "Burst Time: ";
        cin >> process.burstTime;
        cout << "Priority (1 for high priority, 0 for low priority): ";
        cin >> process.priority;
        process.remainingTime = process.burstTime;
        process.waitingTime = 0;

        if (process.priority == 1) {
            highPriorityQueue.push(process);
        } else {
            lowPriorityQueue.push(process);
        }
    }

    // Perform priority-based round-robin scheduling with feedback
    priorityRoundRobinScheduling(highPriorityQueue, lowPriorityQueue, timeQuantum, maxWaitingTime);

    return 0;
}

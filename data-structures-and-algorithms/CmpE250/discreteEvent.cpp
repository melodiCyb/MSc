#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

double total_running_time = 0;
class Job;
class Unit;

class Job{
public:
    int id;
    double arrival_time;
    double finish_time;
};

class Unit{
private:
    vector<int> adjacency;
    vector<Job*> job_queue;
    Job* currentJob;
public:
    Unit(){
        processing_time = 0;
        status = 0;
        max_queue_length = 0;
        busy_time = 0;
    }
    int index;
    double processing_time;
    double busy_time;
    double next_time_event;
    int max_queue_length;
    
    int status; // 0: inactive , 1: processing
    
    void addAdjacency(int index){
        adjacency.push_back(index);
    }
    
    vector<int> getAdjacency(){
        return adjacency;
    }
    
    void addJob(Job* job, vector<double>& timeEvents, double currentTime){
        if(status == 0){
            next_time_event = currentTime;
            timeEvents.push_back(next_time_event);
            sort(timeEvents.begin(), timeEvents.end());
        }
        job_queue.push_back(job);
        if(job_queue.size() > max_queue_length)
            max_queue_length = job_queue.size();
    }
    
    void processRandom(vector<double>& timeEvents, int sizeOfUnit, Unit* units, double currentTime){
        if(status == 0 && job_queue.size() > 0){
            next_time_event += processing_time;// if unit is free and there is any job in queue, processing job and fire time event
            timeEvents.push_back(next_time_event);
            sort(timeEvents.begin(), timeEvents.end());
            total_running_time+= processing_time;// increase running time
            currentJob = job_queue[0];
            job_queue.erase(job_queue.begin());
            status = 1;
        }else
        if(status == 1){
            // if job is completed, post to next unit
                vector<int> nextUnits;
                for(int i = 0; i < sizeOfUnit; i++){
                    for(int j = 0; j < units[i].getAdjacency().size(); j++){
                        if(units[i].getAdjacency()[j] == index){
                            nextUnits.push_back(i);
                            break;
                        }
                    }
                }
            if(nextUnits.size() > 0){
                int selectedIndex = rand() % nextUnits.size();
                units[selectedIndex].addJob(currentJob, timeEvents, currentTime);
            }else{
                currentJob->finish_time = currentTime;
            }
            /* Process next job in queue*/
            if(job_queue.size() > 0){
                currentJob = job_queue[0];
                job_queue.erase(job_queue.begin());
                next_time_event += processing_time;//fire time event
                total_running_time+= processing_time;// increase running time
                timeEvents.push_back(next_time_event);
                sort(timeEvents.begin(), timeEvents.end());
            }else{
                status = 0;
            }
            busy_time+= processing_time;
            
        }
    }
    
    void processShortest(vector<double>& timeEvents, int sizeOfUnit, Unit* units, double currentTime){
        if(status == 0 && job_queue.size() > 0){
            next_time_event += processing_time;// if unit is free and there is any job in queue, processing job and fire time event
            timeEvents.push_back(next_time_event);
            sort(timeEvents.begin(), timeEvents.end());
            total_running_time+= processing_time;// increase running time
            currentJob = job_queue[0];
            job_queue.erase(job_queue.begin());
            status = 1;
        }else
            if(status == 1){
                /* if job is completed post to next unit*/
                vector<int> nextUnits;
                for(int i = 0; i < sizeOfUnit; i++){
                    for(int j = 0; j < units[i].getAdjacency().size(); j++){
                        if(units[i].getAdjacency()[j] == index){
                            nextUnits.push_back(i);
                            break;
                        }
                    }
                }
                
                if(nextUnits.size() > 0){
                    int selectedIndex = 0;
                    for(int i = 1; i <nextUnits.size(); i++)
                    {
                        if(units[nextUnits[i]].getQueueLength() < units[nextUnits[selectedIndex]].getQueueLength()){
                            selectedIndex = i;
                        }
                    }
                    units[selectedIndex].addJob(currentJob, timeEvents, currentTime);
                }else{
                    cout << currentTime << endl;
                    currentJob->finish_time = currentTime;
                }
                /* Process next job in queue*/
                if(job_queue.size() > 0){
                    currentJob = job_queue[0];
                    job_queue.erase(job_queue.begin());
                    next_time_event += processing_time; //fire time event
                    total_running_time+= processing_time; // increase running time
                    timeEvents.push_back(next_time_event);
                    sort(timeEvents.begin(), timeEvents.end());
                }else{
                    status = 0;
                }
                busy_time+= processing_time;
                
            }
    }

    
    int getQueueLength(){
        return job_queue.size();
    }
};


void randomAlgorithm(int sizeOfUnit, Unit* units, int sizeOfJob, Job* jobs, ofstream& outputFile){
    total_running_time = 0;
    srand(0);
    vector<double> timeEvents;
    for(int i = 0; i < sizeOfJob; i++){
        timeEvents.push_back(jobs[i].arrival_time);
    }
    sort(timeEvents.begin(), timeEvents.end());
    int nextJob = 0;
    while(timeEvents.size()>0)
	{ 
        double currentTime = timeEvents[0]; //get current time from time event
        timeEvents.erase(timeEvents.begin());
        
        /* check for new job arrival*/
        if(jobs[nextJob].arrival_time == currentTime){
           // find input unit
            vector<int> inputUnits;
            for(int i = 0; i < sizeOfUnit; i++)
            {
                if(units[i].getAdjacency().size() == 0) inputUnits.push_back(i);
            }
            
			int indexOfUnit =inputUnits[rand() % inputUnits.size()]; //select random unit
            units[indexOfUnit].addJob(&jobs[nextJob], timeEvents, currentTime); //pointer Next Job
            nextJob++;
        }
        /*processing time in unit*/
        for(int i = 0; i < sizeOfUnit; i++){
            if(units[i].next_time_event == currentTime){
                units[i].processRandom(timeEvents, sizeOfUnit, units, currentTime);
            }
        }
    }
    outputFile << total_running_time << endl;
    
    //
    for(int i = 0; i < sizeOfUnit; i++){
        outputFile <<  i << " " << units[i].busy_time << " " << units[i].max_queue_length << endl;
    }
    //
    for(int i = 0; i < sizeOfJob; i++){
        outputFile << jobs[i].finish_time  - jobs[i].arrival_time << endl;
    }
}

void shortestQueueFirst(int sizeOfUnit, Unit units[], int sizeOfJob, Job jobs[], ofstream& outputFile){
    total_running_time = 0;
    srand(0);
    vector<double> timeEvents;
    for(int i = 0; i < sizeOfJob; i++){
        timeEvents.push_back(jobs[i].arrival_time);
    }
    sort(timeEvents.begin(), timeEvents.end());
    int nextJob = 0;
    while(timeEvents.size()>0){
        // get current time from time event
        double currentTime = timeEvents[0];
        timeEvents.erase(timeEvents.begin());
        
        /*Check for New job arrival*/
        if(jobs[nextJob].arrival_time == currentTime){
            // find input unit
            vector<int> inputUnits;
            for(int i = 0; i < sizeOfUnit; i++)
            {
                if(units[i].getAdjacency().size() == 0) inputUnits.push_back(i);
            }
            // shortest queue length
            int indexOfUnit = 0;
            for(int i = 1; i < inputUnits.size(); i++){
                if(units[indexOfUnit].getQueueLength() < units[i].getQueueLength()){
                    indexOfUnit = i;
                }
            }
            units[indexOfUnit].addJob(&jobs[nextJob], timeEvents, currentTime);
            //pointer next job
            nextJob++;
        }
        /*Processing time in Unit*/
        for(int i = 0; i < sizeOfUnit; i++){
            if(units[i].next_time_event == currentTime){
                units[i].processRandom(timeEvents, sizeOfUnit, units, currentTime);
            }
        }
    }
    outputFile << total_running_time << endl;
    
    //
    for(int i = 0; i < sizeOfUnit; i++){
        outputFile <<  i << " " << units[i].busy_time << " " << units[i].max_queue_length << endl;
    }
    //
    for(int i = 0; i < sizeOfJob; i++){
        outputFile << jobs[i].finish_time  - jobs[i].arrival_time << endl;
    }

}

int main(int argc, char* argv[]){
    int numberOfUnit, numberOfJobs;
    Unit* units;
    Job* jobs;
    if(argc!=4){
        cout << "Usage: main.exe input_file output_file_1 output_file_2" << endl;
    }
    ifstream inputFile(argv[1]);
    /* read information of unit*/
    inputFile >> numberOfUnit;
    units = new Unit[numberOfUnit];
    string line, temp;
    getline(inputFile, line); // ignore first line
    for(int i = 0; i < numberOfUnit; i++){
        getline(inputFile, line);
        temp = line.substr(0, line.find(' '));// index of unit
        int indexOfUnit = stoi(temp);
        Unit* unit = &units[indexOfUnit];
        unit->index = i;
        line = line.substr(line.find(' ') + 1);
        temp = line.substr(0, line.find(' '));// processing time
        unit->processing_time = stod(temp);
        line = line.substr(line.find(' ') + 1);
        while(line.find(' ') != string::npos){
            temp = line.substr(0, line.find(' '));// next unit
            unit->addAdjacency(stoi(temp));
            line = line.substr(line.find(' ') + 1);
            if(line.find(' ') == string::npos){
                unit->addAdjacency(stoi(line));
            }
        }
    }
    /*Read information of job*/
    inputFile >> numberOfJobs;
    jobs = new Job[numberOfJobs];
    getline(inputFile, line); // ignore Number Of Job Line
    for(int i = 0; i < numberOfJobs; i++){
        getline(inputFile, line); // read arrival_time of job
        jobs[i].arrival_time = stod(line);
        //cout <<jobs[i].arrival_time << endl;
    }
    
    inputFile.close();
    
    /* Start Simulation*/
    ofstream randomOutputFile(argv[2]);
    randomAlgorithm(numberOfUnit, units, numberOfJobs, jobs, randomOutputFile);
    randomOutputFile.close();
    
    ofstream outputFile(argv[3]);
    shortestQueueFirst(numberOfUnit, units, numberOfJobs, jobs, outputFile);
    outputFile.close();
    /*
    std::srand(std::time(0));
    for(int i=0; i<10; i++){
        std::cout<<std::rand()<<std::endl;
    }*/
    return 0;
}

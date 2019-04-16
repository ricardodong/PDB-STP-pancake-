#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

static short PDB[518918400] = {0};
//short MD1[518918400] = {0};


int factor(const short i, const short j) {
	int factor = 1;
	for (short k = j+1; k <= i; ++k) {
		factor *= k;
	}
	return factor;
}

void swap(short *a,short *b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}



class STPnode {
private:
	short state[16];
	short gcost;
	short nextGcost;
	short zeroPosi;
	short generatedMethod;
	//0 for left, 1 for right, 2 for up, 3 for down;
	//all these refer to the move of zero tile
	
	STPnode switch_tile(short direction){
		short newState[16];
		for (short i = 0; i < 16; ++i){
		    newState[i] = this->state[i];
		}
		//should use vector at here for state
		short action;
		switch(direction){
			case 0: {action = -1; break;}
			case 1: {action = 1;  break;}
			case 2: {action = -4; break;}
			case 3: {action = 4;  break;}
			default: {action = -1;break;}
		}
		swap(&newState[this->zeroPosi], &newState[this->zeroPosi+action]);
		return STPnode(this->nextGcost, newState, direction);
	}

public:	
	STPnode(short gcost, short *newState, short genMethod) {

		for (short i = 0; i < 16; ++i){
		    this->state[i] = newState[i];
		}
		this->generatedMethod = genMethod;
		
		this->gcost = gcost;
		this->nextGcost = gcost + 1;
		
		for (short i = 0; i < 16; ++i){
			if(this->state[i] == 0){
			    this->zeroPosi = i;
                break;
			}
		}
	}
	
	short setNeigh(vector<STPnode>& neighVec){
		short zeroPosiMod = this->zeroPosi%4;
		short zeroPosiDiv = this->zeroPosi/4;
		short numNeigh = 0;
		if(zeroPosiMod != 0 && this->generatedMethod != 1){
		    //move 0 left
			neighVec[numNeigh] = this->switch_tile(0);
			//neighVec[numNeigh].print();
		    numNeigh++;
		}
		if(zeroPosiMod != 3 && this->generatedMethod != 0){
		    //move 0 right
			neighVec[numNeigh] = this->switch_tile(1);
			//neighVec[numNeigh].print();
		    numNeigh++;
		}
		if(zeroPosiDiv != 0 && this->generatedMethod != 3){
		    //move 0 up
			neighVec[numNeigh] = this->switch_tile(2);
			//neighVec[numNeigh].print();
		    numNeigh++;
		}
		if(zeroPosiDiv != 3 && this->generatedMethod != 2){
		    //move 0 down
			neighVec[numNeigh] = this->switch_tile(3);
			//neighVec[numNeigh].print();
		    numNeigh++;
		}
		return numNeigh;
	}

	int getRank() {
		//0-7, k-permutation
		short dual[8] = {0};
		for (short i = 0; i < 16; ++i) {
			short currTile = this->state[i];
			if (currTile < 8) {
				dual[currTile] = i;
			}
		}
		//fixed rank
		short dualProb[8];
		for (short i = 0; i < 8; ++i) {
			dualProb[i] = 15-i;
		}
		//converting to mixed rank
		for (short i = 0; i < 8; ++i ) {
			for (short j = i+1; j < 8; ++j) {
				if (dual[j] > dual[i] && dual[j] > 0) {
					//cannot be equal if it's real permutation 
					--dual[j];
				}
			}
		}
		//compute rank
		int rank = 0;
		for (short i = 0; i < 8; i++) {
			rank += dual[i] * factor(dualProb[i], 8);
		}
		return rank;
	}
	
	int getRank1(){
		//0, 8-9, 12-13, k-permutation
		short dual[5] = {0};
		for (short i = 0; i < 16; ++i) {
			short currTile = this->state[i];
			switch(currTile){
				case 0: {dual[0] = i; break;}
			    case 8: {dual[1] = i; break;}
			    case 9: {dual[2] = i; break;}
			    case 12: {dual[3] = i; break;}
			    case 13: {dual[4] = i; break;}
			    default: {break;}
			}
		}
		//fixed rank
		short dualProb[5]={15, 14, 13, 12, 11};
		//converting to mixed rank
		for (short i = 0; i < 5; ++i ) {
			for (short j = i+1; j < 5; ++j) {
				if (dual[j] > dual[i] && dual[j] > 0) {
					//cannot be equal if it's real permutation 
					--dual[j];
				}
			}
		}
		//compute rank
		int rank = 0;
		for (short i = 0; i < 5; i++) {
			rank += dual[i] * factor(dualProb[i], 11);
		}
		return rank;
	}
	
	int getRank2(){
		//0, 8-9, 12-13, k-permutation
		short dual[5] = {0};
		for (short i = 0; i < 16; ++i) {
			short currTile = this->state[i];
			switch(currTile){
				case 0: {dual[0] = i; break;}
			    case 10: {dual[1] = i; break;}
			    case 11: {dual[2] = i; break;}
			    case 14: {dual[3] = i; break;}
			    case 15: {dual[4] = i; break;}
			    default: {break;}
			}
		}
		//fixed rank
		short dualProb[5]={15, 14, 13, 12, 11};
		//converting to mixed rank
		for (short i = 0; i < 5; ++i ) {
			for (short j = i+1; j < 5; ++j) {
				if (dual[j] > dual[i] && dual[j] > 0) {
					//cannot be equal if it's real permutation 
					--dual[j];
				}
			}
		}
		//compute rank
		int rank = 0;
		for (short i = 0; i < 5; i++) {
			rank += dual[i] * factor(dualProb[i], 11);
		}
		return rank;
	}
	
	int MD(){
		//default goal, only for 0-7 PDB delta
		int result = 0;
		
		int currPosi;
		int targPosi;
		for(int i=0;i<16;i++){
			currPosi = i;
			//当前位置，具体值是目标位置 
			targPosi = this->state[i];
			//目标位置，具体值 
			if(this->state[i] == 0 || this->state[i] > 7){continue;}
			int currx = currPosi%4;
		    int curry = currPosi/4;
		    int targx = targPosi%4;
		    int targy = targPosi/4;
		    result += abs(currx-targx) + abs(curry-targy);
		}
		
		return result;
	}
	
	int getDeltaHeur(){
		return (this->gcost - this->MD());
	}
	
	short getGcost(){
		return this->gcost;
	}
	
	void print(){
		for (short i=0; i < 16;++i){
			cout<<this->state[i]<<" ";
		}
		cout<<endl;
	}
};

class pancake{
private:
    short state[16];
    short nextGcost;
    short generatedMethod;

    pancake flipAction(int act){

        short newState[16];
        for(int i = 0; i < 16; i++){
            newState[i] = state[i];
        }

        int upper = 0;
        int lower = act - 1;
        int temp;

        while(upper < lower){
            temp = newState[upper];
            newState[upper] = newState[lower];
            newState[lower] = temp;
            upper++;
            lower--;
        }

        return pancake(this->nextGcost, newState, act);
    }
public:
    short gcost;
    short heuristic;
    short fcost;

    pancake(short gcost, short *newState, short genMethod){
        for(int i=0; i < 16; i++){
            state[i] = newState[i];
        }
        this->gcost = gcost;
        this->nextGcost = gcost + 1;
        this->generatedMethod = genMethod;
    }

    vector<pancake> getNeigh(){
        vector<pancake> neigh;

        for(int i = 16; i>=2; i--){
            if(i == generatedMethod) continue;
            neigh.push_back(flipAction(i));
        }

        return neigh;
    }

    int getHeuristic(){
        return 0;
    }

    void print(){
        for(int i=0; i < 16; i++){
            cout<<state[i]<<" ";
        }
        cout<<endl;
    }

    bool isGoal(){
        for(int i=0; i < 16; i++){
            if(state[i] != i) return false;
        }
        return true;
    }
};

class BFS{
private:
    vector<STPnode>	neighCache;
    //we don't store neighbours so we cache the vector

public:
	BFS(STPnode start){
	    for(short i=0;i<4;++i)
		    this->neighCache.push_back(start);
	}
	
	void search(STPnode start){
		queue<STPnode> nodeQueue;
		nodeQueue.push(start);
		int frontRank;
		
		while(!nodeQueue.empty()){
			STPnode currFront(nodeQueue.front());
			//may be changed
			frontRank = currFront.getRank2();
			nodeQueue.pop();
			if (PDB[frontRank] != -1){
				//cout<<"exists, queue length don't increase: "<<nodeQueue.size()<<endl;
			}
			else{
				PDB[frontRank] = currFront.getGcost();
				//MD1[frontRank] = currFront.MD();
				//cout<<"rank: "<<frontRank<<endl;
				
		        short neighNum = currFront.setNeigh(this->neighCache);
		        //cout<<"neigh: "<<neighNum<<endl;
		        
		        for(short i = 0; i < neighNum; ++i){
		    	    //this->neighCache[i].print();
		            nodeQueue.push(this->neighCache[i]);
				}
				//cout<<"queue length: "<<nodeQueue.size()<<endl;
			}
		}
		cout<<"end"<<endl;
	}
};

int main()
{
	for(int i = 0; i < 518918400 ; ++i ){
		PDB[i] = -1;
	}
	cout<<"INIT"<<endl;
	short startState[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};
	short maxState[16] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
	STPnode a(0, startState, 1);
	cout<<a.MD()<<endl;
	BFS newSearch(a);
	newSearch.search(a);
	ofstream PDBfile("PDB1011.txt");
	double average = 0;
	for(int i = 0; i < 518918400 ; ++i ){
		if(PDB[i]==-1 && PDB[i+1]==-1) break;
		PDBfile<<PDB[i]<<endl;
		//average += double(PDB[i])/double(518918400);
	}
	
	cout <<"uncomprassed average: "<< average<<endl; 
	
	/*average = 0;
	int comprassed;
	for(int i = 0; i < 518918400 ; i = i+2){
		comprassed = min(PDB[i],PDB[i+1]);
		average += double(comprassed)/double(259459200);
	}
	*/
    cout <<"comprassed average: "<< average<<endl; 
    
    return 0;
}

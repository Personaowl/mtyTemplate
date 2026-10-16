// q1 holds ceil(n/2) smaller values; q2 holds the remaining larger values.
struct MedianHeap{
	//int k; 固定第k小时 注意init里需要赋值
    priority_queue<long long> q1;
    priority_queue<long long,vector<long long>,greater<long long>> q2;
    void init() {q1={};q2={};}
    int size()const {return q1.size()+q2.size();}
    bool empty()const {return q1.empty();}
    void add(long long x){
        if(q1.empty() || x<=q1.top()) q1.push(x);
        else q2.push(x);
       
        if(q1.size()>q2.size()+1) {q2.push(q1.top());q1.pop();}
        if(q1.size()<q2.size()) {q1.push(q2.top());q2.pop();}
        
        // 固定第k小
        // while(q1.size()>k) {q2.push(q1.top());q1.pop();}
        // while(q1.size()<k) {q1.push(q2.top());q2.pop();}
    }
    long long query()const {return q1.top();}
    pair<long long, long long> middle()const {
        if(q1.size()==q2.size()) return {q1.top(),q2.top()};
        return {q1.top(),q1.top()};
    }
};


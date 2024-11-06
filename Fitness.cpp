#include <bits/stdc++.h> 
#include <algorithm>
using namespace std;
//n la so luong customer
// nt la so luong technitian
// ndr la so luong drone
int n,nt,ndr;
// vt la van toc technitian. vd la van toc drone
float vt= 0.58;
float vd= 0.83;
float TL=120;
float DL=30;
//ham distance de tinh khoang cach giua 2 diem khi biet toa do
float distance(pair<float,float> diem1,pair<float,float> diem2){
	return sqrt((diem1.first-diem2.first)*(diem1.first-diem2.first)+(diem1.second-diem2.second)*(diem1.second-diem2.second));
}
vector<pair<float,float>> d; //vector d luu mang gia tri cac dia diem cua customers
void input(){
	cout<<"nhap so luong customer: ";
	cin>>n;
	cout<<"nhap so luong technitian: ";
	cin>>nt;
	cout<<"nhap so luong drone: ";
	cin>>ndr;
	cout<<"nhap toa do cac customers:"<<endl;
	pair<float,float> diem;
	diem.first=0;diem.second=0;
	d.push_back(diem);
	for(int i=0;i<n;i++){
		float x,y;
		cin>>x>>y;
		diem.first=x;diem.second=y;
		d.push_back(diem);
	}
}
void checkinput(){
	cout<<"so luong customer: ";
	cout<<n<<endl;
	cout<<"so luong technitian: ";
	cout<<nt<<endl;
	cout<<"so luong drone: ";
	cout<<ndr<<endl;
	cout<<"toa do cac customers:"<<endl;
	for(int i=0;i<=n;i++){
		cout<<d[i].first<<"   "<<d[i].second<<endl;
	}
}
// Tinh fitness va checksolution viec cua thuan

int findElementIndex(const vector<int>& vec, int element) {
    auto it = std::find(vec.begin(), vec.end(), element);
    
    if (it != vec.end()) {
        return it - vec.begin();
    } else {
        return -1;
    }
}

bool compareBySecond(const pair<int, float>& a, const pair<int, float>& b) {
    return a.second < b.second;  // So sánh theo 'second'
}

float fitness(vector<int> NSTT, vector<int> NSTD){
	vector<pair<int,float>> T;
	vector<float> D;
	vector<float> Ds1;
	vector<float> Df1;
	vector<float> Ds2;
	vector<float> Df2;
	vector<int> Dlab1;
	vector<int> Dlab2;
	vector<float>Tf;
	Ds1.push_back(0);
	Ds2.push_back(0);
	float tt=0;
	for(int i=0;i<NSTT.size();i++){
		if(i==0||NSTT[i-1]>n){
			tt=0;
			pair<int,float> t;
			t.first=NSTT[i];
			tt=tt+distance(d[NSTT[i]],d[0])/vt;
			t.second=tt;
			T.push_back(t);
		}
		else{
			pair<int,float> t;
			t.first=NSTT[i];
			tt=tt+distance(d[NSTT[i]],d[0])/vt;
			t.second=tt;
			T.push_back(t);
		}			
		}	
	sort(T.begin(), T.end(), compareBySecond);
	int past=0;
	float td1=0;
	float td2=0;
	for(int i=0;i<NSTD.size();i=i+2){
		if(NSTD[i]==1){
			td1=td1+distance(d[T[i].first],d[past])/vd;
			D.push_back(td1);
			past=T[i].first;
			if(NSTD[i+1]==1){
				td1=td1+distance(d[T[i].first],d[0])/vd;
				past=0;
				Ds1.push_back(td1);
				Df1.push_back(td1);
				Dlab1.push_back(T[i].first);
		}
		}
		if(NSTD[i]==2){
			td2=td2+distance(d[T[i].first],d[past])/vd;
			D.push_back(td2);
			past=T[i].first;
			if(NSTD[i+1]==1){
				td2=td2+distance(d[T[i].first],d[0])/vd;
				past=0;
				Ds2.push_back(td1);
				Df2.push_back(td1);
				Dlab1.push_back(T[i].first);
			}
		}
		else{
			D.push_back(0);
		}
	}
	for(int i=0;i<T.size();i++){
		if(D[i]!=0){
			if(T[i].second>D[i]){
				int x=findElementIndex(Dlab1, T[i].first);
				if(x!=-1){
					for(int j=x;j<Ds1.size();j++){
						Ds1[j]=T[i].second-distance(d[T[i].first],d[0])/vd;
					}
				}
				x=findElementIndex(Dlab2, T[i].first);
				if(x!=-1){
					for(int j=x;j<Ds1.size();j++){
						Ds2[j]=T[i].second-distance(d[T[i].first],d[0])/vd;
					}
				}
				for(int j=i;j<D.size();j++){
					if(NSTD[2*T[i].first+1]==NSTD[2*T[j].first+1]){
						D[j]=D[j]+T[i].second-D[i];
					}
				}
			}
			if(T[i].second<D[i]){
				int check=0;
				for(int k=0;k<NSTT.size();k++){
					if(NSTT[k]>n){
						check=0;
					}
					if(NSTT[k]==T[i].first){
						check=1;
					}
					if(check==1){
						for(int j=i;j<T.size();j++){
							if(T[j].first=NSTT[k]){
								T[j].second=T[j].second-T[i].second+D[i];
							}
						}
					}
				}
			}
		}
	}
	for(int i=0;i<NSTT.size();i++){
		if(NSTT[i]>n){
			for(int j=0;j<T.size();j++){
				if(NSTT[i-1]==T[j].first){
					float tt=T[j].second+distance(d[NSTT[i]],d[0])/vt;
					Tf.push_back(tt);
					if(tt>TL){
						return -1;
					}
					break;
				}
			}
		}	
	}
	for(int i=0;i<Df1.size();i++){
		if(Df1[i]-Ds1[i]>DL){
			return -2;
		}
	}
	for(int i=0;i<Df2.size();i++){
		if(Df2[i]-Ds2[i]>DL){
			return -2;
		}
	}
	int fitness=0;
	int check[100];
	for(int i=0;i<=n;i++){
		check[i]=0;
	}
	int x=0;
	int lt=0;
	for(int i=0;i<NSTT.size();i++){
		if(NSTT[i]>n){
			for(int j=x;i<i;j++){
				for(int k=0;k<T.size();k++){
					if(NSTT[i]==T[k].first){
						fitness=fitness+Tf[lt]-T[k].second;
						break;
					}
				}
				lt=lt+1;
				x=i;
			}
		}
		for(int j=0;j<Dlab1.size();j++){
			if(NSTT[i]==Dlab1[j]){
				for(int j=x;i<i;j++){
					for(int k=0;k<T.size();k++){
						if(NSTT[i]==T[k].first){
							fitness=fitness+Df1[j]-T[k].second;
							break;
						}
					}
					x=i;
				}	
			}
		}
		for(int j=0;j<Dlab2.size();j++){
			if(NSTT[i]==Dlab2[j]){
				for(int j=x;i<i;j++){
					for(int k=0;k<T.size();k++){
						if(NSTT[i]==T[k].first){
							fitness=fitness+Df2[j]-T[k].second;
							break;
						}
					}
					x=i;
				}	
			}
		}
	}
	return fitness;
}

// Hàm main
int main(){
	input();
	checkinput();
}

#include <bits/stdc++.h> 
using namespace std;
//n la so luong customer
// nt la so luong technitian
// ndr la so luong drone
int n,nt,ndr;
// vt la van toc technitian. vd la van toc drone
float vt= 0.58;
float vd= 0.83;
int TL=120;
int DL=30;
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

//Tinh fitness va check solution

//Cau truc de luu thoi diem den 1 customer cua drone va techinitian
typedef struct TimeCustomer{
	int customer;
	float timedrone,timetechnitian;
} TimeCustomer;


//So sanh dua tren thoi diem den cac Customer cua Technitian
bool compareByTechnitianTime(const TimeCustomer& a, const TimeCustomer& b) {
    return a.timetechnitian<b.timetechnitian;
}

//ham tim index 1 phan tu trong 1 vector
int findElementIndex(const vector<int>& vec, int element) {
    auto it = std::find(vec.begin(), vec.end(), element);
    
    if (it != vec.end()) {
        return it - vec.begin();
    } else {
        return -1;
    }
}


//Ham tinh fitness va check
float findFitness(vector<int> NSTT, vector<int> NSTD){
	vector<TimeCustomer> T; 		//vector luu thu tu den cac customer cua cac technitian va drone va thoi diem den diem do
	vector<float> Ds1;				//vector luu thoi diem cac lan xuat phat cua drone 1
	vector<float> Df1;				//vector luu thoi diem cac lan ve cua drone 1
	vector<float> Ds2;				//vector luu thoi diem cac lan xuat phat cua drone 2
	vector<float> Df2;				//vector luu thoi diem cac lan ve cua drone 2
	vector<int> Dlab1;				//vector luu cac customer duoc drone 1 mang mau ve
	vector<int> Dlab2;				//vector luu cac customer duoc drone 2 mang mau ve
	vector<float>Tf;				//vector luu cac thoi diem ve cua cac Technitian
	
	
	//ban dau coi nhu cac drone deu xuat phat tai thoi diem 0
	Ds1.push_back(0);
	Ds2.push_back(0);
	
	
	//Khoi tao thoi diem den cac customer cua Technitian dua vao NST cua Techinitian
	float tt=0;
	for(int i=0;i<NSTT.size();i++){
		if(NSTT[i]>n){
			continue;
		}
		else if(i==0||NSTT[i-1]>n){
			tt=0;
			TimeCustomer t;
			t.customer=NSTT[i];
			tt=tt+distance(d[NSTT[i]],d[0])/vt;
			t.timetechnitian=tt;
			T.push_back(t);
		}
		else{
			TimeCustomer t;
			t.customer=NSTT[i];
			tt=tt+distance(d[NSTT[i]],d[NSTT[i-1]])/vt;
			t.timetechnitian=tt;
			T.push_back(t);
		}			
	}
	
	
	//Sap xep lai vector T theo thoi diem den cac customer cua Technition
	sort(T.begin(), T.end(), compareByTechnitianTime);
	//Khoi tao thoi diem den cac customer cua Drone dua vao NST cua Drone
	int past1=0;
	int past2=0;
	float td1=0;
	float td2=0;
	for(int i=0;i<NSTD.size();i=i+2){
		if(NSTD[i]==1){
			td1=td1+distance(d[T[i/2].customer],d[past1])/vd;
			T[i/2].timedrone=td1;
			past1=T[i/2].customer;
			if(NSTD[i+1]==1){
				td1=td1+distance(d[T[i/2].customer],d[0])/vd;
				past1=0;
				Ds1.push_back(td1);
				Df1.push_back(td1);
				Dlab1.push_back(T[i/2].customer);
			}
		}
		else if(NSTD[i]==2){
			td2=td2+distance(d[T[i/2].customer],d[past2])/vd;
			T[i/2].timedrone=td2;
			past2=T[i/2].customer;
			if(NSTD[i+1]==1){
				td2=td2+distance(d[T[i/2].customer],d[0])/vd;
				past2=0;
				Ds2.push_back(td2);
				Df2.push_back(td2);
				Dlab2.push_back(T[i/2].customer);
			}
		}
		else{
			T[i/2].timedrone=0;
		}
	}
	//Cap nhat thoi diem chinh xac den cac customer cua Drone va Technitian
	int is1=0,is2=0;
	int id1=0;
	int id2=0;
	for(int i=0;i<T.size();i++){
		//dieu kien drone co den lay mau tai customer
		if(T[i].timedrone!=0){
			
			//dieu kien Technitian den customer sau drone thi drone se phai + them thoi gian doi
			if(T[i].timetechnitian>T[i].timedrone){
				if(findElementIndex(Dlab1, T[i].customer)!=-1){
					id1=1;
				};
				if(findElementIndex(Dlab2, T[i].customer)!=-1){
					id2=1;
				};
				//Neu diem truoc do la diem xuat phat cua drone thi drone chi can xuat phat muon hon 
				if(id1==1){
					float t=T[i].timetechnitian-distance(d[T[i].customer],d[0])/vd-Ds1[is1];
					for(int j=is1;j<Ds1.size();j++){
						Df1[j]=Df1[j]+t;
						Ds1[j]=Ds1[j]+t;
						id1=0;
						continue;
					}
					is1++;
				}
				else if(id2==1){
					float t=T[i].timetechnitian-distance(d[T[i].customer],d[0])/vd-Ds2[is2];
					for(int j=is2;j<Ds2.size();j++){
						Df2[j]=Df2[j]+t;
						Ds2[j]=Ds2[j]+t;
						id2=0;
						continue;
					}
					is2++;
				}
				
				//neu khong ta phai cap nhat cac gia tri T[i].timedrone, cac vector Ds1,Ds2,Df1,Df2
				for(int j=i;j<T.size();j++){
					if(NSTD[2*T[i].customer]==NSTD[2*T[j].customer]){
						T[j].timedrone=T[j].timedrone+T[i].timetechnitian-T[i].timedrone;
						if(NSTD[2*T[i].customer+1]==1&&NSTD[2*T[i].customer]==1){
							is1=is1+1;
							for(int k=is1;k<Ds1.size();k++){
								Ds1[k]=Ds1[k]+T[i].timetechnitian-T[i].timedrone;
								Df1[k]=Df1[k]+T[i].timetechnitian-T[i].timedrone;
							}
						}
						if(NSTD[2*T[i].customer+1]==1&&NSTD[2*T[i].customer]==2){
							is2=is2+1;
							for(int k=is2;k<Ds2.size();k++){
								Ds2[k]=Ds2[k]+T[i].timetechnitian-T[i].timedrone;
								Df2[k]=Df2[k]+T[i].timetechnitian-T[i].timedrone;
							}
						}
					}
				}
			}
			
			//neu nhu drone den sau Technitian thi Technitian se phai doi Drone
			if(T[i].timetechnitian<T[i].timedrone){
				int check=0;
				for(int k=0;k<NSTT.size();k++){
					if(NSTT[k]>n){
						check=0;
					}
					if(NSTT[k]==T[i].customer){
						check=1;
					}
					if(check==1){
						for(int j=i;j<T.size();j++){
							if(T[j].customer==NSTT[k]){
								T[j].timetechnitian=T[j].timetechnitian-T[i].timetechnitian+T[i].timedrone;
							}
							if(NSTT[k]>n){
								break;
								check=0;
							}
						}
					}
				}
			}
		}
	}
	//check dieu kien tong thoi gian di cua Technitian
	for(int i=0;i<NSTT.size();i++){
		if(NSTT[i]>=n){
			for(int j=0;j<T.size();j++){
				if(NSTT[i-1]==T[j].customer){
					float tt=T[j].timetechnitian+distance(d[NSTT[i-1]],d[0])/vt;
					Tf.push_back(tt);
					if(tt>TL){
						return -1;
					}
					break;
				}
			}
		}	
	}
	
	//check dieu kien thoi gian bay cua drone
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
	cout<<endl;
	//tinh fitness
	float fitness=0;
	int x=0;
	int lt=0;
	for(int i=0;i<NSTT.size();i++){
		if(NSTT[i]>n){
			for(int j=x;j<i;j++){
				for(int k=0;k<T.size();k++){
					if(NSTT[j]==T[k].customer){
						fitness=fitness+Tf[lt]-T[k].timetechnitian;
						break;
					}
				}
			}
			lt=lt+1;
			x=i;
		}
		for(int j=0;j<Dlab1.size();j++){
			if(NSTT[i]==Dlab1[j]){
				for(int j=x;j<i;j++){
					for(int k=0;k<T.size();k++){
						if(NSTT[i]==T[k].customer){
							fitness=fitness+Df1[j]-T[k].timetechnitian;
							break;
						}
					}
				}
				x=i;	
			}
			
		}
		for(int j=0;j<Dlab2.size();j++){
			if(NSTT[i]==Dlab2[j]){
				for(int j=x;j<i;j++){
					for(int k=0;k<T.size();k++){
						if(NSTT[i]==T[k].customer){
							fitness=fitness+Df2[j]-T[k].timetechnitian;
							break;
						}
					}
				}	
			}
			x=i;
		}
	}
	return fitness;
}


int main(){
	input();
	float x=findFitness({1,2,4,3,5},{1,1,0,0,1,1});
	cout<<x<<endl;
	float y=findFitness({1,2,4,3,5},{0,0,0,0,1,1});
	cout<<y;
}

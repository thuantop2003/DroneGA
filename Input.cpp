#include <bits/stdc++.h> 
using namespace std;
//n la so luong customer
// nt la so luong technitian
// ndr la so luong drone
int n,nt,ndr;
// vt la van toc technitian. vd la van toc drone
float vt= 0.58;
float vd= 0.83;
//ham distance de tinh khoang cach giua 2 diem khi biet toa do
int distance(pair<int,int> diem1,pair<int,int> diem2){
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
int main(){
	input();
	checkinput();
}

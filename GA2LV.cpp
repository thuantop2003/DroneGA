#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>  // std::rand, std::srand
#include <ctime>    // std::time
#include "fitness.h"
using namespace std;

// Định nghĩa số lượng cá thể trong quần thể
const int POPULATION_SIZE = 50;
const int GENERATIONS = 300;
const float MUTATION_RATE = 0.2;

// Biến toàn cục
int n;      // Số lượng khách hàng
int nt;     // Số lượng kỹ thuật viên
int ndr;    // Số lượng drone
float vt;   // Vận tốc của kỹ thuật viên (m/s)
float vd;   // Vận tốc của drone (m/s)
float TL;   // Giới hạn thời gian của kỹ thuật viên (s)
float DL;   // Giới hạn thời gian bay của drone (s)

// Vector lưu tọa độ các điểm
vector<pair<float, float>> d;

// Hàm tính khoảng cách giữa 2 điểm
float distance(pair<float, float> diem1, pair<float, float> diem2) {
    return sqrt(pow(diem1.first - diem2.first, 2) + pow(diem1.second - diem2.second, 2));
}

// Hàm nhập dữ liệu
void input() {
    cout << "Number of customers: ";
    cin >> n;
    cout << "Number of technitians: ";
    cin >> nt;
    cout << "Number of drones: ";
    cin >> ndr;
    cout << "Veclocity of techinitian (m/s): ";
    cin >> vt;
    cout << "Veclocity of drone (m/s): ";
    cin >> vd;
    cout << "Limited time of techinitian (s): ";
    cin >> TL;
    cout << "Limited time of drone (s): ";
    cin >> DL;

    cout << "Customers's locations:" << endl;
    pair<float, float> diem;
    diem.first = 0; diem.second = 0;
    d.push_back(diem);  // Thêm điểm xuất phát (gốc tọa độ)
    for (int i = 0; i < n; i++) {
        float x, y;
        cin >> x >> y;
        diem.first = x; diem.second = y;
        d.push_back(diem);
    }
}

// Hàm kiểm tra đầu vào
void checkinput() {
    cout << "Number of customers: " << n << endl;
    cout << "Number of technitians: " << nt << endl;
    cout << "Number of drones: " << ndr << endl;
    cout << "Veclocity of techinitian (m/s): " << vt << endl;
    cout << "Veclocity of drone (m/s): " << vd << endl;
    cout << "Limited time of techinitian (s): " << TL << endl;
    cout << "Limited time of drone (s): " << DL << endl;
    cout << "Customers's locations:" << endl;
    for (int i = 0; i <= n; i++) {
        cout << d[i].first << " " << d[i].second << endl;
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
float calculateFitness(vector<int> NSTT, vector<int> NSTD){
    vector<TimeCustomer> T; 		//vector luu thu tu den cac customer cua cac technitian va drone va thoi diem den diem do
	vector<float> Ds1;				//vector luu thoi diem cac lan xuat phat cua drone 1
	vector<float> Df1;				//vector luu thoi diem cac lan ve cua drone 1
	vector<float> Ds2;				//vector luu thoi diem cac lan xuat phat cua drone 2
	vector<float> Df2;				//vector luu thoi diem cac lan ve cua drone 2
	vector<float> Ds3;				//vector luu thoi diem cac lan xuat phat cua drone 3
	vector<float> Df3;				//vector luu thoi diem cac lan ve cua drone 3
	vector<int> Dlab1;				//vector luu cac customer duoc drone 1 mang mau ve
	vector<int> Dlab2;				//vector luu cac customer duoc drone 2 mang mau ve
	vector<int> Dlab3;				//vector luu cac customer duoc drone 2 mang mau ve
	vector<float>Tf;				//vector luu cac thoi diem ve cua cac Technitian
	float Tcome[100];
	for(int i=0;i<100;i++){
		Tcome[i]=-1;
	}
	for(int i=1;i<NSTD.size();i=i+2){
		if(NSTD[i]==1&&NSTD[i-1]==0){
			return -3;
		}
	}

	//ban dau coi nhu cac drone deu xuat phat tai thoi diem 0
	Ds1.push_back(0);
	Ds2.push_back(0);
	Ds3.push_back(0);


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
	int past3=0;
	float td1=0;
	float td2=0;
	float td3=0;
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
		else if(NSTD[i]==3){
			td3=td3+distance(d[T[i/2].customer],d[past3])/vd;
			T[i/2].timedrone=td3;
			past3=T[i/2].customer;
			if(NSTD[i+1]==1){
				td3=td3+distance(d[T[i/2].customer],d[0])/vd;
				past3=0;
				Ds3.push_back(td3);
				Df3.push_back(td3);
				Dlab3.push_back(T[i/2].customer);
			}
		}
		else{
			T[i/2].timedrone=0;
		}
	}
	//Cap nhat thoi diem chinh xac den cac customer cua Drone va Technitian
	int is1=0,is2=0,is3=0;
	int id1=1;
	int id2=1;
	int id3=1;
	for(int i=0;i<T.size();i++){
		//dieu kien drone co den lay mau tai customer
		Tcome[i]=T[i].timetechnitian;
		if(NSTD[i*2]!=0){
			//dieu kien Technitian den customer sau drone thi drone se phai + them thoi gian doi
			if(T[i].timetechnitian>T[i].timedrone){
				float t=T[i].timetechnitian-T[i].timedrone;
				T[i].timedrone=T[i].timetechnitian;
				//Neu diem truoc do la diem xuat phat cua drone thi drone chi can xuat phat muon hon
				if(id1==1&&NSTD[i*2]==1){
					for(int j=is1;j<Dlab1.size();j++){
						Df1[j]=Df1[j]+t;
						Ds1[j]=Ds1[j]+t;
					}
					id1=0;
					if(NSTD[i*2+1]==1){
						id1=1;
					}
				}
				else if(id2==1&&NSTD[i*2]==2){
					for(int j=is2;j<Dlab2.size();j++){
						Df2[j]=Df2[j]+t;
						Ds2[j]=Ds2[j]+t;
					}
					id2=0;
					if(NSTD[i*2+1]==1){
						id2=1;
					}
				}
				else if(id3==1&&NSTD[i*2]==3){
					for(int j=is3;j<Dlab3.size();j++){
						Df3[j]=Df3[j]+t;
						Ds3[j]=Ds3[j]+t;
					}
					id3=0;
					if(NSTD[i*2+1]==1){
						id3=1;
					}
				}
				//neu khong ta phai cap nhat cac gia tri T[i].timedrone, cac vector Ds1,Ds2,Df1,Df2
				else{
					for(int j=i+1;j<T.size();j++){
						if(NSTD[2*i]==NSTD[2*j]){
							T[j].timedrone=T[j].timedrone+t;
						}
					}
					if(NSTD[2*i]==1){
						id1=0;
						Df1[is1]=Df1[is1]+t;
						for(int k=is1+1;k<Ds1.size();k++){
							Ds1[k]=Ds1[k]+t;
							Df1[k]=Df1[k]+t;
						}
						if(NSTD[2*i+1]==1){
							id1=1;is1++;
						}
					}
					else if(NSTD[2*i]==2){
						id2=0;
						Df2[is2]=Df2[is2]+t;
						for(int k=is2+1;k<Ds2.size();k++){
							Ds2[k]=Ds2[k]+t;
							Df2[k]=Df2[k]+t;
						}
						if(NSTD[2*i+1]==1){
							id2=1;is2++;
						}
					}
					else if(NSTD[2*i]==3){
						id3=0;
						Df3[is3]=Df3[is3]+t;
						for(int k=is3+1;k<Ds3.size();k++){
							Ds3[k]=Ds3[k]+t;
							Df3[k]=Df3[k]+t;
						}
						if(NSTD[2*i+1]==1){
							id3=1;is3++;
						}
					}
				}
			}
			//neu nhu drone den sau Technitian thi Technitian se phai doi Drone
			if(T[i].timetechnitian<T[i].timedrone){
				if(NSTD[2*i]==1){
					id1=0;
					if(NSTD[2*i+1]==1){
						id1=1;
						is1++;
					}
				}
				if(NSTD[2*i]==2){
					id2=0;
					if(NSTD[2*i+1]==1){
						id2=1;
						is2++;
					}
				}
				if(NSTD[2*i]==3){
					id3=0;
					if(NSTD[2*i+1]==1){
						id3=1;
						is3++;
					}
				}
				float t=-T[i].timetechnitian+T[i].timedrone;
				T[i].timetechnitian=T[i].timedrone;
				int check=0;
				for(int k=0;k<NSTT.size();k++){
					if(check==1){
						if(NSTT[k]>n){
							break;
						}
						for(int j=i;j<T.size();j++){
							if(T[j].customer==NSTT[k]){
								T[j].timetechnitian=T[j].timetechnitian+t;
							}
						}
					}
					if(NSTT[k]==T[i].customer){
						check=1;
					}
				}
			}
		}
	}
	//check dieu kien tong thoi gian di cua Technitian
	int pt=0;
	float ptt=0;
	for(int i=0;i<NSTT.size()+1;i++){
		if(NSTT[i]>n||i==NSTT.size()){
			for(int j=0;j<T.size();j++){
				if(NSTT[i-1]==T[j].customer){
					float tt=T[j].timetechnitian+distance(d[NSTT[i-1]],d[0])/vt;
					Tf.push_back(tt);
					if(tt>TL){
						pt=1;
						if(tt-TL>pt){
							ptt=tt-TL;
						}
					}
					break;
				}
			}
		}
	}
	int pr=0;
	float prr=0;
	//check dieu kien thoi gian bay cua drone
	for(int i=0;i<Df1.size();i++){
		if(Df1[i]-Ds1[i]>DL){
			pr=1;
			if(Df1[i]-Ds1[i]-DL>prr){
				prr=Df1[i]-Ds1[i]-DL;
			}
		}
	}
	for(int i=0;i<Df2.size();i++){
		if(Df2[i]-Ds2[i]>DL){
			pr=1;
			if(Df2[i]-Ds2[i]-DL>prr){
				prr=Df2[i]-Ds2[i]-DL;
			}
		}
	}
	//tinh fitness
	float fitness=0;
	int x=0;
	int lt=0;
	for(int i=0;i<NSTT.size()+1;i++){
		if(NSTT[i]>n||i==NSTT.size()){
			for(int j=x;j<i;j++){
				for(int k=0;k<T.size();k++){
					if(NSTT[j]==T[k].customer){
						fitness=fitness+Tf[lt]-Tcome[k];
						break;
					}
				}
			}
			if(NSTT[i+1]<=n){
				lt=lt+1;
				x=i+1;
			}
		}
		for(int j=0;j<Dlab1.size();j++){
			if(NSTT[i]==Dlab1[j]){
				for(int jj=x;jj<=i;jj++){
					for(int k=0;k<T.size();k++){
						if(NSTT[jj]==T[k].customer){
							fitness=fitness+Df1[j]-Tcome[k];
							break;
						}
					}
				}
				x=i+1;
				break;
			}
		}
		for(int j=0;j<Dlab2.size();j++){
			if(NSTT[i]==Dlab2[j]){
				for(int jj=x;jj<=i;jj++){
					for(int k=0;k<T.size();k++){
						if(NSTT[jj]==T[k].customer){
							fitness=fitness+Df2[j]-Tcome[k];
							break;
						}
					}
				}
				x=i+1;
				break;
			}
		}
		for(int j=0;j<Dlab3.size();j++){
			if(NSTT[i]==Dlab3[j]){
				for(int jj=x;jj<=i;jj++){
					for(int k=0;k<T.size();k++){
						if(NSTT[jj]==T[k].customer){
							fitness=fitness+Df3[j]-Tcome[k];
							break;
						}
					}
				}
				x=i+1;
				break;
			}
		}
	}
	if(pt==1){
		fitness=fitness*(1+0.8*ptt/(TL+0.2));
	}
	if(pr==1){
		fitness=fitness*(1+0.8*prr/(DL+0.2));
	}
	T.clear();
	Ds1.clear();
	Df1.clear();
	Ds2.clear();
	Df2.clear();
	Ds3.clear();
	Df3.clear();
	Dlab1.clear();
	Dlab2.clear();
	Dlab3.clear();
	return fitness;
}


// Hàm fitness trả về giá trị ngẫu nhiên trong khoảng [0, 100]
/*float calculateFitness(const std::vector<int>& chromosome_tech, const std::vector<int>& chromosome_drone) {
    // Khởi tạo seed cho số ngẫu nhiên (nên gọi std::srand() một lần trong chương trình chính)
    static bool seed_initialized = false;
    if (!seed_initialized) {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed dựa trên thời gian hiện tại
        seed_initialized = true;
    }

    // Sinh số ngẫu nhiên trong khoảng [0, 100]
    float random_fitness = static_cast<float>(std::rand()) / RAND_MAX * 100.0f;
    return random_fitness;
}

// Hàm tìm đoạn con liên tiếp dài nhất giữa hai vector
int longestCommonSubsequence(const vector<int>& seq1, const vector<int>& seq2) {
    int m = seq1.size();
    int n = seq2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (seq1[i - 1] == seq2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}*/

// Hàm kiểm tra xem cá thể mới có trùng với cá thể trong quần thể không
bool isDuplicate(const vector<int>& newNSTT, const vector<pair<vector<int>, vector<int>>>& population) {
    /*for (const auto& individual : population) {
        int commonLength = longestCommonSubsequence(newNSTT, individual.first);
        if (commonLength > 9) { // Giả định đoạn trùng dài nhất >= 5 là đủ để coi là trùng
            return true;
        }
    }*/

    return false;
}
// Hàm khởi tạo quần thể có kiểm tra trùng
vector<pair<vector<int>, vector<int>>> initializePopulation() {
    vector<pair<vector<int>, vector<int>>> population;

    srand(time(0)); // Khởi tạo seed cho random

    int attempts = 0; // Đếm số lần thử
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<int> NSTT, NSTD;

        // Tạo chuỗi NSTT ngẫu nhiên cho các technician (kỹ thuật viên)
        vector<int> customers;
        for (int j = 1; j <= n; j++) {
            customers.push_back(j);  // Khách hàng từ 1 đến n
        }

        // Trộn các khách hàng để tạo một tuyến đường ngẫu nhiên
        random_shuffle(customers.begin(), customers.end());
        NSTT = customers;

        // Thêm các số lớn hơn n vào NSTT sao cho cách xa nhau
        int minGap = (n + nt - 1) / 2.5;
        int maxGap = (n + nt - 1) / 1.5;

        for (int j = 0; j < nt - 1; j++) {
            int delimiterValue = n + j + 1;  // Tạo số lớn hơn n (n+1, n+2, ...)
            int gap = minGap + (rand() % (maxGap - minGap + 1));  // Chọn khoảng cách ngẫu nhiên trong phạm vi

            // Tìm vị trí để chèn với khoảng cách tối thiểu là `gap`
            int insertPosition = gap;
            while (insertPosition < NSTT.size() && NSTT[insertPosition - 1] > n) {
                insertPosition += gap;  // Nếu vị trí trùng với một số lớn hơn n, tăng khoảng cách
            }

            // Chèn vào vị trí đã tìm được
            if (insertPosition >= NSTT.size()) {
                NSTT.push_back(delimiterValue);  // Nếu không còn vị trí phù hợp, chèn vào cuối
            } else {
                NSTT.insert(NSTT.begin() + insertPosition, delimiterValue);
            }
        }

        // Tạo chuỗi NSTD ngẫu nhiên cho các drone
        vector<int> droneChromosome(2 * n, 0);  // Chuỗi NSTD (Drone) dài 2n, mặc định là 0

        // Mã hóa cho Drone: mỗi khách hàng sẽ có 2 bit
        int drone1LastCustomer = -1, drone2LastCustomer = -1;
        for (int j = 0; j < NSTT.size(); j++) {
            if (NSTT[j] <= n) { // Nếu là khách hàng
                int droneAssigned = (rand() % 2) + 1;  // Chọn drone 1 hoặc drone 2
                int returnToLab = (rand() % 2);  // Quay về hoặc không quay về

                // Đảm bảo cuối cùng các drone luôn quay về phòng lab
                if (j == NSTT.size() - 1) {
                    returnToLab = 1;
                }

                // Cập nhật thông tin của drone
                int index = 2 * (NSTT[j] - 1);
                droneChromosome[index] = droneAssigned;  // Chọn drone
                droneChromosome[index + 1] = returnToLab; // Quay về phòng thí nghiệm hoặc không

                // Cập nhật vị trí khách hàng cuối cùng được phục vụ cho mỗi drone
                if (droneAssigned == 1) {
                    drone1LastCustomer = j;
                } else if (droneAssigned == 2) {
                    drone2LastCustomer = j;
                }
            }
        }

        // Đảm bảo rằng drone cuối cùng quay về phòng thí nghiệm
        if (drone1LastCustomer != -1) {
            int index = 2 * (NSTT[drone1LastCustomer] - 1);
            droneChromosome[index + 1] = 1;  // Drone 1 quay về
        }
        if (drone2LastCustomer != -1) {
            int index = 2 * (NSTT[drone2LastCustomer] - 1);
            droneChromosome[index + 1] = 1;  // Drone 2 quay về
        }

        // Thêm 4 bit cuối cùng để đảm bảo các drone luôn quay về lab
        droneChromosome[2 * n - 4] = 1;
        droneChromosome[2 * n - 3] = 1;
        droneChromosome[2 * n - 2] = 2;
        droneChromosome[2 * n - 1] = 1;

        // Kiểm tra trùng lặp
        if (isDuplicate(NSTT, population)) {
            i--;  // Giảm chỉ số vòng lặp, thử lại cá thể mới
            attempts++;
            if (attempts > 100) { // Nếu thử quá nhiều lần thì dừng để tránh vòng lặp vô hạn
                cout << "Warning: Too many duplicate attempts, stopping early." << endl;
                break;
            }
            continue;
        }

        // Lưu chuỗi drone vào population
        population.push_back({NSTT, droneChromosome});
    }

    return population;
}

// Hàm chọn lọc theo giải đấu nhị phân
pair<vector<int>, vector<int>> select(const vector<pair<vector<int>, vector<int>>>& population, const vector<float>& fitnessValues) {
    // Chọn ngẫu nhiên hai cá thể từ quần thể
    int index1 = rand() % population.size();
    int index2 = rand() % population.size();

    // So sánh giá trị fitness của hai cá thể và chọn cá thể tốt hơn
    if (fitnessValues[index1] < fitnessValues[index2]) {  // Sử dụng dấu "<" để chọn cá thể có fitness nhỏ hơn (thời gian chờ ít hơn)
        return population[index1];
    } else {
        return population[index2];
    }
}

// //Lai ghép điểm cắt
// pair<vector<int>, vector<int>> crossover(const pair<vector<int>, vector<int>>& parent1, const pair<vector<int>, vector<int>>& parent2, const vector<pair<vector<int>, vector<int>>>& population) {
//     int cutPoint = rand() % n;  // Chọn điểm cắt ngẫu nhiên

//     // Khởi tạo NSTT và NSTD từ cha mẹ
//     vector<int> child_NSTT = parent1.first;
//     vector<int> child_NSTD = parent1.second;

//     // Lai ghép từ điểm cắt
//     for (int i = cutPoint; i < n; i++) {
//         child_NSTT[i] = parent2.first[i];
//         child_NSTD[2 * i] = parent2.second[2 * i];
//         child_NSTD[2 * i + 1] = parent2.second[2 * i + 1];
//     }

//     // Kiểm tra trùng lặp trong quần thể
//     int attempts = 0;  // Đếm số lần thử
//     while (isDuplicate(child_NSTT, population)) {
//         // Nếu trùng lặp, tạo lại cá thể con từ cha mẹ
//         cutPoint = rand() % n;

//         // Khởi tạo lại cá thể con
//         child_NSTT = parent1.first;
//         child_NSTD = parent1.second;

//         // Lai ghép lại từ điểm cắt
//         for (int i = cutPoint; i < n; i++) {
//             child_NSTT[i] = parent2.first[i];
//             child_NSTD[2 * i] = parent2.second[2 * i];
//             child_NSTD[2 * i + 1] = parent2.second[2 * i + 1];
//         }

//         // Tăng số lần thử
//         attempts++;
//         if (attempts > 500) {  // Nếu thử quá nhiều lần, dừng và cảnh báo
//             cout << "Warning: Too many duplicate attempts in crossover, stopping early." << endl;
//             break;
//         }
//     }

//     // Sửa đổi NSTT để đảm bảo không có giá trị trùng lặp
//     unordered_set<int> seen;
//     vector<int> missing;
//     for (int i = 1; i <= n + nt - 1; i++) {
//         seen.insert(i);
//     }

//     for (int& gene : child_NSTT) {
//         if (seen.find(gene) != seen.end()) {
//             seen.erase(gene);
//         } else {
//             gene = -1;  // Đánh dấu giá trị trùng lặp để thay thế sau
//         }
//     }

//     for (int missingValue : seen) {
//         missing.push_back(missingValue);
//     }

//     int missingIndex = 0;
//     for (int& gene : child_NSTT) {
//         if (gene == -1) {
//             gene = missing[missingIndex++];
//         }
//     }

//     return {child_NSTT, child_NSTD};
// }

// Lai ghép PMX (Partially Mapped Crossover)
pair<vector<int>, vector<int>> crossover(const pair<vector<int>, vector<int>>& parent1, const pair<vector<int>, vector<int>>& parent2, const vector<pair<vector<int>, vector<int>>>& population) {
    int size = parent1.first.size();

    // Tạo ngẫu nhiên hai điểm cắt
    int cutPoint1 = rand() % size;
    int cutPoint2 = rand() % size;
    if (cutPoint1 > cutPoint2) {
        swap(cutPoint1, cutPoint2);
    }

    // Khởi tạo cá thể con
    vector<int> child_NSTT(size, -1);

    // Bản đồ gen từ parent1 sang parent2
    unordered_map<int, int> mapping;

    // Sao chép đoạn giữa từ parent1 sang con
    for (int i = cutPoint1; i <= cutPoint2; i++) {
        child_NSTT[i] = parent1.first[i];
        mapping[parent2.first[i]] = parent1.first[i];
    }

    // Điền các gen còn lại từ parent2
    for (int i = 0; i < size; i++) {
        if (i >= cutPoint1 && i <= cutPoint2) {
            continue; // Bỏ qua đoạn giữa đã sao chép
        }

        int gene = parent2.first[i];

        // Kiểm tra xem gen đã tồn tại trong đoạn giữa
        while (mapping.find(gene) != mapping.end()) {
            gene = mapping[gene];
        }

        child_NSTT[i] = gene;
    }

    // Kiểm tra trùng lặp trong quần thể
    int attempts = 0;
    while (isDuplicate(child_NSTT, population)) {
        // Nếu trùng lặp, tạo lại các điểm cắt
        cutPoint1 = rand() % size;
        cutPoint2 = rand() % size;
        if (cutPoint1 > cutPoint2) {
            swap(cutPoint1, cutPoint2);
        }

        // Khởi tạo lại cá thể con
        child_NSTT.assign(size, -1);
        mapping.clear();

        // Sao chép lại đoạn giữa từ parent1 sang con
        for (int i = cutPoint1; i <= cutPoint2; i++) {
            child_NSTT[i] = parent1.first[i];
            mapping[parent2.first[i]] = parent1.first[i];
        }

        // Điền lại các gen còn lại từ parent2
        for (int i = 0; i < size; i++) {
            if (i >= cutPoint1 && i <= cutPoint2) {
                continue;
            }

            int gene = parent2.first[i];
            while (mapping.find(gene) != mapping.end()) {
                gene = mapping[gene];
            }

            child_NSTT[i] = gene;
        }

        attempts++;
        if (attempts > 25) {
            cout << "Warning: Too many duplicate attempts in crossover, trying again." << endl;
            break;
        }
    }

    // Sửa đổi NSTT để đảm bảo không có giá trị trùng lặp
    unordered_set<int> seen;
    vector<int> missing;
    for (int i = 1; i <= size; i++) {
        seen.insert(i);
    }

    for (int& gene : child_NSTT) {
        if (seen.find(gene) != seen.end()) {
            seen.erase(gene);
        } else {
            gene = -1; // Đánh dấu giá trị trùng lặp để thay thế sau
        }
    }

    for (int missingValue : seen) {
        missing.push_back(missingValue);
    }

    int missingIndex = 0;
    for (int& gene : child_NSTT) {
        if (gene == -1) {
            gene = missing[missingIndex++];
        }
    }

    // NSTT đã hoàn thành, copy trực tiếp NSTD từ cha mẹ
    vector<int> child_NSTD = parent1.second; // hoặc có thể thêm logic lai ghép cho NSTD

    return {child_NSTT, child_NSTD};
}


// Đột biến thay thế
// void mutate(pair<vector<int>, vector<int>>& individual) {
//     // Đột biến NSTT (mã tuyến đường kỹ thuật viên)
//     for (int i = 0; i < individual.first.size(); i++) {
//         if ((float)rand() / RAND_MAX < MUTATION_RATE) {
//             individual.first[i] = rand() % (n + nt);  // Giá trị có thể là khách hàng (1...n) hoặc delimiter (n+1...n+nt-1)
//         }
//     }

//     // Sửa đổi NSTT để đảm bảo không có giá trị trùng lặp
//     unordered_set<int> seen;
//     vector<int> missing;
//     for (int i = 1; i <= n + nt - 1; i++) {
//         seen.insert(i);
//     }

//     for (int& gene : individual.first) {
//         if (seen.find(gene) != seen.end()) {
//             seen.erase(gene);
//         } else {
//             gene = -1;
//         }
//     }

//     for (int missingValue : seen) {
//         missing.push_back(missingValue);
//     }

//     int missingIndex = 0;
//     for (int& gene : individual.first) {
//         if (gene == -1) {
//             gene = missing[missingIndex++];
//         }
//     }

//     // Đảm bảo các vách ngăn có khoảng cách hợp lệ (như đã được giải thích ở trên)
//     vector<int> delimitersPositions;
//     for (int i = 0; i < individual.first.size(); i++) {
//         if (individual.first[i] > n) {
//             delimitersPositions.push_back(i);
//         }
//     }

//     for (int i = 1; i < delimitersPositions.size(); i++) {
//         int prevDelimiter = delimitersPositions[i - 1];
//         int currDelimiter = delimitersPositions[i];
//         int minDistance = (n + nt - 1) / 2.5;
//         int maxDistance = (n + nt - 1) / 1.5;

//         if (currDelimiter - prevDelimiter < minDistance) {
//             int distanceToMove = minDistance - (currDelimiter - prevDelimiter);
//             for (int j = currDelimiter; j < individual.first.size(); j++) {
//                 if (individual.first[j] <= n) {
//                     swap(individual.first[currDelimiter], individual.first[j]);
//                     break;
//                 }
//             }
//         }
//     }

//     // Đảm bảo không có vách ngăn đứng đầu hoặc cuối NSTT
//     if (individual.first[0] > n) {
//         for (int i = 1; i < individual.first.size(); i++) {
//             if (individual.first[i] <= n) {
//                 swap(individual.first[0], individual.first[i]);
//                 break;
//             }
//         }
//     }

//     if (individual.first[individual.first.size() - 1] > n) {
//         for (int i = individual.first.size() - 2; i >= 0; i--) {
//             if (individual.first[i] <= n) {
//                 swap(individual.first[individual.first.size() - 1], individual.first[i]);
//                 break;
//             }
//         }
//     }

//     // Đột biến NSTD (mã trạng thái drone)
//     for (int i = 0; i < individual.second.size(); i++) {
//         if ((float)rand() / RAND_MAX < MUTATION_RATE) {
//             if (i % 2 == 0) {
//                 individual.second[i] = (rand() % 2) + 1;
//             } else {
//                 individual.second[i] = rand() % 2;
//             }
//         }
//     }

//     // Đảm bảo rằng 4 bit cuối có dạng 1 1 2 1 hoặc 2 1 1 1
//     int lastIndex = individual.second.size() - 4;
//     if (rand() % 2 == 0) {
//         individual.second[lastIndex] = 1;
//         individual.second[lastIndex + 1] = 1;
//         individual.second[lastIndex + 2] = 2;
//         individual.second[lastIndex + 3] = 1;
//     } else {
//         individual.second[lastIndex] = 2;
//         individual.second[lastIndex + 1] = 1;
//         individual.second[lastIndex + 2] = 1;
//         individual.second[lastIndex + 3] = 1;
//     }
// }

//Đột biến 1 điểm gene
void mutate(pair<vector<int>, vector<int>>& individual) {
    // Sửa đổi NSTT (mã tuyến đường kỹ thuật viên) với logic đột biến mới
    // int n = individual.first.size();  // Giả sử individual.first là vector chứa mã tuyến đường kỹ thuật viên
    int mutationEnd = 2 * n - 4;  // Chỉ thực hiện mutation trên 2n - 4 bit đầu tiên

    // Đột biến: Chọn ngẫu nhiên 1 gene trong đoạn 2n - 4 bit đầu tiên
    for (int i = 0; i < mutationEnd; i++) {
    if ((float)rand() / RAND_MAX < MUTATION_RATE) {
        // Nếu là vị trí lẻ
        if (i % 2 != 0) {
            // Đột biến cho các giá trị: 1 -> 2, 2 -> 1, hoặc 0 -> 1 hoặc 2
            if (individual.first[i] == 1) {
                individual.first[i] = 2;  // 1 -> 2
            } else if (individual.first[i] == 2) {
                individual.first[i] = 1;  // 2 -> 1
            } else if (individual.first[i] == 0) {
                individual.first[i] = (rand() % 2) + 1;  // 0 -> 1 hoặc 2
            }
        }
        // Nếu là vị trí chẵn
        else {
            // Đột biến cho các giá trị: 0 -> 1, hoặc 1 -> 0
            if (individual.first[i] == 0) {
                individual.first[i] = 1;  // 0 -> 0
            } else if (individual.first[i] == 1) {
                individual.first[i] = 0;  // 1 -> 0
            }
        }
    }
}


    // Sửa đổi NSTT để đảm bảo không có giá trị trùng lặp
    unordered_set<int> seen;
    vector<int> missing;
    for (int i = 1; i <= n + nt - 1; i++) {
        seen.insert(i);
    }

    for (int& gene : individual.first) {
        if (seen.find(gene) != seen.end()) {
            seen.erase(gene);
        } else {
            gene = -1;
        }
    }

    for (int missingValue : seen) {
        missing.push_back(missingValue);
    }

    int missingIndex = 0;
    for (int& gene : individual.first) {
        if (gene == -1) {
            gene = missing[missingIndex++];
        }
    }

    // Đảm bảo các vách ngăn có khoảng cách hợp lệ
    vector<int> delimitersPositions;
    for (int i = 0; i < individual.first.size(); i++) {
        if (individual.first[i] > n) {
            delimitersPositions.push_back(i);
        }
    }

    for (int i = 1; i < delimitersPositions.size(); i++) {
        int prevDelimiter = delimitersPositions[i - 1];
        int currDelimiter = delimitersPositions[i];
        int minDistance = (n + nt - 1) / 2.5;
        int maxDistance = (n + nt - 1) / 1.5;

        if (currDelimiter - prevDelimiter < minDistance) {
            int distanceToMove = minDistance - (currDelimiter - prevDelimiter);
            for (int j = currDelimiter; j < individual.first.size(); j++) {
                if (individual.first[j] <= n) {
                    swap(individual.first[currDelimiter], individual.first[j]);
                    break;
                }
            }
        }
    }

    // Đảm bảo không có vách ngăn đứng đầu hoặc cuối NSTT
    if (individual.first[0] > n) {
        for (int i = 1; i < individual.first.size(); i++) {
            if (individual.first[i] <= n) {
                swap(individual.first[0], individual.first[i]);
                break;
            }
        }
    }

    if (individual.first[individual.first.size() - 1] > n) {
        for (int i = individual.first.size() - 2; i >= 0; i--) {
            if (individual.first[i] <= n) {
                swap(individual.first[individual.first.size() - 1], individual.first[i]);
                break;
            }
        }
    }

    // Đột biến NSTD (mã trạng thái drone)
    for (int i = 0; i < individual.second.size(); i++) {
        if ((float)rand() / RAND_MAX < MUTATION_RATE) {
            if (i % 2 == 0) {
                individual.second[i] = (rand() % 2) + 1;
            } else {
                individual.second[i] = rand() % 2;
            }
        }
    }

    // Đảm bảo rằng 4 bit cuối có dạng 1 1 2 1 hoặc 2 1 1 1
    int lastIndex = individual.second.size() - 4;
    if (rand() % 2 == 0) {
        individual.second[lastIndex] = 1;
        individual.second[lastIndex + 1] = 1;
        individual.second[lastIndex + 2] = 2;
        individual.second[lastIndex + 3] = 1;
    } else {
        individual.second[lastIndex] = 2;
        individual.second[lastIndex + 1] = 1;
        individual.second[lastIndex + 2] = 1;
        individual.second[lastIndex + 3] = 1;
    }
}



// Hàm kiểm tra xem fitness có trùng lặp với các cá thể trong quần thể hay không
bool checkDuplicateFitness(const vector<float>& fitnessValues, float newFitness, float tolerance = 1e-8) {
    /*for (float fitness : fitnessValues) {
        if (fabs(fitness - newFitness) < tolerance) {
            return true;  // Nếu có sự trùng lặp trong phạm vi sai số, trả về true
        }
    }*/
    return false;  // Nếu không có sự trùng lặp
}

void geneticAlgorithm() {
    vector<pair<vector<int>, vector<int>>> population = initializePopulation();

    pair<vector<int>, vector<int>> bestIndividual;
    float bestFitness = FLT_MAX;
    float prevBestFitness = FLT_MAX; // Initialize previous best fitness

    // Tiến hành nhiều thế hệ
    for (int gen = 0; gen < GENERATIONS; gen++) {
        vector<float> fitnessValues;

        // Tính fitness cho từng cá thể
        for (const auto& individual : population) {
            float fitness = calculateFitness(individual.first, individual.second);
            fitnessValues.push_back(fitness);

            // Cập nhật cá thể tốt nhất
            if (fitness < bestFitness && fitness > 0) {
                bestFitness = fitness;
                bestIndividual = individual;
            }
        }

        // Cập nhật previous best fitness
        prevBestFitness = bestFitness;

        // Sắp xếp quần thể theo fitness (từ tốt nhất đến tồi nhất)
        vector<pair<pair<vector<int>, vector<int>>, float>> fitnessSortedPopulation;
        for (size_t i = 0; i < population.size(); ++i) {
            fitnessSortedPopulation.push_back(make_pair(population[i], fitnessValues[i]));
        }
        sort(fitnessSortedPopulation.begin(), fitnessSortedPopulation.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        // Tạo quần thể mới
        vector<pair<vector<int>, vector<int>>> newPopulation;

        // Giữ lại 10 cá thể tốt nhất (elitism)
        for (int i = 0; i < 5; ++i) {
            newPopulation.push_back(fitnessSortedPopulation[i].first);
        }

        // Duy trì quần thể mới với crowding và lai ghép + đột biến
        for (int i = 5; i < POPULATION_SIZE; i++) {
            auto parent1 = select(population, fitnessValues);
            auto parent2 = select(population, fitnessValues);

            auto child = crossover(parent1, parent2, population);
            mutate(child);

            bool isDuplicate = false;
            for (const auto& individual : newPopulation) {
                if (fabs(calculateFitness(child.first, child.second) - calculateFitness(individual.first, individual.second)) < 1e-6) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                newPopulation.push_back(child);
            } else {
                // Thay thế bằng cá thể từ thế hệ cũ nếu bị trùng lặp
                newPopulation.push_back(population[rand() % population.size()]);
            }
        }

        // Nếu thiếu cá thể, bổ sung các cá thể có fitness tốt từ thế hệ trước
        while (newPopulation.size() < POPULATION_SIZE) {
            for (const auto& individual : fitnessSortedPopulation) {
                if (newPopulation.size() >= POPULATION_SIZE) break;
                newPopulation.push_back(individual.first);
            }
        }

        // Thay thế quần thể cũ bằng quần thể mới
        population = newPopulation;

        // In ra thông tin của thế hệ hiện tại
        cout << "Generation " << gen + 1 << " - Best Fitness: " << bestFitness << endl;
    }

    // In ra cá thể tốt nhất cuối cùng và fitness của chúng
    cout << "\n\n";
    for (int gene : bestIndividual.first) {
        cout << gene << " ";
    }
    cout << "\n";
    for (int gene : bestIndividual.second) {
        cout << gene << " ";
    }
    cout << "\n" << bestFitness << endl;
}


/*void geneticAlgorithm() {
    vector<pair<vector<int>, vector<int>>> population = initializePopulation();

    pair<vector<int>, vector<int>> bestIndividual;
    float bestFitness = FLT_MAX;
    float prevBestFitness = FLT_MAX; // Initialize previous best fitness

    // Tiến hành nhiều thế hệ
    for (int gen = 0; gen < GENERATIONS; gen++) {
        vector<float> fitnessValues;

        // Tính fitness cho từng cá thể
        for (const auto& individual : population) {
            float fitness = calculateFitness(individual.first, individual.second);
            fitnessValues.push_back(fitness);

            // Cập nhật cá thể tốt nhất
            if (fitness < bestFitness && fitness > 0) {
                bestFitness = fitness;
                bestIndividual = individual;
            }
        }

        // Hiển thị fitness và gen của tất cả cá thể trong thế hệ hiện tại
        cout << "Generation " << gen + 1 << " - Fitness values and genes:\n";
        for (size_t i = 0; i < fitnessValues.size(); ++i) {
            cout << "Individual " << i + 1 << " - Fitness: " << fitnessValues[i] << " - Genes: [ ";
            for (int gene : population[i].first) {
                cout << gene << " ";
            }
            cout << "] [ ";
            for (int gene : population[i].second) {
                cout << gene << " ";
            }
            cout << "]\n";
        }

        // Kiểm tra nếu chất lượng không thay đổi đáng kể
        //if (gen > 0 && fabs(bestFitness - prevBestFitness) < 1e-6) {
        //    cout << "No significant improvement in fitness. Terminating early." << endl;
        //    break;  // Dừng sớm nếu fitness không thay đổi đáng kể
        //}

        // Cập nhật previous best fitness
        prevBestFitness = bestFitness;

        // Sắp xếp quần thể theo fitness (từ tốt nhất đến tồi nhất)
        vector<pair<pair<vector<int>, vector<int>>, float>> fitnessSortedPopulation;
        for (size_t i = 0; i < population.size(); ++i) {
            fitnessSortedPopulation.push_back(make_pair(population[i], fitnessValues[i]));
        }
        sort(fitnessSortedPopulation.begin(), fitnessSortedPopulation.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        // Tạo quần thể mới, giữ lại 10 cá thể tốt nhất (elitism)
        vector<pair<vector<int>, vector<int>>> newPopulation;

        // Giữ lại 10 cá thể tốt nhất
        for (int i = 0; i < 5; ++i) {
            newPopulation.push_back(fitnessSortedPopulation[i].first);
        }

        // Duy trì quần thể mới với crowding và lai ghép + đột biến
        for (int i = 10; i < POPULATION_SIZE; i++) {
            // Chọn hai cha mẹ
            auto parent1 = select(population, fitnessValues);
            auto parent2 = select(population, fitnessValues);

            // Lai ghép
            auto child = crossover(parent1, parent2, population);

            // Đột biến
            mutate(child);

            // Kiểm tra xem cá thể con có trùng lặp với cá thể trong quần thể mới hay không
            bool isDuplicate = false;
            for (const auto& individual : newPopulation) {
                float fitness1 = calculateFitness(child.first, child.second);
                float fitness2 = calculateFitness(individual.first, individual.second);
                // Nếu fitness quá giống, cá thể này là trùng lặp
                if (fabs(fitness1 - fitness2) < 1e-6) {
                    isDuplicate = true;
                    break;
                }
            }

            // Nếu không phải là cá thể trùng lặp, thêm vào quần thể mới
            if (!isDuplicate) {
                newPopulation.push_back(child);
            } else {
                // Nếu là cá thể trùng lặp, thay thế bằng cá thể lai ghép khác
                i--; // Giảm chỉ số để thử lại
            }
        }

        // Thay thế quần thể cũ bằng quần thể mới
        population = newPopulation;

        // In ra thông tin của thế hệ hiện tại
        cout << "Generation " << gen + 1 << " - Best Fitness: " << bestFitness << endl;
    }

    // In ra cá thể tốt nhất cuối cùng và fitness của chúng
    cout << "\nBest Individual:\n";
    for (int gene : bestIndividual.first) {
        cout << gene << " ";
    }
    cout << "\n";
    for (int gene : bestIndividual.second) {
        cout << gene << " ";
    }
    cout << "\nBest Fitness: " << bestFitness << endl;
}*/



int main() {
    srand(time(0));


    // Nhập dữ liệu
    input();
    checkinput();

    // Chạy thuật toán di truyền
    geneticAlgorithm();

    return 0;
}

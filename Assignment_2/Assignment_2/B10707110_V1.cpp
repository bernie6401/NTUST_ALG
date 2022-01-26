#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <limits>
#include <ctime>

using namespace std;
int infinity = numeric_limits<int>::max();



struct InputData
{
	int numGridx;
	int numGridy;
	int capacity;
	int numNet;
	vector < vector<int> > coord_s;
	vector < vector<int> > coord_t;
};

struct Shortdist_Pred
{
	vector <int> shortest_dist;
	vector <int> pred;
};

InputData Read_input_data(char input_file[100])
{
	// open the input file
	fstream fin;
	fin.open(input_file, fstream::in);
	if (!fin.is_open())
	{
		cout << "Error: the input file is not opened!!" << endl;
		exit(1);
	}


	char buffer[100];

	fin >> buffer;
	fin >> buffer;
	int numGridx = atoi(buffer);
	fin >> buffer;
	int numGridy = atoi(buffer);
	fin >> buffer;
	fin >> buffer;
	int capacity = atoi(buffer);
	fin >> buffer;
	fin >> buffer;
	fin >> buffer;
	int numNet = atoi(buffer);


	// cout << "#Chords:" << numChord << endl;
	cout << "Grids(x, y):" << numGridx << ", " << numGridy << endl;
	cout << "#Capacity:" << capacity << endl;
	cout << "#Net:" << numNet << endl;

	vector < vector<int> > coord_s;
	vector < vector<int> > coord_t;

	//initial variable
	coord_s.resize(numNet);
	coord_t.resize(numNet);
	for (int i = 0; i < numNet; i++)
	{
		coord_s[i].resize(2);
		coord_t[i].resize(2);
	}

	//get the data from input file
	for (int i = 0; i < numNet; i++)
	{
		fin >> buffer;
		fin >> buffer;
		coord_s[i][0] = atoi(buffer);
		fin >> buffer;
		coord_s[i][1] = atoi(buffer);
		fin >> buffer;
		coord_t[i][0] = atoi(buffer);
		fin >> buffer;
		coord_t[i][1] = atoi(buffer);
	}

	InputData input_data = { numGridx, numGridy, capacity, numNet, coord_s, coord_t };

	return input_data;
}

int indexofSmallestElement(int array[], int size)
{
	int index = 0;
	if (size != 1)
	{
		int n = array[0];
		for (int i = 1; i < size; i++)
		{
			if (array[i] < n)
			{
				n = array[i];
				index = i;
			}
		}

		//如果四周都是已經走過或是邊界則回傳index = 4
		if (n >= infinity)
		{
			index = 4;
		}
	}
	return index;
}

void check_is_pass(int x, int y, int temp_array[], vector < vector < vector<int> > > tile_bound_weight, InputData input_data, vector <bool> tile_stack)
{
	if (tile_bound_weight[x][y][0] != infinity)
	{
		if (tile_stack[input_data.numGridx * (y + 1) + x] == false)
			temp_array[0] = tile_bound_weight[x][y][0];     //相對source位置:上
		else temp_array[0] = infinity;
	}
	else temp_array[0] = infinity;

	if (tile_bound_weight[x][y][1] != infinity)
	{
		if (tile_stack[input_data.numGridx * (y - 1) + x] == false)
			temp_array[1] = tile_bound_weight[x][y][1];     //相對source位置:下
		else temp_array[1] = infinity;
	}
	else temp_array[1] = infinity;

	if (tile_bound_weight[x][y][2] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x - 1] == false)
			temp_array[2] = tile_bound_weight[x][y][2];     //相對source位置:左
		else temp_array[2] = infinity;
	}
	else temp_array[2] = infinity;

	if (tile_bound_weight[x][y][3] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x + 1] == false)
			temp_array[3] = tile_bound_weight[x][y][3];     //相對source位置:右
		else temp_array[3] = infinity;
	}
	else temp_array[3] = infinity;

}

Shortdist_Pred check_min_weight(int x, int y, vector < vector < vector<int> > > tile_bound_weight, vector <int> shortest_dist, InputData input_data, vector <int> pred, int temp_array[], vector <bool> tile_stack)
{
	if (tile_bound_weight[x][y][0] != infinity)	//確定該方向不是牆壁
	{
		if (tile_stack[input_data.numGridx * (y + 1) + x] == false)
		{
			if (temp_array[0] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * (y + 1) + x])  //上
			{
				shortest_dist[input_data.numGridx * (y + 1) + x] = temp_array[0] + shortest_dist[input_data.numGridx * y + x];
				pred[input_data.numGridx * (y + 1) + x] = x + input_data.numGridx * y;
			}
		}
		
	}
	if (tile_bound_weight[x][y][1] != infinity)
	{
		if (tile_stack[input_data.numGridx * (y - 1) + x] == false)
		{
			if (temp_array[1] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * (y - 1) + x])  //下
			{
				shortest_dist[input_data.numGridx * (y - 1) + x] = temp_array[1] + shortest_dist[input_data.numGridx * y + x];
				pred[input_data.numGridx * (y - 1) + x] = x + input_data.numGridx * y;
			}
		}
	}
	if (tile_bound_weight[x][y][2] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x - 1] == false)
		{
			if (temp_array[2] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * y + x - 1])  //左
			{
				shortest_dist[input_data.numGridx * y + x - 1] = temp_array[2] + shortest_dist[input_data.numGridx * y + x];
				pred[input_data.numGridx * y + x - 1] = x + input_data.numGridx * y;
			}
		}
	}
	if (tile_bound_weight[x][y][3] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x + 1] == false)
		{
			if (temp_array[3] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * y + x + 1])  //右tile_bound_weight[x][y]
			{
				shortest_dist[input_data.numGridx * y + x + 1] = temp_array[3] + shortest_dist[input_data.numGridx * y + x];
				pred[input_data.numGridx * y + x + 1] = x + input_data.numGridx * y;
			}
		}
	}

	Shortdist_Pred shortdist_pred = { shortest_dist, pred };

	return shortdist_pred;
}

vector < vector < vector<int> > > deman_add_one(vector< vector< vector<int> > > bound_demand, int pred, int terminal, InputData input_data)
{
	if (terminal == pred - input_data.numGridx)	//代表terminal往上到pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][1]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][0]++;
	}
	if (terminal == pred + input_data.numGridx)	//代表terminal往下到pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][0]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][1]++;
	}
	if (terminal == pred + 1)	//代表terminal往左到pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][3]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][2]++;
	}
	if (terminal == pred - 1)	//代表terminal往右到pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][2]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][3]++;
	}

	return bound_demand;
}

int main(int argc, char** argv)
{
	/*if (argc != 3)
	{
		cout << "Usage: ./[exe] [input file] [output file]" << endl;
		exit(1);
	}*/
	int fail_count_cont = 0;
	
	char input_file[11] = "./20x20.in";
	InputData input_data = Read_input_data(input_file);//argv[1]
	// open the output file
	fstream fout;
	fout.open("./20x20.out", fstream::out);//argv[2]
	if (!fout.is_open())
	{
		cout << "Error: the output file is not opened!!" << endl;
		exit(1);
	}

	vector< vector< vector<int> > > tile_bound_weight;  //紀錄每一個tile的boundary目前的權重多少
	vector< vector< vector<int> > > bound_demand;       //紀錄每一個tile的boundary目前穿過多少次
	

	//initial two variable with vector type
	tile_bound_weight.resize(input_data.numGridx);
	bound_demand.resize(input_data.numGridx);
	for (int i = 0; i < input_data.numGridx; i++)
	{
		tile_bound_weight[i].resize(input_data.numGridy);
		bound_demand[i].resize(input_data.numGridy);
		for (int j = 0; j < input_data.numGridy; j++)
		{
			tile_bound_weight[i][j].resize(4);      //不管是demand還是weight，初始值都是零，因為公式:w=demand/capacity
			bound_demand[i][j].resize(4);
		}
	}

	//把所有邊線的demand和weight都設成無限大，這樣才不會超出Grid的範圍
	for (int i = 0; i < input_data.numGridx; i++)
	{
		tile_bound_weight[i][input_data.numGridy - 1][0] = infinity;
		tile_bound_weight[i][0][1] = infinity;
		tile_bound_weight[0][i][2] = infinity;
		tile_bound_weight[input_data.numGridx - 1][i][3] = infinity;
		bound_demand[i][input_data.numGridy - 1][0] = infinity;
		bound_demand[i][0][1] = infinity;
		bound_demand[0][i][2] = infinity;
		bound_demand[input_data.numGridx - 1][i][3] = infinity;

	}
	

	//Dijkstra Part	
	for (int i = 0; i < input_data.numNet; i++)
	{
		//每一次進來都要把short_dist和pred歸零
		vector <int> shortest_dist;
		vector <int> pred;
		shortest_dist.resize(input_data.numGridx * input_data.numGridy);
		pred.resize(input_data.numGridx * input_data.numGridy);

		//Initial All the Value
		for (int j = 0; j < input_data.numGridx * input_data.numGridy; j++)  //每一次有新的路徑要利用Dijkstra做的時候都要初始化
			shortest_dist[j] = infinity;

		//因為shortest_dist和pred是一維的，所以相對位置的公式就是二維的y_value*4+x_value
		shortest_dist[input_data.coord_s[i][0] + input_data.numGridx * input_data.coord_s[i][1]] = 0;   //source的最短路徑要設成0
		pred[input_data.coord_s[i][0] + input_data.numGridx * input_data.coord_s[i][1]] = infinity;     //原點的前一個element也要初始化成無限大，之後可以做判斷
		int x = input_data.coord_s[i][0], y = input_data.coord_s[i][1];
		vector <bool> tile_stack;
		tile_stack.resize(input_data.numGridx * input_data.numGridy);
		
		
		for (int a = 0; a < input_data.numGridx * input_data.numGridy; a++)  //總共要判斷 #tile size 的次數(grid_x * grid_y)
		{
		Loop1:	//如果遇到四周都已經走過的tile就會從這邊開始

			//先找上下左右哪一個weight比較小
			int temp_array[4] = { 0, 0, 0, 0 };
			int temp_index, temp_x, temp_y;


			//往下執行之前先檢查是不是已經走過了
			check_is_pass(x, y, temp_array, tile_bound_weight, input_data, tile_stack);

			//如果目前的value有比原本的value小，就把該位置(tile)的value改成他目前的最小weight
			Shortdist_Pred shortdist_pred = check_min_weight(x, y, tile_bound_weight, shortest_dist, input_data, pred, temp_array, tile_stack);
			shortest_dist = shortdist_pred.shortest_dist;
			pred = shortdist_pred.pred;

			//現在的temp_array就是四周的更新過後的tile
			if (tile_bound_weight[x][y][0] != infinity)	temp_array[0] += shortest_dist[input_data.numGridx * (y + 1) + x];//確定該方向不是牆壁
			if (tile_bound_weight[x][y][1] != infinity)	temp_array[1] += shortest_dist[input_data.numGridx * (y - 1) + x];
			if (tile_bound_weight[x][y][2] != infinity)	temp_array[2] += shortest_dist[input_data.numGridx * y + x - 1];
			if (tile_bound_weight[x][y][3] != infinity)	temp_array[3] += shortest_dist[input_data.numGridx * y + x + 1];
			
			temp_index = indexofSmallestElement(temp_array, 4);
			if (temp_index == 4)
			{
				tile_stack[input_data.numGridx * y + x] = true;
				for (int k = 0; k < input_data.numGridx * input_data.numGridy; k++)
				{
					if (tile_stack[k] == false)
					{
						x = k % input_data.numGridx, y = k / input_data.numGridx;
						goto Loop1;
					}
				}
				break;	//如果tile_stack中所有的tile都已經走過了才會跑到這行，那就代表結束了
			}
			if (temp_index == 0) temp_x = x, temp_y = y + 1;		//上
			else if (temp_index == 1) temp_x = x, temp_y = y - 1;	//下
			else if (temp_index == 2) temp_x = x - 1, temp_y = y;	//左
			else temp_x = x + 1, temp_y = y;						//右
			tile_stack[input_data.numGridx * y + x] = true;
			x = temp_x, y = temp_y;
		}
		//判斷完之後要實際走過並記錄在tile_bound_weight和bound_demand這兩個變數中，demand要雙向都加，因為vector是分開的
		int terminal = input_data.numGridx * input_data.coord_t[i][1] + input_data.coord_t[i][0];
		int fail_count = 0;
		cout << terminal;
		vector <int> temp_stack;
		

		do
		{
			cout << "<-" << pred[terminal];
			temp_stack.push_back(terminal);
			terminal = pred[terminal];

			if (temp_stack.back() == pred[terminal])
			{
				fail_count++;
				fail_count_cont++;
				break;
			}
		} while (terminal != input_data.numGridx * input_data.coord_s[i][1] + input_data.coord_s[i][0]);	//當走到起點的時候就break
		temp_stack.push_back(terminal);
		cout << endl;

		if (fail_count != 0)
		{
			vector <int> temp_stack1;
			temp_stack.clear();
			for (int x = 0; x <= abs(input_data.coord_t[i][0] - input_data.coord_s[i][0]); x++)
			{
				if (input_data.coord_s[i][0] < input_data.coord_t[i][0])
					temp_stack1.push_back(input_data.numGridx* input_data.coord_s[i][1] + input_data.coord_s[i][0] + x);

				else
					temp_stack1.push_back(input_data.numGridx * input_data.coord_s[i][1] + input_data.coord_s[i][0] - x);
			}
			for (int y = 1; y <= abs(input_data.coord_t[i][1] - input_data.coord_s[i][1]); y++)
			{
				if (input_data.coord_s[i][1] < input_data.coord_t[i][1])
					temp_stack1.push_back(input_data.numGridx * (input_data.coord_s[i][1] + y) + input_data.coord_t[i][0]);

				else
					temp_stack1.push_back(input_data.numGridx * (input_data.coord_s[i][1] - y) + input_data.coord_t[i][0]);
			}
			int b = temp_stack1.size();
			for (int a = 0; a < b; a++)
			{
				temp_stack.push_back(temp_stack1.back());
				temp_stack1.pop_back();
			}
			for (int a = 0; a < temp_stack.size() - 1; a++)
				bound_demand = deman_add_one(bound_demand, temp_stack[a + 1], temp_stack[a], input_data);
		}

		else
		{
			for(int a = 0; a < temp_stack.size() - 1; a++)
			bound_demand = deman_add_one(bound_demand, temp_stack[a + 1], temp_stack[a], input_data);
		}
		
		// 寫檔到output file
		fout << i << " " << temp_stack.size() - 1 << endl;
		int stack_size = temp_stack.size();
		for (int a = 0; a < stack_size - 1; a++)
		{
			fout << temp_stack.back() % input_data.numGridx << " " << temp_stack.back() / input_data.numGridx << " ";
			cout << temp_stack.back() % input_data.numGridx << " " << temp_stack.back() / input_data.numGridx << " ";
			temp_stack.pop_back();
			fout << temp_stack.back() % input_data.numGridx << " " << temp_stack.back() / input_data.numGridx;
			cout << temp_stack.back() % input_data.numGridx << " " << temp_stack.back() / input_data.numGridx;
			fout << endl;
			cout << endl;
		}


		//更新tile_bound_weight和bound_demand這兩個變數
		for (int a = 0; a < input_data.numGridy; a++)
		{
			for (int b = 0; b < input_data.numGridx; b++)
			{
				for (int c = 0; c < 4; c++)
				{
					if (tile_bound_weight[b][a][c] != infinity)
						tile_bound_weight[b][a][c] = bound_demand[b][a][c] / 4;
				}
			}
		}
	}

	cout << "The total fail is : " << fail_count_cont;
	system("pause");
	return 0;
}
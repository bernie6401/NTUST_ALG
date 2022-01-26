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

		//�p�G�|�P���O�w�g���L�άO��ɫh�^��index = 4
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
			temp_array[0] = tile_bound_weight[x][y][0];     //�۹�source��m:�W
		else temp_array[0] = infinity;
	}
	else temp_array[0] = infinity;

	if (tile_bound_weight[x][y][1] != infinity)
	{
		if (tile_stack[input_data.numGridx * (y - 1) + x] == false)
			temp_array[1] = tile_bound_weight[x][y][1];     //�۹�source��m:�U
		else temp_array[1] = infinity;
	}
	else temp_array[1] = infinity;

	if (tile_bound_weight[x][y][2] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x - 1] == false)
			temp_array[2] = tile_bound_weight[x][y][2];     //�۹�source��m:��
		else temp_array[2] = infinity;
	}
	else temp_array[2] = infinity;

	if (tile_bound_weight[x][y][3] != infinity)
	{
		if (tile_stack[input_data.numGridx * y + x + 1] == false)
			temp_array[3] = tile_bound_weight[x][y][3];     //�۹�source��m:�k
		else temp_array[3] = infinity;
	}
	else temp_array[3] = infinity;

}

Shortdist_Pred check_min_weight(int x, int y, vector < vector < vector<int> > > tile_bound_weight, vector <int> shortest_dist, InputData input_data, vector <int> pred, int temp_array[], vector <bool> tile_stack)
{
	if (tile_bound_weight[x][y][0] != infinity)	//�T�w�Ӥ�V���O���
	{
		if (tile_stack[input_data.numGridx * (y + 1) + x] == false)
		{
			if (temp_array[0] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * (y + 1) + x])  //�W
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
			if (temp_array[1] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * (y - 1) + x])  //�U
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
			if (temp_array[2] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * y + x - 1])  //��
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
			if (temp_array[3] + shortest_dist[input_data.numGridx * y + x] < shortest_dist[input_data.numGridx * y + x + 1])  //�ktile_bound_weight[x][y]
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
	if (terminal == pred - input_data.numGridx)	//�N��terminal���W��pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][1]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][0]++;
	}
	if (terminal == pred + input_data.numGridx)	//�N��terminal���U��pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][0]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][1]++;
	}
	if (terminal == pred + 1)	//�N��terminal������pred[terminal]
	{
		bound_demand[pred % input_data.numGridx][pred / input_data.numGridx][3]++;
		bound_demand[terminal % input_data.numGridx][terminal / input_data.numGridx][2]++;
	}
	if (terminal == pred - 1)	//�N��terminal���k��pred[terminal]
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

	vector< vector< vector<int> > > tile_bound_weight;  //�����C�@��tile��boundary�ثe���v���h��
	vector< vector< vector<int> > > bound_demand;       //�����C�@��tile��boundary�ثe��L�h�֦�
	

	//initial two variable with vector type
	tile_bound_weight.resize(input_data.numGridx);
	bound_demand.resize(input_data.numGridx);
	for (int i = 0; i < input_data.numGridx; i++)
	{
		tile_bound_weight[i].resize(input_data.numGridy);
		bound_demand[i].resize(input_data.numGridy);
		for (int j = 0; j < input_data.numGridy; j++)
		{
			tile_bound_weight[i][j].resize(4);      //���ެOdemand�٬Oweight�A��l�ȳ��O�s�A�]������:w=demand/capacity
			bound_demand[i][j].resize(4);
		}
	}

	//��Ҧ���u��demand�Mweight���]���L���j�A�o�ˤ~���|�W�XGrid���d��
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
		//�C�@���i�ӳ��n��short_dist�Mpred�k�s
		vector <int> shortest_dist;
		vector <int> pred;
		shortest_dist.resize(input_data.numGridx * input_data.numGridy);
		pred.resize(input_data.numGridx * input_data.numGridy);

		//Initial All the Value
		for (int j = 0; j < input_data.numGridx * input_data.numGridy; j++)  //�C�@�����s�����|�n�Q��Dijkstra�����ɭԳ��n��l��
			shortest_dist[j] = infinity;

		//�]��shortest_dist�Mpred�O�@�����A�ҥH�۹��m�������N�O�G����y_value*4+x_value
		shortest_dist[input_data.coord_s[i][0] + input_data.numGridx * input_data.coord_s[i][1]] = 0;   //source���̵u���|�n�]��0
		pred[input_data.coord_s[i][0] + input_data.numGridx * input_data.coord_s[i][1]] = infinity;     //���I���e�@��element�]�n��l�Ʀ��L���j�A����i�H���P�_
		int x = input_data.coord_s[i][0], y = input_data.coord_s[i][1];
		vector <bool> tile_stack;
		tile_stack.resize(input_data.numGridx * input_data.numGridy);
		
		
		for (int a = 0; a < input_data.numGridx * input_data.numGridy; a++)  //�`�@�n�P�_ #tile size ������(grid_x * grid_y)
		{
		Loop1:	//�p�G�J��|�P���w�g���L��tile�N�|�q�o��}�l

			//����W�U���k���@��weight����p
			int temp_array[4] = { 0, 0, 0, 0 };
			int temp_index, temp_x, temp_y;


			//���U���椧�e���ˬd�O���O�w�g���L�F
			check_is_pass(x, y, temp_array, tile_bound_weight, input_data, tile_stack);

			//�p�G�ثe��value����쥻��value�p�A�N��Ӧ�m(tile)��value�令�L�ثe���̤pweight
			Shortdist_Pred shortdist_pred = check_min_weight(x, y, tile_bound_weight, shortest_dist, input_data, pred, temp_array, tile_stack);
			shortest_dist = shortdist_pred.shortest_dist;
			pred = shortdist_pred.pred;

			//�{�b��temp_array�N�O�|�P����s�L�᪺tile
			if (tile_bound_weight[x][y][0] != infinity)	temp_array[0] += shortest_dist[input_data.numGridx * (y + 1) + x];//�T�w�Ӥ�V���O���
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
				break;	//�p�Gtile_stack���Ҧ���tile���w�g���L�F�~�|�]��o��A���N�N�����F
			}
			if (temp_index == 0) temp_x = x, temp_y = y + 1;		//�W
			else if (temp_index == 1) temp_x = x, temp_y = y - 1;	//�U
			else if (temp_index == 2) temp_x = x - 1, temp_y = y;	//��
			else temp_x = x + 1, temp_y = y;						//�k
			tile_stack[input_data.numGridx * y + x] = true;
			x = temp_x, y = temp_y;
		}
		//�P�_������n��ڨ��L�ðO���btile_bound_weight�Mbound_demand�o����ܼƤ��Ademand�n���V���[�A�]��vector�O���}��
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
		} while (terminal != input_data.numGridx * input_data.coord_s[i][1] + input_data.coord_s[i][0]);	//����_�I���ɭԴNbreak
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
		
		// �g�ɨ�output file
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


		//��stile_bound_weight�Mbound_demand�o����ܼ�
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
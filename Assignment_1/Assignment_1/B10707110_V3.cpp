#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;
const int buffsize = 10000;
vector < vector<int> > MIJ;         //create the dynamic variable named MIJ to store the # of chord
int num_buffer[buffsize];
vector < vector < vector<int> > > trace_MIJ;   //the diagram to store one of the number on the chord we got in MIJ
vector < int > trace_buffer;        //to store the optimal subproblem


int main(int argc, char** argv)
{
    /*
    if (argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }
*/
    // open the input file
    fstream fin;
    fin.open("./5000.in", fstream::in);//argv[1]
    if (!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    char buffer[100];

    fin >> buffer;
    int numChord = atoi(buffer);

    cout << "#Chords:" << numChord << endl;


    MIJ.resize(numChord);
    trace_MIJ.resize(numChord);
    for (int i = 0; i < numChord; ++i)
    {
        MIJ[i].resize(numChord);
        trace_MIJ[i].resize(numChord);
    }

    bool first_or_not[5000];       //store the element on the chord is first or not numChord
    for (int i = 0; i < numChord / 2; ++i)   //store the input file data in num_buffer which is 1-d array
    {
        fin >> buffer;
        int point1 = atoi(buffer);
        fin >> buffer;
        int point2 = atoi(buffer);

        first_or_not[point1] = true;
        first_or_not[point2] = false;

        num_buffer[point1] = point2;
        num_buffer[point2] = point1;
    }


    for (int i = 0; i < numChord; ++i)
    {
        MIJ[i][i] = 0;            //CASE 0 : MIJ[i][i] = 0
        // trace_MIJ[i][i].push_back();
    }

    for (int i = 1; i < numChord; i++)      //i represent the gap of original i and j, eg.[4, 10] --> i=10-4=6
    {
        for (int j = i; j < numChord; j++)  //j represent the array element j
        {
            int k = num_buffer[j];
            if ((k > (j - i) && k > j) || (k < (j - i) && k < j))   //CASE 1 : MIJ(i, j-1) if k is not in [i, j]
            {
                MIJ[j - i][j] = MIJ[j - i][j - 1];
                for (int l = 0; l < trace_MIJ[j - i][j - 1].size(); l++)
                    trace_MIJ[j - i][j].push_back(trace_MIJ[j - i][j - 1][l]);
            }

            if (k > (j - i) && k < j)                             //CASE 2 : max(MIJ(i, j-1), MIJ(i, k-1)+1+MIJ(k+1, j-1)) if k is in [i, j]
            {
                if (MIJ[j - i][j - 1] > (MIJ[j - i][k - 1] + 1 + MIJ[k + 1][j - 1]))
                {
                    MIJ[j - i][j] = MIJ[j - i][j - 1];
                    for (int l = 0; l < trace_MIJ[j - i][j - 1].size(); l++)
                        trace_MIJ[j - i][j].push_back(trace_MIJ[j - i][j - 1][l]);
                }

                else
                {
                    MIJ[j - i][j] = MIJ[j - i][k - 1] + 1 + MIJ[k + 1][j - 1];
                    for (int l = 0; l < trace_MIJ[j - i][k - 1].size(); l++)
                        trace_MIJ[j - i][j].push_back(trace_MIJ[j - i][k - 1][l]);

                    for (int l = 0; l < trace_MIJ[k + 1][j - 1].size(); l++)
                        trace_MIJ[j - i][j].push_back(trace_MIJ[k + 1][j - 1][l]);

                    if (first_or_not[k] == true)
                        trace_MIJ[j - i][j].push_back(k);
                    else trace_MIJ[j - i][j].push_back(j);
                }
            }

            if (k == j || k == (j - i))                           //CASE 3 : MIJ(i+1, j-1) + 1 if ij is a Chord
            {
                MIJ[j - i][j] = MIJ[j - i + 1][j - 1] + 1;
                for (int l = 0; l < trace_MIJ[j - i + 1][j - 1].size(); l++)
                    trace_MIJ[j - i][j].push_back(trace_MIJ[j - i + 1][j - 1][l]);

                if (first_or_not[j - i] == true)
                    trace_MIJ[j - i][j].push_back(j - i);
                else trace_MIJ[j - i][j].push_back(j);
            }
        }
    }


    // open the output file
    fstream fout;
    fout.open("./5000.out", fstream::out);//argv[2]
    if (!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        exit(1);
    }

    // output something to the output file
    fout << MIJ[0][numChord - 1] << endl;
    sort(trace_MIJ[0][numChord - 1].begin(), trace_MIJ[0][numChord - 1].end());

    for (int i = 0; i < trace_MIJ[0][numChord - 1].size(); i++)
    {
        fout << trace_MIJ[0][numChord - 1][i] << ' ' << num_buffer[trace_MIJ[0][numChord - 1][i]] << endl;
        cout << trace_MIJ[0][numChord - 1][i] << ' ' << num_buffer[trace_MIJ[0][numChord - 1][i]] << endl;
    }

    system("pause");
    return 0;
}
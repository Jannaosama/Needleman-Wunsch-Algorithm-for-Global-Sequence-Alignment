#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 

using namespace std;
//reverse of the resulted sequence
string reverse(string s)
{
    string str = "";
    for (int i = s.length()-1; i >= 0;i--)
    {
        str += s[i];
    }

    return str;
}
// max of 3 numbers
int max(int a, int b, int c)
{

    if (a > b)
        if (a > c)
            return a;
        else
            return c;
    else
        if (c > b)
            return c;
        else
            return b;
}
// intialize grid
void intialize(int** grid, int size1, int size2 , int gap) {
    
    int t = gap;
    for (int i = 0; i < size2 + 1; i++)
    {
        int* row = new int[size1 + 1];
        int k = 0;
        for (int j = 0; j < size1 + 1; j++)
        {
            if (i == 0)
            {
                row[j] = k;
                k = k + gap;
            }
            else if (j == 0)
            {
                row[j] = t;
                t = t + gap;
            }
            else
                row[j] = 0;
        }
        grid[i] = row;
    }

}

// evaluating Grid
void evaluategrid(int ** grid, int size1, int size2 , string seq1 , string seq2, int match, int mismatch, int gap)
{
    for (int i = 1; i < size2 + 1; i++)
    {
        for (int j = 1; j < size1 + 1; j++)
        {
            if (seq1[j - 1] == seq2[i - 1])
                grid[i][j] = match + grid[i - 1][j - 1];
            else
            {
                grid[i][j] = max(grid[i - 1][j - 1] + mismatch, grid[i - 1][j] + gap, grid[i][j - 1] + gap);
            }
        }
    }
}
// print grid
void printgrid(int** grid, int size1, int size2)
{
    for (int i = 0; i < size2 + 1; i++)
    {
        for (int j = 0; j < size1 + 1; j++)
        {
            cout << setw(5) << grid[i][j] << " ";
        }
        cout << endl;
    }
  
}
// parsing the file
void parsingfile(string filename, string seq[] , int size,int& Match,int& Mismatch,int& Gap)
{
    ifstream inputfile(filename);
    if (!inputfile.is_open())
    {
        cout << "Can not open file!" << endl;
        return;
    }
    string line;
    int i = 0;
    while (getline(inputfile,line))
    {
        if (i < 2)
        {
            //cout << i << ": " <<line << endl;
            int spaceindex = line.find(' ');
            if (spaceindex == -1)
            {
                //cout << "Incorrect file format!" << endl;
                return;
            }
            seq[i] = line.substr(spaceindex + 1);

        }
        else
        {
            int spaceindex = line.find(' ');
            switch(i)
            {
            case 2: Match = stoi(line.substr(spaceindex + 1));
                    break;
            case 3: Mismatch = stoi(line.substr(spaceindex + 1));
                    break;
            case 4: Gap = stoi(line.substr(spaceindex + 1));
                    break;

            }
        }
        i++;

        if (inputfile.eof())
            break;

    }
}
//alignment
void alignment(int** grid, int size1, int size2 , string seq[], int seqsize, string &res1 , string &res2)
{
    int i = size2;
    int j = size1;
    string res_seq1 = "";
    string res_seq2 = "";
    while (j != 0 && i != 0)
    {
        bool flagmatch = false;
        if (grid[i - 1][j - 1] >= grid[i - 1][j])
        {
            if (grid[i - 1][j - 1] >= grid[i][j - 1])
            {
                // match
                flagmatch = true;
                res_seq1 += seq[0][j - 1];
                res_seq2 += seq[1][i - 1];
                i--;
                j--;
            }

        }
        else if (i != 0 && j != 0)
        {
            if (seq[0][j - 1] == seq[1][i - 1])
            {
                flagmatch == true;
                res_seq1 += seq[0][j - 1];
                res_seq2 += seq[1][i - 1];
                i--;
                j--;
            }
        }
        if (!flagmatch)
        {
            if (grid[i][j - 1] <= grid[i - 1][j])
            {
                res_seq1 += '_';
                res_seq2 += seq[1][i - 1];
                i--;
            }
            else
            {
                res_seq1 += seq[0][j - 1];
                res_seq2 += '_';
                j--;
            }
        }

    }
   
    res1 = reverse(res_seq1);
    res2= reverse(res_seq2);
    return ;
}
int main()
{
    string filename;
    cout << "Enter the file name" << endl;
    cin >> filename;
    string seq[2];
    int gap, match, mismatch;
    string res_seq1;
    string res_seq2;
    parsingfile(filename, seq, 2, match, mismatch, gap);

    int** grid;
    //check larger seq and swap if needed
    if (seq[0].length() > seq[1].length())
    {
        seq[0].swap(seq[1]);
    }

    cout << seq[0] << endl;
    cout << seq[1] << endl;
    // intializing sizes
    int size1 = seq[0].length(); // size of first seq
    int size2 = seq[1].length(); // size of second seq
    grid = new int* [size2 + 1];

    //initialize grid
    intialize(grid, size1, size2, gap);

    // printing grid
    cout << "Grid Intialization\n";
    printgrid(grid, size1, size2);
    cout << endl;

    //Evaluating Grid
    evaluategrid(grid, size1, size2, seq[0], seq[1], match, mismatch, gap);

    // printing grid
    cout << "After Evaluation\n";
    printgrid(grid, size1, size2);
    cout << endl;
    cout << " The score of this alignment = " << grid[size2][size1] << endl;

    //alignment 
    cout << "Sequence Alignment \n";
    alignment(grid, size1, size2, seq, 2 , res_seq1 , res_seq2);

   cout<< "Sequence 1:" << res_seq1 << endl;
   cout<< "Sequence 2:" << res_seq2 << endl;
   return 0;
}

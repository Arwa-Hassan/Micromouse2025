#include <iostream>
#include<queue>
using namespace std;
int main()
{
    int n;
    n=16;
    int arr[n][n];
    char obs_ver[n][n-1];
    char obs_horz[n-1][n];
    bool frq[n][n];
    for(int c=0; c<n-1; ++c)
    {
        for(int v=0; v<n; ++v)
            obs_horz[c][v]=' ';
    }
    for(int c=0; c<n; ++c)
    {
        for(int v=0; v<n-1; ++v)
            obs_ver[c][v]=' ';
    }
    string obs_ver_maze[n]={
               "|                               |",
               "| |                             |",
               "| | |                           |",
               "| | | |     |       | |   |     |",
               "| | |       |       | | | | | | |",
               "| | | |     |         | | |   | |",
               "| | | | | | |       | | | |   | |",
               "| | | |   | | |   | | | | |   | |",
               "| | | |     |     | |   |   | | |",
               "| | | | |   |       | |   |   | |",
               "| | | |   | | |   |   | |   | | |",
               "| | | | | | |   |   | |       | |",
               "| | | |     |       | |   |   | |",
               "|   | |                 |   | | |",
               "| | |                         | |",
               "| |                             |"};
    string obs_horz_maze[n-1]={
                              "+ +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ +",
                              "+ + +-+-+-+-+-+-+-+-+-+-+-+-+-+ +",
                              "+ + + + +-+-+ +-+-+-+-+-+-+-+-+ +",
                              "+ + + +-+-+ +-+-+-+ + + + + + + +",
                              "+ + + +-+ +-+ +-+-+-+ + + + +-+ +",
                              "+ + + + + +-+-+-+-+-+ + + +-+ + +",
                              "+ + + + +-+ + +-+-+ + + + + +-+ +",
                              "+ + + +-+ + + + + + + + + +-+ + +",
                              "+ + + + +-+ + +-+-+ + + +-+ + + +",
                              "+ + + +-+ + +-+-+ +-+-+ + +-+ + +",
                              "+ + + + + +-+ + +-+ + +-+-+ +-+ +",
                              "+ + + + +-+ + +-+ +-+ +-+-+-+ + +",
                              "+ + + +-+ +-+-+-+-+-+ + + + + + +",
                              "+ + + +-+-+-+-+-+-+-+-+-+-+-+ + +",
                              "+ + +-+-+-+-+-+-+-+-+-+-+-+-+-+ +"
                              };
    for(int c=0;c<n;++c)
    {
        for(int v=0;v<obs_ver_maze[c].size();++v)
        {
            if(v%2==0&&v!=0&&v!=obs_ver_maze[c].size()-1)
            obs_ver[c][(v-2)/2]=obs_ver_maze[c][v];
        }
    }
    for(int c=0;c<n-1;++c)
    {
        for(int v=0;v<obs_horz_maze[c].size();++v)
        {
            char chr=obs_horz_maze[c][v];
            if(chr=='-')
                chr='_';
            if(v%2==1)
            obs_horz[c][v/2]=chr;
        }
    }
    for(int c=0; c<n; ++c)
    {
        for(int v=0; v<n; ++v)
        {
            frq[c][v]=0;
        }
    }
    queue<pair<int,int>>myque;
    int checked;
        arr[n/2][n/2]=0;
        arr[n/2-1][n/2-1]=0;
        arr[n/2-1][n/2]=0;
        arr[n/2][n/2-1]=0;
        frq[n/2][n/2]=1;
        frq[n/2-1][n/2-1]=1;
        frq[n/2-1][n/2]=1;
        frq[n/2][n/2-1]=1;
        checked=4;
        myque.push({n/2,n/2});
        myque.push({n/2-1,n/2-1});
        myque.push({n/2,n/2-1});
        myque.push({n/2-1,n/2});
    while(checked<n*n&!myque.empty())
    {
        if(frq[myque.front().first+1][myque.front().second]==0&&
                myque.front().first+1<n&&
           (obs_horz[myque.front().first][myque.front().second]==' '))
        {
            arr[myque.front().first+1][myque.front().second]=
                arr[myque.front().first][myque.front().second]+1;
            frq[myque.front().first+1][myque.front().second]=1;
            myque.push({myque.front().first+1,myque.front().second});
            ++checked;
        }
        if(frq[myque.front().first][myque.front().second+1]==0&&
                myque.front().second+1<n&&
                obs_ver[myque.front().first][myque.front().second]==' ')
        {
            arr[myque.front().first][myque.front().second+1]=
                arr[myque.front().first][myque.front().second]+1;
            frq[myque.front().first][myque.front().second+1]=1;
            myque.push({myque.front().first,myque.front().second+1});
            ++checked;
        }
        if(frq[myque.front().first][myque.front().second-1]==0&&
                myque.front().second-1>=0&&
                obs_ver[myque.front().first][myque.front().second-1]==' ')
        {
            arr[myque.front().first][myque.front().second-1]=
                arr[myque.front().first][myque.front().second]+1;
            frq[myque.front().first][myque.front().second-1]=1;
            myque.push({myque.front().first,myque.front().second-1});
            ++checked;
        }
        if(frq[myque.front().first-1][myque.front().second]==0&&
                myque.front().first-1>=0&&
           obs_horz[myque.front().first-1][myque.front().second]==' ')
        {
            arr[myque.front().first-1][myque.front().second]=
                arr[myque.front().first][myque.front().second]+1;
            frq[myque.front().first-1][myque.front().second]=1;
            myque.push({myque.front().first-1,myque.front().second});
            ++checked;
        }
        myque.pop();
    }
    cout<<"Maze shape\n";
    for(int c=0; c<n; ++c)
    {
        if(c==0)
        {
            cout<<"_";
            for(int v=0; v<n; ++v)
            {
                cout<<"__";
            }
            cout<<"\n";
        }
        for(int v=0; v<n; ++v)
        {
            if(v==0)
            cout<<'|';
            if(v<n-1)
            {
            if(c<n-1)
            cout<<obs_horz[c][v]<<obs_ver[c][v];
            else
            cout<<" "<<obs_ver[c][v];
            }
            if(v==n-1)
            {
            if(c<n-1)
            cout<<obs_horz[c][v]<<'|';
            else
            cout<<" "<<'|';
            }
        }
        cout<<"\n";
        if(c==n-1)
        {
            cout<<"-";
            for(int v=0; v<n; ++v)
            {
                cout<<"--";
            }
        }
    }
    cout<<"\n"<<"solution"<<"\n";
    for(int c=0; c<n; ++c)
    {
        for(int v=0; v<n; ++v)
        {
            cout<<arr[c][v]<<" ";
        }
        cout<<"\n";
    }
    return 0;
}

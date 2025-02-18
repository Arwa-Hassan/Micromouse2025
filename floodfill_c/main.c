#include <stdio.h>
#include <stdlib.h>
struct point
{
    int first;// row
    int second;// column
};
int main()
{
    int n;
    printf("enter the size of maze");
    scanf("%d",&n);
    int arr[n][n];
    char frq[n][n];
    int start=0;
    int end=0;
    struct point myque[n*n];
    struct point stepstaken[n*n];
    for(int c=0; c<n; ++c)//  micro seconds
    {
        for(int v=0; v<n; ++v)
            frq[c][v]=0;
    }
    char obs_ver[n][n-1];
    char obs_horz[n-1][n];
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
    obs_ver[0][0]='|';
    obs_ver[n/2][n/2]='|';
    obs_horz[n/2-1][n/2]='_';
    obs_ver[n/2][n/2-1]='|';
    obs_ver[n-1][0]='|';
    obs_ver[n-2][0]='|';
    obs_ver[n-3][0]='|';
    obs_horz[n/2][n/2-1]='_';
    obs_horz[n/2][n/2+1]='_';
    obs_ver[n/2][n/2+1]='|';
    int checked;
    if(n%2==1)
    {
        arr[n/2][n/2]=0;
        frq[n/2][n/2]=1;
        checked=1;
        myque[end].first=n/2;
        myque[end].second=n/2;
        ++end;
    }
    else
    {
        arr[n/2][n/2]=0;
        arr[n/2-1][n/2-1]=0;
        arr[n/2-1][n/2]=0;
        arr[n/2][n/2-1]=0;
        frq[n/2][n/2]=1;
        frq[n/2-1][n/2-1]=1;
        frq[n/2-1][n/2]=1;
        frq[n/2][n/2-1]=1;
        checked=4;
        myque[end].first=n/2;
        myque[end].second=n/2;
        ++end;
        myque[end].first=n/2-1;
        myque[end].second=n/2-1;
        ++end;
        myque[end].first=n/2-1;
        myque[end].second=n/2;
        ++end;
        myque[end].first=n/2;
        myque[end].second=n/2-1;
        ++end;
    }
    while(checked<n*n&&start<end)
    {
        if(frq[myque[start].first+1][myque[start].second]==0&&
           myque[start].first+1<n&&
           obs_horz[myque[start].first][myque[start].second]==' ')
        {
            arr[myque[start].first+1][myque[start].second]=
            arr[myque[start].first][myque[start].second]+1;
            frq[myque[start].first+1][myque[start].second]=1;
            myque[end].first=myque[start].first+1;
            myque[end].second=myque[start].second;
            ++end;
            ++checked;
        }
        if(frq[myque[start].first-1][myque[start].second]==0&&
           myque[start].first-1>=0
           &&obs_horz[myque[start].first-1][myque[start].second]==' ')
        {
            arr[myque[start].first-1][myque[start].second]=
            arr[myque[start].first][myque[start].second]+1;
            frq[myque[start].first-1][myque[start].second]=1;
            myque[end].first=myque[start].first-1;
            myque[end].second=myque[start].second;
            ++end;
            ++checked;
        }
        if(frq[myque[start].first][myque[start].second-1]==0&&
           myque[start].second-1>=0&&
           obs_ver[myque[start].first][myque[start].second-1]==' ')
        {
            arr[myque[start].first][myque[start].second-1]=
            arr[myque[start].first][myque[start].second]+1;
            frq[myque[start].first][myque[start].second-1]=1;
            myque[end].first=myque[start].first;
            myque[end].second=myque[start].second-1;
            ++end;
            ++checked;
        }
        if(frq[myque[start].first][myque[start].second+1]==0&&
           myque[start].second+1<n&&
           obs_ver[myque[start].first][myque[start].second]==' ')
        {
            arr[myque[start].first][myque[start].second+1]=
            arr[myque[start].first][myque[start].second]+1;
            frq[myque[start].first][myque[start].second+1]=1;
            myque[end].first=myque[start].first;
            myque[end].second=myque[start].second+1;
            ++end;
            ++checked;
        }
        ++start;
    }
        printf("Maze shape\n");
    for(int c=0; c<n; ++c)
    {
        if(c==0)
        {
            for(int v=0; v<n; ++v)
            {
                printf("_________");
            }
            printf("\n");
        }
        for(int v=0; v<n; ++v)
        {
            if(v==0)
            printf("|");
            if(v<n-1)
            {
            if(c<n-1)
            printf("%c%c",obs_horz[c][v],obs_ver[c][v]);
            else
            printf(" %c",obs_ver[c][v]);
            }
            if(v==n-1)
            {
            if(c<n-1)
            printf("%c|",obs_horz[c][v]);
            else
            printf(" |");
            }
        }
        printf("\n");
        if(c==n-1)
        {
            for(int v=0;v<n;++v)
            {
                printf("_________");
            }
        }
    }
    printf("\nsolution\n");
    for(int c=0; c<n; ++c)
    {
        for(int v=0; v<n; ++v)
        {
            printf("%d ",arr[c][v]);
        }
        printf("\n");
    }
    return 0;
}
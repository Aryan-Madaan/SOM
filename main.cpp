#include <stdio.h>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#define lldd long double
#define rep(i,n) for(int i =0;i<n;i++)

//using statlog (Australian Credti Card Information) dataset

using namespace std;

default_random_engine engine;
uniform_real_distribution<lldd> dist(0.0,1.0);

class SOM{
public:
    
    vector<vector<vector<lldd> > > wd;
    vector<vector<lldd> > expl;
    vector<vector<lldd> > heatmap;
    int height;
    int width;
    int itr;
    int examples;
    int feature;
    lldd learn_rate;
    int max_itr;
    lldd radius;
    int initial_radius;
    lldd min_learn_rate;
    lldd theta;
    int choice;
    
    SOM()
    {
        printf("Enter 1 for Random Colormap and 2 for Credit Card Dataset\n");
        scanf("%d",&choice);
        if(choice == 1){
            printf("Enter Height of Map\n");
            scanf("%d",&height);
            printf("Enter Width of Map\n");
            scanf("%d",&width);
            printf("Enter Number of Training Iterations\n");
            scanf("%d",&itr);
            max_itr = itr;
            printf("Enter Number of Features\n");
            scanf("%d",&feature);
            printf("Enter Number of Training Examples\n");
            scanf("%d",&examples);
            printf("Enter Initial Learn Rate\n");
            scanf("%Lf",&learn_rate);
            printf("Enter Minimum Learn Rate\n");
            scanf("%Lf",&min_learn_rate);
            expl.resize(examples);
            rep(i,examples)
            {
                expl[i].resize(feature);
                rep(j,feature)
                {
                    expl[i][j] = dist(engine);
                }
            }
        }
        else if (choice == 2)
        {
            printf("Loading Credit Card Dataset File\n");
            loadFile(false,expl);
            printf("Loaded Credit Card Dataset File\n");
            printf("Enter Height of Map\n");
            scanf("%d",&height);
            printf("Enter Width of Map\n");
            scanf("%d",&width);
            printf("Enter Number of Training Iterations\n");
            scanf("%d",&max_itr);
            itr = max_itr;
            printf("Enter Initial Learn Rate\n");
            scanf("%Lf",&learn_rate);
            printf("Enter Minimum Learn Rate\n");
            scanf("%Lf",&min_learn_rate);
            //max_itr = itr;
//            expl.erase(expl.begin());
//            expl.pop_back();
            rep(j,expl.size())
            {
//                if(expl[j].size()==0)
//                {
//                    expl.erase(expl.begin()+ j);
//                }
//                else{
                printf(" i-> %d ",j);
                    rep(k,expl[j].size())
                    {
                        printf(" %Lf ", expl[j][k]);
                    }
                    printf("\n");
//                }
            }
            printf("\nFeature Size %ld \n",expl[0].size());
            printf("\nExample Size %ld \n",expl.size());
            feature = expl[0].size();
            examples = expl.size();
        };
        wd.resize(height);
        initial_radius = min(height,width)/2;
        rep(i,height)
        {
            wd[i].resize(width);
            rep(j,width)
            {
                wd[i][j].resize(feature);
                rep(k,feature)
                {
                    wd[i][j][k] = dist(engine);
                }
            }
        }
    }

     void loadFile(bool header,vector<vector<lldd>>&content)
    {
        string fname = "/Users/aryanmadaan/Desktop/3-1/FoDs/Application.csv";
        
//        vector<vector<long double>> content;
        vector<long double> row;
        string line, word;
        vector<lldd> mxx(16,LLONG_MIN),mnn(16,LLONG_MAX);
        
        fstream file (fname, ios::in);
        if(file.is_open())
        {
            int i = 0;
            if(header)
                i = 1;
            else
                i = 0;

            while(getline(file, line))
            {
                row.clear();
                if(i){
                    i = 0;
                    continue;
                }

                stringstream str(line);
                int qq =0;
                while(getline(str, word, ','))
                {
                    printf(" %s ",word.c_str());
                    try{
                        if(qq!=0&&qq!=15){
                            row.push_back(std::stold(word));
                            mxx[qq] = max(mxx[qq],stold(word));
                            mnn[qq] = min(mnn[qq],stold(word));
                        }
                    }
                    catch(exception e){
                        continue;
                    }
                    qq++;
                }
                printf("\n");
                content.push_back(row);
                
            }
            rep(j,content.size())
            {
                rep(k,content[j].size())
                {
//                    if(k!=0&&k!=15){
                        content[j][k] /= (mxx[k+1]-mnn[k+1]);
//                    }
                }
            }
        }
        else
            cout<<"Could not open the file\n";
        // for(int i=0;i<content.size();i++)
        // {
        //     for(int j=0;j<content[i].size();j++)
        //         cout<<content[i][j]<<" ";
        //     cout<<"\n";
        // }
//        return content;
    }


    
    void learn()
    {
//        if(max_itr-itr > 20)
        learn_rate = learn_rate*exp(-((lldd)(max_itr-itr)/max_itr));
        radius = initial_radius*(1-exp(-((lldd)(max_itr-itr)/max_itr)));
        if(learn_rate<min_learn_rate)
        {
            learn_rate = min_learn_rate;
        }
        lldd min_dist = LLONG_MAX;
        int n_h = 0;
        int n_w = 0;
        rep(i,examples)
        {
            rep(j,height)
            {
                rep(k,width)
                {
                    if(min_dist>euclidean_dist(j,k,i))
                    {
                        min_dist = euclidean_dist(j,k,i);
                        n_h = j;
                        n_w = k;
                    }
                }
            }
//            cout << max(n_h-radius,0) << " " << min(height,n_h+radius) << " \n";
            for(int j = max((int)(n_h-radius),0);j<min(height,(int)(n_h+radius));j++)
            {
                for(int k = max((int)(n_w-radius),0);k<min(width,(int)(n_w+radius));k++)
                {
                    lldd dist2 = (n_h - j)*(n_h - j) + (n_w - k)*(n_w - k);
                    rep(l,feature)
                    {
                        if(radius>0)
                           theta = exp(-( dist2/(2*radius*radius)));
                        else
                            theta = 1.0;
                        wd[j][k][l] += theta*learn_rate*(expl[i][l] - wd[j][k][l]);
                    }
                }
            }
        }
    }
    
    lldd euclidean_dist(int&i,int&j,int&l)
    {
        lldd sum=0.0;
        rep(k,feature)
        {
            sum += pow((wd[i][j][k]-expl[l][k]),2);
        }
        return sqrt(sum);
    }
    lldd euclidean_dist2(int h,int j)
    {
        lldd sum=0.0;
        rep(k,feature)
        {
            sum += pow((expl[h][k]-expl[j][k]),2);
        }
        return sqrt(sum);
    }
    void train(int choice)
    {
        while(itr--)
        {
            printf("Current Iteration -> %d\n",itr+1);
            learn();
        }
        if(choice == 1)
        saveMap("colourmap.csv"); // Change File saving name before running otherwise it will append
        else if(choice == 2)
        saveMap("creditcard.csv"); // Change File saving name before running otherwise it will append
        
        create_heat_map();
        saveHeatMap("heatmap.csv"); // Change File saving name before running otherwise it will append
    }
    void create_heat_map()
    {
        heatmap.resize(height);
        rep(i,height)
        {
            heatmap[i].resize(width);
        }
        lldd mn=LLONG_MAX,mx=LLONG_MIN;
        rep(i,height)
        {
            rep(j,width)
            {
                heatmap[i][j] = euclidean_dist2(i,j);
                mn = min(heatmap[i][j],mn);
                mx = max(heatmap[i][j],mx);
            }
        }
        rep(i,height)
        {
            rep(j,width)
            {
                heatmap[i][j] /= (mx-mn);
            }
        }
    }
    void saveMap(string fname)
    {
        fstream fout;
        
        fout.open(fname, ios::out | ios::app);
        fout<<feature<<','<<height<<','<<width<<"\n";

        int t = 1;
        if(choice == 1)
        t = 255;

//        int i,j,k;
        rep(i,height)
        {
            rep(j,width)
            {
                rep(k,feature)
                {
                    if(j<width-1)
                        fout<<wd[i][j][k]*t<<',';
                    else{
                        if(k<feature-1)
                            fout<<wd[i][j][k]*t<<',';
                        else
                            fout<<wd[i][j][k]*t;
                    }
                }
            }
            fout<<'\n';
        }
       cout<<"\nHeat Map Saved\n";
    }
    void saveHeatMap(string fname)
    {
        fstream fout;
        fout.open(fname, ios::out | ios::app);
        fout<<height<<','<<width<<"\n";

//        int t = 1;
//        if(choice == 1)
//        t = 255;

//        int i,j,k;
        rep(i,height)
        {
            rep(j,width)
            {
                    if(j<width-1)
                        fout<<heatmap[i][j]<<',';
                    else{
                        fout<<heatmap[i][j];
                    }
            }
            fout<<'\n';
        }
       cout<<"\nMap Saved\n";
    }
};

int main()
{
    SOM s;
    s.train(s.choice);
}


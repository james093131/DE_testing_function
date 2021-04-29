#include<stdio.h>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#include<vector>
#include <algorithm>
#include<math.h>
#include<float.h>
#include <sys/stat.h>

using namespace std;

typedef vector<char> c1d;
typedef vector<c1d> c2d;
typedef vector<c2d> c3d;
typedef vector<c3d> c4d;
typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double>d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;


class DE{
    public:
        d1d Each_Run_Iteration_Best;
        d1d Each_Run_Result;
        double RUN_BEST;
        double RUN_AVG;
   
    public:
        void ALL(int run,int iteration,int pop,int DIM,const char *F,int OUTPUT_NODE_QUANTITY,double R_CR,double R_FACTOR)
        {   
            srand( time(NULL) );
            INI_RUN(iteration,run);
            int r = 0;
            double START,END;

            START = clock();
            while(r <run)
            {
                int ITER = 0 ;
                INI( iteration, DIM, pop, OUTPUT_NODE_QUANTITY,R_CR,R_FACTOR);
                for(int i= 0;i<pop;i++)
                {
                    INI_FUNCTION(DIM,i,F);
                }
                
                while(ITER<iteration)
                {
                    Evaluation(pop);


                    Mutation_Crossver(pop,DIM);


                    Selection(pop,DIM,F);


                    RANDOM_RECORD_NODE(OUTPUT_NODE_QUANTITY/iteration);
                    Each_Run_Iteration_Best[ITER] = Current_inf.Current_Best_Value;


                    if(CURRENT_RECORD_NODE%2000==0)
                        OUTPUT_RECORD_NODE(F,DIM,CURRENT_RECORD_NODE/20);
                    ITER++;
                } 

                Each_Run_Result[r] = Each_Run_Iteration_Best[iteration-1];

                r++;
            }
            END = clock();
            OUTPUT(run, START, END,iteration,pop,DIM,OUTPUT_NODE_QUANTITY,F);


            

        }

        
       
        
    private :

        double Scaling_Factor;
        double CR;
        double max;
        double min;
        int CURRENT_RECORD_NODE; //現在存了多少個點了

        struct Current_best
        {
            double Current_Best_Value;
            d1d Current_Best_Coordinate;        
        };
        
       
        struct DE_Parameter{
            d2d Position;
            d2d Vector;
            d1d Objecive_Value;
            
        };
        struct Record{
            d2d Record_Coordinate;
            d1d Objecive_Value;
        };

        struct OUTPUT_NODE
        {
            d2d NODE_Coordinate;
            d1d NODE_Objective_Value;
        };
        DE_Parameter De_inf;
        Record  Record_inf;
        Current_best Current_inf;
        OUTPUT_NODE Output_Node;
    private:
    void INI(int iteration,int DIM,int pop,int OUTPUT_NODE_QUANTITY,double RCR,double RFACTOR)
        {
            Output_Node.NODE_Coordinate.clear();
            Output_Node.NODE_Coordinate.swap(Output_Node.NODE_Coordinate);

            Output_Node.NODE_Objective_Value.clear();
            Output_Node.NODE_Objective_Value.swap(Output_Node.NODE_Objective_Value);  

            Current_inf.Current_Best_Coordinate.clear();
            Current_inf.Current_Best_Coordinate.swap(Current_inf.Current_Best_Coordinate);

            De_inf.Position.clear();
            De_inf.Position.swap(De_inf.Position);

            De_inf.Vector.clear();
            De_inf.Vector.swap(De_inf.Vector);

            
            De_inf.Objecive_Value.clear();
            De_inf.Objecive_Value.swap(De_inf.Objecive_Value);


            Record_inf.Objecive_Value.clear();
            Record_inf.Objecive_Value.swap(Record_inf.Objecive_Value);

            Record_inf.Record_Coordinate.clear();
            Record_inf.Record_Coordinate.swap(Record_inf.Record_Coordinate);

            De_inf.Position.assign(pop,d1d(DIM));
            De_inf.Vector.assign(pop,d1d(DIM));
            De_inf.Objecive_Value.resize(pop,DBL_MAX);

            Record_inf.Record_Coordinate.assign(OUTPUT_NODE_QUANTITY,d1d(DIM));
            Record_inf.Objecive_Value.resize(OUTPUT_NODE_QUANTITY);
            
            Current_inf.Current_Best_Coordinate.resize(DIM);
            Current_inf.Current_Best_Value = DBL_MAX;

            Output_Node.NODE_Objective_Value.resize(OUTPUT_NODE_QUANTITY);
            Output_Node.NODE_Coordinate.assign(OUTPUT_NODE_QUANTITY,d1d(DIM));

            Scaling_Factor = RFACTOR;
            CR = RCR;
                        
            CURRENT_RECORD_NODE = 0;

        }   
    void INI_RUN(int iteration,int run)
    {
        Each_Run_Iteration_Best.resize(iteration,0);
        Each_Run_Result.resize(run,DBL_MAX);
    }
    void Mutation_Crossver(int pop,int DIM)
    {
        for(int i=0;i<pop;i++)
        {
            for(int j=0;j<DIM;j++)
            {
                double x = (double) rand() / (RAND_MAX + 1.0);
                int x1 = rand() % ((pop-1) - 0 + 1) + 0;
                int x2 = rand() % ((pop-1) - 0 + 1) + 0;
                while(x1 ==x2)
                {
                    x2 = rand() % ((pop-1) - 0 + 1) + 0;
                }

                if(x<CR)
                    De_inf.Vector[i][j] = De_inf.Position[i][j] + Scaling_Factor*(De_inf.Position[x1][j]-De_inf.Position[x2][j]);
                else    
                    De_inf.Vector[i][j] = De_inf.Position[i][j];
            }
        }
    }
    void Selection(int pop,int DIM,const char* F)
    {
        for(int i= 0;i<pop;i++)
        {
            double FIT = FUNCTION(DIM,De_inf.Vector[i],F);
            if(FIT < De_inf.Objecive_Value[i])
            {
                De_inf.Position[i].assign(De_inf.Vector[i].begin(), De_inf.Vector[i].end());
                De_inf.Objecive_Value[i] = FIT;
            }

        }
    }
    void Evaluation(int pop)
    {
        for(int i=0;i<pop;i++)
        {   
            if(De_inf.Objecive_Value[i] < Current_inf.Current_Best_Value)
            {
                Current_inf.Current_Best_Value = De_inf.Objecive_Value[i];
                Current_inf.Current_Best_Coordinate.assign(De_inf.Position[i].begin(),De_inf.Position[i].end());
            }
        }
    }
    void FIND_AVG_BEST()
        {
            RUN_AVG = 0;
            RUN_BEST = Each_Run_Result[0];
            for(int i=0;i<Each_Run_Result.size();i++)
            {
                RUN_AVG += Each_Run_Result[i];
                if(Each_Run_Result[i] < RUN_BEST)
                    RUN_BEST = Each_Run_Result[i];
            }
            RUN_AVG = RUN_AVG / Each_Run_Result.size();
        }
    void OUTPUT(int run,double START,double END,int iteration,int pop,int DIM,int record_Node,const char *F)
    {
        FIND_AVG_BEST();
        for(int i=0;i<Each_Run_Iteration_Best.size();i++)
        {
            cout<<i+1<<' '<<Each_Run_Iteration_Best[i]/run<<endl;
        }

        fstream file;
        string FUN;
        if(F == std::string("A"))
             FUN = "Ackley";
        else if(F == std::string("R"))
            FUN = "Rastrigin";
        else if(F == std::string("Z"))
            FUN = "Zakharov";
        else if(F == std::string("B"))
            FUN = "Bent_Cigar";
        else if(F == std::string("RO"))
            FUN = "Rosenbrock";
        else if(F == std::string("S"))
            FUN = "Schaffer_F7";  
        cout<<"# Testing Function : "<<FUN<<endl;
        cout<<"# Run : "<<run<<endl;
        cout <<"# Iteration :"<<iteration<<endl;   
        cout<<"# Best Objective Value "<<RUN_BEST<<endl;
        cout<<"# Average Objective Value "<<RUN_AVG<<endl;
        cout<<"# Record Node "<<record_Node<<endl;
        cout<<"# Execution Time :"<<(END - START) / CLOCKS_PER_SEC<<"(s)"<<endl;

    
       
    };
        void print(i2d X)
        {
            for(int i=0;i<X.size();i++)
            {
                for(int j=0;j<X[i].size();j++)
                {
                    cout<<X[i][j]<<' ';
                }
                cout<<endl;
            }
        }
        void print(d2d X)
        {
            for(int i=0;i<X.size();i++)
            {
                for(int j=0;j<X[i].size();j++)
                {
                    cout<<X[i][j]<<' ';
                }
                cout<<endl;
            }
        }       
      
     
       

        //testing function
        void INI_FUNCTION(int DIM,int index,const char *F)
        {
            if(F == std::string("A"))
            {
                ACKLEY(DIM,index);
            }
            else if (F == std::string("R"))
                RASTRIGIN(DIM,index);
            else if(F ==std::string("B"))
                Bent_Cigar(DIM,index);
            else if(F ==std::string("Z"))
                Zakharov(DIM,index);
            else if(F ==std::string("RO"))
                ROSENBROCK(DIM,index);  
            else if(F ==std::string("S"))
                Schaffer_F7(DIM,index);  

        }
        double FUNCTION(int DIM,d1d arr,const char *F)
        {
            double R = 0.0;
            if(F == std::string("A"))
            {
                R = ACKLEY_OBJECTIVE_VALUE(DIM,arr);
            }
            else if (F == std::string("R"))
                R = RASTRIGIN_OBJECTIVE_VALUE(DIM,arr);
            else if (F == std::string("B"))
                R = Bent_Cigar_OBJECTIVE_VALUE(DIM,arr);
            else if (F == std::string("Z"))
                R = Zakharov_OBJECTIVE_VALUE(DIM,arr);
            else if (F == std::string("RO"))
                R = ROSENBROCK_OBJECTIVE_VALUE(DIM,arr);
            else if (F == std::string("S"))
                R = Schaffer_F7_OBJECTIVE_VALUE(DIM,arr);
            return R;
        }
        void ACKLEY(int DIM,int index) //random initial in ACKLEY Function and using RADVIZ calculate 2 dimension coordinates
        {
            max = 40;
            min = -40;
            double sum1= 0;
            double sum2 = 0;
            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;

                sum1 += pow(a,2);
                sum2 += cos(2*M_PI*a);

            }

            double F = ACKLEY_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }
        double ACKLEY_OBJECTIVE_VALUE(int DIM,d1d arr) //random initial in ACKLEY Function and using RADVIZ calculate 2 dimension coordinates
        {
            double sum1= 0;
            double sum2 = 0;
            for(int i=0;i<DIM;i++)
            {

                sum1 += pow(arr[i],2);
                sum2 += cos(2*M_PI*arr[i]);

            }
            double F = -20*(exp((-0.2)*sqrt(sum1/DIM)))-exp(sum2/DIM)+20+exp(1);
            return F;
        }
      
         double RASTRIGIN_OBJECTIVE_VALUE(int DIM,d1d arr)
        {
            double sum1= 0;
            double sum2 = 0;
            for(int i=0;i<DIM;i++)
            {

                sum1 += pow(arr[i],2);
                sum2 += cos(2*M_PI*arr[i]);

            }
            double F =  sum1 - 10*sum2 +10*DIM;
            return F;
        }
          
        
        void RASTRIGIN(int DIM,int index) //random initial in RASTRIGIN Function and using RADVIZ calculate 2 dimension coordinates
        {
            max = 5.12;
            min = -5.12;

            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;
            }
    
            double F = RASTRIGIN_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }
        double Bent_Cigar_OBJECTIVE_VALUE(int DIM,d1d arr)
        {
            double sum1= 0;
            double sum2 = 0;
            sum1 = pow(arr[0],2);
            for(int i=1;i<DIM;i++)
            {

                sum2 += pow(arr[i],2);

            }
            double F =  sum1 +1000000*sum2;
            return F;
        }
        void Bent_Cigar(int DIM,int index)
        {
            max = 100;
            min = -100;
      
            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;
            }

            double F = Bent_Cigar_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }
        double Zakharov_OBJECTIVE_VALUE(int DIM,d1d arr)
        {
            double sum1 = 0;
            double sum2 = 0;
            for(int i=0;i<DIM;i++)
            {
                sum1 += pow(arr[i],2);
                sum2 += 0.5*(i+1)*arr[i];

            }
            double F =  sum1 +pow(sum2,2)+pow(sum2,4);
            return F;
        }
        void Zakharov(int DIM,int index)
        {
            max = 10;
            min = -5;
      
            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;
            }

            double F = Zakharov_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }
        double ROSENBROCK_OBJECTIVE_VALUE(int DIM,d1d arr)
        {
            double sum1 = 0;
            double sum2 = 0;
            for(int i=1;i<DIM;i++)
            {
                sum1 += pow (arr[i] - pow(arr[i-1],2),2) ;
                sum2 += pow(arr[i]-1 ,2);
            // cout<<"S "<<sum1<<' '<<sum2<<endl;

            }
            double F =  100*sum1 +sum2;
            return F;
        }
        void ROSENBROCK(int DIM,int index)
        {
            max = 10;
            min = -5;
      
            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;
            }

            double F = ROSENBROCK_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }
          double Schaffer_F7_OBJECTIVE_VALUE(int DIM,d1d arr)
        {
            double F = 0;
            for(int i=0;i<DIM-1;i++)
            {
                double si = sqrt( pow(arr[i],2)+pow(arr[i+1],2) );
                double F1 = sqrt(si)* (sin(50*pow(si,0.2))+1);
                F += pow(F1/(DIM-1) , 2);

            }
            
            return F;
        }
        void Schaffer_F7(int DIM,int index)
        {
            max = 100;
            min = -100;
      
            for(int i=0;i<DIM;i++)
            {
                double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                De_inf.Position[index][i] = a;
            }

            double F = Schaffer_F7_OBJECTIVE_VALUE(DIM,De_inf.Position[index]);
            De_inf.Objecive_Value[index] = F;
        }




        void MAKE_GIF(const char *F,int DIM,int iter)
        {
            fstream file;
            if(iter == 0)
            {
                string Z = "GIF/PLT_AND_TXT/";
                string K  = Z + F + to_string(DIM);
                const char* dirname = K.c_str();
                mkdir( dirname,0777);
            }
           
            else if(iter %100 == 0)
            {
                string Z = "GIF/PLT_AND_TXT/";
                string A = Z+ F + to_string(DIM)+"/"+F+to_string(DIM)+to_string(iter)+".txt";
                file.open(A,ios::out);
               
                for(int i=0;i<CURRENT_RECORD_NODE;i++)
                {
                   for(int j=0;j<Output_Node.NODE_Coordinate[i].size();j++)
                   {
                        file << Output_Node.NODE_Coordinate[i][j]<<' ';
                   }

                   file<<Output_Node.NODE_Objective_Value[i]<<endl;
                }
            }
           
        }

        void RANDOM_RECORD_NODE(int EACH_Iteration_Record)
        {
            int len = De_inf.Position.size();
            i1d index(len,0);
            for(int i=0;i<len;i++)
            {
                index[i] = i;
            }   

            i1d CHOOSE(EACH_Iteration_Record,0);
            int k = 0;
            while(k != CHOOSE.size())
            {
                  int x = rand() % ( (index.size()-1) - 0 + 1) + 0;
                  if(index[x]!=-1)
                  {
                    CHOOSE[k] = index[x];
                    index[x] = -1;
                    k++;
                  }
            }

            for(int i=0;i<CHOOSE.size();i++)
            {
                Output_Node.NODE_Coordinate[CURRENT_RECORD_NODE].assign(De_inf.Position[CHOOSE[i]].begin(),De_inf.Position[CHOOSE[i]].end());
                Output_Node.NODE_Objective_Value[CURRENT_RECORD_NODE] = De_inf.Objecive_Value[CHOOSE[i]];
                CURRENT_RECORD_NODE++;
            }   

        }


        void OUTPUT_RECORD_NODE(const char *F,int DIM,int iter)
        {
            fstream file;
           
            string Z = "RESULT/";
            string A = Z+F+to_string(DIM)+to_string(iter)+".txt";
            file.open(A,ios::out);
            
            for(int i=0;i<CURRENT_RECORD_NODE;i++)
            {
                for(int j=0;j<Output_Node.NODE_Coordinate[i].size();j++)
                {
                    file << Output_Node.NODE_Coordinate[i][j]<<' ';
                }

                file<<Output_Node.NODE_Objective_Value[i]<<endl;
            }
        
           
        }
};
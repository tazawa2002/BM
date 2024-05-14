#include "bm.h"

// BMクラスのコンストラクタ
BM::BM(int n){
    int i,j;
    N = n;
    totalStates = pow(2,N);
    X = (int*)malloc(sizeof(int)*N);
    b = (double*)malloc(sizeof(double)*N);
    w = (double**)malloc(sizeof(double*)*N);
    p_distr = (double*)malloc(sizeof(double)*totalStates);
    histgram = (int*)malloc(sizeof(int)*totalStates);
    traindatanum = 0;

    std::random_device rd;
    gen = std::mt19937(rd()); // 乱数生成器の初期化
    for(i=0;i<N;i++){
        w[i] = (double*)malloc(sizeof(double)*N);
        X[i] = 0;
        b[i] = (random_num()>0.5) ? 1 : -1;
    }
    for(i=0;i<N;i++){
        for(j=i;j<N;j++){
            if(i==j){
                w[i][j] = 0;
            }else{
                int t = (random_num()>0.5) ? 1 : -1;
                w[i][j] = t;
                w[j][i] = t;
            }
            
        }
    }
    learn_rate = 0.01;
    
}

BM::~BM(){
    int i;
    free(X);
    free(b);
    for(i=0;i<N;i++){
        free(w[i]);
    }
    free(w);
    free(p_distr);
    free(histgram);
    if(traindatanum!=0){
        for(i=0;i<traindatanum;i++){
            free(traindata[i]);
        }
        free(traindata);
    }
}

// ボルツマンマシンのエネルギーを計算する関数
double BM::energy_calc(){
    int i,j;
    double energy = 0;
    for(i=0;i<N;i++){
        for(j=i+1;j<N;j++){
            energy -= w[i][j]*X[i]*X[j];
        }
        energy -= b[i]*X[i];
    }
    return energy;
}

// ボルツマンマシンの1つのノードに対する周囲のノードからの影響
double BM::lambda(int i){
    int j;
    double lambda = b[i];
    for(j=0;j<N;j++){
        lambda += w[i][j]*X[j];
    }
    return lambda;
}

double BM::random_num(){
    return dis(gen);
}

// シグモイド関数
double BM::sig(double x){
    double exp_val;
    if(x>0){
        return 1 / ( 1 + exp(-x) );
    }else{
        exp_val = exp(x);
        return exp_val / ( 1 + exp_val );
    }
}

// BMの確率分布を計算する関数
void BM::p_distr_calc(){
    int i,j;
    double Z = 0;

    for(i=0;i<totalStates;i++){
        setX(i);
        p_distr[i] = exp(-energy_calc());
    }
    for(i=0;i<totalStates;i++){
        Z += p_distr[i];
    }
    for(i=0;i<totalStates;i++){
        p_distr[i] = p_distr[i] / Z;
    }
}

// 現在の状態を2進数で返す関数
int BM::x_num(){
    int i;
    int num = 0;
    for(i=0;i<N;i++){
        num += pow(2,i)*X[i];
    }
    return num;
}

// BMの状態をgibbsサンプリングで更新する関数
int* BM::update(){
    int i;
    double p, u;
    for(i=0;i<N;i++){
        p = sig(lambda(i));
        u = random_num();
        if(p >= u){
            X[i] = 1;
        }else{
            X[i] = 0;
        }
    }
    return X;
}

// 指定した回数サンプリングを行う関数
void BM::sampling(int n){
    int i,j;
    for(i=0;i<totalStates;i++){
        histgram[i] = 0;
    }

    for(i=0;i<1000;i++){
        update();
    }

    for(i=0;i<n;i++){
        for(j=0;j<10;j++){
            update();
        }
        // cout << X[0] << " " << X[1] << " " << X[2] << " " << X[3] << endl;
        histgram[x_num()] += 1;
    }
}

void BM::dataGen(int num){
    int i,j;
    FILE *datafile;
    datafile = fopen("./data/data.dat", "w");

    for(i=0;i<1000;i++){
        update();
    }

    for(i=0;i<num;i++){
        for(j=0;j<10;j++) update();
        fprintf(datafile, "%d\n", x_num());
        histgram[x_num()] += 1;
    }
    fclose(datafile);
}

void BM::dataRead(int num){
    int i, j, x;
    FILE *datafile;
    traindatanum = num;
    traindata = (int**)malloc(sizeof(int*)*num);
    for(i=0;i<num;i++){
        traindata[i] = (int*)malloc(sizeof(int)*N);
    }
    datafile = fopen("./data/data.dat", "r");
    for(i=0;i<num;i++){
        fscanf(datafile, "%d", &x);
        setX(x);
        for(j=0;j<N;j++){
            traindata[i][j] = X[j];
        }
    }
    fclose(datafile);
}

void BM::train(){
    int i,j,k;
    double x_ave_model;
    double x_ave_data;
    double xx_ave_model;
    double xx_ave_data;
    double gradient = 10;
    double *gradient_b;
    double **gradient_w;
    gradient_b = (double*)malloc(sizeof(double)*N);
    gradient_w = (double**)malloc(sizeof(double*)*N);
    for(i=0;i<N;i++){
        gradient_w[i] = (double*)malloc(sizeof(double)*N);
    }

    p_distr_calc();
    while(gradient>0.0001){
        for(i=0;i<N;i++){

            // xのデータ平均を求める処理
            x_ave_data = 0;
            for(j=0;j<traindatanum;j++){
                x_ave_data += traindata[j][i];
            }
            x_ave_data /= traindatanum;

            // xのモデル平均
            x_ave_model = 0;
            for(j=0;j<totalStates;j++){
                x_ave_model += p_distr[j]*((j>>i)&1);
            }
            gradient_b[i] = x_ave_data - x_ave_model;

            for(j=i+1;j<N;j++){
                // xxのデータ平均を求める処理
                xx_ave_data = 0;
                for(k=0;k<traindatanum;k++){
                    xx_ave_data += traindata[k][i]*traindata[k][j];
                }
                xx_ave_data /= traindatanum;
                // xxのモデル平均
                xx_ave_model = 0;
                for(k=0;k<totalStates;k++){
                    xx_ave_model += p_distr[k]*((k>>i)&1)*((k>>j)&1);
                }
                gradient_w[i][j] = xx_ave_data - xx_ave_model;
                gradient_w[j][i] = gradient_w[i][j];
            }
        }

        // パラメータの更新と勾配の計算
        gradient = 0;
        for(i=0;i<N;i++){
            gradient += gradient_b[i]*gradient_b[i];
            b[i] += learn_rate*N*gradient_b[i];
        }
        for(i=0;i<N;i++){
            for(j=i+1;j<N;j++){
                gradient += gradient_w[i][j]*gradient_w[i][j];
                w[i][j] += learn_rate*N*gradient_w[i][j];
                w[j][i] = w[i][j];
            }
        }
        gradient = sqrt(gradient);
        cout << "\r" << gradient;
        p_distr_calc();
    }
    cout << endl;
    for(i=0;i<N;i++){
        free(gradient_w[i]);
    }
    free(gradient_w);
    free(gradient_b);
}

void BM::setX(int num){
    int i;
    for(i=0;i<N;i++){
        X[i] = (num >> i) & 1;
    }
}
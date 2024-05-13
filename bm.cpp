#include "bm.h"

// BMクラスのコンストラクタ
BM::BM(int n){
    int i,j;
    N = n;
    time = pow(2,N);
    X = (int*)malloc(sizeof(int)*N);
    b = (double*)malloc(sizeof(double)*N);
    w = (double**)malloc(sizeof(double*)*N);
    p_distr = (double*)malloc(sizeof(double)*time);
    histgram = (int*)malloc(sizeof(int)*time);
    traindatanum = 0;
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

// ボルツマンマシンの1つのノードに対する
int BM::lambda(int i){
    int j;
    int lambda = b[i];
    for(j=0;j<N;j++){
        lambda += w[i][j]*X[j];
    }
    return lambda;
}

double BM::random_num(){
    // 乱数生成器の初期化
    std::random_device rd;  // ハードウェア乱数生成器または擬似乱数生成器からの乱数シードを提供するオブジェクト
    std::mt19937 gen(rd()); // メルセンヌ・ツイスター乱数生成器（32ビット版）を使用

    // 一様実数分布の定義 [0, 1)
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    // 乱数生成
    return dis(gen);
}

// シグモイド関数
double BM::sig(int x){
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

    for(i=0;i<time;i++){
        setX(i);
        p_distr[x_num()] = exp(-energy_calc());
    }
    for(i=0;i<time;i++){
        Z += p_distr[i];
    }
    for(i=0;i<time;i++){
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
    for(i=0;i<pow(2,N);i++){
        histgram[i] = 0;
    }

    for(i=0;i<100;i++){
        update();
    }

    for(i=0;i<n;i++){
        for(j=0;j<1;j++){
            update();
        }
        // cout << X[0] << " " << X[1] << " " << X[2] << " " << X[3] << endl;
        histgram[x_num()] += 1;
    }
}

void BM::dataGen(int num){
    int i;
    FILE *datafile;
    datafile = fopen("./data/data.dat", "w");

    for(i=0;i<1000;i++){
        update();
    }

    for(i=0;i<num;i++){
        update();
        fprintf(datafile, "%d\n", x_num());
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
    while(gradient>0.001){
        for(i=0;i<N;i++){

            // xのデータ平均を求める処理
            x_ave_data = 0;
            for(j=0;j<traindatanum;j++){
                x_ave_data += traindata[j][i];
            }
            x_ave_data /= traindatanum;

            // xのモデル平均
            x_ave_model = 0;
            for(j=0;j<time;j++){
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
                for(k=0;k<time;k++){
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
            b[i] = b[i] + learn_rate*N*gradient_b[i];
        }
        for(i=0;i<N;i++){
            for(j=i+1;j<N;j++){
                gradient += gradient_w[i][j]*gradient_w[i][j];
                w[i][j] = w[i][j] + learn_rate*N*gradient_w[i][j];
                w[j][i] = w[i][j];
            }
        }
        p_distr_calc();
        cout << gradient << endl;
    }
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